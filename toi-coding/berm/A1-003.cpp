#include <iostream>

using namespace std;

void findMax(int a, int b, int c) {
    int max = a;

    if (b > max) max = b;
    if (c > max) max = c;

    cout << max;
}

int main() {
    int a, b, c;
    cin >> a >> b >> c;

    findMax(a, b, c);

    return 0;
}
