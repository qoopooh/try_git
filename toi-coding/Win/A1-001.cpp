#include <iostream>
#include <string>
using namespace std;
int main() {
string name;
string surname;
cin >> name;
cin >> surname;
cout << "Hello " << name << " " << surname << "\n";
cout << name.substr (0,2) << surname.substr (0,2);
return 0;
}   
