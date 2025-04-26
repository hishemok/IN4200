#include <stdlib.h>

void free_array3D(int kmax, int jmax, double ***array) {
    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            free(array[k][j]);
        }
        free(array[k]);
    }
    free(array);
}