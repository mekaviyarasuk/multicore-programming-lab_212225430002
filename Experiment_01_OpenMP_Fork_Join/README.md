# OpenMP Fork-Join Parallelism Demonstration

This repository contains a simple C program designed to demonstrate the **Fork-Join** model used by OpenMP. In this model, a single master thread "forks" into multiple parallel threads to execute a block of code and then "joins" back into a single thread once the block is complete.

---

## 🎯 Aim
To write and execute a simple program that demonstrates how OpenMP manages parallel regions using the Fork-Join mechanism.

## ⚙️ Algorithm
1.  **Start.**
2.  **Include Header:** Import the OpenMP header file `<omp.h>`.
3.  **Initial State:** Print the number of active threads before entering the parallel region.
4.  **Fork:** Use the `#pragma omp parallel` directive to create a parallel region.
5.  **Parallel Execution:** Inside the region, each thread retrieves and prints its unique **Thread ID**.
6.  **Join:** Once the parallel block ends, threads are synchronized and terminated, leaving only the master thread.
7.  **Final State:** Print the number of active threads after the parallel region.
8.  **Stop.**

---

## 💻 Source Code

```c
#include <stdio.h>
#include <omp.h>

int main(void) {
    // 1. Initial Serial Region
    printf("Before: Number of threads is %d\n", omp_get_num_threads());
    
    printf("-- Starting Parallel Region (Fork) --\n");

    // 2. Parallel Region (The Fork)
    #pragma omp parallel
    {
        // Each thread executes this block independently
        int id = omp_get_thread_num();
        printf("Hello from Thread ID: %d\n", id);
    }
    // 3. Back to Serial Region (The Join)

    printf("-- Parallel Region Ended (Join) --\n");
    printf("After: Number of threads is %d\n", omp_get_num_threads());
    
    return 0;
}
```

---

## 📊 Expected Output

> **Note:** The order of the "Thread ID" lines may vary each time you run the program because threads execute concurrently.

```text
Before: Number of threads is 1
-- Starting Parallel Region (Fork) --
Thread ID is 0
Thread ID is 3
Thread ID is 1
Thread ID is 2
-- Parallel Region Ended (Join) --
After: Number of threads is 1
```

---

## 🚀 How to Compile and Run

To run this program, you need a compiler that supports OpenMP (like `gcc`).

1.  **Compile** the code using the `-fopenmp` flag:
    ```bash
    gcc -fopenmp fork_join.c -o fork_join
    ```

2.  **Run** the executable:
    ```bash
    ./fork_join
    ```

3.  *(Optional)* **Set the number of threads** manually before running:
    ```bash
    export OMP_NUM_THREADS=4
    ./fork_join
    ```

---

## ✅ Result
The program successfully demonstrates the transition from a single master thread to multiple threads and back, validating the **OpenMP Fork-Join Parallelism** concept.
