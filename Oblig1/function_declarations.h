#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H

void read_file_from_graph(char *filename, int *N, double ***hyperlink_matrix);
void PageRank_iterations1 (int N, double **hyperlink_matrix, double d, double epsilon,double *scores);



#endif