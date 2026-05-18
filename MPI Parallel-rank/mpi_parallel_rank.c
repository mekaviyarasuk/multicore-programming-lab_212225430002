#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, 

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, 

    // Seed random number generator
    srand(time(NULL) + world_rank);

    // Each process generates a random number
    float rand_num = (float)rand() / RAND_MAX;

    // Allocate array to gather all numbers
    float *all_nums = NULL;
    if (world_rank == 0) {
        all_nums = (float *)malloc(world_size * sizeof(float));
    }

    // Gather all random numbers to root
    MPI_Gather(

    // Broadcast all numbers to all processes
    MPI_Bcast(all_nums, world_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calculate rank (number of elements smaller than current)
    int rank = 0;
    for (int i = 0; i < world_size; i++) {
        if (all_nums[i] < rand_num || (all_nums[i] == rand_num && i < world_rank)) {
            rank++;
        }
    }

    printf("Process %d: value = %f, rank = %d\n", world_rank, rand_num, rank);

    // Gather sorted order at root
    float *sorted = NULL;
    if (world_rank == 0) {
        sorted = (float *)malloc(world_size * sizeof(float));
    }

    MPI_Gather(

    // Root prints sorted array
    if (world_rank == 0) {
        printf("\nSorted values: ");

        // Simple insertion sort for display
        for (int i = 0; i < world_size - 1; i++) {
            for (int j = i + 1; j < world_size; j++) {
                if (sorted[i] > sorted[j]) {
                    float temp = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = temp;
                }
            }
        }

        for (int i = 0; i < world_size; i++) {
            printf("%f ", sorted[i]);
        }
        printf("\n");

        free(sorted);
    }

    free(all_nums);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
