# OpenMP Reduction: Array Sum and Maximum Value

This README document outlines a parallel C program that efficiently calculates both the sum and the maximum value of an array using **OpenMP reduction clauses**.

---

## 🎯 Aim
To implement a parallel program using OpenMP directives to compute the total sum of elements and identify the largest value within an array.

## ⚙️ Algorithm
1.  **Start.**
2.  **Initialize:** Define an array with a set of integers.
3.  **Setup Variables:** Initialize `sum` to 0 and `max_val` to the first element of the array.
4.  **Parallel Region:** Use `#pragma omp parallel` to begin the parallel execution block.
5.  **Sum Reduction:** Use `#pragma omp for reduction(+:sum)` to split the array among threads and aggregate their local sums into a global total.
6.  **Max Reduction:** Use `#pragma omp for reduction(max:max_val)` to split the array among threads and determine the global maximum.
7.  **Results:** Print the original array, the calculated sum, and the maximum value.
8.  **Stop.**

---

## 💻 Source Code

```c
#include <stdio.h>
#include <omp.h>

int main() {
    int arr[] = {1, 5, 7, 9, 11, 3, 8, 2};
    int n = 8;
    int sum = 0;
    int max_val = arr[0];
    int i;

    // Parallel sum and max computation
    #pragma omp parallel
    {
        // Compute sum using reduction
        #pragma omp for reduction(+:sum)
        for (i = 0; i < n; i++) {
            sum += arr[i];
        }

        // Compute max using reduction
        #pragma omp for reduction(max:max_val)
        for (i = 0; i < n; i++) {
            if (arr[i] > max_val) {
                max_val = arr[i];
            }
        }
    }

    // Print results
    printf("Array elements: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    
    printf("\nSum: %d\n", sum);
    printf("Largest number: %d\n", max_val);

    return 0;
}
```

---

## 📊 Expected Output

```text
Array elements: 1 5 7 9 11 3 8 2 
Sum: 46
Largest number: 11
```

---

## 💡 Key Concept: Reduction
In OpenMP, the `reduction` clause is crucial for avoiding **race conditions**. 

* **How it works:** Each thread creates a private copy of the reduction variable (like `sum`). At the end of the parallel loop, the private copies are combined (reduced) into the original global variable using the specified operator (`+` or `max`).

---

## ✅ Result
The program successfully utilizes OpenMP reduction directives to perform data aggregation in parallel, ensuring both accuracy and improved performance on multi-core systems.
