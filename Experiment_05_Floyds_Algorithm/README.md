# Experiment 5: All-Pairs Shortest Path (Floyd-Warshall Algorithm)

This document demonstrates the implementation of the **Floyd-Warshall Algorithm** using **OpenMP**. This algorithm finds the shortest distances between every pair of vertices in a weighted graph.

---

## 🎯 Aim
To implement the All-Pairs Shortest-Path problem using Floyd’s Algorithm and optimize it by parallelizing the computation with OpenMP directives.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialization:** Create a distance matrix `dist[N][N]` and initialize it with edge weights. Use a large value (`INF`) for vertices with no direct connection and `0` for self-loops.
3.  **Intermediate Vertex Loop (k):** Iterate through each vertex $k$ from $0$ to $N-1$. This loop **cannot** be parallelized because each iteration depends on the previous one.
4.  **Parallelization (i):** Parallelize the outer loop for source vertices $i$ using `#pragma omp parallel for`.
5.  **Inner Loop (j):** For each destination vertex $j$, check if a shorter path exists through vertex $k$.
6.  **Update Rule:** If `dist[i][k] + dist[k][j] < dist[i][j]`, update `dist[i][j]` with the new shorter path.
7.  **Finalization:** Display the resulting shortest-path matrix and the execution time.
8.  **Stop.**

---

## 💻 Source Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 4
#define INF 99999

int main() {
    int dist[N][N] = {
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0, 1},
        {INF, INF, INF, 0}
    };
    int i, j, k;
    double start_time, end_time;

    printf("Initial distance matrix:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (dist[i][j] == INF) printf("INF ");
            else printf("%3d ", dist[i][j]);
        }
        printf("\n");
    }

    start_time = omp_get_wtime();

    // Floyd-Warshall with OpenMP parallelization
    for (k = 0; k < N; k++) {
        // Parallelizing the source vertex loop
        #pragma omp parallel for private(j)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    end_time = omp_get_wtime();

    printf("\nFinal distance matrix (All-Pairs Shortest Paths):\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (dist[i][j] == INF) printf("INF ");
            else printf("%3d ", dist[i][j]);
        }
        printf("\n");
    }

    printf("\nTime taken: %f seconds\n", end_time - start_time);

    return 0;
}
```

---

## 📊 Expected Output

```text
Initial distance matrix:
  0   5 INF  10 
INF   0   3 INF 
INF INF   0   1 
INF INF INF   0 

Final distance matrix (All-Pairs Shortest Paths):
  0   5   8   9 
INF   0   3   4 
INF INF   0   1 
INF INF INF   0 

Time taken: 0.000123 seconds
```

---

## 🔍 Implementation Notes
* **Parallel Efficiency:** By parallelizing the `i` loop, we divide the work of checking source-to-destination paths across available CPU cores.
* **Data Race Prevention:** The variable `j` is made `private(j)` to ensure each thread maintains its own loop counter, preventing conflicts.
* **The 'k' Constraint:** The $k$ loop must remain serial because the matrix values at step $k$ are used to calculate values for step $k+1$. Parallelizing the $k$ loop would lead to incorrect results.

---

## ✅ Result
The All-Pairs Shortest-Path problem was successfully implemented using the Floyd-Warshall algorithm. The use of OpenMP significantly optimizes the computation for larger matrices by utilizing multi-core processing.
