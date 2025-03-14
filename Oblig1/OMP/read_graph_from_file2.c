#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_graph_from_file2 (char *filename, int *N, int **row_ptr, int **col_idx,
    double **val){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }
    /*
    This function reads a webgraph text files, and creates a PageRank matrix, stored as sparse matrix.
    The matrix is stored in Compressed Sparse Row (CSR) format.
    */

    char line[256];
    int edges;

    //Find number of nodes
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#') {
            // Search for "Nodes: x" and extract the value of x
            char *temp_line = strstr(line, "Nodes:");
            // printf("temp_line: %s\n", temp_line);
            if (temp_line != NULL) {
                sscanf(temp_line, "Nodes: %d   Edges: %d", N, &edges);
                printf("Number of nodes found: %d\n", *N);
                printf("Number of edges found: %d\n", edges);
                break;
            }
        }
        else{
            break;
        }
    }

    int *outgoings = (int *) malloc(*N * sizeof(int));
    if (outgoings == NULL) {
        printf("Error: Memory allocation failed for outgoings\n");
        exit(1);
    }

    *row_ptr = (int *) malloc((*N + 1) * sizeof(int));
    *col_idx = (int *) malloc(edges * sizeof(int));
    *val = (double *) malloc(edges * sizeof(double));

    if (*row_ptr == NULL || *col_idx == NULL || *val == NULL) {
        printf("Error: Memory allocation failed for row_ptr, col_idx or val\n");
        exit(1);
    }

    //Count outgoings
    while (fgets(line,sizeof(line),file) != NULL){
        if (line[0] != '#'){
            int from, to;
            sscanf(line, "%d %d", &from, &to);
            outgoings[from]++;
        }
    }


    free(outgoings);
    fclose(file);
}