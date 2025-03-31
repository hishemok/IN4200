#include <stdio.h>
#include <stdlib.h>
#include "math.h"



void find_dangling_pages (int N, double **hyperlink_matrix, int *dangling_indices, int *is_dangling){
    for (int j = 0; j < N; j++){
        double column_sum = 0.0;
        for (int i = 0; i < N; i++){
            column_sum += hyperlink_matrix[i][j];
        }
        if (column_sum == 0.0 || hyperlink_matrix[j][j] == 1.0){
            dangling_indices[j] = 1;
            *is_dangling = 1;
        }
        else{
            dangling_indices[j] = 0;
        }

    }
}

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
    
    int iterations = 0;
    double diff = 2.0;

    while (diff > epsilon ){
        //Find indices of dangling webpages
        int *dangling_indices = malloc(N * sizeof(int));
        int is_dangling = 0;


        find_dangling_pages(N, hyperlink_matrix, dangling_indices, &is_dangling);

        // Find W if we have dangling pages
        double W_prev = 0.0;
        if (is_dangling == 1){
            for (int i = 0; i < N; i++){
                if (dangling_indices[i] == 1){
                    W_prev += xprev[i];
                }
            }
        }    

        //Update Current X
        for (int i = 0; i < N; i++){
                xcurr[i] = (1.0 - d + d * W_prev) / N;
                for (int j = 0; j < N; j++){
                    double adding_term = d * hyperlink_matrix[i][j] * xprev[j];
                    xcurr[i] += adding_term;
                }
        }


        diff = 0.0;
        for (int i = 0; i < N; i++){
            // Set diff equal to the largest diff in the set
            double curr_diff = fabs(xcurr[i] - xprev[i]);
            if (curr_diff > diff){
                diff = curr_diff;
            }
            //update xprev
            xprev[i] = xcurr[i];
        }


        if (iterations > 10000){
            printf("Warning: Have not managed to converge within 10 000 iterations. Chekc if convergence threshold epsilon is too small\n");
        }

        free(dangling_indices);
        iterations ++;
    }
    // Set scores equal to x and free up memory
    for (int i = 0; i < N; i++) {
        scores[i] = xcurr[i];
    }

    printf("Converged after iterations: %d\n", iterations);
    free(xprev);
    free(xcurr);
}