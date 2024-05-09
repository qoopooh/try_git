#include <iostream>
#include <vector>
//#include <limits>
#include <climits>

using namespace std;

int main() {
    vector<int> numbers;

    for (int i = 0; i < 3; ++i) {
        int input;
        cin >> input;
        numbers.push_back(input);
    }

    // Initialize max_value with the smallest possible integer
    //int max_value = numeric_limits<int>::min();
    int max_value = INT_MIN;

    // Iterate through the vector to find the maximum value
    for (int num : numbers) {
        if (num > max_value) {
            max_value = num;
        }
    }

    cout << max_value << endl;

    return 0;
}
