#include <iostream>
using namespace std;

template <class T>
class Box {
   private:
    T value;

   public:
    Box(T val) : value(val) {
    }

    T getValue() {
        return value;
    }
};

int main() {
    Box<int> intBox(123);
    Box<string> strBox("Hello, World!");

    cout << "Integer Box contains: " << intBox.getValue() << endl;
    cout << "String Box contains: " << strBox.getValue() << endl;

    return 0;
}
