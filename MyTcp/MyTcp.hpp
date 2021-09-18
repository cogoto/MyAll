// ================================================ //
///
/// @file   MyTcp.hpp
/// @brief  Class libraly of MyTcp.
/// @author cogoto
///
/// @note   Document style is for Doxygen.
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#ifndef _MYALL_MYTCP_HPP_
#define _MYALL_MYTCP_HPP_

//

#include <string>

#include <vector>

#include <netinet/in.h>   // for network structures.

//

namespace MyAll {

// ================================================ //
///
/// @class   MyTcpMaster
/// @brief   My master class for TCP connection.
/// @details This is the master class of MyTcpClient & MyTcpServer.
///          This class has many common members of these child classes.
///          Be careful not to operate this alone.
/// @date    2020/01/01
///
// ================================================ //

class MyTcpMaster {

 public:

    MyTcpMaster();          ///< Constractor :

    virtual ~MyTcpMaster(); ///< Destractor :

    ///
    /// @name  Preparing.
    /// @{

    bool IsInit();  ///< Initialized or not? :

    // bool IsConnect();  ///< Success connection against sever / client? :

    void SetTimeOut(const double &sec__ = -1); ///< Timeout intervals[sec.], default is not to use time-out. :

    // void SetTimeOut(const int &sec__ = -1, const int &usec__ = -1); ///< Timeout intervals[sec.], default is not to use time-out. :

    double GetTimeOut(); ///< Gets the value of the timeout interval. :

    //

    void SetRetry(const int &n_retry__ = 5, const int &wait_usec_retry__ = 200000); ///< Sets retry parameters (use in Wait & Connect) :

    void SetMaxRecvSize(const int &max_recv_size__ = 16384);                        ///< The maximum receiving size [Byte]. :

    /// @}
    /// @name  Receiving and sending messages.
    /// @{

    virtual int Recv(char* data__, const int &sizeof_data__, const int &useSock__ = -1); ///< Receives messages (char) :

    virtual int Recv(std::vector<char> &data__, const int &useSock__ = -1);              ///< Receives messages (char) :

    virtual int Recv(std::string &data__, const int &useSock__       = -1);              ///< Receives a message string :

    //

    virtual int Send(const char* data__, const int &n_byte__, const int &useSock__ = -1, const bool &pre_check__ = true); ///< Sends messages (char) :

    virtual int Send(const std::vector<char> &data__, const int &useSock__ = -1, const bool &pre_check__ = true);         ///< Sends messages (char) :

    virtual int Send(const std::string &data__, const int &useSock__       = -1, const bool &pre_check__ = true);         ///< Sends a message string :

    //

    virtual int SR(const std::vector<char> &send_data__, std::vector<char> &recv_data__, const int &useSock__ = -1); ///< Sends mesasges, and then receives replied ones. :

    virtual int SR(const std::string &send_data__, std::string &recv_data__, const int &useSock__ = -1);             ///< Sends a string, and then receives the replied one. :

    //

    virtual int RS(std::vector<char> &recv_data__, const std::vector<char> &send_data__, const int wait_usec__ = 0, const int &useSock__ = -1); ///< Receives messages, and then sends replies :

    virtual int RS(std::string &recv_data__, const std::string &send_data__, const int wait_usec__ = 0, const int &useSock__ = -1);             ///< Receives a string, and then sends the reply. :

    /// @}
    /// @name  The current error message.
    /// @{

    std::string ErrMsg(); ///< Gets the current error message.

    /// @}
    /// @name  Displaying message packets.
    /// @{

    std::vector<std::string> DisplayPacket(const std::vector<char> &data__, const bool std_out__ = true); ///< Formats packets & prints these if you need. :

    std::vector<std::string> DisplayPacket(const std::string &data__, const bool std_out__       = true); ///< Formats the packet & prints these if you need. :

    /// @}
    /// @name  Information.
    /// @{

    std::string GetIp_server(); ///< The IP address of the server. :

    std::string GetIp_client(); ///< The IP address of the client. :

    //

    int GetPort_server(); ///< The port numper of the server. :

    int GetPort_client(); ///< The port numper of the client. :

    /// @}
    /// @name  Message information.
    /// @{

    int GetN_recv(); ///< Gets the number of received packets or error no.(if <0) :

    int GetN_send(); ///< Gets the number of sent packets or error no.(if <0) :

    /// @}
    /// @name  Checking the connection.
    /// @{

    bool CheckConnect(const int &useSock__ = -1); ///< Checks current connection status. :

    /// @}
    /// @name  Closing.
    /// @{

    virtual bool Close() = 0; ///< A pure virtual function for closing sockets. :

    /// @}

    // bool IsTimeOut(); // タイムアウトしたかどうか？

    // void SetStatusOut(const bool true_or_false__); // ステータスを標準出力するか設定する．

 protected:

    ///
    /// @name  Socket creating.
    /// @{

    bool CreateSocket(); ///< Creates the socket. :

    /// @}
    /// @name  Preparing communications.
    /// @{

    virtual int PreRecv(const int &useSock__ = -1); ///< Previous check before receiving & waits for the time-out interval. :

    virtual int PreSend(const int &useSock__ = -1); ///< Previous check before sending & waits for the time-out interval. :

    /// @}

    // void StdOut(const std::string msg__); /// ステータス標準出力用関数

    // -------------------------------------------- //

    ///
    /// @name  Parameters for connections.
    /// @{

    int selfSock_;  ///< Self socket. :

    int useSock_;   ///< The socket using send & receive messages. :

    //

    fd_set chk_fd_; ///< File descriptor structure for the select function. :

    //

