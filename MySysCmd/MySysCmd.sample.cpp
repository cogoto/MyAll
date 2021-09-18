#include <string>
#include <vector>
#include <iostream>

#include <unistd.h>
// #include <sys/resource.h>

#include "MySysCmd.hpp"

using namespace std;


int main() {

    cout << "TEST OF [MySysCmd::System]" << endl;

    MyAll::MySysCmd my;

    my.System("ls");

    vector<string> return_;
    my.System("ls -1", return_);

    for (auto &it : return_) {
        cout << "[MySysCmd] : " << it << endl;
    }

    ///

    cout << endl << "TEST OF [MySysCmd::BeepSound]" << endl;

    my.BeepSound(1);

    sleep(1);

    my.BeepSound(3);

    ///

    cout << endl;

    my.Print("TEST OF [MySysCmd::ColorPrint] : color is magenta!",       "magenta");

    my.Print("White bold string with red background !",                  "white",  "red",   true, false);

    my.Print("Magenta bold string with green background & under line !", "yellow", "green", true, true);


    sleep(1); /// Wait till finishing beep sounds.


}
