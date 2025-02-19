#include <stdlib.h>
#include <math.h>

//These code snippets are not to run, just to save and find the code balance

void Dense_matrix_multiplication(double **A, double *B, double *y, int N){
    /*
    FLOPs per iteration: 1 multiplication, 1 addition -> 2 FLOPs
    Memeory traffic per iteration:
        Load A_ij: 8 bytes
        Load B_i: 8 bytes
        Load y_j: 8 bytes
        Store y_j: 8 bytes
        Total: 32 bytes
    Code_balance = Memory_traffic / FLOPs = 32 / 2 = 16 bytes/FLOP
        */

    for (int j = 0; j < N; j++)
        for (int i = 0; i < N; i++)
            y[j] += A[j][i] * B[i];
}

void Vector_norm(double *A, int N){
    /*
    FLOPs per iteration: 1 multiplication, 1 addition -> 2 FLOPs
    Memeory traffic per iteration:
        Load A_i: 8 bytes
        Load s: 8 bytes
        Store s: 8 bytes
        Total: 24 bytes
    Code_balance = Memory_traffic / FLOPs = 24 / 2 = 12 bytes/FLOP
    */


    double s = 0;
    for (int i = 0; i < N; i++){
        s += A[i] * A[i];
    }
}

void Scalar_product(double *A, double *B, int N){
    /*
    FLOPs per iteration: 1 multiplication, 1 addition -> 2 FLOPs
    Memeory traffic per iteration:
        Load A_i: 8 bytes
        Load B_i: 8 bytes
        Load s: 8 bytes
        Store s: 8 bytes
        Total: 32 bytes
    Code_balance = Memory_traffic / FLOPs = 32 / 2 = 16 bytes/FLOP
    */

    double s = 0;
    for (int i = 0; i < N; i++){
        s += A[i] * B[i];
    }
}

void Scalar_product_indirect_access(double *A, double *B, int *K, int N){
    /*
    FLOPs per iteration: 1 multiplication, 1 addition -> 2 FLOPs
    Memeory traffic per iteration:
        Load A_i: 8 bytes
        Load K_i: 4 bytes (assuming int)
        Load B[K_i]: 8 bytes
        Load s: 8 bytes
        Store s: 8 bytes
        Total: 36 bytes
    Code_balance = Memory_traffic / FLOPs = 36 / 2 = 18 bytes/FLOP
    */

    double s = 0;
    for (int i = 0; i < N; i++){
        s += A[i] * B[K[i]];
    }
}