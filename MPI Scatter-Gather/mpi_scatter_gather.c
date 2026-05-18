#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

float* create_rand_nums(int num_elements) {
    float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
    assert(rand_nums != NULL);

    for (int i = 0; i < num_elements; i++) {
        rand_nums[i] = (rand() / (float)RAND_MAX);
    }
    return rand_nums;
}

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

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    float *rand_nums = NULL;
    if (world_rank == 0) {
        rand_nums = create_rand_nums(num_elements_per_proc * world_size);
        printf("Root process created %d random numbers\n", num_elements_per_proc * world_size);
    }

    float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
    assert(sub_rand_nums != NULL);

    // Scatter data to all processes
    MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT,
                sub_rand_nums, num_elements_per_proc, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    // Compute local average
    float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);
    printf("Process %d: local average = %f\n", world_rank, sub_avg);

    // Gather all averages to all processes using Allgather
    float *sub_avgs = (float *)malloc(sizeof(float) * world_size);
    assert(sub_avgs != NULL);

    MPI_Allgather(&sub_avg, 1, MPI_FLOAT,
                  sub_avgs, 1, MPI_FLOAT,
                  MPI_COMM_WORLD);

    // Compute global average
    float avg = compute_avg(sub_avgs, world_size);
    printf("Process %d: global average = %f\n", world_rank, avg);

    // Cleanup
    if (world_rank == 0) {
        free(rand_nums);
    }
    free(sub_avgs);
    free(sub_rand_nums);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
