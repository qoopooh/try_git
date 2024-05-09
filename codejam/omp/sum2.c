// gcc -fopenmp -O2 sum2.c -o out-sum2 && ./out-sum2
// gcc -Xclang -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp -O2 sum2.c -o out-sum2 && ./out-sum2
#include <stdio.h>
#include <omp.h>

int main(void) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= 100; ++i)
        sum += i;

    printf("Sum 1..100 = %d\n", sum);
    return 0;
}
