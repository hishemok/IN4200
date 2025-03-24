#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"
#include <omp.h>

int main(){
    int N;
    int *row_ptr, *col_idx;
    double *val;

    read_graph_from_file2("large_webpage_graph.txt", &N, &row_ptr, &col_idx, &val);

    // printf("row_ptr = [");
    // for (int i = 0; i <= N; i++) {
    //     printf("%d, ", row_ptr[i]);
    // }
    // printf("]\n");

    // printf("col_idx = [");
    // for (int i = 0; i < row_ptr[N]; i++) {
    //     printf("%d, ", col_idx[i]);
    // }
    // printf("]\n");

    // printf("val = [");
    // for (int i = 0; i < row_ptr[N]; i++) {
    //     printf("%f, ", val[i]);
    // }
    // printf("]\n");


    double d = 0.85; // Damping factor
    double epsilon = 1e-8; // Convergence threshold
    double *scores = malloc(N * sizeof(double)); // Array to store PageRank scores

    PageRank_iterations2(N, row_ptr, col_idx, val, d, epsilon, scores);


    int n = 3;
    top_n_webpages(N, scores, n);

    free(scores);
    free(row_ptr);
    free(col_idx);
    free(val);

    return 0;
}