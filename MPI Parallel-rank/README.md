# Experiment 10: Parallel Sorting (Ranking) with MPI

This experiment demonstrates a **Parallel Ranking Algorithm**, a simple form of sorting where each process determines the final position of its data by comparing it against all other elements in the distributed system.

---

## 🎯 Aim
To implement a parallel sorting program in C using MPI, where each process determines the rank of its locally generated random number relative to the entire set.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialization:** Initialize the MPI environment and seed the random number generator using the process rank to ensure different values across processes.
3.  **Data Generation:** Each process generates a single random floating-point number.
4.  **Collective Gathering:**
    * Use `MPI_Gather` to collect all numbers into a buffer on the root process (Rank 0).
    * Use `MPI_Bcast` from the root to distribute the complete set of numbers back to every process.
5.  **Rank Calculation:** Each process iterates through the collected set and counts how many numbers are smaller than its own value. 
    * *Tie-breaking:* If two numbers are identical, the process with the lower rank is considered "smaller."
6.  **Sorting & Display:**
    * Each process prints its value and calculated rank.
    * The root process gathers the numbers again and performs a final sort for a consolidated display of the results.
7.  **Finalize:** Clean up memory and terminate the MPI environment.
8.  **Stop.**

---

## 💻 Source Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    // Seed random number generator uniquely for each process
    srand(time(NULL) + world_rank);
    
    // Each process generates a random number
    float rand_num = (float)rand() / RAND_MAX;
    
    // Buffer to gather all numbers (allocated on all processes for Bcast)
    float *all_nums = (float *)malloc(world_size * sizeof(float));
    
    // Gather all random numbers to root
    MPI_Gather(&rand_num, 1, MPI_FLOAT, all_nums, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    // Broadcast the full set to all processes so they can compute their rank
    MPI_Bcast(all_nums, world_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    // Calculate Rank: how many elements are smaller than 'my' number?
    int rank = 0;
    for (int i = 0; i < world_size; i++) {
        if (all_nums[i] < rand_num || (all_nums[i] == rand_num && i < world_rank)) {
            rank++;
        }
    }
    
    printf("Process %d: value = %f, rank = %d\n", world_rank, rand_num, rank);
    
    // Final visualization at Root
    if (world_rank == 0) {
        printf("\nSorted values: ");
        // Simple selection sort for final output display
        for (int i = 0; i < world_size - 1; i++) {
            for (int j = i + 1; j < world_size; j++) {
                if (all_nums[i] > all_nums[j]) {
                    float temp = all_nums[i];
                    all_nums[i] = all_nums[j];
                    all_nums[j] = temp;
                }
            }
        }
        for (int i = 0; i < world_size; i++) {
            printf("%f ", all_nums[i]);
        }
        printf("\n");
    }
    
    free(all_nums);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
```

---

## 📊 Expected Output

```text
$ mpirun -n 4 ./parallel_sort
Process 0: value = 0.234567, rank = 0
Process 1: value = 0.876543, rank = 3
Process 2: value = 0.456789, rank = 1
Process 3: value = 0.654321, rank = 2

Sorted values: 0.234567 0.456789 0.654321 0.876543
```

---

## 🔍 Key Concept: Parallel Ranking
In this approach, the "sorting" is done by determining the **ordinal position** (rank) of each element. In a system with $P$ processes, each process performs $O(P)$ comparisons. This is a simple and effective parallelization for small to medium datasets where every node can afford to store the metadata of the entire set.

---

## ✅ Result
The parallel ranking program was successfully implemented. By combining `MPI_Gather` and `MPI_Bcast`, the program ensured that every process had the necessary global information to determine its local value's position in the final sorted array.
