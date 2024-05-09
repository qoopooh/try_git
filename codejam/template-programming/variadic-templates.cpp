#include <iostream>
using namespace std;

void print() {
    cout << endl << "-------" << endl;
}

template <typename T, typename... Args>
void print(T first, Args... args) {
    cout << first << " ";
    print(args...);
}

int main() {
    print(1, 2.5, "Hello", 'A', "World", 42);
    return 0;
}
