#include <iostream>
using namespace std;

template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    cout << "Adding integers: " << add(3, 5) << endl;
    cout << "Adding doubles: " << add(2.5, 4.3) << endl;
    return 0;
}
