#include <iostream>
using namespace std;

template <typename T, typename U>
class Pair {
   public:
    T first;
    U second;

    Pair(T a, U b) : first(a), second(b) {
    }
};

int main() {
    Pair<int, double> p1(1, 2.5);
    cout << "Pair 1: (" << p1.first << ", " << p1.second << ")" << endl;

    Pair<string, int> p2("Age", 30);
    cout << "Pair 2: (" << p2.first << ", " << p2.second << ")" << endl;

    return 0;
}
