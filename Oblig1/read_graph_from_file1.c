#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void read_file_from_graph(char *filename, int *N, double ***hyperlink_matrix){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }
    printf("File opened successfully: %s\n", filename);


    int n = 0;
    char line[256];
    int *outgoings = NULL;


    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#') {
            // Search for "Nodes: x" and extract the value of x
            if (strstr(line, "Nodes:") != NULL) {
                sscanf(line, "# Nodes: %d", N); // Extract number of nodes
                printf("Number of nodes found: %d\n", *N);
            }
        }
    }
    printf("attempting memory allocation\n");
    double **matrix = malloc(*N * sizeof(double *));
    if (matrix == NULL) {
        printf("Error: Memory allocation failed for matrix rows\n");
        exit(1);
    }


    for (int i = 0; i < *N; i++){
        matrix[i] = malloc(*N * sizeof(double));
        for (int j = 0; j < *N; j++){
            matrix[i][j] = 0.0;
        }
    }

    outgoings = (int *) malloc(*N * sizeof(int));
    if (outgoings == NULL) {
        printf("Error: Memory allocation failed for outgoings\n");
        exit(1);
    }
    for (int i = 0; i < *N; i++){
        outgoings[i] = 0;
    }

    rewind(file);
    printf("Counting outgoings\n");
    while (fgets(line,sizeof(line),file) != NULL){
        if (line[0] != '#'){
            int from, to;
            sscanf(line, "%d %d", &from, &to);
            outgoings[from]++;
        }
    }

    rewind(file);
    printf("Creating matrix\n");
    while (fgets(line,sizeof(line),file) != NULL){
        if (line[0] != '#'){
            int from, to;
            sscanf(line, "%d %d", &from, &to);
            printf("from: %d, to: %d\n", from, to);
            matrix[to][from] = 1.0 / outgoings[from];
        }
    }


    printf("Closing file\n");
    fclose(file);


    *hyperlink_matrix = matrix;
    printf("Matrix pointer: %p\n", *hyperlink_matrix);


}