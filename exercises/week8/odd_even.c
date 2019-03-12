#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>


int main (int argc, char **argv)
{
    void parallel_odd_even(int n, float *a);
    void serial_odd_even(int n, float *a);
    void make_random_array (int n, float *a);
    void is_sorted (int n, float *a);
    
    int N = 100000;
    int i = 0;

    float *float_array = (float *) malloc (N * sizeof(float));

    double start, end, elapsed;

    srand(time(NULL));
    make_random_array(N, float_array);
    is_sorted(N, float_array);
    
    printf("Sorting array ... ");
    
    start = omp_get_wtime();
    serial_odd_even(N, float_array);
    end = omp_get_wtime();
    
    is_sorted(N, float_array);
    printf("Wall time for serial code is %f sec\n", end - start);
    printf("Parallelized code: \n");
    
    make_random_array(N, float_array);
    is_sorted(N, float_array);

    printf("Sorting array ... ");

    start = omp_get_wtime();
    parallel_odd_even(N, float_array);
    end = omp_get_wtime();
    
    is_sorted(N, float_array);
    printf("Wall time for serial code is %f sec\n", end - start);

    free(float_array);
    return 0;
}


void serial_odd_even(int n, float *a)
{
    void compare_exchange(float *a, int small, int big, bool *done); 
    void is_sorted (int n, float *a);
    
    bool done;
    int i, j;

    for (i = 1; i <= n; i++) {
        done = true;
        
        if (i % 2 == 1) { 
            for (j = 0; j <= n/2 - 1; j++) {
                compare_exchange(a, 2*j+1, 2*j+2, &done);
            }
        } 
        else { 
            for (j = 0; j <= n/2 - 1; j++) {
                compare_exchange(a, 2*j, 2*j+1, &done);
            }
        }

        if (done) {
            break;
        }
    }
}


void parallel_odd_even(int n, float *a)
{
    void compare_exchange(float *a, int small, int big, bool *done); 
    void is_sorted (int n, float *a);
    
    bool done;
    int i, j;

    #pragma omp parallel for private(i, j) schedule(static, n/8)
    for (i = 1; i <= n; i++) {
        done = true;
        
        if (i % 2 == 1) { 
            for (j = 0; j <= n/2 - 1; j++) {
                compare_exchange(a, 2*j+1, 2*j+2, &done);
            }
        } 
        else { 
            for (j = 0; j <= n/2 - 1; j++) {
                compare_exchange(a, 2*j, 2*j+1, &done);
            }
        }

        if (!done) {
            continue;
        }
    }
}


void compare_exchange(float *a, int small, int big, bool *done) 
{
    float tmp = a[small];
    
    if (a[big] < a[small]) {
        a[small] = a[big];
        a[big] = tmp;

        if (*done) {
            *done = false;
        }
    }
}


void is_sorted (int n, float *a)
{
    for (int i = 0; i < n-1; i++) {
        if (a[i+1] < a[i]) {
            printf("Array is not sorted.\n");
            return;
        }
    }
    printf("Array is sorted.\n");
}


void make_random_array (int n, float *a)
{
    float fRAND_MAX = (float) RAND_MAX;
    
    for (int i = 0; i < n; i++) {
        a[i] = ((float) rand()) / fRAND_MAX;
    }
}
