#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void read_file_from_graph(char *filename, int *N, double ***hyperlink_matrix){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

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
    double **matrix = malloc(*N * sizeof(double *));
    if (matrix == NULL) {
        printf("Error: Memory allocation failed for matrix rows\n");
        exit(1);
    }
    for (int i = 0; i < *N; i++){
        matrix[i] = malloc(*N * sizeof(double));
        outgoings[i] = 0;
        for (int j = 0; j < *N; j++){
            matrix[i][j] = 0.0;
        }
    }
    printf("Default values set\n");


    rewind(file);
    //Count outgoings
    while (fgets(line,sizeof(line),file) != NULL){
        if (line[0] != '#'){
            int from, to;
            sscanf(line, "%d %d", &from, &to);
            outgoings[from]++;
        }
    }

    rewind(file);
    //Fill matrix
    while (fgets(line,sizeof(line),file) != NULL){
        if (line[0] != '#'){
            int from, to;
            sscanf(line, "%d %d", &from, &to);
            matrix[to][from] = 1.0 / outgoings[from];
        }
    }

    *hyperlink_matrix = matrix;

    free(outgoings);
    fclose(file);

}