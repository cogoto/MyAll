#include <iostream>
#include <unistd.h>

#include "MySignal.hpp"

using namespace std;


int main() {

    cout << "TEST OF [MySignal]" << endl << endl;

    MyAll::MySignal sig;

    sig.Print();

    while (1) {
        if (sig.SignalCheck()) {
            cout << "Received Signal!!!" << endl;
            break;
        }
        sleep(10);
    }

    cout << "TEST END." << endl;

    return 0;

}
