#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static const int LINE_CUT = 10;
static const int MAX_RAND = RAND_MAX / 10000;

int main (int argc, char **argv)
{
    void sort (int arr[], int beg, int end);
    void print_array (int arr[], int n);
    void swap (int *a, int *b);

    int n = 100;

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    //int *arr = (int *) malloc (n * sizeof(int));
    int arr[n];
    int perm[n];

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() / MAX_RAND;
    }
    
    print_array(arr, n);

    sort(arr, 0, n);

    print_array(arr, n);

    //free(arr);
    return 0;
}


void swap (int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}


void sort (int arr[], int beg, int end)
{
    void swap (int *a, int *b);
    
    if (end > beg + 1) {
        int piv = arr[beg];
        int l = beg + 1;
        int r = end;

        while (l < r) {
            if (arr[l] <= piv) {
                l++;
            }
            else {
                swap (&arr[l], &arr[--r]);
            }
        }

        swap(&arr[--l], &arr[beg]);
        sort(arr, beg, l);
        sort(arr, r, end);
    }
}


void print_array (int arr[], int n)
{
    printf("-------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%4d, ", arr[i]);
        
        if ((i+1) % LINE_CUT == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
