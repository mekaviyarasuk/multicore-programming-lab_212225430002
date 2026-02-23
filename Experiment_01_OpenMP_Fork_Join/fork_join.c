#include <stdio.h> 
#include <omp.h> 
 
int main(void) { 
    printf("Before: total thread number is %d\n", omp_get_num_threads()); 
    #pragma omp parallel 
    { 
        printf("Thread id is %d\n", omp_get_thread_num()); 
    } 
    printf("After: total thread number is %d\n", omp_get_num_threads()); 
    return 0; 
} 
