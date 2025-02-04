#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

void limit();
void findMinMax(int n);
void Matrix(int n, int m);
void Array_3d(int nx, int ny, int nz);
void temperature(char *filename);
double **random_2d_array(int n, int m, bool empty);
double **smooth(double **v_new, double **v, int n,int m, double c);
void swap(int *a, int*b);
void sort(int arr[], int perm[],int beg, int end);
void print_sorted(int arr[], int perm[], int n);



int main(){
    printf("Exercise 1\n");
    printf("\nFinding limit\n");
    limit();
    printf("\nMinMax\n");
    findMinMax(30);
    printf("\nMatrix\n");
    Matrix(5,5);
    printf("\n3D Array\n");
    Array_3d(3,3,3);
    
    printf("\n");
    printf("Exercise 2\n");
    printf("\nTemperature\n");
    temperature("temperature_log.txt");
    printf("\n");
    printf("Smooth\n");

    //create 2 2D array
    int n = 5;
    int m = 5;
    double c = 1;
    double **v = random_2d_array(n,m, false);
    double **v_new = random_2d_array(n,m, true);

    double **v_smooth = smooth(v_new, v, n, m, c);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            printf("n: %.2f, o: %.2f \n", v_smooth[i][j], v[i][j]);
        }
        printf("\n");
    }        

    //free memory
    for (int i = 0; i < n; i++){
        free(v[i]);
        free(v_new[i]);
        // free(v_smooth[i]); // same reason as belowø
    }
    free(v);
    free(v_new);
    // free(v_smooth); // No need, as v_smooth is just a pointer to v_new

    printf("\n");
    printf("Exercise 3\n");    

    int arr[] = {3, 5, 1, 4, 2};
    int N = sizeof(arr)/sizeof(arr[0]);
    int perm[N];
    for (int i = 0; i < N; i++){
        perm[i] = i;
    }

    sort(arr, perm, 0, N);
    print_sorted(arr, perm, N);

    
    return 0;
}

//Exercise 1
void limit(){
    bool check = false;
    double lim;
    lim = 4.0/5.0;

    double tol;
    tol = 0.0000001;
    
    double x = 1.0;
    int i = 2;
    int sign = -1;
    while (fabs(x - lim) > tol || i < 100){
        int denominator = pow(2,i);
        x += sign * 1.0/denominator;
        sign *= -1;
        i += 2;
        if (fabs(x - lim) < tol){
            check = true;
            break;
        }
    }
    printf("After %d iterations, limit reached: %s\n", i, check ? "true" : "false");
    printf("Limit: %f, actual: %f\n", x, lim);
}

void findMinMax(int n){

    if (n < 0){
        printf("Array must have positive length\n");
    }

    //Allocate memory for array
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL){
        printf("Memory allocation failed\n");
        return;
    }

    //Fill array with random numbers
    for (int i = 0; i < n; i++){
        arr[i] = rand() % 100; //Random
        printf("%d ", arr[i]);
    }
    int min,max;
    min = arr[0];
    max = arr[0];
    for (int i = 1; i < n; i++){
        if (arr[i] < min){
            min = arr[i];
        }
        if (arr[i] > max){
            max = arr[i];
        }
    }
    printf("\nMin: %d, Max: %d\n", min, max);
    free(arr);
}   

void Matrix(int n, int m){
    //Allocate memory for matrix
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++){
        matrix[i] = (int *)malloc(m * sizeof(int));
    }
    int **matrix2 = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++){
        matrix2[i] = (int *)malloc(n * sizeof(int));
    }

    srand(time(0));
    clock_t start, end;
    double row_time, col_time;

    start = clock();

    //Fill matrix with random numbers
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            matrix[i][j] = rand() % 100; //Random
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }  

    end = clock();

    row_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\n");

    start = clock();
    for (int j = 0; j < m; j++){
        for (int i = 0; i < n; i++){
            matrix2[i][j] = rand() % 100;
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }
    end = clock();

    col_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    //Free memory
    for (int i = 0; i < n; i++){
        free(matrix[i]);
    } 
    for (int i = 0; i < m; i++){
        free(matrix2[i]);
    }
    free(matrix);
    free(matrix2);
    printf("Row time: %f, Column time: %f\n", row_time, col_time);
}

