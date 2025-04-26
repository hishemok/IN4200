#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void allocate_array3D (int kmax, int jmax, int imax, double ****array);
void GS_iteration_normal (int kmax, int jmax, int imax, double ***phi);
void GS_iteration_2_chunks (int kmax, int jmax, int imax, double ***phi);
double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2);

#endif