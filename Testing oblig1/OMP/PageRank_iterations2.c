#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>'

void find_dangling_pages_csr(int N, int *row_ptr, int *col_idx, double *val, int *dangling_indices, int *is_dangling) {
    // Initialize all nodes as potentially dangling
    for (int i = 0; i < N; i++) {
        dangling_indices[i] = 1;
    }
    
    // For each node, check if it has outgoing edges
    // In CSR format, we're looking at which columns (source nodes) appear in the matrix
    for (int i = 0; i < N; i++) {
        for (int j = row_ptr[i]; j < row_ptr[i+1]; j++) {
            int source_node = col_idx[j];
            // If a node is a source for any edge, it's not dangling
            dangling_indices[source_node] = 0;
        }
    }
    
    // Check if any dangling page exists
    *is_dangling = 0;
    for (int i = 0; i < N; i++) {
        if (dangling_indices[i] == 1) {
            *is_dangling = 1;
            break;
        }
    }
}

void PageRank_iterations2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores) {
    printf("\n\nPageRank_iterations2 with CSR and OpenMP\n");
    
    // Allocate memory for PageRank vectors
    double *xprev = malloc(N * sizeof(double));
    double *xcurr = malloc(N * sizeof(double));
    
    if (xprev == NULL || xcurr == NULL) {
        printf("Error: Memory allocation failed for PageRank vectors\n");
        exit(1);
    }
    
    // Initialize PageRank vector with uniform values
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        xprev[i] = 1.0 / N;
    }
    
    int iterations = 0;
    double diff = 2.0;
    int *dangling_indices = malloc(N * sizeof(int));
    
    // Main PageRank iteration loop
    while (diff > epsilon && iterations < 10000) {
        // Find dangling pages
        int is_dangling = 0;
        find_dangling_pages_csr(N, row_ptr, col_idx, val, dangling_indices, &is_dangling);
        
        // Calculate sum of PageRank values for dangling nodes
        double W_prev = 0.0;
        if (is_dangling == 1) {
            #pragma omp parallel for reduction(+:W_prev)
            for (int i = 0; i < N; i++) {
                if (dangling_indices[i] == 1) {
                    W_prev += xprev[i];
                }
            }
        }
        
        // Calculate the teleportation factor + dangling nodes contribution
        double teleport_and_dangling = (1.0 - d) / N + d * W_prev / N;
        
        // Initialize current PageRank vector with teleportation + dangling contribution
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            xcurr[i] = teleport_and_dangling;
        }
        
        // Add contribution from matrix (link structure)
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            // Process incoming links to node i
            for (int j = row_ptr[i]; j < row_ptr[i+1]; j++) {
                int source = col_idx[j];
                double link_val = val[j];
                xcurr[i] += d * link_val * xprev[source];
            }
        }
        
        // Calculate convergence metric (maximum difference)
        diff = 0.0;
        #pragma omp parallel
        {
            double local_diff = 0.0;
            
            #pragma omp for
            for (int i = 0; i < N; i++) {
                double curr_diff = fabs(xcurr[i] - xprev[i]);
                if (curr_diff > local_diff) {
                    local_diff = curr_diff;
                }
                xprev[i] = xcurr[i]; // Update for next iteration
            }
            
            #pragma omp critical
            {
                if (local_diff > diff) {
                    diff = local_diff;
                }
            }
        }
        
        iterations++;
    }
    
    // Copy final PageRank values to output array
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        scores[i] = xprev[i];
    }
    
    printf("Iterations: %d\n", iterations);
    
    // Clean up
    free(xprev);
    free(xcurr);
    free(dangling_indices);
}