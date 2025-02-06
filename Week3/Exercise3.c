#include <stdio.h>
#include <time.h>
#include <math.h>


double custom_pow(double x, int y){
    double result = 1;
    for (int i = 0; i < y; i++){
        result *= x;
    }
    return result;
}



int main(){
    
    int power = 100;
    double x = 1.001;

    clock_t start, end;
    start = clock();
    double result = custom_pow(x, power);
    end = clock();
    double time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Result: %f, Time: %f\n", result, time);

    start = clock();
    result = pow(x, power);
    end = clock();
    time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Result: %f, Time: %f\n", result, time);


    return 0;
}