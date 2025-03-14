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

    int *row_counts = calloc(*N, sizeof(int));
    int *temp_row_counts = calloc(*N, sizeof(int));
    if (row_counts == NULL || temp_row_counts == NULL) {
        printf("Error: Memory allocation failed for row_counts\n");
        exit(1);
    }

    // Count non-zero values per row
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#') {
            int from, to;
            sscanf(line, "%d %d", &from, &to);
            temp_row_counts[to]++; // Increment count for the row corresponding to 'to'
        }
    }

    // Allocate memory for CRS arrays
    *row_ptr = malloc((*N + 1) * sizeof(int));
    *col_idx = malloc(edges * sizeof(int));
    *val = malloc(edges * sizeof(double));

    if (*row_ptr == NULL || *col_idx == NULL || *val == NULL) {
        printf("Error: Memory allocation failed for CRS arrays\n");
        exit(1);
    }

    // Build row_ptr
    (*row_ptr)[0] = 0;
    for (int i = 0; i < *N; i++) {
        (*row_ptr)[i + 1] = (*row_ptr)[i] + temp_row_counts[i];
    }

    // Reset row_counts to use as a temporary index for each row
    memcpy(row_counts, temp_row_counts, *N * sizeof(int));
    memset(temp_row_counts, 0, *N * sizeof(int));

    // Rewind the file to read edges again
    rewind(file);

    // Skip the header
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue;
        break;
    }


    // Fill col_idx and val
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#') {
            int from, to;
            sscanf(line, "%d %d", &from, &to);

            // Calculate the index in col_idx and val
            int idx = (*row_ptr)[to] + temp_row_counts[to];
            (*col_idx)[idx] = from;
            (*val)[idx] = 1.0 / row_counts[to];

            // Increment the count for this row
            temp_row_counts[to]++;
        }
    }

    // Free temporary memory
    free(temp_row_counts);
    free(row_counts);
    fclose(file);
}