#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

void foo(int N, double **mat, double **s,int *v){
    int i,j;
    double val;
    for (j = 0; j < N; j++){
        for ( i= 0; i < N; i++){
            val = 1.0*(v[j]%256);
            mat[i][j] = s[i][j]*(sin(val)*sin(val) - cos(val)*cos(val));
        }
    }
}

void optimal_foo(int N, double **mat, double **s,int *v){
    int i,j;
    double val,sin_val,cos_val,sinsin,coscos,input;
    for (j = 0; j < N; j++){
        val = v[j]%256;
        sin_val = sin(val);
        cos_val = cos(val);
        sinsin = sin_val*sin_val;
        coscos = cos_val*cos_val;
        input = sinsin - coscos;

        for ( i= 0; i < N; i++){
            mat[i][j] = s[i][j]*input;
        }
    }
}


int main(){
    int N = 1000;
    double **mat = (double **)malloc(N * sizeof(double *));
    double **s = (double **)malloc(N * sizeof(double *));
    int *v = (int *)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++){
        mat[i] = (double *)malloc(N * sizeof(double));
        s[i] = (double *)malloc(N * sizeof(double));
        v[i] = rand();
        for (int j = 0; j < N; j++){
            s[i][j] = (double)rand() / RAND_MAX;
        }
    }

    clock_t start = clock();
    foo(N, mat, s, v);
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by foo: %f\n", cpu_time_used);

    start = clock();
    optimal_foo(N, mat, s, v);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by optimal_foo: %f\n", cpu_time_used);


    for (int i = 0; i < N; i++){
        free(mat[i]);
        free(s[i]);
    }
    free(mat);
    free(s);
    free(v);
    



    return 0;
}