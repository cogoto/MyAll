// The simple TCP server & client.
//
// server :
//
//  ./MyTcp.sample.exe -t server -p [port number]
//
// client :
//
//  ./MyTcp.sample.exe -t client -h [server IP address] -p [server port] [message]
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

#include "MyTcp.hpp"

//
//    MAIN
//

int main(int argc, char *argv[]) {

    string TYPE    = "client";

    string IP_ADDR = "127.0.0.1";
    int    IP_PORT = 50001;

    string buff_client_Send = "";
    string buff_client_Recv;

    string buff_server_Send = "";
    string buff_server_Recv;

    // outions //

    int opt;
    while ((opt = getopt(argc, argv, "t:h:p:")) != -1) {
        switch (opt) {
        case 't':
            TYPE = optarg;
            break;
        case 'h':
            IP_ADDR = optarg;
            break;
        case 'p':
            IP_PORT = atoi(optarg);
            break;
        default:
            printf("Warning! \'%c\' \'%c\'\n", opt, optopt);
            // return -1;
        }
    }

    for (int i = optind; i < argc; i++) {
        buff_client_Send += argv[i];
    }

    // client //

    if (TYPE == "client") {

        MyAll::MyTcpClient client;
        client.Init(IP_ADDR, IP_PORT);

        client.Send(buff_client_Send);
        cout << "[TcpClient] send                : " << buff_client_Send.c_str() << endl;
        client.DisplayPacket(buff_client_Send);

        if (client.Recv(buff_client_Recv) > 0) {

            cout << "[TcpClient]    Received : " << buff_client_Recv.c_str() << endl;
            client.DisplayPacket(buff_client_Recv);

        }

    }

    // server //

    if (TYPE == "server") {

        MyAll::MyTcpServer server;
        server.Init(IP_PORT); // Including Wait()

        while (1) {

            if (server.Recv(buff_server_Recv) > 0) {

                cout << "[TcpServer] Received : " << buff_server_Recv.c_str() << endl;
                server.DisplayPacket(buff_server_Recv);

                buff_server_Send  = "You said? -> ";
                buff_server_Send += buff_server_Recv;

                server.Send(buff_server_Send);
                cout << "[TcpServer] send         : " << buff_server_Send.c_str() << endl;
                server.DisplayPacket(buff_server_Send);

            }

            server.Wait();

        }

    }

}
