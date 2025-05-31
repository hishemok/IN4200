#include <stdio.h>
#include <math.h>

double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2){
    double distance = 0.0;
    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++){
            for (int i = 0; i < imax; i++) {
                float d = arr1[k][j][i] - arr2[k][j][i];
                distance += d * d ;
            }
        }
    }
    return sqrt(distance);
}