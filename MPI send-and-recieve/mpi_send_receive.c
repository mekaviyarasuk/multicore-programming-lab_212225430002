#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int *array = NULL;
    int tag = 1;
    int size, rank;
    MPI_Status status;

    MPI_Init(
    MPI_Comm_size(MPI_COMM_WORLD, 
    MPI_Comm_rank(MPI_COMM_WORLD, 

    if (size < 2) {
        printf("This program requires at least 2 processes\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        array = (int *)malloc(10 * sizeof(int));
        if (!array) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Initialize array
        for (int i = 0; i < 10; i++) {
            array[i] = i * 10;
        }

        printf("Process 0 sending array: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        MPI_Send(array, 10, MPI_INT, 1, tag, MPI_COMM_WORLD);
        printf("Process 0: Data sent to process 1\n");

        free(array);
    }
    else if (rank == 1) {
        array = (int *)malloc(10 * sizeof(int));
        if (!array) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        MPI_Recv(array, 10, MPI_INT, 0, tag, MPI_COMM_WORLD, 

        printf("Process 1 received array: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        free(array);
    }
    else {
        printf("Process %d: Not participating in communication\n", rank);
    }

    MPI_Finalize();
    return 0;
}
