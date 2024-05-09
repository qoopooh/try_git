#include <iostream>
#include <string>
using namespace std;

int main() {
    string leave;
    int work,mid,final;
    cout << "    made by \n";
    cout << "             go \n";
    cout << "             eat \n";
    cout << "             corn \n\n";
    cout << " enter work score: \n";
    cin >> work;
    cout << "\n enter midterm scores: \n";
    cin >> mid;
    cout << "\n enter final scores: \n";
    cin >> final;

    if (5 > work || work > 10) {
        cout << "\n result = failed / the score entered is not correct";
        cout << "\n\n\nenter anything to exit\n";
        cin >> leave;
        return 0;
    }
    
    if (20 > mid || mid > 40) {
        cout << "\n result = failed / the score entered is not correct";
         cout << "\n\n\nenter anything to exit\n";
        cin >> leave;
        return 0;
    }

    if (25 > final || final > 50) {
        cout << "\n result = failed / the score entered is not correct";
         cout << "\n\n\nenter anything to exit\n";
        cin >> leave;
        return 0;
    }

    if ( (mid + final + work) == 100) {
        cout << "\n full score";
        cout << "\n\n\nenter anything to exit\n";
        cin >> leave;
        return 0;
    }

    else {
        cout << "\n result = pass";
        cout << "\n\n\nenter anything to exit\n";
        cin >> leave;
        return 0;
    } 
}