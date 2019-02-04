#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main (int argc, char **argv)
{
    void print_pow(double ans, clock_t start, clock_t end);
    double pow100 (double x);
    double pow_test (double x);
    double pow_fastest (double x);
    
    clock_t start, end;

    double x = 2.23;
    double ans;

    if (argc > 1) {
        x = atof(argv[1]);
    }

    start = clock();
    ans = pow(x, 100.0);
    end = clock();

    print_pow(ans, start, end);

    start = clock();
    ans = pow100(x);
    end = clock();

    print_pow(ans, start, end);

    start = clock();
    ans = pow_test(x);
    end = clock();
    
    print_pow(ans, start, end);
    
    start = clock();
    ans = pow_fastest(x);
    end = clock();
    
    print_pow(ans, start, end);
    
    return 0;
}


void print_pow(double ans, clock_t start, clock_t end)
{
    double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The method used %.16f secs and gave %g as answear\n", elapsed, ans);
}


double pow100 (double x)
{
    double sum1 = x * x;
    double sum2 = x;

    for (int i = 0; i < 5; i++) {
        sum1 *= sum1;
        sum2 *= sum2;
    }

    return sum1 * sum2 * x * x * x * x;
}


double pow_test (double x)
{
    double sum = 1;

    for (int i = 0; i < 100; i++) {
        sum *= x;
    }

    return sum;
}


double pow_fastest (double x)
{
    double sum = x * x * x * x * x;
    sum = sum * sum * sum * sum * sum;
    sum = sum * sum;
    sum = sum * sum;

    return sum;
}
