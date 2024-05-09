// gcc -fopenmp p-sum.c -o out-sum && OMP_NUM_THREADS=4 ./out-sum
// gcc -Xclang -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp p-sum.c -o out-sum && OMP_NUM_THREADS=4 ./out-sum
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    const int N = 1000000;
    double *a = malloc(N * sizeof *a);
    for (int i = 0; i < N; i++) a[i] = 1.0;

    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += a[i];
    }

    printf("Sum = %f\n", sum);
    free(a);
    return 0;
}
