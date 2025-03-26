#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "function_declarations.h"

void top_n_webpages_omp(int N, double *scores, int n) {
    if (n > N) {
        printf("Error: n is larger than the number of webpages\n");
        exit(1);
    }

    printf("Top %d webpages (Parallel Version):\n", n);
    
    // Allocate thread-local top lists
    int num_threads = omp_get_max_threads();
    double **thread_top_scores = malloc(num_threads * sizeof(double *));
    int **thread_top_indices = malloc(num_threads * sizeof(int *));
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        thread_top_scores[tid] = malloc(n * sizeof(double));
        thread_top_indices[tid] = malloc(n * sizeof(int));
        
        // Initialize local top list
        for (int i = 0; i < n; i++) {
            thread_top_scores[tid][i] = -1.0;
        }
        
        // Each thread processes its portion of the array
        #pragma omp for
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < n; j++) {
                if (scores[i] > thread_top_scores[tid][j]) {
                    // Shift elements down
                    for (int k = n-1; k > j; k--) {
                        thread_top_scores[tid][k] = thread_top_scores[tid][k-1];
                        thread_top_indices[tid][k] = thread_top_indices[tid][k-1];
                    }
                    // Insert new element
                    thread_top_scores[tid][j] = scores[i];
                    thread_top_indices[tid][j] = i;
                    break;
                }
            }
        }
    }

    // Merge thread-local top lists
    double *final_top_scores = malloc(n * sizeof(double));
    int *final_top_indices = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        final_top_scores[i] = -1.0;
    }

    for (int t = 0; t < num_threads; t++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (thread_top_scores[t][i] > final_top_scores[j]) {
                    // Shift elements down
                    for (int k = n-1; k > j; k--) {
                        final_top_scores[k] = final_top_scores[k-1];
                        final_top_indices[k] = final_top_indices[k-1];
                    }
                    // Insert new element
                    final_top_scores[j] = thread_top_scores[t][i];
                    final_top_indices[j] = thread_top_indices[t][i];
                    break;
                }
            }
        }
    }

    // Print results
    for (int i = 0; i < n; i++) {
        printf("%d. Webpage %d: %.6f\n", i+1, final_top_indices[i], final_top_scores[i]);
    }

    // Cleanup
    for (int t = 0; t < num_threads; t++) {
        free(thread_top_scores[t]);
        free(thread_top_indices[t]);
    }
    free(thread_top_scores);
    free(thread_top_indices);
    free(final_top_scores);
    free(final_top_indices);
}