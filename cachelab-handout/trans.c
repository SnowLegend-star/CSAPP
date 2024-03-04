/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
    
    Created By Michael  // Note_time:2023-10-18 18:58:46
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

void trans_32x32_Mtx(int M, int N, int A[N][M], int B[M][N]){
    int i,j,k,s;
    for(i=0;i<32;i+=8)//汗流浃背了，把i+=8写成了i++；j也是这样
        for(j=0;j<32;j+=8){
            for(k=0;k<8;k++){
                int a0=A[i+k][j+0];
                int a1=A[i+k][j+1];
                int a2=A[i+k][j+2];
                int a3=A[i+k][j+3];
                int a4=A[i+k][j+4];
                int a5=A[i+k][j+5];
                int a6=A[i+k][j+6];
                int a7=A[i+k][j+7];
                B[j+k][i+0]=a0;
                B[j+k][i+1]=a1;            
                B[j+k][i+2]=a2;
                B[j+k][i+3]=a3;
                B[j+k][i+4]=a4;
                B[j+k][i+5]=a5;
                B[j+k][i+6]=a6;
                B[j+k][i+7]=a7;
                }
            for(k=0;k<8;k++)
                for(s=k+1;s<8;s++){
                    int tmp=B[j+k][i+s];
                    B[j+k][i+s]=B[j+s][i+k];
                    B[j+s][i+k]=tmp;
            }
    }

    // for(i=0;i<32;i+=8)
    //     for(j=0;j<32;j+=8){
    // //         for(k=0;k<8;k++){
    //             int a0=A[i+k][j+0];
    //             int a1=A[i+k][j+1];
    //             int a2=A[i+k][j+2];
    //             int a3=A[i+k][j+3];
    //             int a4=A[i+k][j+4];
    //             int a5=A[i+k][j+5];
    //             int a6=A[i+k][j+6];
    //             int a7=A[i+k][j+7];
    //             B[j+0][i+k]=a0;
    //             B[j+1][i+k]=a1;
    //             B[j+2][i+k]=a2;
    //             B[j+3][i+k]=a3;
    //             B[j+4][i+k]=a4;
    //             B[j+5][i+k]=a5;
    //             B[j+6][i+k]=a6;
    //             B[j+7][i+k]=a7;
    //         }
    // }


    // for (int i = 0; i < N; i += 8)
    //     for (int j = 0; j < M; j += 8)
    //         for (int k = 0; k < 8; k++)
    //             for (int s = 0; s < 8; s++)
    //                 B[j + s][i + k] = A[i + k][j + s];

}

void trans_64x64_Mtx(int M, int N, int A[N][M], int B[M][N]){
    int i,j,k;
    for(i=0;i<64;i+=4)
        for(j=0;j<64;j+=4){
            for(k=0;k<4;k++){
                int a0=A[i+k][j+0];
                int a1=A[i+k][j+1];
                int a2=A[i+k][j+2];
                int a3=A[i+k][j+3];
                // int a4=A[i+k][j+4];
                // int a5=A[i+k][j+5];
                // int a6=A[i+k][j+6];
                // int a7=A[i+k][j+7];
                B[j+0][i+k]=a0;
                B[j+1][i+k]=a1;
                B[j+2][i+k]=a2;
                B[j+3][i+k]=a3;
                // B[j+4][i+k]=a4;
                // B[j+5][i+k]=a5;
                // B[j+6][i+k]=a6;
                // B[j+7][i+k]=a7;
            }

    }

}

void trans_61x67_Mtx(int M,int N,int A[N][M],int B[M][N]){
    for (int i = 0; i < N; i += 16)
        for (int j = 0; j < M; j += 16)
            for (int k = i; k < i + 16 && k < N; k++)
                for (int s = j; s < j + 16 && s < M; s++)
                    B[s][k] = A[k][s];
    // int i,j,k;
    // for(i=0;i<67;i+=8)
    //     for(j=0;j<61;j+=8){
    //         for(k=0;i+k<67&&k<8;k++){
    //             int a0=A[i+k][j+0];
    //             int a1=A[i+k][j+1];
    //             int a2=A[i+k][j+2];
    //             int a3=A[i+k][j+3];
    //             int a4=A[i+k][j+4];
    //             int a5=A[i+k][j+5];
    //             int a6=A[i+k][j+6];
    //             int a7=A[i+k][j+7];
    //             B[j+0][i+k]=a0;
    //             B[j+1][i+k]=a1;
    //             B[j+2][i+k]=a2;
    //             B[j+3][i+k]=a3;
    //             B[j+4][i+k]=a4;
    //             B[j+5][i+k]=a5;
    //             B[j+6][i+k]=a6;
    //             B[j+7][i+k]=a7;
    //         }
    // }
}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(N==32&&M==32)
        trans_32x32_Mtx(M,N,A,B);
    if(N==64&&M==64)
        trans_64x64_Mtx(M,N,A,B);
    if(N==67&&M==61)
        trans_61x67_Mtx(M,N,A,B);
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







