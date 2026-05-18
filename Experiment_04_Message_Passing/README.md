# Experiment 4: Parallel Regions in OpenMP

This document demonstrates the creation of parallel regions using OpenMP, where a single block of code is executed simultaneously by multiple threads.

---

## 🎯 Aim
To write and execute a C program that demonstrates the use of parallel regions, retrieving individual thread IDs, and determining the total number of threads active in the region.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialize Variables:** Define integers for the total number of threads (`nthreads`) and the specific thread ID (`tid`).
3.  **Parallel Fork:** Enter the parallel region using the `#pragma omp parallel` directive. 
    * *Note: `tid` is declared as `private` so each thread has its own unique instance of the variable.*
4.  **Identify Threads:** Each thread calls `omp_get_thread_num()` to find its unique ID.
5.  **Execution:** Each thread prints a "Hello World" message along with its ID.
6.  **Master Thread Task:** Use a conditional check (`if tid == 0`) to allow only the master thread to call `omp_get_num_threads()` and print the total thread count.
7.  **Join:** Exit the parallel region.
8.  **Stop.**

---

## 💻 Source Code

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
```

---

## 📊 Expected Output

> **Note:** Because threads execute asynchronously, the "Hello World" messages from threads 1, 2, and 3 may appear in a different order every time the program is run.

```text
Hello World from thread = 0
Total number of threads in this region = 4
Hello World from thread = 2
Hello World from thread = 1
Hello World from thread = 3
```

---

## 🛠 Compilation Command

To compile this on a system with GCC, use the following command to link the OpenMP library:

```bash
gcc -fopenmp experiment4.c -o experiment4
```

---

## ✅ Result
The program was successfully implemented, demonstrating how OpenMP forks a master thread into multiple parallel threads and allows for thread-specific logic (like identifying the master thread) within a parallel block.
