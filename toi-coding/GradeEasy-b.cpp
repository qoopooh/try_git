#include <iostream>
using namespace std;

int main() {
    int score;
    cin >> score;

    string grade;
    if (score >= 80 && score <= 100) {
        grade = "A";
    } else if (score >= 75) {
        grade = "B+";
    } else if (score >= 70) {
        grade = "B";
    } else if (score >= 65) {
        grade = "C+";
    } else if (score >= 60) {
        grade = "C";
    } else if (score >= 55) {
        grade = "D+";
    } else if (score >= 50) {
        grade = "D";
    } else if (score >= 0) {
        grade = "F";
    } else {
        cout << "Invalid score entered." << endl;
        return 1;
    }

    cout << grade << endl;
    return 0;
}
