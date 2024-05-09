#include <iostream>
#include <string>
using namespace std;
int main() {
int one, two, three;
cin >> one;
cin >> two;
cin >> three;
if (one < 5 || two < 20 || three < 25) {
    cout << "fail";
}
else {cout << "pass";}
return 0;}