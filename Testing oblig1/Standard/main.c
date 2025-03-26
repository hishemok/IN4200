#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Printing message
#include "function_declarations.h"

void print_help(char *program_name) {
    printf("\nUsage: %s  <small_graph> <large_graph> <damping> <epsilon> <top_n>\n", program_name);
    printf("\nArguments:\n");
    printf("  small_graph    Path to small webgraph file\n");
    printf("  large_graph    Path to large webgraph file\n");
    printf("  damping        Damping factor (0 < d < 1, typically 0.85)\n");
    printf("  epsilon        Convergence threshold (e.g., 1e-8)\n");
    printf("  top_n          Number of top pages to display\n");
    printf("\nExamples:\n");
    printf("  %s small.txt large.txt 0.85 1e-8 10\n", program_name);
    exit(0);
}

int main(int argc, char *argv[]) {

    if (argc != 6) {
        fprintf(stderr, "Error: Requires 4 arguments\n");
        print_help(argv[0]);
    }

    char *small_file_to_read = argv[1];
    char *large_file_to_read = argv[2];
    double d = atof(argv[3]);
    double epsilon = atof(argv[4]);
    int n = atoi(argv[5]);


    if (d <= 0 || d >= 1) {
        printf("Error: Damping factor must be (0,1), got %f\n", d);
        return 1;
    }
    if (epsilon <= 0) {
        printf("Error: Epsilon must be positive, got %e\n", epsilon);
        return 1;
    }
    if (n <= 0) {
        printf("Error: top_n must be positive, got %d\n", n);
        return 1;
    }

    printf("Config: d=%.2f, ε=%.0e, top_n=%d\n", d, epsilon, n);

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