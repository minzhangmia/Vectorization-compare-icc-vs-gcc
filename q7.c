#include <stdlib.h>
#include <stdio.h>


/* This attribute asks the compiler not to inline the function  *
 * --- we don't want this adding extra optimisations here.      */
__attribute__ ((noinline))
float loops_v1(float *a, float *b, float *c, const int N, const int R)
{

    for (int r=0; r<R; r++)
    {
    	#pragma omp simd
        for (int i=0; i<N; i++)
            a[i] += 0.027f * b[i] + c[i];
    }

    return a[N-1];
}


__attribute__ ((noinline))
float loops_v2(float *a, float *b, float *c,
                const int test_num, const int N, const int R)
{

    for (int r=0; r<R; r++)
    {
    	#pragma omp simd
        for (int i=0; i<N; i++)
            a[i] = 0.027f * b[i] + c[i];

        /* To prevent the compiler from taking a McClane optimisation */
        if (a[N-1] < test_num-2)
            a[0] += 1.0f;
    }

    return a[N-1];
}


int main(int argc, char *argv[])
{
    const int N = 1024;     // Array length
    const int R = (int)5e7; // Number of repetitions, to get better timing

    float *a = malloc(N * sizeof(float));
    float *b = malloc(N * sizeof(float));
    float *c = malloc(N * sizeof(float));

    for (int i=0; i<N; i++)
    {
        a[i] = 0.0f;
        b[i] = 0.18f * i;
        c[i] = 0.03f * i;
    }

    int loop_version = atoi(argv[1]);
    float result = 0.0f;

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


    printf("%f\n", result);

    free(a);
    free(b);
    free(c);

    return 0;
}
