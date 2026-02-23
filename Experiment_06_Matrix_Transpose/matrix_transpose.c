#include <stdio.h> 
#include <omp.h> 
#define N 3 
 
int main() { 
    int A[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; 
    int B[N][N]; 
    int i, j; 
    printf("Original Matrix A:\n"); 
    for (i = 0; i < N; i++) { 
        for (j = 0; j < N; j++) { 
            printf("%d ", A[i][j]); 
        } 
        printf("\n"); 
    } 
    #pragma omp parallel for private(j) 
    for (i = 0; i < N; i++) { 
        for (j = 0; j < N; j++) { 
            B[j][i] = A[i][j]; 
        } 
    } 
    printf("\nTransposed Matrix B:\n"); 
    for (i = 0; i < N; i++) { 
        for (j = 0; j < N; j++) { 
            printf("%d ", B[i][j]); 
        } 
        printf("\n"); 
    } 
    return 0; 
} 
