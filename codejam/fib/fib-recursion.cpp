/**
 * Recursive Fibonacci implementation; reads n from stdin.
 */
#include <iostream>

unsigned long long fib(int n) {
    if (n < 2) return n;

    return fib(n - 2) + fib(n - 1);
}

int main() {
    int n;
    std::cin >> n;
    std::cout << fib(n);
    return 0;
}
