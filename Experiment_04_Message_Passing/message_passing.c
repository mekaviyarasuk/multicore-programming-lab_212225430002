```c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int nthreads, tid;
    
    // Beginning of parallel region
    // The 'private(tid)' clause ensures each thread has its own 'tid' variable
    #pragma omp parallel private(tid)
    {
        // Obtain thread id
        tid = omp_get_thread_num();
        printf("Hello World from thread = %d\n", tid);
        
        // Only the master thread (thread 0) executes this block
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Total number of threads in this region = %d\n", nthreads);
        }
    } 
    // Ending of parallel region - Implicit Barrier (Join)
    
    return 0;
}
