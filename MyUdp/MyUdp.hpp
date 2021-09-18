// ================================================ //
///
/// @file   MyUdp.hpp
/// @brief  Class libraly of MyUdp.
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

#ifndef _MYALL_MYUDP_HPP_
#define _MYALL_MYUDP_HPP_

//

#include <netinet/in.h>   // for "sockaddr_in" structures

//

#include <string>

#include <vector>

//

namespace MyAll {

// ================================================ //
///
/// @class   MyUdpMaster
/// @brief   My master class for UDP connection.
/// @details This is the master class of MyUdpComm & MyUdpServer.
///          This class has many common members of these child classes.
///          Be careful not to operate this alone.
/// @date    2020/01/01
///
// ================================================ //


class MyUdpMaster {

 public:

    MyUdpMaster();          ///< Constractor :

    virtual ~MyUdpMaster(); ///< Destractor :

    ///
    /// @name  Preparing.
    /// @{

    bool   IsInit();                             ///< Initialized or not? :

    void   SetTimeOut(const double &sec__ = -1); ///< timeout intervals [sec.], the default is not to use time-out. :

    double GetTimeOut();                         ///< Gets the value of the timeout interval. :

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

    virtual int Send(const char* data__, const int &n_byte__, const int &useSock__ = -1); ///< Sends messages (char) :

    virtual int Send(const std::vector<char> &data__, const int &useSock__ = -1);         ///< Sends messages (char) :

    virtual int Send(const std::string &data__, const int &useSock__       = -1);         ///< Sends a message string :

    /// @}
    /// @name  The current error message.
    /// @{

    std::string ErrMsg(); ///< Gets the current error message.

    /// @}
    /// @name  Displaying message packets.
    /// @{

    std::vector<std::string> DisplayPacket(const std::vector<char> &data__, const bool std_out__ = true); /// パケットの表示

    std::vector<std::string> DisplayPacket(const std::string &data__, const bool std_out__       = true); /// パケットの表示

    /// @}
    /// @name  Partner information.
    /// @{

    std::string GetIp_partner(); ///< The IP address of the partner. :

    std::string GetIp_self();    ///< The IP address of myself. :

    //

    int GetPort_partner(); ///< The port number of the partner. :

    int GetPort_self();    ///< The port number of myself. :

    /// @}
    /// @name  Message information.
    /// @{

    int GetN_recv(); ///< Gets the number of received packets or error no.(if <0) :

    int GetN_send(); ///< Gets the number of sent packets or error no.(if <0) :

    /// @}
    /// @name  Closing.
    /// @{

    virtual bool Close(const int &useSock__ = -1); ///< Closes sockets. :

    /// @}

 protected:

    ///
    /// @name  Socket creating.
    /// @{

    bool CreateSocket(); ///< Creates the socket. :

    /// @}
    /// @name  Connection utilities.
    /// @{

    sockaddr_in GetSockAddr_self();                    ///< Gets the data structure using myself. :

    sockaddr_in GetSockAddr_partner();                 ///< Gets the data structure of the destination partner. :

    bool        CheckReceivable(const int &useSock__); ///< Checks the receivable condition.

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Parameters for connections.
    /// @{

    int useSock_;  ///< Self socket using send & receive messages. :

    //

    fd_set chk_fd_; ///< A file descriptor structure for the select function. :

    //

    sockaddr_in self_sockaddr_in_;    ///< Self network data structure. :

    sockaddr_in partner_sockaddr_in_; ///< Network data structure for the partner. :

    //

    std::string self_ip_;    ///< Self IP address. :

    std::string partner_ip_; ///< IP address of the partner. :

    std::string sender_ip_;  ///< IP address of the sender which issues the current packets. :

    int self_port_;          ///< Self port number. :

    int partner_port_;       ///< Port number of the partner. :

    int sender_port_;        ///< Port number of the sender which issues the current packets. :

    /// @}
    /// @name  Information parameters.
    /// @{

    bool isInit_; ///< Initialized or not. :

