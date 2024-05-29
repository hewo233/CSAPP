/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    //int a1;
    int a1, a2, a3, a4, a5, a6, a7, a8;
    int block_size = 8;
    for(int i = 0; i < N; i += block_size)
    {
        for(int j = 0;j < M; j += block_size)
        {
            for(int ki = i; ki < i + block_size && ki < N; ki++)
            {
                a1 = A[ki][j];
                a2 = A[ki][j + 1];
                a3 = A[ki][j + 2];
                a4 = A[ki][j + 3];
                a5 = A[ki][j + 4];
                a6 = A[ki][j + 5];
                a7 = A[ki][j + 6];
                a8 = A[ki][j + 7];

                B[j][ki] = a1;
                B[j + 1][ki] = a2;
                B[j + 2][ki] = a3;
                B[j + 3][ki] = a4;
                B[j + 4][ki] = a5;
                B[j + 5][ki] = a6;
                B[j + 6][ki] = a7;
                B[j + 7][ki] = a8;
            }
        }
    }
}

char transpose_submit_desc2[] = "Transpose submission2";
void transpose_submit2(int M, int N, int A[N][M], int B[M][N])
{

    int a1,a2,a3,a4,a5,a6,a7,a8;
    int block_size = 8;
    for(int i = 0;i < N;i += block_size)
    {
        for(int j = 0;j < M;j += block_size)
        {
            for(int ki = i; ki < i + 4 && ki < N; ki++)
            {
                a1 = A[ki][j], a2 = A[ki][j + 1], a3 = A[ki][j + 2], a4 = A[ki][j + 3];
                a5 = A[ki][j + 4], a6 = A[ki][j + 5], a7 = A[ki][j + 6], a8 = A[ki][j + 7];

                B[j][ki] = a1, B[j + 1][ki] = a2, B[j + 2][ki] = a3, B[j + 3][ki] = a4;
                B[j][ki + 4] = a5, B[j + 1][ki + 4] = a6, B[j + 2][ki + 4] = a7, B[j + 3][ki + 4] = a8;
            }

            for(int kj = j; kj < j + 4 && kj < M; kj++)
            {
                a1 = A[i + 4][kj], a2 = A[i + 5][kj], a3 = A[i + 6][kj], a4 = A[i + 7][kj];
                a5 = B[kj][i + 4], a6 = B[kj][i + 5], a7 = B[kj][i + 6], a8 = B[kj][i + 7];

                B[kj][i + 4] = a1, B[kj][i + 5] = a2, B[kj][i + 6] = a3, B[kj][i + 7] = a4;
                B[kj + 4][i] = a5, B[kj + 4][i + 1] = a6, B[kj + 4][i + 2] = a7, B[kj + 4][i + 3] = a8;
            }

            for(int ki = i + 4; ki < i + 8 && ki < N; ki++)
            {
                a1 = A[ki][j + 4], a2 = A[ki][j + 5], a3 = A[ki][j + 6], a4 = A[ki][j + 7];

                B[j + 4][ki] = a1, B[j + 5][ki] = a2, B[j + 6][ki] = a3, B[j + 7][ki] = a4;
            }
        }
    }
}

char transpose_submit_desc3[] = "Transpose submission3";
void transpose_submit3(int M, int N, int A[N][M], int B[M][N])
{
    //int a1;
    int block_size = 17;
    for(int i = 0; i < N; i += block_size)
    {
        for(int j = 0;j < M; j += block_size)
        {
            for(int ki = i; ki < i + block_size && ki < N; ki++)
            {
                for(int kj = j; kj < j + block_size && kj < M; kj++)
                {
                    B[kj][ki] = A[ki][kj];
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    registerTransFunction(transpose_submit2, transpose_submit_desc2);

    registerTransFunction(transpose_submit3, transpose_submit_desc3);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

