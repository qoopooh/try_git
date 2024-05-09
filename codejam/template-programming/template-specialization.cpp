#include <iostream>
using namespace std;

template <typename T>
class Printer {
   public:
    static void print(T value) {
        cout << value << endl;
    }
};

template <>
class Printer<char*> {
   public:
    static void print(char* value) {
        cout << "String: " << value << endl;
    }
};

int main() {
    Printer<int>::print(42);       // Outputs: 42
    Printer<double>::print(3.14);  // Outputs: 3.14
    char str[] = "Hello, World!";
    Printer<char*>::print(str);  // Outputs: String: Hello, World!
    return 0;
}
