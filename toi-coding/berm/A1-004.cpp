#include <iostream>
#include <vector>

using namespace std;

void evaluate(int* scores) {
    vector<int> fullscores = {10, 40, 50};

    for (int i = 0; i < fullscores.size(); i++) {
        if (scores[i] < fullscores[i] / 2) {
            cout << "fail";
            return;
        }
    }
    cout << "pass";
}

int main() {
    int scores[3];
    for (int i = 0; i < 3; i++) {
        cin >> scores[i];
    }

    evaluate(scores);

    return 0;
}
