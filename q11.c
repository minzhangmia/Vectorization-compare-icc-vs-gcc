#include <stdlib.h>
#include <stdio.h>


/* This attribute asks the compiler not to inline the function  *
 * --- we don't want this adding extra optimisations here.      */
__attribute__ ((noinline))
double loops_v1(double *a, double *b, double *c, const int N, const int R)
{

    for (int r=0; r<R; r++)
    {
    	#pragma omp simd
        for (int i=0; i<N; i++)
            a[i] += b[i] + c[i];
    }

    return a[N-1];
}


__attribute__ ((noinline))
double loops_v2(double *a, double *b, double *c,
                const int test_num, const int N, const int R)
{

    for (int r=0; r<R; r++)
    {
    	#pragma omp simd
        for (int i=0; i<N; i++)
            a[i] = b[i] + c[i];

        /* To prevent the compiler from taking a McClane optimisation */
        if (a[N-1] < test_num-2)
            a[0] += 1.0;
    }

    return a[N-1];
}


int main(int argc, char *argv[])
{
    const int N = 1024;     // Array length
    const int R = (int)5e7; // Number of repetitions, to get better timing

    double *a = malloc(N * sizeof(double));
    double *b = malloc(N * sizeof(double));
    double *c = malloc(N * sizeof(double));

    for (int i=0; i<N; i++)
    {
        a[i] = 0.0;
        b[i] = 0.18 * i;
        c[i] = 0.03 * i;
    }

    int loop_version = atoi(argv[1]);
    double result = 0.0;

    switch (loop_version)
    {
        case 1:
           result = loops_v1(a, b, c, N, R);
           break;
        case 2:
           result = loops_v2(a, b, c, argc, N, R);
           break;
        default:
           printf("Don't forget to pass a loop type, 1 or 2!\n");
           break;
    }


    printf("%lf\n", result);

    free(a);
    free(b);
    free(c);

    return 0;
}
