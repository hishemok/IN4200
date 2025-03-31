#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"

void failed_input(char *program_name) {
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

int main(int argc, char *argv[]){

    if (argc != 6){
        failed_input(argv[0]);
    }
    char *small_file = argv[1];
    char *large_file = argv[2];
    double d = atof(argv[3]);
    double epsilon = atof(argv[4]);
    int n = atoi(argv[5]);

    if (d <= 0 || d >= 1){
        printf("Error: Damping factor must be between (0,1), got %f\n Damping factor set to default: d = 0.85\n",d);
        d = 0.85;
    }
    if (epsilon <= 0 ){
        printf("Error: Invalid epsilon must be between greater than 0, got %f\n Convergence factor set to default: epsilon = 1e-5\n",epsilon);
        epsilon = 1e-5;
    }
    if (n <= 0){
        printf("Error: n must be greater than 0, got %d\n Number of ranked pages set to default: n = 3\n",n);
        n = 3;
    }
    printf("Config: d=%.2f, epsilon=%.0e, top_n=%d\n",d,epsilon,n);

    printf("\n********* Processing %s Webgraph with Hyperlink Matrix *********\n",small_file);

    int N;
    double **hyperlink_matrix;
    printf("Reading graph from file\n");
    read_graph_from_file1 (small_file, &N, &hyperlink_matrix);

    printf("\nScoring webpages\n");
    double *scores = malloc(N * sizeof(double));
    PageRank_iterations1 (N, hyperlink_matrix, d,  epsilon, scores);

    printf("\nPrinting top %d webpages\n",n);
    top_n_webpages ( N,  scores,  n);


    for (int i = 0; i < N; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    

    printf("\n********* Processing %s Webgraph with CRS format *********\n",large_file);

    free(scores);

    int *row_ptr, *col_idx;
    double *val;
    printf("Reading graph from file\n");
    read_graph_from_file2(large_file,&N,&row_ptr,&col_idx,&val);

    double *scores_large = malloc(N * sizeof(double));
    printf("Scoring Webpages\n");
    PageRank_iterations2(N, row_ptr, col_idx, val, d, epsilon, scores_large);

    printf("\nPrinting top %d webpages\n",n);
    top_n_webpages ( N,  scores_large,  n);
    
    free(scores_large);
    free(row_ptr);
    free(col_idx);
    free(val);
    return 0;
}