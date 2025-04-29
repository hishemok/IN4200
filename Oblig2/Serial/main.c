#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "functions.h"


int main(int nargs, char **args) {
    if (nargs != 5){
        printf("Usage: %s <n_iterations> <kmax> <jmax> <imax>\n", args[0]);
        return 1;
    }
    int n_iterations = atoi(args[1]);
    int kmax = atoi(args[2]);
    int jmax = atoi(args[3]);
    int imax = atoi(args[4]);
    if (n_iterations <= 0 || kmax <= 0 || jmax <= 0 || imax <= 0) {
        printf("Error: All dimensions must be greater than 0.\n");
        return 1;
    }
    
    printf("\n____________________________________\n");

    double ***arr1;
    double ***arr2;
    // Allocate memory for the 3D arrays
    allocate_array3D(kmax, jmax, imax, &arr1);
    allocate_array3D(kmax, jmax, imax, &arr2);


    // Initialize arr1 with some values
    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            for (int i = 0; i < imax; i++) {
                double value =   (double)( k * (k + 1) + (j - 1) * (i + 2) );// Some random varying value
                arr1[k][j][i] = value;
                arr2[k][j][i] = value;
            }
        }
    }



    for (int i = 0; i < n_iterations; i++){    
        // Call the GS_iteration_normal function
        GS_iteration_normal(kmax, jmax, imax, arr1);
        GS_iteration_2_chunks(kmax, jmax, imax, arr2);
    }



    // Calculate the Euclidean distance
    double distance = euclidean_distance(kmax, jmax, imax, arr1, arr2);


    printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
        n_iterations, kmax, jmax, imax, distance);
    // Free allocated memory

    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            free(arr1[k][j]);
            free(arr2[k][j]);
        }
        free(arr1[k]);
        free(arr2[k]);
    }
    free(arr1);
    free(arr2);


    return 0;
}