#include <stdio.h>
#include <math.h>

typedef unsigned int uint;

int main (int argc, char **argv)
{
    double fraction_sum (int denominator, uint n);

    double fsum = fraction_sum(2, 100);

    printf("The limit = %.16f \n", fsum);

    return 0;
}


double fraction_sum (int denominator, uint n)
{
    double dem = pow(denominator, denominator);
    double fsum = 1.0;
    double tmp_dem = 1.0; 
    
    int prefix = 1;
    uint i;

    // Compute sum
    for (i = 0; i < n; i++) {
        prefix = prefix * (-1);
        tmp_dem *= dem;
        
        fsum += prefix / tmp_dem;
    }

    return fsum;
}
