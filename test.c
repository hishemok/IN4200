#include <stdio.h>

double func1(int a, int b, int c);
double func2(double a, double b, double c);


int main() {
    printf("Hello, World!\n");
    int a,b,c;
    a = 2;
    b = 7;
    c = 3;
    double val;
    val = func1(a, b, c);
    printf("(%d - %d)/%d = %f \n", a, b, c, val);

    double val2;
    val2 = func2(a, b, c);
    printf("(%d - %d)/%d = %f \n", a, b, c, val2);

    printf("Variables adress: \n");
    printf("a: %p\n", &a);
    printf("b: %p\n", &b);
    printf("c: %p\n", &c);
    printf("val: %p\n", &val);
    printf("val2: %p\n", &val2);

    
    return 0;
}

double func1(int a, int b, int c){
    double d = (double)(a-b)/c;
    return d;
}

double func2(double a, double b, double c){
    double d = (a-b)/c;
    return d;
}