    std::string server_ip_; ///< The server IP address. :

    std::string client_ip_; ///< The client IP address. :

    int server_port_;       ///< The server port. :

    int client_port_;       ///< The client port. :

    /// @}
    /// @name  Information parameters.
    /// @{

    bool isInit_;  ///< Initialized or not. :

    // bool isConnect_;  // server に接続できたかどうか? / client から接続があったかどうか?

    //

    int n_read_;  ///< The number of received packets or error no.(if <0). :

    int n_write_; ///< The number of sent packets or error no.(if <0). :

    //

    std::string error_msg_; ///< The current error message. :

    /// @}
    /// @name  Time-out parameters.
    /// @{

    bool use_timeout_;           ///< Use time-out function or not. :

    struct timeval timeout_val_; ///< A structure for setting time-out. :

    // bool isTimeOut_; // time out したかどうかの判定用．

    // int timeout_sec_; // time out 時間（秒）

    // int timeout_usec_; // time out 時間（マイクロ秒）

    /// @}
    /// @name  Retry parameters.
    /// @{

    int n_retry_;                          ///< The number of retry. :

    int wait_usec_retry_;                  ///< The wait time for retry [micro sec.] :

    const int N_RETRY_C_ = 5;              ///< The number of retry. (default constant) :

    const int WAIT_USEC_RETRY_C_ = 200000; ///< The wait time for retry [micro sec.] (default constant) :

    /// @}
    /// @name  Parameters for the receiving size.
    /// @{

    int max_recv_size_;                    ///< The current char array size for received packets. :

    const int MAX_RECV_SIZE_C_ = 16384;    ///< The maximum char array size for received packets. (default constant) :

    /// @}

};


// ================================================ //
///
/// @class   MyTcpClient : MyTcpMaster
/// @brief   My class for the TCP client.
/// @details This creats a client object to comunicate with servers.
///          Please call Init() right after creating the object.
/// @date    2020/01/01
///
// ================================================ //

class MyTcpClient : public MyTcpMaster {

 public:

    MyTcpClient();          ///< Constractor :

    virtual ~MyTcpClient(); ///< Destractor :

    ///
    /// @name  Initialization.
    /// @{

    bool Init(
        const std::string &server_ip__,
        const int &        server_port__,
        const std::string &client_ip__   = "",
        const int &        client_port__ = -1); ///< Initialization. :

    bool Init(
        const std::string &server_ip__,
        const int &        server_port__,
        const int &        client_port__); ///< Initialization. :

    /// @}
    /// @name  Close.
    /// @{

    bool Close(); ///< Closes the socket. :

    /// @}

 private:

    ///
    /// @name  Initialize the client.
    /// @{

    bool InitClient(
        const std::string &server_ip__,
        const int &        server_port__,
        const std::string &client_ip__   = "",
        const int &        client_port__ = -1); ///< Initialization. :

    /// @}
    /// @name  Connection utilities.
    /// @{

    bool        Connect(const sockaddr_in &server__); ///< Connects the server. :

    sockaddr_in GetSockAddr_server();                 ///< Gets the data structure of the destination server. :

    sockaddr_in GetSockAddr_client();                 ///< Gets the data structure using myself (client). :

    /// @}

};


// ================================================ //
///
/// @class   MyTcpServer : MyTcpMaster
/// @brief   My class for the TCP server.
/// @details This creats a server object to comunicate with several clients.
///          Please call Init() right after creating the object.
/// @date    2020/01/01
///
// ================================================ //

class MyTcpServer : public MyTcpMaster {

 public:

    MyTcpServer();                                                         ///< Constractor :

    ~MyTcpServer();                                                        ///< Destractor :

    ///
    /// @name  Initialization.
    /// @{

    bool Init(const int &server_port__, const bool &wait_client__ = true); ///< Initialization. :

    bool Init(
        const std::string &server_ip__,
        const int &        server_port__,
        const bool &       wait_client__ = true); ///< Initialization. :

    /// @}
    /// @name  Setting the listen number.
    /// @{

    void SetListenNo(const int &n_listen__ = 5);             // Sets the listen numbers of the server. :

    /// @}
    /// @name  Waiting clients.
    /// @{

    bool Wait();                                             ///< Waits clients. :

    bool Wait(std::string &client_ip__, int &client_port__); ///< Waits clients, and then, returns client information. :

    bool Wait(int &useSock__);                               ///< Waits clients, and then, returns the socket. :

    // client の接続を待ち，接続があれば受送信用ソケットも返す（内部では受送信用ソケットを保持しない）

    bool Wait(int &useSock__, std::string &client_ip__, int &client_port__); ///< Waits clients, and then, returns the socket & client information. :

    /// @}
    /// @name  Closing.
    /// @{

    bool CloseServer(); ///< Closes the server socket.:

    bool CloseClient(); ///< Close the sockets with clients. :

    bool CloseClient(int &useSock__); ///< Close the socket with the selected client. :

    bool Close(); ///< Closes all sockets. :

    bool Close(int &useSock__); ///< Closes the server socket and the selected client socket. :

    /// @}

 private:

    ///
    /// @name  Initializing the server.
    /// @{

    bool InitServer(
        const std::string &server_ip__,
        const int &        server_port__,
        const bool &       wait_client__); ///< Initialization. :

    /// @}
    /// @name  Information.
    /// @{

    sockaddr_in GetSockAddr_server();      ///< Gets the sockaddr_in structure using this server. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Parameters.
    /// @{

    int n_listen_;  ///< The listen number. :

    /// @}

};

}  // MyAll

#endif //_MYALL_TCPLIB_HPP
