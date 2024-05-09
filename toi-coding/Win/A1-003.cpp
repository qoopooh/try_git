#include <iostream>
using namespace std;
void biggestValue (int first, int second, int third) {
    int most = first;
    if (second > most)
    most = second;

    if (third > most)
    most = third;
    cout << most;
}
int main() {
    int first, second , third;
    cin >> first;
    cin >> second;
    cin >> third;
    biggestValue (first, second, third);
    return 0;
}