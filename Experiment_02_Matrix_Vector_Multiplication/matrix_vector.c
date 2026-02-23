#include <stdio.h> 
#include <omp.h> 
 
int main() { 
    float A[2][2] = {{1, 2}, {3, 4}}; 
    float x[] = {8, 10}; 
    float b[2]; 
    int i, j; 
    #pragma omp parallel for private(j) 
    for (i = 0; i < 2; i++) { 
        b[i] = 0; 
        for (j = 0; j < 2; j++) { 
            b[i] = b[i] + A[i][j] * x[j]; 
        } 
    } 
    for (i = 0; i < 2; i++) { 
        printf("b[%d] = %f\n", i, b[i]); 
    } 
    return 0; 
} 
