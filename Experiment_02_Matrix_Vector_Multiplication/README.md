# Experiment 2: Matrix-Vector Multiplication and Data Parallelism with OpenMP

This document provides a collection of programs demonstrating essential parallel programming concepts—including matrix-vector multiplication, reduction operations (max and sum), and matrix transposition—using **OpenMP**.

---

## A) Matrix-Vector Multiplication ($b = Ax$)

### 🎯 Aim
To compute a simple matrix-vector multiplication using OpenMP directives to distribute the row-wise calculations across multiple threads.

### ⚙️ Algorithm
1.  **Initialize:** Matrix $A$ and vector $x$ with predefined values.
2.  **Setup:** Create a result vector $b$ initialized to zero.
3.  **Parallelize:** Use `#pragma omp parallel for` on the outer loop (rows).
4.  **Compute:** For each row $i$, calculate the dot product of row $i$ with vector $x$ and store it in $b[i]$. 
    * *Note: Variable $j$ must be private to each thread to avoid race conditions.*
5.  **Display:** Print the final result vector.

### 💻 Source Code
```c
#include <stdio.h>
#include <omp.h>

int main() {
    float A[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float x[3] = {1, 2, 3};
    float b[3];
    int i, j;

    // Compute b = Ax using OpenMP
    #pragma omp parallel for private(j)
    for (i = 0; i < 3; i++) {
        b[i] = 0;
        for (j = 0; j < 3; j++) {
            b[i] = b[i] + A[i][j] * x[j];
        }
    }

    // Print result
    printf("Result vector b:\n");
    for (i = 0; i < 3; i++) {
        printf("b[%d] = %f\n", i, b[i]);
    }

    return 0;
}
```

**Output:**
```text
Result vector b:
b[0] = 14.000000
b[1] = 32.000000
b[2] = 50.000000
```

---

## B) Finding the Largest Number in an Array

This program demonstrates the **reduction** clause, which efficiently combines results from multiple threads.

### 💻 Source Code
```c
#include <stdio.h>
#include <omp.h>

int main() {
    int arr[] = {3, 1, 2, 5, 4, 0, 9, 7, 8, 6};
    int n = 10;
    int max_val = arr[0];
    int i;

    #pragma omp parallel for reduction(max: max_val)
    for (i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    printf("Largest number in array: %d\n", max_val);
    return 0;
}
```

---

## C) Sum of Array Elements

Another use of the **reduction** clause to safely calculate a running sum across parallel threads.

### 💻 Source Code
```c
#include <stdio.h>
#include <omp.h>

int main() {
    int arr[] = {3, 1, 2, 5, 4, 0};
    int n = 6;
    int sum = 0;
    int i;

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n; i++) {
        sum += arr[i];
    }

    printf("Sum of array elements: %d\n", sum);
    return 0;
}
```

---

## D) Matrix Transpose

This program demonstrates parallelizing nested loops to swap rows and columns ($T_{ji} = A_{ij}$).

### 💻 Source Code
```c
#include <stdio.h>
#include <omp.h>

int main() {
    int A[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int T[3][3];
    int i, j;

    #pragma omp parallel for private(j)
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            T[j][i] = A[i][j];
        }
    }

    printf("Transpose of matrix:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", T[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```

---

## ✅ Result
All programs—Matrix-Vector Multiplication, Array Max, Array Sum, and Matrix Transpose—were successfully implemented and executed using OpenMP parallel constructs.
