#include <stdio.h> 
#include <omp.h> 
#define N 4 
#define INF 99999 
#ifndef min 
#define min(a,b) (((a) < (b)) ? (a) : (b)) 
#endif 
 
int main() { 
    int dist[N][N] = {{0, 3, INF, 7}, {3, 0, 2, INF}, {INF, 2, 0, 1}, {7, INF, 1, 0}}; 
    int i, j, k; 
    printf("Original Distance Matrix:\n"); 
    for (i = 0; i < N; i++) { 
        for (j = 0; j < N; j++) { 
            if (dist[i][j] == INF) printf("INF "); else printf("%d   ", dist[i][j]); 
        } 
        printf("\n"); 
    } 
    for (k = 0; k < N; k++) { 
        #pragma omp parallel for private(i, j) shared(dist) 
        for (i = 0; i < N; i++) { 
            for (j = 0; j < N; j++) { 
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]); 
            } 
        } 
    } 
    printf("\nAll-Pairs Shortest Path Matrix:\n"); 
    for (i = 0; i < N; i++) { 
        for (j = 0; j < N; j++) { 
            if (dist[i][j] == INF) printf("INF "); else printf("%d   ", dist[i][j]); 
        } 
        printf("\n"); 
    } 
    return 0; 
} 
