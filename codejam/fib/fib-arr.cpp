/**
 * Fibonacci using a dynamic array to build sequence; reads n from stdin.
 */
#include <iostream>
#include <vector>

unsigned long long fib(int n) {
    if (n < 2) return n;

    std::vector<unsigned long long> vec(n);
    vec[0] = 0;
    vec[1] = 1;

    for (int i = 2; i < n; i++) {
        vec[i] = vec[i - 1] + vec[i - 2];
    }

    return vec[n - 1] + vec[n - 2];
}

int main() {
    int N;
    std::cin >> N;
    std::cout << fib(N);
    return 0;
}