void Array_3d(int nx, int ny, int nz){
    //Allocate memory for 3D array
    int ***arr3d = (int ***)malloc (nx * sizeof(int **));
    for (int i = 0; i < ny; i++){
        arr3d[i] = (int **)malloc(ny * sizeof(int *));
        for (int j = 0; j < nz; j++){
            arr3d[i][j] = (int *)malloc(nz *sizeof(int));
        }
    }

    for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
            for (int k = 0; k <nz;k++){
                arr3d[i][j][k] = i * ny*nz + j*nz + k;
                printf("%d ", arr3d[i][j][k]);
            }
            
        }
        printf("\n");
    }

    //Free memory
    for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
            free(arr3d[i][j]);
        }
        free(arr3d[i]);
    }

}   

void temperature(char *filename){
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL){
        printf("File not found\n");
        return;
    }
    
    char line [50]; //Assuming each line has at most 50 characters
    char **timestamp = NULL; //array for timestamp
    double *temp = NULL; //array for temperature
    int n = 0; //number of lines

    int l = 0;
    while (fgets(line, sizeof(line), fptr) != NULL) {
        l++;
    }

    //Allocate memory for arrays
    timestamp = (char **)malloc(l * sizeof(char *));
    temp = (double *)malloc(l * sizeof(double));

    if (timestamp == NULL || temp == NULL){
        printf("Memory allocation failed\n");
        fclose(fptr);
        return;
    }
    


    rewind(fptr);

    int i = 0;
    while(fgets(line, sizeof(line),fptr) != NULL){
        timestamp[i] = (char *)malloc(6 * sizeof(char));
        if (timestamp[i] == NULL){
            printf("Memory allocation failed\n");
            return;
        }
        sscanf(line, "%5s %lf", timestamp[i], &temp[i]);
        i++;
    }

    fclose(fptr);

    //print results
    
    // printf("\n");
    // for (int i = 0; i < l; i++){
    //     printf("Time: %s \t Temperature: %.1f\n", timestamp[i], temp[i]);
    //     // free(timestamp[i]);
    // }


    //Find min-max and avg temperature
    double avg,min,max;
    min = temp[0];
    max = temp[0];
    char *min_time, *max_time;

    for (int i = 0; i < l; i++){
        if (temp[i] < min){
            min = temp[i];
            min_time = timestamp[i];
        }
        if (temp[i] > max){
            max = temp[i];
            max_time = timestamp[i];
        }
        avg += temp[i];

    }
    avg /= l;

    printf("\nMin: %.1f at %s\n", min, min_time);
    printf("Max: %.1f at %s\n", max, max_time);
    printf("Average: %.1f\n", avg); 


    //Free memory
    for (int i = 0; i < l; i++){
        free(timestamp[i]);
    }


    free(timestamp);
    free(temp);
}


//Exercise 2
double **random_2d_array(int n, int m, bool empty){
    double **arr = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++){
        arr[i] = (double *)malloc(m * sizeof(double));
        if (arr[i] == NULL){
            printf("Memory allocation failed\n");
            return NULL;
        }
    }

    if (empty){
        return arr;
    }
    
    srand(time(0));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            arr[i][j] = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
        }
    }

    return arr;
}


double **smooth(double **v_new, double **v, int n,int m, double c){
    int i,j;
    for (i=1; i<n-1; i++){
        for (j=1; j<m-1; j++){
            v_new[i][j]  = v[i][j] + c*(v[i+1][j] + v[i-1][j] + v[i][j+1] + v[i][j-1] - 4*v[i][j]);
        
        }
        //first and last values
        v_new[i][0] = v[i][0];
        v_new[i][m-1] = v[i][m-1];  
    }    
    //first and last rows     
    for (j=0; j<m; j++){
        v_new[0][j] = v[0][j];
        v_new[n-1][j] = v[n-1][j];
    }
    return v_new;

}

//Exercise 3
void swap(int *a, int*b){
    int t = *a; 
    *a = *b;
    *b = t;
}

void sort(int arr[], int perm[] ,int beg, int end){
    if (end > beg +1){
        int piv = arr[perm[beg]], l = beg +1, r = end;
        while (l < r){
            if (arr[perm[l]] <= piv) 
                l++;
            else swap(&perm[l], &perm[--r]);
        }
        swap(&perm[--l], &perm[beg]);
        sort(arr,perm, beg, l);
        sort(arr,perm, r, end);

    }
}

void print_sorted(int arr[], int perm[], int n){
    printf("Sorted array: ");
    for (int i = 0; i < n; i++){
        printf("%d ", arr[perm[i]]);
    }
}