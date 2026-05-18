# Experiment 9: MPI Point-to-Point Communication (Send and Receive)

This experiment demonstrates the most fundamental form of communication in MPI: **Point-to-Point communication**. Unlike collective operations (like Broadcast or Scatter), this involves a specific sender and a specific receiver.

---

## 🎯 Aim
To write a C program that demonstrates how to send an array of integers from one process (Rank 0) and receive it in another process (Rank 1) using `MPI_Send` and `MPI_Recv`.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialize MPI:** Standard setup using `MPI_Init`, `MPI_Comm_rank`, and `MPI_Comm_size`.
3.  **Check Size:** Ensure at least 2 processes are running to perform a send-receive operation.
4.  **Sender Logic (Rank 0):**
    * Allocate memory for an array of 10 integers.
    * Initialize the array with data (e.g., $i \times 10$).
    * Call `MPI_Send` specifying the destination (Rank 1), the data count (10), and a unique tag.
    * Free the allocated memory.
5.  **Receiver Logic (Rank 1):**
    * Allocate memory for an array to hold the incoming data.
    * Call `MPI_Recv` specifying the source (Rank 0), the expected count, and the matching tag.
    * Print the received array to verify data integrity.
    * Free the allocated memory.
6.  **Finalize:** Clean up the MPI environment using `MPI_Finalize`.
7.  **Stop.**

---

## 💻 Source Code

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int *array = NULL;
    int tag = 1;
    int size, rank;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Safety check for process count
    if (size < 2) {
        if (rank == 0) printf("This program requires at least 2 processes\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // --- SENDER ---
        array = (int *)malloc(10 * sizeof(int));
        
        // Initialize array with multiples of 10
        for (int i = 0; i < 10; i++) {
            array[i] = i * 10;
        }
        
        printf("Process 0 sending array: ");
        for (int i = 0; i < 10; i++) printf("%d ", array[i]);
        printf("\n");
        
        // Sending to Rank 1
        MPI_Send(array, 10, MPI_INT, 1, tag, MPI_COMM_WORLD);
        printf("Process 0: Data sent to process 1\n");
        
        free(array);
    }
    else if (rank == 1) {
        // --- RECEIVER ---
        array = (int *)malloc(10 * sizeof(int));
        
        // Receiving from Rank 0
        MPI_Recv(array, 10, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        
        printf("Process 1 received array: ");
        for (int i = 0; i < 10; i++) printf("%d ", array[i]);
        printf("\n");
        
        free(array);
    }
    else {
        printf("Process %d: Not participating in communication\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

---

## 📊 Expected Output

```text
$ mpirun -n 2 ./send_recv
Process 0 sending array: 0 10 20 30 40 50 60 70 80 90 
Process 0: Data sent to process 1
Process 1 received array: 0 10 20 30 40 50 60 70 80 90
```

---

## 🔍 Key Functions Explained

* **`MPI_Send(data, count, type, dest, tag, comm)`**:
    * `dest`: The rank of the process that should receive the message.
    * `tag`: An integer ID used to distinguish between different types of messages.
* **`MPI_Recv(data, count, type, source, tag, comm, status)`**:
    * `source`: The rank of the process that is sending the message.
    * `status`: A structure that provides information on the received message (e.g., actual count or error codes).

---

## ✅ Result
The program was successfully implemented, demonstrating a robust point-to-point communication link between two processes. The receiver successfully mirrored the data initialized by the sender, validating the message-passing logic.
