// Checks, if it is possible, how to use integer operations + - * / on 4 unreplaced operands to get 24.
// Division must yield integer quotient and zero remainder.
// Prints the solution (if found) in 3 separate steps.
// Last updated 2024-12-23

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define INVALID -1


enum operation{ADD, SUB, MUL, DIV};

struct expression {
    int operandA;
    enum operation operation;
    int operandB;
    int result;
};


bool find_4(int A, int B, int C, int D, int tempA[4], int tempB[4], int tempC[4], struct expression * step3, struct expression * step2, struct expression * step1);
bool find_3(int A, int B, int C, int tempA[4], int tempB[4], struct expression * step3, struct expression * step2);
bool find_2(int A, int B, int temp[4], struct expression * step3);
void merge(int A, int B, int res[4], struct expression * step);


int main(int argc, char * argv[]) {
    int A = atoi(argv[1]);
    int B = atoi(argv[2]);
    int C = atoi(argv[3]);
    int D = atoi(argv[4]);
    int tempA[4], tempB[4], tempC[4];
    struct expression * steps[3];
    for (int i = 0; i < 3; i++) {
        steps[i] = malloc(sizeof(struct expression));
    }
    bool res = find_4(A, B, C, D, tempA, tempB, tempC, steps[2], steps[1], steps[0]);
    char operations[4] = {'+', '-', '*', '/'};
    if (res) {
        printf("Solution found\n");
        for (int i = 0; i < 3; i++) {
            printf("%d %c %d = %d\n", steps[i]->operandA, operations[steps[i]->operation], steps[i]->operandB, steps[i]->result);
        }
    }
    else printf("No solution found\n");
}



bool find_4(int A, int B, int C, int D, int tempA[4], int tempB[4], int tempC[4], struct expression * step3, struct expression * step2, struct expression * step1) {
    // Merge A and B
    merge(A, B, tempA, step1);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_3(tempA[i], C, D, tempB, tempC, step3, step2)) {
            step1->operation = i;
            step1->result = tempA[i];
            return true;
        }
    }
    // Merge A and C
    merge(A, C, tempA, step1);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_3(tempA[i], B, D, tempB, tempC, step3, step2)) {
            step1->operation = i;
            step1->result = tempA[i];
            return true;
        }
    }
    // Merge A and D
    merge(A, D, tempA, step1);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_3(tempA[i], B, C, tempB, tempC, step3, step2)) {
            step1->operation = i;
            step1->result = tempA[i];
            return true;
        }
    }
    // Merge B and C
    merge(B, C, tempA, step1);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_3(tempA[i], A, D, tempB, tempC, step3, step2)) {
            step1->operation = i;
            step1->result = tempA[i];
            return true;
        }
    }
    // Merge B and D
    merge(B, D, tempA, step1);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_3(tempA[i], A, C, tempB, tempC, step3, step2)) {
            step1->operation = i;
            step1->result = tempA[i];
            return true;
        }
    }
    // Merge C and D
    merge(C, D, tempA, step1);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_3(tempA[i], A, B, tempB, tempC, step3, step2)) {
            step1->operation = i;
            step1->result = tempA[i];
            return true;
        }
    }
    // 
    return false;
}


bool find_3(int A, int B, int C, int tempA[4], int tempB[4], struct expression * step3, struct expression * step2) {
    // Merge A and B
    merge(A, B, tempA, step2);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_2(tempA[i], C, tempB, step3)) {
            step2->operation = i;
            step2->result = tempA[i];
            return true;
        }
    }
    // Merge A and C
    merge(A, C, tempA, step2);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_2(tempA[i], B, tempB, step3)) {
            step2->operation = i;
            step2->result = tempA[i];
            return true;
        }
    }
    // Merge B and C
    merge(B, C, tempA, step2);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && find_2(tempA[i], A, tempB, step3)) {
            step2->operation = i;
            step2->result = tempA[i];
            return true;
        }
    }
    //
    return false;
}


bool find_2(int A, int B, int temp[4], struct expression * step3) {
    merge(A, B, temp, step3);
    for (int i = 0; i < 4; i++) {
        if (temp[i] == 24) {
            step3->operation = i;
            step3->result = temp[i];
            return true;
        }
    }
    return false;
}


// Enters operands A and B into step in correct order, enters results of four operations into res
void merge(int A, int B, int res[4], struct expression * step) {
    if (A < B) {
        int temp = A;
        A = B;
        B = temp;
    }
    step->operandA = A;
    step->operandB = B;
    res[0] = A + B;
    res[1] = A - B;
    res[2] = A * B;
    if (B != 0 && A % B == 0) res[3] = A / B;
    else res[3] = INVALID;
}


