#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void monteCarlo(int N, int K) {
    int pCircle = 0;
    int pSquare = 0;
    double x, y, d;
    int i;

    #pragma omp parallel firstprivate(x, y, d, i) reduction(+:pCircle, pSquare) num_threads(K)
    {
        unsigned int seed = (unsigned int)(time(NULL)) ^ (omp_get_thread_num() + 1);

        for (i = 0; i < N; i++) {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            x = (double)(seed & 0xFFFFFF) / (double)0xFFFFFF;

            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            y = (double)(seed & 0xFFFFFF) / (double)0xFFFFFF;
            d = (x * x) + (y * y);

            if (d <= 1.0) {
                pCircle++;
            }
            pSquare++;
        }
    }

    double pi = 4.0 * ((double)pCircle / (double)(pSquare));
    printf("Final Estimation of Pi = %f\n", pi);
    printf("Points in circle: %d, Total points: %d\n", pCircle, pSquare);
}

int main() {
    int N = 1000000;  // Points per thread
    int K = 4;        // Number of threads

    printf("Monte Carlo Pi Estimation\n");
    printf("Points per thread: %d\n", N);
    printf("Number of threads: %d\n", K);
    printf("Total points: %d\n\n", N * K);

    monteCarlo(N, K);

    return 0;
}
