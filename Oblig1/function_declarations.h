#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H

void read_file_from_graph(char *filename, int *N, double ***hyperlink_matrix);
void PageRank_iterations1 (int N, double **hyperlink_matrix, double d, double epsilon,double *scores);
void find_dangling_pages (int N, double **hyperlink_matrix, int *dangling_indices, int is_dangling);
void top_n_webpages (int N, double *scores, int n);

#endif