#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"

int main() {
    // Input handling (fixed buffer sizes)
    char small_file_to_read[256];
    char large_file_to_read[256];
    double d;
    double epsilon;
    int n;

    printf("Input:\n");
    printf("1. Small file to read: ");
    if (scanf("%255s", small_file_to_read) != 1) {
        printf("Error reading filename\n");
        return 1;
    }

    printf("2. Large file to read: ");
    if (scanf("%255s", large_file_to_read) != 1) {
        printf("Error reading filename\n");
        return 1;
    }

    printf("3. Damping factor (0-1): ");
    if (scanf("%lf", &d) != 1 || d <= 0 || d >= 1) {
        printf("Invalid damping factor\n");
        return 1;
    }

    printf("4. Convergence threshold (0-1): ");
    if (scanf("%lf", &epsilon) != 1 || epsilon <= 0 || epsilon >= 1) {
        printf("Invalid epsilon\n");
        return 1;
    }

    printf("5. Top n pages: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid n\n");
        return 1;
    }
    while (getchar() != '\n'); // Clear input buffer

    /***** Process Small Graph *****/
    printf("\n=== Processing small graph ===\n");
    int N_small;
    double **hyperlink_matrix;
    read_graph_from_file1(small_file_to_read, &N_small, &hyperlink_matrix);

    double *scores_small = malloc(N_small * sizeof(double));
    PageRank_iterations1(N_small, hyperlink_matrix, d, epsilon, scores_small);
    top_n_webpages(N_small, scores_small, n > N_small ? N_small : n);

    // Clean small graph
    for (int i = 0; i < N_small; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    free(scores_small);

    /***** Process Large Graph *****/
    printf("\n=== Processing large graph ===\n");
    int N_large;
    int *row_ptr, *col_idx;
    double *val;
    read_graph_from_file2(large_file_to_read, &N_large, &row_ptr, &col_idx, &val);

    double *scores_large = malloc(N_large * sizeof(double));
    PageRank_iterations2(N_large, row_ptr, col_idx, val, d, epsilon, scores_large);
    top_n_webpages(N_large, scores_large, n > N_large ? N_large : n);

    // Clean large graph
    free(row_ptr);
    free(col_idx);
    free(val);
    free(scores_large);

    return 0;
}