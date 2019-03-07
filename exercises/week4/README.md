# Excercise Set 4

## Measuring the memory bandwidth

The program `memband.c` makes use of the loop:

```C
for (i=0; i<N, i++)
    A[i] = s*B[i];
```
to measure the realistically achivable memory bandwidht of the CPU.
