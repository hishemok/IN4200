#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"




void top_n_webpages (int N, double *scores, int n){
    if (n > N){
        printf("Warning: n is larger than the number of webpages\nPrinting all scores:\n");
        n = N;
    }

    int top_indices[n];
    double top_scores[n];
    for (int i = 0; i < n; i++){
        // start all with -1
        top_scores[i] = -1.0;
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < n; j++){
            if (scores[i] > top_scores[j]){
                //push all previous top scores back 1 index
                for (int k = n-1; k > j; k--){
                    top_scores[k] = top_scores[k-1];
                    top_indices[k] = top_indices[k-1];
                }
                //resest top score
                top_scores[j] = scores[i];
                top_indices[j] = i;
                break;
            }
        }
    }

    for (int i = 0; i < n; i++){
        printf("Rank %d: Webpage %d (Score: %.6f)\n", i+1, top_indices[i], top_scores[i]);    }

}