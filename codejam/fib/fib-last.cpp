/**
 * Iterative Fibonacci using rolling variables; reads n from argv or stdin.
 */
#include <iostream>

unsigned long long fib(int n) {
    if (n < 1) return 0L;
    if (n < 3) return 1L;

    unsigned long long first = 0, second = 1, next;
    for (int i = 0; i < n; i++) {
        next = first + second;
        first = second;
        second = next;
    }

    return first;
}

int main(int argc, char** argv) {
    int n;
    if (argc > 1) {
        n = std::stoi(argv[1]);
    } else {
        if (!(std::cin >> n)) return 1;
    }
    if (n < 0) {
        std::cerr << "n must be >= 0\n";
        return 1;
    }
    if (n > 93) {
        std::cerr << "n too large for unsigned long long (max 93)\n";
        return 1;
    }
    std::cout << fib(n);
    return 0;
}
