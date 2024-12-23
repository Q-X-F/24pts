// Counts the number of 24-solvable hands
// Last updated 2024-12-22

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define INVALID -1


bool verify_4(int A, int B, int C, int D, int tempA[4], int tempB[4], int tempC[4]);
bool verify_3(int A, int B, int C, int tempA[4], int tempB[4]);
bool verify_2(int A, int B, int temp[4]);
void merge(int A, int B, int res[4]);


int count_solvable_nums(int tempA[4], int tempB[4], int tempC[4]) {
    int count = 0;
    for (int A = 1; A <= 13; A++) {
        for (int B = 1; B <= 13; B++) {
            for (int C = 1; C <= 13; C++) {
                for (int D = 1; D <= 13; D++) {
                    if (verify_4(A, B, C, D, tempA, tempB, tempC)) count++;
                }
            }
        }
    }
    return count;
}

int count_solvable_hands(int tempA[4], int tempB[4], int tempC[4]) {
    int count = 0;
    for (int A = 1; A <= 13; A++) {
        for (int B = A; B <= 13; B++) {
            for (int C = B; C <= 13; C++) {
                for (int D = C; D <= 13; D++) {
                    if (verify_4(A, B, C, D, tempA, tempB, tempC)) {
                        if (A == B == C == D) count++; // four-of-a-kind
                        else if (A == B == C || B == C == D) count += 16; // three-of-a-kind
                        else if (A == B && C == D) count += 36; // two pairs
                        else if (A == B || B == C || C == D) count += 96; // one pair
                        else count += 256; // distinct
                    }
                }
            }
        }
    }
    return count;
}

int main(void) {
    int tempA[4], tempB[4], tempC[4];
    //printf("%d out of %d\n", count_solvable_nums(tempA, tempB, tempC), 28561);
    int N = count_solvable_hands(tempA, tempB, tempC);
    printf("%d out of %d hands are solvable, P(solvable) = ~%.2lf\n", N, 270725, ((float) N) / (float) 270725); // 270725 = (52 C 4)
}



bool verify_4(int A, int B, int C, int D, int tempA[4], int tempB[4], int tempC[4]) {
    // Merge A and B
    merge(A, B, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_3(tempA[i], C, D, tempB, tempC)) return true;
    }
    // Merge A and C
    merge(A, C, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_3(tempA[i], B, D, tempB, tempC)) return true;
    }
    // Merge A and D
    merge(A, D, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_3(tempA[i], B, C, tempB, tempC)) return true;
    }
    // Merge B and C
    merge(B, C, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_3(tempA[i], A, D, tempB, tempC)) return true;
    }
    // Merge B and D
    merge(B, D, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_3(tempA[i], A, C, tempB, tempC)) return true;
    }
    // Merge C and D
    merge(C, D, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_3(tempA[i], A, B, tempB, tempC)) return true;
    }
    // 
    return false;
}


bool verify_3(int A, int B, int C, int tempA[4], int tempB[4]) {
    // Merge A and B
    merge(A, B, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_2(tempA[i], C, tempB)) return true;
    }
    // Merge A and C
    merge(A, C, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_2(tempA[i], B, tempB)) return true;
    }
    // Merge B and C
    merge(B, C, tempA);
    for (int i = 0; i < 4; i++) {
        if (tempA[i] != INVALID && verify_2(tempA[i], A, tempB)) return true;
    }
    //
    return false;
}


bool verify_2(int A, int B, int temp[4]) {
    merge(A, B, temp);
    for (int i = 0; i < 4; i++) {
        if (temp[i] == 24) return true;
    }
    return false;
}



void merge(int A, int B, int res[4]) {
    if (A < B) {
        int temp = A;
        A = B;
        B = temp;
    }
    res[0] = A + B;
    res[1] = A - B;
    res[2] = A * B;
    if (B != 0 && A % B == 0) res[3] = A / B;
    else res[3] = INVALID;
}


