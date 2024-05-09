#include <iostream>
using namespace std;

void hello(string name, string surname) {
    cout << "Hello " << name << " " << surname << endl;
    cout << name.substr(0, 2) << surname.substr(0, 2) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string name, surname;
    cin >> name >> surname;

    hello(name, surname);

    return 0;
}