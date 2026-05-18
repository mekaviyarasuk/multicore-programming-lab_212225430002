# Experiment 6: Parallel Monte Carlo Pi Estimation

This document outlines the implementation of a **Monte Carlo Method** to estimate the value of $\pi$ using **OpenMP**. The program leverages parallel random number generation to simulate thousands of points being "thrown" at a square.

---

## 🎯 Aim
To implement a Parallel Random Number Generator using the Monte Carlo method to estimate the value of $\pi$ by calculating the ratio of points falling inside a circular quadrant versus a square.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialize Parameters:** Set the number of points to be generated per thread ($N$) and the number of threads ($K$).
3.  **Setup Counters:** Initialize `pCircle` (points inside the circle) and `pSquare` (total points) to zero.
4.  **Parallel Region:** Use `#pragma omp parallel` with a `reduction(+: pCircle, pSquare)` clause.
5.  **Thread-Safe Randomness:** Each thread creates a unique `seed` using its thread ID to ensure the random sequences are independent.
6.  **Point Generation:** * Generate random coordinates $x$ and $y$ between $0$ and $1$ using `rand_r()`.
    * Calculate the distance from the origin: $d = x^2 + y^2$.
7.  **Categorization:** * If $d \leq 1$, the point is inside the circle; increment `pCircle`.
    * Increment `pSquare` for every point generated.
8.  **Calculate $\pi$:** Use the formula:
    $$\pi \approx 4 \times \frac{\text{points in circle}}{\text{total points}}$$
9.  **Display Results:** Print the estimated value of $\pi$ and the point counts.
10. **Stop.**

---

## 💻 Source Code

```c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void monteCarlo(int N, int K) {
    int pCircle = 0;
    int pSquare = 0;
    
    // x, y, d, and i are local to the parallel region for each thread
    #pragma omp parallel reduction(+ : pCircle, pSquare) num_threads(K)
    {
        double x, y, d;
        int i;
        // Create a unique seed for each thread for thread-safe random generation
        unsigned int seed = (unsigned int)(time(NULL)) ^ (omp_get_thread_num() + 1);
        
        for (i = 0; i < N; i++) {
            // rand_r is thread-safe compared to rand()
            x = (double)rand_r(&seed) / RAND_MAX;
            y = (double)rand_r(&seed) / RAND_MAX;
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
```

---

## 📊 Expected Output

```text
Monte Carlo Pi Estimation
Points per thread: 1000000
Number of threads: 4
Total points: 4000000

Final Estimation of Pi = 3.141625
Points in circle: 3141625, Total points: 4000000
```

---

## 💡 Important Considerations

* **`rand_r()` vs `rand()`:** Standard `rand()` is not thread-safe because it uses a hidden global state. Using `rand_r(&seed)` ensures that each thread manages its own state, preventing data races and bottlenecking.
* **Reduction Clause:** The `reduction(+: pCircle, pSquare)` clause is vital. It allows threads to work on their own local counts and then safely adds them together when the parallel region ends.
* **Seeding:** We XOR the current time with the thread ID (`omp_get_thread_num()`). If we only used `time(NULL)`, every thread might start with the exact same seed, resulting in the same "random" points and a very poor estimation.

---

## ✅ Result
The value of $\pi$ was successfully estimated using a parallelized Monte Carlo simulation. The accuracy of the result increases as the number of total points ($N \times K$) increases.
