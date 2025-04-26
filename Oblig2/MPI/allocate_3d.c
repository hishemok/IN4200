#include <stdio.h>
#include <stdlib.h>

void allocate_array3D (int kmax, int jmax, int imax, double ****array) {
    // Allocate memory for the 3D array
    *array = (double ***)malloc(kmax * sizeof(double **));
    if (*array == NULL) {
        fprintf(stderr, "Memory allocation failed for array\n");
        exit(1);
    }

    for (int k = 0; k < kmax; k++) {
        (*array)[k] = (double **)malloc(jmax * sizeof(double *));
        if ((*array)[k] == NULL) {
            fprintf(stderr, "Memory allocation failed for array[%d]\n", k);
            exit(1);
        }
        for (int j = 0; j < jmax; j++) {
            (*array)[k][j] = (double *)malloc(imax * sizeof(double));
            if ((*array)[k][j] == NULL) {
                fprintf(stderr, "Memory allocation failed for array[%d][%d]\n", k, j);
                exit(1);
            }
        }
    }
}