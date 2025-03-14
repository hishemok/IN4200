#include <stdio.h>
#include <stdlib.h>
#include "function_declarations.h"
#include <omp.h>

int main(){
    int N;
    int *row_ptr, *col_idx;
    double *val;

    read_graph_from_file2("small_webpage_graph.txt", &N, &row_ptr, &col_idx, &val);



    free(row_ptr);
    free(col_idx);
    free(val);

    return 0;
}