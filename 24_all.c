// Checks, if it is possible, how to use integer operations + - * / on 4 unreplaced operands to get 24.
// Prints the solution (if found) in one line with parentheses.
// Last updated 2025-04-27

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// CONSTANTS

#define NO_OPERATOR -1
#define TARGET 24
#define NUM_OPERANDS 4
#define NUM_OPERATORS 5


// STRUCTS AND ENUMS

enum operator{ADD, DIF, MUL, DIV, RDIV};

struct expression {
    struct expression * operandA;
    enum operator operator;
    struct expression * operandB;
    struct rational * result;
};

struct rational {
    int p;
    int q;
};



// FUNCTION PROTOTYPES

struct expression * solve(
    int nnums, struct rational ** nums, struct rational *target
);

// Functions on rationals
struct rational * new_rational(int p, int q);
void negate(struct rational * a);
int sign(struct rational * a);
void simplify(struct rational * a);
int compare(struct rational * a, struct rational * b);
struct rational * add(struct rational * a, struct rational * b);
struct rational * sub(struct rational * a, struct rational * b);
struct rational * dif(struct rational * a, struct rational * b);
struct rational * mul(struct rational * a, struct rational * b);
struct rational * dvd(struct rational * a , struct rational * b);
struct rational * combine(struct rational * a, struct rational * b, enum operator operator);

// Helpers to functions on rationals
int gcd(int a, int b);
int lcm(int a, int b);

// Functions on expression trees and pretty-printing
struct expression * new_expression(struct rational * result);
bool insert_expression(
    struct expression * root, 
    struct rational * c, 
    struct rational * a, struct rational * b, enum operator op
);
void print_expression_tree(struct expression * root);
void free_expression_tree(struct expression * root);



// FUNCTIONS


bool f(int A, int B, int C, int D) {
    bool res = false;
    struct rational ** array = malloc(NUM_OPERANDS * sizeof(struct rational *));
    array[0] = new_rational(A, 1);
    array[1] = new_rational(B, 1);
    array[2] = new_rational(C, 1);
    array[3] = new_rational(D, 1);
    struct rational * target = new_rational(TARGET, 1);
    struct expression * solution = solve(NUM_OPERANDS, array, target);

    if (solution != NULL) {
        res = true;
        print_expression_tree(solution);
        printf(" = %d\n", TARGET);
        free_expression_tree(solution);
    } else printf("No solutions found\n");
    
    for (int i = 0; i < NUM_OPERANDS; i++) {
        free(array[i]);
    }
    free(array);
    return res;
}


int main(int argc, char * argv[]) {
    int total = 0;
    int solvable = 0;
    for (int i = 1; i <= 13; i++) {
        for (int j = i; j <= 13; j++) {
            for (int k = j; k <= 13; k++) {
                for (int l = k; l <= 13; l++) {
                    total++;
                    if (f(i, j, k, l)) solvable++;
                }
            }
        }
    }
    printf("Total: %d\nSolvable: %d\n", total, solvable);
}




/*
    Given a number of rationals and a target, constructs a tree of operations 
    (expression tree) that uses the rationals to compute the target, if such 
    tree exists.
*/
struct expression * solve(
    int nnums, struct rational ** nums, struct rational *target
) {    
    if (nnums == 0) return NULL;
    if (nnums == 1) {
        if (nums[0] != NULL && compare(nums[0], target) == 0) return new_expression(target);
        else return NULL;
    }
    struct rational ** copy = malloc((nnums - 1) * sizeof(struct rational *));
    struct expression * res = NULL;
    for (int i = 0; i < nnums - 1; i++) {
        for (int j = i + 1; j < nnums; j++) {
            // For each combination
            int k = 0;
            for (int l = 0; l < nnums - 2; l++) {
                if (k == i) k++;
                if (k == j) k++;
                copy[l] = nums[k];
                k++;
            }
            // Copy made
            for (int op = 0; op < NUM_OPERATORS; op++) {
                copy[nnums - 2] = combine(nums[i], nums[j], op);
                if (copy[nnums - 2] == NULL) continue;
                res = solve(nnums - 1, copy, target);
                if (res != NULL) {
                    // search for copy[nnums - 2]
                    if (!insert_expression(res, copy[nnums - 2], nums[i], nums[j], op)) {
                        // Insert failed error
                        print_expression_tree(res);
                        fprintf(stderr, "Insert failed! nnums = %d, i = %d, j = %d\n", nnums, i, j);
                        exit(1);
                    }
                    free(copy);
                    return res;
                } else free(copy[nnums - 2]);
            }
        }
    }
    free(copy);
    return NULL;
}

/* Creates a new rational out of integer numerator and denominator. */
struct rational * new_rational(int p, int q) {
    if (q == 0) return NULL;
    struct rational * n = malloc(sizeof(*n));
    n->p = p;
    n->q = q;
    return n;
}

/* Negates a rational by negating its numerator. */
void negate(struct rational * a) {
    if (a != NULL) a->p *= (-1);
}

