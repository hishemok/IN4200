#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int from;
    int to;
} Edge;

typedef struct {
    int col;
    double val;
} Entry;

int compare_entries(const void *a, const void *b) {
    return ((Entry*)a)->col - ((Entry*)b)->col;
}

void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx, double **val) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }
    
    char line[256];
    int edges = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#') {
            char *temp_line = strstr(line, "Nodes:");
            if (temp_line != NULL) {
                if (sscanf(temp_line, "Nodes: %d   Edges: %d", N, &edges) != 2) {
                    printf("Error: Failed to parse node and edge count\n");
                    exit(1);
                }
                printf("Number of nodes found: %d\n", *N);
                printf("Number of edges found: %d\n", edges);
                break;
            }
        } else {
            break;
        }
    }
    
    // Validate node and edge counts
    if (*N <= 0 || edges <= 0) {
        printf("Error: Invalid node or edge count: N=%d, edges=%d\n", *N, edges);
        exit(1);
    }
    
        
    // Allocate edge list with error checking
    Edge *edge_list = malloc((size_t)edges * sizeof(Edge));
    if (edge_list == NULL) {
        printf("Error: Memory allocation failed for edge list (requested %zu bytes)\n", 
               (size_t)edges * sizeof(Edge));
        exit(1);
    }
    

    
    // Read edges with progress tracking
    int edge_count = 0;
    int line_count = 0;
    
    rewind(file); // Go back to start of file
    
    // Skip header lines again
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#') break;
    }
    
    // Now read the actual edges
    do {
        if (line[0] != '#') {
            int from, to;
            if (sscanf(line, "%d %d", &from, &to) == 2) {
                if (from >= 0 && from < *N && to >= 0 && to < *N) {
                    edge_list[edge_count].from = from;
                    edge_list[edge_count].to = to;
                    edge_count++;
                } 

            }
        }
        line_count++;
    } while (edge_count < edges && fgets(line, sizeof(line), file));
    
    
    if (edge_count != edges) {
        printf("Warning: Expected %d edges but read %d edges\n", edges, edge_count);
        // Continue with the edges we have
        edges = edge_count;
    }
    

    //initialize counts
    int *incoming_counts = calloc(*N, sizeof(int));
    int *outgoing_counts = calloc(*N, sizeof(int));
    
    if (incoming_counts == NULL || outgoing_counts == NULL) {
        printf("Error: Memory allocation failed for edge counts\n");
        exit(1);
    }
    
    // Count edges
    for (int i = 0; i < edge_count; i++) {
        incoming_counts[edge_list[i].to]++;
        outgoing_counts[edge_list[i].from]++;
    }
    
    Entry **row_entries = malloc(*N * sizeof(Entry*));
    if (row_entries == NULL) {
        printf("Error: Memory allocation failed for row_entries\n");
        exit(1);
    }
    
    for (int i = 0; i < *N; i++) {
        if (incoming_counts[i] > 0) {
            row_entries[i] = malloc(incoming_counts[i] * sizeof(Entry));
            if (row_entries[i] == NULL) {
                printf("Error: Memory allocation failed for row_entries[%d]\n", i);
                exit(1);
            }
        } else {
            row_entries[i] = NULL; // No incoming edges
        }
    }
    
    //Reset counts to use as indices
    memset(incoming_counts, 0, *N * sizeof(int));
        
    // Fill row entries
    for (int i = 0; i < edge_count; i++) {
        int from = edge_list[i].from;
        int to = edge_list[i].to;
        
        if (outgoing_counts[from] > 0) { // Avoid division by zero
            int idx = incoming_counts[to]++;
            row_entries[to][idx].col = from;
            row_entries[to][idx].val = 1.0 / outgoing_counts[from];
        }
    }
        
    // Sort entries
    for (int i = 0; i < *N; i++) {
        if (incoming_counts[i] > 0) {
            qsort(row_entries[i], incoming_counts[i], sizeof(Entry), compare_entries);
        }
    }
    

    // Allocate CSR arrays
    *row_ptr = malloc((*N + 1) * sizeof(int));
    *col_idx = malloc(edge_count * sizeof(int));
    *val = malloc(edge_count * sizeof(double));
    
    if (*row_ptr == NULL || *col_idx == NULL || *val == NULL) {
        printf("Error: Memory allocation failed for CSR arrays\n");
        exit(1);
    }
    
    // Build CSR format
    (*row_ptr)[0] = 0;
    int current_idx = 0;
    
    for (int i = 0; i < *N; i++) {
        for (int j = 0; j < incoming_counts[i]; j++) {
            (*col_idx)[current_idx] = row_entries[i][j].col;
            (*val)[current_idx] = row_entries[i][j].val;
            current_idx++;
        }
        (*row_ptr)[i+1] = current_idx;
    }
    
    printf("Cleaning up...\n");
    
    // Cleanup
    for (int i = 0; i < *N; i++) {
        if (row_entries[i] != NULL) {
            free(row_entries[i]);
        }
    }
    free(row_entries);
    free(incoming_counts);
    free(outgoing_counts);
    free(edge_list);
    fclose(file);
    
    printf("CSR construction complete\n");
}