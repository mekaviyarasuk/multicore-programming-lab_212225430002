#include <stdio.h> 
#include <omp.h> 
 
int main() { 
    int arr[] = {3, 1, 2, 5, 4, 0, 9, 7, 8, 6}; 
    int n = sizeof(arr) / sizeof(arr[0]); 
    int sum = 0; 
    int max_val = arr[0]; 
    int i; 
    #pragma omp parallel for reduction(+: sum) 
    for (i = 0; i < n; i++) { 
        sum += arr[i]; 
    } 
    #pragma omp parallel for reduction(max: max_val) 
    for (i = 1; i < n; i++) { 
        if (arr[i] > max_val) { 
            max_val = arr[i]; 
        } 
    } 
    printf("Array elements: "); 
    for (i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
    printf("\nSum of elements: %d\n", sum); 
    printf("Largest number: %d\n", max_val); 
    return 0; 
} 
