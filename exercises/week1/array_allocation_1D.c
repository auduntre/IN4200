#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef unsigned int uint;


int main (int argc, char **argv)
{
    int max, min;
    uint i, n;

    if (argc > 1) {
        n = atoi(argv[1]);
    }
    else {
        printf("n must be provided! \n");
        return 1;
    }
    
    int *randomints = (int *) malloc (n * sizeof(int)); 

    srand(time(NULL));
    
    // Allocate array values
    for (i = 0; i < n; i++) {
        randomints[i] = rand();
    }

    // Find max and min
    max = randomints[0];
    min = max;

    for (i = 1; i < n; i++) {
        if (randomints[i] > max) {
            max = randomints[i];
        }
        else {
            if (randomints[i] < min) {
                min = randomints[i];
            }
        }
    }

    free(randomints);

    printf("Maximum value = %d, Minimum value = %d \n", max, min);
    return 0;
}
