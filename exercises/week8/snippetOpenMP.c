#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main (int argc, char **argv)
{
    int total_sum = 0;
    int i;

    #pragma omp parallel default(shared) reduction(+:total_sum)
    {
        int my_id = omp_get_thread_num();
        int my_sum = 0;

        #pragma omp for schedule(static, 10)
        for (i = 1; i <= 100; i++)
            my_sum += i;

        printf("From threads No.%d: my_sum = %d\n", my_id, my_sum);
        total_sum += my_sum;
    }

    printf("Total sum = %d\n", total_sum);
    return 0;
}
