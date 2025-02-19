#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>




void firstfunc(double **a, double **b, double *c, double *d, double *e, int n){
    /*
    a: nxn matrix (Empty)
    b: nxn matrix (pre-filled)
    c: n long array (Empty)
    d: n long array (pre-filled)
    e: n long array (pre-filled)
    n: size of the arrays
    */
    for (int i = 0; i<n; i++){
        c[i] = exp(1.0*1/n) + sin(3.1415926*i/n);
        for (int j=0; j<n; j++){
            a[j][i] = b[j][i] + d[j]*e[i];
        }
        printf("c[%d] = %f\n", i, c[i]);
    }

 
}


void fasterfunc(double **a, double **b, double *c, double *d, double *e, int n){
    /*
    a: nxn matrix (Empty)
    b: nxn matrix (pre-filled)
    c: n long array (Empty)
    d: n long array (pre-filled)
    e: n long array (pre-filled)
    n: size of the arrays
    */
   double exp_val = exp(1.0*1/n);
    for (int i = 0; i<n; i++){
        double angle = 3.1415926*i/n;
        c[i] = exp_val+ sin(angle);
        for (int j=0; j<n; j++){
            a[i][j] = b[i][j] + d[j]*e[i]; //Access row-major order
        }
        printf("c[%d] = %f\n", i, c[i]);
    }   
}


int main(){

    int n = 1000;
    double **a = (double **)malloc(n * sizeof(double *));
    double **b = (double **)malloc(n * sizeof(double *));
    double *c = (double *)malloc(n * sizeof(double));
    double *d = (double *)malloc(n * sizeof(double));
    double *e = (double *)malloc(n * sizeof(double));

    for (int i = 0; i < n; i++){
        a[i] = (double *)malloc(n * sizeof(double));
        b[i] = (double *)malloc(n * sizeof(double));
    }

    //pre-fill b, d, e with random values
    for (int i = 0; i<n; i++){
        d[i] = rand() % (20) *i/n;
        e[i] = rand() % (20) *i/n;
        printf("d[%d] = %f\n", i, d[i]);
        printf("e[%d] = %f\n", i, e[i]);
        for (int j=0; j<n; j++){
            b[j][i] = rand() % (10)*i/n * exp(rand() % (10) *j/n);
        }
    }

    //time the functions
    clock_t start, end;
    double cpu_time_used0, cpu_time_used;
    start = clock();
    firstfunc(a, b, c, d, e, n);
    end = clock();
    cpu_time_used0 = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by firstfunc: %f\n", cpu_time_used0);

    start = clock();
    fasterfunc(a, b, c, d, e, n);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by fasterfunc: %f\n", cpu_time_used);

    printf("Speedup: %f\n", cpu_time_used0/cpu_time_used);


    for (int i = 0; i < n; i++){
        free(a[i]);
        free(b[i]);
    }
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);


}