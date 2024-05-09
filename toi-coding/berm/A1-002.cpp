#include <iostream>
using namespace std;

void getCoins(int amount) {
    int coins[] = {10, 5, 2, 1};

    for (int i = 0; i < sizeof(coins) / sizeof(coins[0]); i++) {
        int count = amount / coins[i];
        amount -= count * coins[i];
        cout << coins[i] << " = " << count << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int amount;
    cin >> amount;
    getCoins(amount);

    return 0;
}