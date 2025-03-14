#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"



int main(){
    int N;
    double **hyperlink_matrix;
    read_graph_from_file1("small_webpage_graph.txt", &N, &hyperlink_matrix);


    // Initialize PageRank parameters
    double d = 0.85; // Damping factor
    double epsilon = 1e-8; // Convergence threshold
    double *scores = malloc(N * sizeof(double)); // Array to store PageRank scores

    // Compute PageRank scores
    PageRank_iterations1(N, hyperlink_matrix, d, epsilon, scores);

    //print top n webpages
    int n = 3;
    top_n_webpages(N, scores, n);



    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    free(scores);

    return 0;
}