    // bool status_out_; // 現在のステータスを標準出力するか？

    //

    int n_read_;  ///< The number of received packets or error no.(if <0) :

    int n_write_; ///< The number of sent packets or error no.(if <0) :

    //

    std::string error_msg_; ///< The current error message. :

    /// @}
    /// @name  Time-out parameters.
    /// @{

    bool use_timeout_;           ///< Uses time-out function or not. :

    struct timeval timeout_val_; ///< A structure for setting time-out. :

    // bool isTimeOut_; // Recv が time out したかどうかの判定用．

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

    int max_recv_size_;                 ///< The current char array size for received packets. :

    const int MAX_RECV_SIZE_C_ = 16384; ///< The maximum char array size for received packets. (default constant) :

    /// @}

};


// ================================================ //
///
/// @class   MyUdpComm : MyUdpMaster
/// @brief   My class for communicating with UDP.
/// @details This creats a object to comunicate with partners using UDP connections.
///          Please call Init() right after creating the object.
///          If one partner is selected in "Init()",
///          you can communicate with only the partner using "Recv_partner()",
///          while "MyUdpMaster::Recv()" will accept any messages.
///          If you wanna use like broadcast/multicast, you shoud set an appropriate partner's IP address.
///          For example,
///
///          @li "255.255.255.255" : limited broadcast address
///          @li "192.168.1.255" : directed broadcast address. This communicates with under the 192.168.1.*
///          @li "224.0.0.0" to "224.0.0.255" : multicast address (link-local scope)
///          @li "224.0.1.0" to "238.255.255.255" : multicast address (global scope)
///          @li "239.0.0.0" to "239.255.255.255" : multicast address (private scope)
///
/// @date    2020/01/01
///
// ================================================ //

class MyUdpComm : public MyUdpMaster {

 public:

    MyUdpComm();  ///< Constractor :

    ~MyUdpComm(); ///< Destractor :

    ///
    /// @name  Initialization.
    /// @{

    bool Init(const std::string partner_ip__, const int partner_port__, const int self_port__); ///< Initialization. :

    bool Init(const std::string partner_ip__, const int partner_port__);                        ///< Initialization. : partner_port = self_port

    /// @}
    /// @name  Receiving messages from partners.
    /// @{

    int Recv_partner(std::vector<char> &data__); ///< Receives messages from the partner (char) :

    int Recv_partner(std::string &data__);       ///< Receives messages from the partner (string) :

    /// @}

 private:

};


// ================================================ //
///
/// @class   MyUdpServer : MyUdpMaster
/// @brief   My class for the UDP server.
/// @details This creats a server object to comunicate with clients using UDP connections.
///          Please call Init() right after creating the object.
///          When a client connects this server for the first time, the client will be set the partner.
///          After that, this server will communicate only with the partner until executing "Reset()" or "Init()".
///          You can communicate with the partner using "Recv_partner()".
///          After the resetting, this server will wait for a new client connection.
///
/// @date    2020/01/01
///
// ================================================ //

class MyUdpServer : public MyUdpMaster {

 public:

    MyUdpServer();  ///< Constractor :

    ~MyUdpServer(); ///< Destractor :

    ///
    /// @name  Initialization.
    /// @{

    bool Init(const int self_port__); ///< Initialization. :

    /// @}
    /// @name  Receiving messages from partners.
    /// @{

    int Recv_partner(std::vector<char> &data__); ///< Receives messages from the client (char) :

    int Recv_partner(std::string &data__); ///< Receives messages from the client (string) :

    /// @}
    /// @name  Reset of the client connection.
    /// @{

    void Reset(); ///< Resets the client connection. :

    /// @}
    /// @name  Close.
    /// @{

    bool Close(); ///< Closes all sockets. :

    /// @}

 private:

    ///
    /// @name  Parameters.
    /// @{

    bool isPartner_; ///< Exists the partner (client) or not. :

    /// @}

};

}  // MyAll

#endif //_MYALL_UDPLIB_HPP
