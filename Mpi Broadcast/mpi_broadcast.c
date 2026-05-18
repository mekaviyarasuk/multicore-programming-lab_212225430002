#include <stdio.h>

int main() {
    int rank = 0;
    int size = 1;
    int buf;

    // Single-process simulation fallback for gcc without MPI.
    buf = 777;
    printf("Process %d broadcasting value: %d\n", rank, buf);

    // In a single-process version, the received value is the same.
    printf("Process %d received value: %d\n", rank, buf);
    printf("(Note: This is a single-process fallback version compatible with plain gcc.)\n");

    return 0;
}
