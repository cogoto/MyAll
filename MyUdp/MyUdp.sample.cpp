// 簡易 UDP サーバ と クライアント．(１対１対応)
//
// サーバとして使うときは
//
//  ./MyUdp.sample.exe -t server -h [クライアントのIPアドレス] -p [受信ポート番号]
//
// クライアントとして使うときは
//
//  ./MyUdp.sample.exe -t client -h [サーバのIPアドレス] -p [サーバ側のポート番号] -q [送信元ポート番号] [メッセージ]
//
// とすればよい．
// 同じパソコン内で試したいときは
//
//  ./MyUdp.sample.exe -t client -h 127.0.0.1 -p 57000 -q 57001 message1
//  ./MyUdp.sample.exe -t client -h 127.0.0.1 -p 57001 -q 57000 message2
//
// のように交互に実行すればよい.
//
// パケットはいっているのに，なぜかうまくつながらない時は
// 一度，両者クライアントモードでやりとりをして，
// サーバとクライアントに戻すとうまくいく時がある．
//

//
// Include Library
//

#include <netdb.h>
#include <unistd.h>

#include <string>
#include <iostream>

using namespace std;

//
// Include My Library
//

#include "MyUdp.hpp"

//
// MAIN
//

int main(int argc, char *argv[]) {

    string TYPE = "client";
    string SERVER_ADDR = "127.0.0.1";
    int    SERVER_PORT = 50002;
    int    CLIENT_PORT = 50003;

    string TEST1       = "";
    string NAME1       = "";
    string NAME2       = "";

    string buff_Send   = "";
    string buff_Recv   = "";

    /// Options

    int opt;
    while ((opt = getopt(argc, argv, "t:h:p:q:")) != -1) {
        switch (opt) {
        case 't':
            TYPE = optarg;
            break;
        case 'h':
            SERVER_ADDR = optarg;
            break;
        case 'p':
            SERVER_PORT = atoi(optarg);
            break;
        case 'q':
            CLIENT_PORT = atoi(optarg);
            break;
        default:
            printf("Warning! \'%c\' \'%c\'\n", opt, optopt);
            // return -1;
        }
    }

    for (int i = optind; i < argc; i++) {
        buff_Send += argv[i];
    }

    /// Client ///

    if (TYPE == "client") {

        MyAll::MyUdpComm comm;
        comm.Init(SERVER_ADDR, SERVER_PORT, CLIENT_PORT);

        comm.SetTimeOut(5.5);

        comm.Send(buff_Send);
        cout << "[UdpClient] send     : " << buff_Send << endl;
        comm.DisplayPacket(buff_Send);

        if (comm.Recv_partner(buff_Recv) <= 0) {
            cout << comm.ErrMsg() << endl;
        }
        else {
            cout << "[UdpClient] Received : " << buff_Recv << endl;
            comm.DisplayPacket(buff_Recv);
        }

    }

    /// Server ///

    if (TYPE == "server") {

        MyAll::MyUdpServer server;
        server.Init(SERVER_PORT);

        server.SetTimeOut(5.0);

        while (1) {

            usleep(100000);
            cout << "[UdpServer] waiting ..." << endl;

            if (server.Recv_partner(buff_Recv) <= 0) {
                cout << server.ErrMsg() << endl;
            }
            else {
                cout << "[UdpServer] Received : " << buff_Recv << endl;
                server.DisplayPacket(buff_Recv);
            }

            buff_Send  = "You said? -> ";
            buff_Send += buff_Recv;

            server.Send(buff_Send);
            cout << "[UdpServer] send     : " << buff_Send << endl;
            server.DisplayPacket(buff_Send);

            server.Reset();

        }

    }

}
