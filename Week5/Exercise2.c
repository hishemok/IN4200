#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void original_loop(double *y, double **A, double *B, int N) {
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            y[j] += A[j][i] * B[i];
        }
    }
}

void unroll_and_jam(double *y, double **A, double *B, int N) {
    int i, j;
    // Main loop with unroll and jam
    for (j = 0; j <= N - 4; j += 4) {
        for (i = 0; i < N; i++) {
            y[j] += A[j][i] * B[i];
            y[j + 1] += A[j + 1][i] * B[i];
            y[j + 2] += A[j + 2][i] * B[i];
            y[j + 3] += A[j + 3][i] * B[i];
        }
    }

    // Handle remaining iterations of the outer loop
    for (; j < N; j++) {
        for (i = 0; i < N; i++) {
            y[j] += A[j][i] * B[i];
        }
    }
}

int main() {
    int N = 32000;
    double *y = (double *)malloc(N * sizeof(double));
    double **A = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
    }
    double *B = (double *)malloc(N * sizeof(double));

    // Initialize arrays (for demonstration)
    for (int i = 0; i < N; i++) {
        y[i] = 0.0;
        B[i] = (double)rand() / RAND_MAX;
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Measure time for original loop
    clock_t start = clock();
    original_loop(y, A, B, N);
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by original_loop: %f\n", cpu_time_used);

    // Reset y to 0 
    for (int i = 0; i < N; i++) {
        y[i] = 0.0;
    }

    // Measure time for unroll_and_jam
    start = clock();
    unroll_and_jam(y, A, B, N);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by unroll_and_jam: %f\n", cpu_time_used);

    // Free memory
    free(y);
    for (int i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);

    return 0;
}