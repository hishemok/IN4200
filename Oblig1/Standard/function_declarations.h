#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H

void read_graph_from_file1(char *filename, int *N, double ***hyperlink_matrix);
void PageRank_iterations1 (int N, double **hyperlink_matrix, double d, double epsilon,double *scores);
void find_dangling_pages (int N, double **hyperlink_matrix, int *dangling_indices, int is_dangling);
void top_n_webpages (int N, double *scores, int n);
void read_graph_from_file2 (char *filename, int *N, int **row_ptr, int **col_idx, double **val);
void PageRank_iterations2 (int N, int *row_ptr, int *col_idx, double *val, double
d, double epsilon, double *scores);
void find_dangling_pages_csr(int N, int *row_ptr, int *col_idx, double *val, int *dangling_indices, int *is_dangling);


#endif