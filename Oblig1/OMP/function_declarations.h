#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H

void read_graph_from_file2 (char *filename, int *N, int **row_ptr, int **col_idx, double **val);
void PageRank_iterations2 (int N, int *row_ptr, int *col_idx, double *val, double
d, double epsilon, double *scores);
void find_dangling_pages_csr(int N, int *row_ptr, int *col_idx, double *val, int *dangling_indices, int *is_dangling);
void top_n_webpages (int N, double *scores, int n);

#endif