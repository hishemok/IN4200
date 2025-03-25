#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declarations.h"
#include <omp.h>

void print_help(char *program_name) {
    printf("\nUsage: %s  <large_graph> <damping> <epsilon> <top_n>\n", program_name);
    printf("\nArguments:\n");
    printf("  large_graph    Path to large webgraph file\n");
    printf("  damping        Damping factor (0 < d < 1, typically 0.85)\n");
    printf("  epsilon        Convergence threshold (e.g., 1e-8)\n");
    printf("  top_n          Number of top pages to display\n");
    printf("\nExamples:\n");
    printf("  %s large.txt 0.85 1e-8 10\n", program_name);
    exit(0);
}


int main(int argc, char *argv[]) {

    // Normal argument processing
    if (argc != 5) {
        fprintf(stderr, "Error: Requires 4 arguments\n");
        print_help(argv[0]);
    }

    char *large_file = argv[1];
    double d = atof(argv[2]);
    double epsilon = atof(argv[3]);
    int n = atoi(argv[4]);

    // Validate parameters
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

    printf("\n=== Parallel PageRank ===\n");
    printf("Config: d=%.2f, ε=%.0e, top_n=%d\n", d, epsilon, n);
    printf("Threads available: %d\n", omp_get_max_threads());

    /***** Process Large Graph (Parallel) *****/
    printf("\nProcessing: %s\n", large_file);
    
    int N;
    int *row_ptr, *col_idx;
    double *val;
    
    // Read graph (serial)
    read_graph_from_file2(large_file, &N, &row_ptr, &col_idx, &val);
    printf("Loaded graph with %d nodes\n", N);

    double *scores = malloc(N * sizeof(double));
    if (scores == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Parallel PageRank
    PageRank_iterations2_omp(N, row_ptr, col_idx, val, d, epsilon, scores);

    // Parallel top-n (if available, otherwise serial)
    // #ifdef _OPENMP_TOP_N
    top_n_webpages_omp(N, scores, n > N ? N : n);
    // #else
    // top_n_webpages(N, scores, n > N ? N : n);
    // #endif

    // Cleanup
    free(row_ptr);
    free(col_idx);
    free(val);
    free(scores);

    return 0;
}