#include <iostream>
#include <string>
using namespace std;
int main() {
    string name, surname;
    // getline(cin, name);
    // getline(cin, surname);
    cin >> name >> surname;
    cout << "สวัสดี " << name << " " << surname << "\n";
    cout << name.substr(0,2) << surname.substr(0, 2) << "\n";
    string hello;
    return 0;
}