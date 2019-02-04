#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const int N = 4;


int main (int argc, char **argv)
{
    double numerical_integration (double x_min, double x_max, int slices);

    double x_min = 0.0;
    double x_max = 1.0;

    int slices_array[4] = {10, 100, 1000, 10000};
    
    for (int i = 0; i < N; i++) {
        double sum_num_int = numerical_integration(x_min, x_max, 
                                                   slices_array[i]);
        printf("abs(integrate %d slices - anylytical value) = %.4g\n",
               slices_array[i], fabs(sum_num_int - M_PI));
    }

    return 0;
}


double numerical_integration (double x_min, double x_max, int slices)
{
    double delta_x = (x_max - x_min) / slices;
    double x, sum = 0.0;

    for (int i = 0; i < slices; i++) {
        x = x_min + (i + 0.5) * delta_x;
        sum = sum + 4.0 / (1.0 + x*x);
    }

    return sum * delta_x;
}
