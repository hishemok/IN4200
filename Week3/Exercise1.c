#include <stdio.h>
#include <time.h>



double numerical_integration(double x_min, double x_max, int slices){
    double delta_x = (x_max - x_min)/slices;
    double x, sum = 0.0;
    for (int i = 0; i < slices; i++){
        x = x_min + (i+0.5)*delta_x;
        sum = sum + 4.0/(1.0+x*x);
    }
    return sum*delta_x;
}


int main(){
    printf("Exercise 1\n");

    double x_min = 0.0;
    double x_max = 1.0;
    // int slices = 100;
    for (int slices = 10; slices <= 1000; slices *= 10){
        double pi = numerical_integration(x_min, x_max, slices);
        printf("Slices: %d Pi: %f\n",slices ,pi);
    }


    int num_divisions = 10000000;
    clock_t start, end;
    double sum = 0.0;
    start = clock();
    for (int i = 0; i < num_divisions; i++){
        
        sum += numerical_integration(x_min, x_max, 100);
    }
    end = clock();

    double time = ((double) (end - start)) / CLOCKS_PER_SEC;
    double time_per_division = time/num_divisions;
    printf("Time per division: %f micro seconds\n", time_per_division*1e6);

    double cpu_clock_freq = 800e6;
    double latency = time_per_division*cpu_clock_freq;
    printf("Latency: %f\n", latency);
    

    return 0;
}