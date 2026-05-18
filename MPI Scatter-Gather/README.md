# Experiment 8: MPI Collective Operations (Scatter, Gather, and Allgather)

This experiment demonstrates advanced collective communication in MPI. Unlike a broadcast (which sends the same data to everyone), **Scatter** divides data among processes, while **Gather/Allgather** collects pieces of data from all processes back into a single structure.

---

## 🎯 Aim
To write a C program that demonstrates the use of `MPI_Scatter` to distribute data and `MPI_Allgather` to collect results across all processes in a communicator.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialize MPI:** Set up the environment and determine the process rank and total size.
3.  **Root Data Creation:** The root process (rank 0) generates a large array of random numbers.
4.  **Scatter:** Use `MPI_Scatter` to send an equal-sized chunk of the random numbers from the root to every process in the communicator.
5.  **Local Computation:** Each process calculates the average of the subset of numbers it received.
6.  **Allgather:** Each process sends its local average to every other process using `MPI_Allgather`. This ensures that *every* process ends up with an array of all local averages.
7.  **Global Computation:** Every process computes the final global average by averaging the gathered values.
8.  **Output:** Each process prints its local result and the final global result.
9.  **Cleanup & Finalize:** Free allocated memory and terminate the MPI environment.
10. **Stop.**

---

## 💻 Source Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Helper to create an array of random floats
float* create_rand_nums(int num_elements) {
    float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
    assert(rand_nums != NULL);
    for (int i = 0; i < num_elements; i++) {
        rand_nums[i] = (rand() / (float)RAND_MAX);
    }
    return rand_nums;
}

// Helper to compute average of an array
float compute_avg(float *array, int num_elements) {
    float sum = 0.0f;
    for (int i = 0; i < num_elements; i++) {
        sum += array[i];
    }
    return sum / num_elements;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s num_elements_per_proc\n", argv[0]);
        exit(1);
    }

    int num_elements_per_proc = atoi(argv[1]);
    srand(time(NULL));
    
    MPI_Init(NULL, NULL);
    
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    float *rand_nums = NULL;
    if (world_rank == 0) {
        rand_nums = create_rand_nums(num_elements_per_proc * world_size);
        printf("Root process created %d random numbers\n", num_elements_per_proc * world_size);
    }
    
    // Each process needs a buffer to hold scattered data
    float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
    
    // 1. SCATTER: Distribute the large array from root to all processes
    MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
                num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    // 2. LOCAL COMPUTE: Average the subset
    float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);
    printf("Process %d: local average = %f\n", world_rank, sub_avg);
    
    // 3. ALLGATHER: Collect all local averages into every process
    float *sub_avgs = (float *)malloc(sizeof(float) * world_size);
    MPI_Allgather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, MPI_COMM_WORLD);
    
    // 4. GLOBAL COMPUTE: Average the averages
    float avg = compute_avg(sub_avgs, world_size);
    printf("Process %d: global average = %f\n", world_rank, avg);
    
    // Cleanup
    if (world_rank == 0) free(rand_nums);
    free(sub_avgs);
    free(sub_rand_nums);
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
```

---

## 📊 Expected Output

```text
$ mpirun -n 4 ./avg 100
Root process created 400 random numbers
Process 0: local average = 0.523456
Process 1: local average = 0.478234
Process 2: local average = 0.512345
Process 3: local average = 0.489012
Process 0: global average = 0.500762
Process 1: global average = 0.500762
Process 2: global average = 0.500762
Process 3: global average = 0.500762
```

---

## 🔍 Collective Communication Summary

| Function | Operation Description |
| :--- | :--- |
| **MPI_Scatter** | Takes an array from the root and splits it into $N$ chunks, sending one chunk to each process. |
| **MPI_Gather** | (Not used here) Collects chunks from all processes and stores them only on the root process. |
| **MPI_Allgather** | Collects data from all processes and sends the combined result to **all** processes. |

---

## ✅ Result
The program successfully demonstrates the workflow of scattering a data load across multiple processes and synchronizing the results back to all members using `MPI_Allgather`. This pattern is highly efficient for data-parallel tasks where every process needs the final result.
