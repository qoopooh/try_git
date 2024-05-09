#include <iostream>
#include <string>
using namespace std;

int main() {
    int work, mid, fin;
    cin >> work >> mid >> fin;

    if (5 > work || work > 10) {
        cout << "ล้มเหลว";
        return 0;
    }

    if (20 > mid || mid > 40) {
        cout << "ล้มเหลว";
        return 0;
    }

    if (25 > fin || fin > 50) {
        cout << "ล้มเหลว";
        return 0;
    }

    cout << "ผ่าน";
    return 0;
}
