# Experiment 7: MPI Broadcast Collective Communication

This document demonstrates the use of **MPI (Message Passing Interface)** for collective communication. Specifically, it uses the `MPI_Bcast` function to send data from a root process to all other processes in the communicator.

---

## 🎯 Aim
To write a C program that demonstrates how a single value can be efficiently distributed from a root process to all other processes using the **MPI Broadcast** mechanism.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialize MPI:** Set up the MPI environment using `MPI_Init`.
3.  **Identify Processes:** * Get the unique rank of the current process using `MPI_Comm_rank`.
    * Get the total number of processes using `MPI_Comm_size`.
4.  **Root Initialization:** If the process rank is `0` (the root), initialize the data buffer (`buf`) with a specific value (e.g., `777`).
5.  **Broadcast:** All processes (including the root) call `MPI_Bcast`. This function acts as a sender for the root and a receiver for all other ranks.
6.  **Verification:** Every process prints its rank and the value stored in the buffer to confirm receipt.
7.  **Finalize:** Clean up the MPI environment using `MPI_Finalize`.
8.  **Stop.**

---

## 💻 Source Code

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int buf;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    
    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        buf = 777; // Data to be shared
        printf("Process 0 (Root) broadcasting value: %d\n", buf);
    }
    
    /* * MPI_Bcast parameters:
     * &buf: Data pointer
     * 1: Count of elements
     * MPI_INT: Data type
     * 0: Rank of the root process
     * MPI_COMM_WORLD: Communicator
     */
    MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Every process now has the same value in 'buf'
    printf("Process %d received value: %d\n", rank, buf);
    
    // Finalize the MPI environment
    MPI_Finalize();
    
    return 0;
}
```

---

## 📊 Expected Output

> **Note:** The order of printing from processes 1, 2, and 3 may vary due to the asynchronous nature of standard output in distributed systems.

```text
Process 0 (Root) broadcasting value: 777
Process 0 received value: 777
Process 1 received value: 777
Process 2 received value: 777
Process 3 received value: 777
```

---

## 🚀 How to Compile and Run

To execute MPI programs, you need an MPI implementation like **OpenMPI** or **MPICH** installed.

1.  **Compile** using the MPI wrapper:
    ```bash
    mpicc mpi_bcast.c -o mpi_bcast
    ```

2.  **Run** using `mpirun` or `mpiexec`, specifying the number of processes (e.g., 4):
    ```bash
    mpirun -np 4 ./mpi_bcast
    ```

---

## 🔍 Key Concept: Collective Communication
Unlike point-to-point communication (`MPI_Send`/`MPI_Recv`), **`MPI_Bcast`** is a collective operation. This means **all** processes in the communicator must call the function simultaneously. If one process fails to call it, the program will hang (deadlock) because the other processes are waiting to synchronize.

---

## ✅ Result
The MPI broadcast program was successfully implemented and executed, confirming that data from process 0 was effectively distributed to all other processes in the communicator.
