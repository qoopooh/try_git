#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5};
    vector<string> words = {"hello", "world"};

    for (auto num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    for (auto word : words) {
        cout << word << " ";
    }
    cout << endl;
    return 0;
}
