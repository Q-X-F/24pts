// Checks if it is possible to use integer operations + - * / on 4 unreplaced operands to get 24.
// Division must yield integer quotient and zero remainder.
// Last updated 2024-12-21

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define INVALID -1


bool verify_4(int A, int B, int C, int D, int tempA[4], int tempB[4], int tempC[4]);
bool verify_3(int A, int B, int C, int tempA[4], int tempB[4]);
bool verify_2(int A, int B, int temp[4]);
void merge(int A, int B, int res[4]);


int main(int argc, char * argv[]) {
    int A = atoi(argv[1]);
    int B = atoi(argv[2]);
    int C = atoi(argv[3]);
    int D = atoi(argv[4]);
    int tempA[4], tempB[4], tempC[4];
    bool res = verify_4(A, B, C, D, tempA, tempB, tempC);
    if (res) printf("Possible\n");
    else printf("Impossible\n");
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


