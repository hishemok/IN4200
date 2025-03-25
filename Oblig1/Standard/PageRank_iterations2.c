#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void find_dangling_pages_csr(int N, int *row_ptr, int *col_idx, double *val, int *dangling_indices, int *is_dangling) {
    // Initialize all nodes as potentially dangling
    for (int i = 0; i < N; i++) {
        dangling_indices[i] = 1;
    }
    
    // Mark nodes with outgoing edges as non-dangling
    for (int i = 0; i < N; i++) {
        for (int j = row_ptr[i]; j < row_ptr[i+1]; j++) {
            int source_node = col_idx[j];
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
    printf("\nPageRank_iterations2 (Serial CSR Version)\n");
    
    double *xprev = malloc(N * sizeof(double));
    double *xcurr = malloc(N * sizeof(double));
    
    if (xprev == NULL || xcurr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Initialize with uniform probability
    for (int i = 0; i < N; i++) {
        xprev[i] = 1.0 / N;
    }

    int iterations = 0;
    double diff = 2.0;
    int *dangling_indices = malloc(N * sizeof(int));
    
    while (diff > epsilon && iterations < 10000) {
        // Find dangling pages
        int is_dangling = 0;
        find_dangling_pages_csr(N, row_ptr, col_idx, val, dangling_indices, &is_dangling);
        
        // Calculate dangling sum
        double W_prev = 0.0;
        if (is_dangling) {
            for (int i = 0; i < N; i++) {
                if (dangling_indices[i] == 1) {
                    W_prev += xprev[i];
                }
            }
        }
        
        // Teleportation + dangling contribution
        double teleport_and_dangling = (1.0 - d) / N + d * W_prev / N;
        
        // Initialize current scores
        for (int i = 0; i < N; i++) {
            xcurr[i] = teleport_and_dangling;
        }
        
        // Process links
        for (int i = 0; i < N; i++) {
            for (int j = row_ptr[i]; j < row_ptr[i+1]; j++) {
                int source = col_idx[j];
                double link_val = val[j];
                xcurr[i] += d * link_val * xprev[source];
            }
        }
        
        // Calculate convergence
        diff = 0.0;
        for (int i = 0; i < N; i++) {
            double curr_diff = fabs(xcurr[i] - xprev[i]);
            if (curr_diff > diff) {
                diff = curr_diff;
            }
            xprev[i] = xcurr[i]; // Update for next iteration
        }
        
        iterations++;
    }
    
    // Copy results
    for (int i = 0; i < N; i++) {
        scores[i] = xprev[i];
    }
    
    printf("Converged after %d iterations\n", iterations);
    
    free(xprev);
    free(xcurr);
    free(dangling_indices);
}