/* 
    Returns the product of the numerator and denominator to help 
    determine the sign of the given rational, no-op if NULL.
*/
int sign(struct rational * a) {
    if (a == NULL) {
        fprintf(stderr, "Tried to take sign of null pointer\n");
        exit(1);
    }
    return (a->p) * (a->q);
}

/* Simplifies a rational, no-op if NULL. */
void simplify(struct rational * a) {
    if (a != NULL) {
        int d = gcd(a->p, a->q);
        a->p /= d;
        a->q /= d;
    }
}

/* Returns a positive int if a>b, zero if a=b and negative if a<b. */
int compare(struct rational * a, struct rational * b) {
    if (a == NULL || b == NULL) {
        fprintf(stderr, "Tried to compare null pointer\n");
        exit(1);
    }
    int q = lcm(a->q, b->q);
    return (q * (a->p) / (a->q) - q * (b->p) / (b->q));
}

/* Returns the sum of two given rationals as a new rational. */
struct rational * add(struct rational * a, struct rational * b) {
    if (a == NULL || b == NULL) return NULL;
    int q = lcm(a->q, b->q);
    struct rational * n = new_rational(q * (a->p) / (a->q) + q * (b->p) / (b->q), q);
    simplify(n);
    return n;
}

/* Returns the difference of two given rationals as a new rational. */
struct rational * sub(struct rational * a, struct rational * b) {
    if (a == NULL || b == NULL) return NULL;
    int q = lcm(a->q, b->q);
    return new_rational(q * (a->p) / (a->q) - q * (b->p) / (b->q), q);
}

/* 
    Returns the absolute difference of two given rationals as a new 
    rational.
*/
struct rational * dif(struct rational * a, struct rational * b) {
    struct rational * n = sub(a, b);
    if (sign(n) < 0) negate(n);
    simplify(n);
    return n;
}

/* Returns the product of two given rationals as a new rational. */
struct rational * mul(struct rational * a, struct rational * b) {
    if (a == NULL || b == NULL) return NULL;
    struct rational * n = new_rational((a->p) * (b->p), (a->q) * (b->q));
    simplify(n);
    return n;
}

/*
    Returns the product of one given rational and the reciprocal 
    of the other given rational, as a new rational. 
*/
struct rational * dvd(struct rational * a , struct rational * b) {
    if (a == NULL || b == NULL) return NULL;
    struct rational * n = new_rational((a->p) * (b->q), (a->q) * (b->p));
    simplify(n);
    return n;
}

/*
    Designed to be used as part of an iterator through the operators 
    in the enum.
*/
struct rational * combine(struct rational * a, struct rational * b, enum operator operator) {
    if (operator == ADD) return add(a, b);
    if (operator == DIF) return dif(a, b);
    if (operator == MUL) return mul(a, b);
    if (operator == DIV) return dvd(a, b);
    if (operator == RDIV) return dvd(b, a);
    return NULL;
}



/* Returns the greatest common divisor of two integers. */
int gcd(int a, int b) {
    int temp;
    if (b > a) {
        temp = a;
        a = b;
        b = temp;
    }
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Returns the least common multiple of two integers. */
int lcm(int a, int b) {
    return a * b / gcd(a, b);
}



/* Creates a new expression as a standalone result field. */
struct expression * new_expression(struct rational * result) {
    struct expression * n = malloc(sizeof(*n));
    n->operandA = NULL;
    n->operator = NO_OPERATOR;
    n->operandB = NULL;
    n->result = new_rational(result->p, result->q);
    return n;
}

/* 
    Recursively inserts an expression into the tree of operations, 
    returns whether successful, frees duplicate memory.
*/
bool insert_expression(
    struct expression * root, 
    struct rational * c, 
    struct rational * a, struct rational * b, enum operator op
) {
    if (root == NULL) return false;
    if (compare(root->result, c) == 0 && root->operandA == NULL && root->operandB == NULL) {
        // Found
        if (op == RDIV || (op == DIF && compare(a, b) < 0)) {
            root->operandA = new_expression(b);
            root->operandB = new_expression(a);
        } else {
            root->operandA = new_expression(a);
            root->operandB = new_expression(b);
        }
        root->operator = op;
        free(c);
        return true;
    }
    else if (insert_expression(root->operandA, c, a, b, op)) return true;
    else if (insert_expression(root->operandB, c, a, b, op)) return true;
    return false;
}

/* Prints the tree of operations in parentheses form. */
void print_expression_tree(struct expression * root) {
    if (root == NULL) return;
    if (root->operandA == NULL) {
        if (root->result->q == 1) {
            printf("%d", root->result->p);
        } else {
            printf("(%d / %d)", root->result->p, root->result->q);
        }
    } else {
        printf("(");
        print_expression_tree(root->operandA);
        if (root->operator == ADD) printf(" + ");
        else if (root->operator == DIF) printf(" - ");
        else if (root->operator == MUL) printf(" * ");
        else printf(" / ");
        print_expression_tree(root->operandB);
        printf(")");
    }
}

// Frees all memory associated with the tree of operations
void free_expression_tree(struct expression * root) {
    if (root == NULL) return;
    free_expression_tree(root->operandA);
    free_expression_tree(root->operandB);
    free(root->result);
    free(root);
}