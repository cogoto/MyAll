// 簡単な TELNET クライアント になります．
// 使うときは
//
//  ./MyTelnet.sample.exe -h [server IP address] -u [user name] -p [passwd] [message]
//
// とすればよい．
//

//
// Include Library
//

#include <string>
#include <iostream>

#include <netdb.h>
#include <unistd.h>

using namespace std;

//
// Include My Library
//

#include "MyTelnet.hpp"

//
//  MAIN
//
int main(int argc, char *argv[]) {

    /// 変数 ///

    string IP_ADDR = "127.0.0.1";

    string UID = "";

    string PWD = "";

    vector<string> buff_send;

    vector<string> buff_recv;

    /// オプション ///

    int opt;
    while ((opt = getopt(argc, argv, "h:u:p:")) != -1) {
        switch (opt) {
        case 'h':
            IP_ADDR = optarg;
            break;
        case 'u':
            UID = optarg;
            break;
        case 'p':
            PWD = optarg;
            break;
        default:
            printf("Warning! \'%c\' \'%c\'\n", opt, optopt);
            // return -1;
        }
    }

    for (int i = optind; i < argc; i++) {
        buff_send.push_back(argv[i]);
    }

    MyAll::MyTelnet telnet_;

    telnet_.Init(UID, PWD, IP_ADDR);

    telnet_.PrintPrompt();

    for(const auto &it : buff_send) {

        cout << "[SEND] " << it << endl;

        telnet_.Cmd(it, buff_recv);

        cout << "[REPLY]" << endl;
        for(const auto &it2 : buff_recv) { cout << it2 << endl; }

    }

    telnet_.Close();

    return 1;


};
