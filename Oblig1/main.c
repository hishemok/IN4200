#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"



int main(){
    int N;
    double **hyperlink_matrix;
    read_file_from_graph("small_webpage_graph.txt", &N, &hyperlink_matrix);

    printf("Number of nodes: %d\n", N);

    // Initialize PageRank parameters
    double d = 0.85; // Damping factor
    double epsilon = 1e-6; // Convergence threshold
    double *scores = malloc(N * sizeof(double)); // Array to store PageRank scores

    // Compute PageRank scores
    PageRank_iterations1(N, hyperlink_matrix, d, epsilon, scores);

    // Print the PageRank scores
    printf("PageRank scores:\n");
    for (int i = 0; i < N; i++) {
        printf("Node %d: %f\n", i, scores[i]);
    }

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    free(scores);

    return 0;
}

