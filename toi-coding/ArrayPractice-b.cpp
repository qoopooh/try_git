#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N;
    // Read the size of the array
    if (!(cin >> N)) return 0;

    // Use a vector (dynamic array) to store N integers
    vector<int> numbers(N);
    for (int i = 0; i < N; i++) {
        cin >> numbers[i];
    }

    int k;
    // Read the number k to search for
    cin >> k;

    // 1. Display numbers in reverse order
    for (int i = N - 1; i >= 0; i--) {
        cout << numbers[i];
        if (i > 0) {
            cout << " ";  // Add space between numbers
        }
    }
    cout << endl;

    // 2. Count how many times k appears
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (numbers[i] == k) {
            count++;
        }
    }
    cout << count << endl;

    return 0;
}
