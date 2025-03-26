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
    printf("\n\nPageRank_iterations1\n");
    double *xprev = malloc(N * sizeof(double));
    double *xcurr = malloc(N * sizeof(double));
    
    if (xprev == NULL || xcurr == NULL){
        printf("Error: Memory allocation failed for xprev or xnext\n");
        exit(1);
    }


    for (int i = 0; i < N; i++){
        xprev[i] = 1.0 / N;
    }
    printf("---Initial guess:\nscores=[");
    for (int i = 0; i < N; i++) printf("%.6f,", xprev[i]);
    printf("]\n");

    int iterations = 0;
    double diff = 2.0;

    while (diff > epsilon && iterations < 10000){
        //Find indices of dangling webpages
        int *dangling_indices = malloc(N * sizeof(int));
        int is_dangling = 0;


        find_dangling_pages(N, hyperlink_matrix, dangling_indices, &is_dangling);


        double W_prev = 0.0;
        if (is_dangling == 1){
            for (int i = 0; i < N; i++){
                if (dangling_indices[i] == 1){
                    W_prev += xprev[i];
                }
            }
        }    

        for (int i = 0; i < N; i++){
                xcurr[i] = (1.0 - d) / N + d * W_prev / N;
                for (int j = 0; j < N; j++){
                    double adding_term = d * hyperlink_matrix[i][j] * xprev[j];
                    xcurr[i] += adding_term;
                }
        }


        diff = 0.0;
        printf("---After iteration %d:\nscores=[", iterations);
        for (int i = 0; i < N; i++){
            double curr_diff = fabs(xcurr[i] - xprev[i]);
            if (curr_diff > diff){
                diff = curr_diff;
            }
            xprev[i] = xcurr[i];
            printf("%.6f,", xprev[i]);
        }
        printf("]\n");

        free(dangling_indices);
        iterations ++;
    }

    for (int i = 0; i < N; i++) {
        scores[i] = xprev[i];
    }

    printf("Iterations: %d\n", iterations);
    free(xprev);
    free(xcurr);
}