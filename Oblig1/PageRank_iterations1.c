#include <stdio.h>
#include <stdlib.h>
#include "math.h"


void PageRank_iterations1 (int N, double **hyperlink_matrix, double d, double epsilon,double *scores){
    double *xprev = malloc(N * sizeof(double));
    double *xcurr = malloc(N * sizeof(double));
    
    if (xprev == NULL || xcurr == NULL){
        printf("Error: Memory allocation failed for xprev or xnext\n");
        exit(1);
    }


    for (int i = 0; i < N; i++){
        xprev[i] = 1.0 / N;
    }

    double diff;
    int iteration = 0;
    while(diff > epsilon){
        diff = 0.0;
        double W_prev = 0.0;
        for (int i = 0; i < N; i++){
            int is_dangling = 1;
            for (int j = 0; j < N; j++){
                if (hyperlink_matrix[j][i] != 0.0){
                    is_dangling = 0;
                    break;
                }
            }
            if (is_dangling){
                W_prev += xprev[i];
            }
        }
        for (int i = 0; i < N; i++) {
            xcurr[i] = (1.0 - d) / N + d * W_prev / N;

            // Add contribution from non-dangling nodes
            for (int j = 0; j < N; j++) {
                xcurr[i] += d * hyperlink_matrix[i][j] * xprev[j];
            }

            // Track the maximum difference
            double curr_diff = fabs(xcurr[i] - xprev[i]);
            if (curr_diff > diff) {
                diff = curr_diff;
            }
        }

        // Swap xcurr and xprev for the next iteration
        double *temp = xprev;
        xprev = xcurr;
        xcurr = temp;

        iteration++;
    }

    for (int i = 0; i < N; i++) {
        scores[i] = xprev[i];
    }

    free(xprev);
    free(xcurr);
}