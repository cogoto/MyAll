// ================================================ //
///
/// @file   MyTelnet.hpp
/// @brief  Class libraly of MyTelnet.
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

#ifndef _MYALL_MYTELNET_HPP_
#define _MYALL_MYTELNET_HPP_

//

#include <string>

#include <vector>

//

#include "MyString.hpp"

#include "MyTcp.hpp"

//

namespace MyAll {

// ================================================ //
///
/// @class   MyTelnet
/// @brief   My class for TELNET connection.
/// @details This class object works the telnet client.
/// @date    2020/01/01
///
// ================================================ //

class MyTelnet {

 public:

    MyTelnet();          ///< Constractor :

    virtual ~MyTelnet(); ///< Destractor :

    ///
    /// @name  Preparing.
    /// @{

    bool Init(
        const std::string &id_request_word__,
        const std::string &id__,
        const std::string &pwd_request_word__,
        const std::string &pwd__,
        const std::string &server_ip__,
        const int &server_port__    = 23,
        const int &wait_usec_init__ = 1000000
    ); ///< Initializes the connection. :

    //

    bool Init(
        const std::string &id__,
        const std::string &pwd__,
        const std::string &server_ip__
    ); ///< Initializes the connection. :

    //

    bool IsInit();  ///< Initialized or not. :

    //

    void SetTimeOut(const double &sec__ = 3); ///< Time-out setting. :

    //

    void SetRetry(const int &n_retry__ = 10, const int &wait_usec_retry__ = 200000); ///< Retry setting. :

    //

    void SetMaxRecvSize(const int &max_recv_size__ = 16384); ///< Maximum receiving size. [Byte] :

    //

    void SetCmdEnd(const std::vector<char> &cmd_end__ = { '\x0d' }); ///< Tail characters of the send command. :

    //

    void SetPrompt(const std::string &prompt__); ///< Sets prompt string manually. :

    //

    void PrintPrompt(); /// Prints prompt string. :

    /// @}
    /// @name  Commands.
    /// @{

    bool Cmd(
        const std::string &send_data__,
        std::vector<std::string> &recv_data__,
        const std::string &end_word__ = ""
    ); ///< Executes the command. :

    //

    bool Cmd2(
        const std::string &send_data__,
        std::vector<std::string> &recv_data__,
        const int &n_recv__   = 1,
        const bool &use_etx__ = false
    ); ///< Executes the command. :

    /// @}
    /// @name  Error messages.
    /// @{

    std::string ErrMsg(); ///< Gets the current error message. :

    /// @}
    /// @name  Information.
    /// @{

    std::string GetIp_server();   ///< Gets the server IP address. :

    //

    int GetPort_server();         ///< Gets the server port no. :

    //

    int GetN_recv();              ///< Gets the number of current received packets or error no.

    /// @}
    /// @name  Closing the connection.
    /// @{

    bool Close(); /// Closes. :

    /// @}

 private:

    ///
    /// @name  Receiving.
    /// @{

    bool Recv(
        std::vector<std::string> &data__,
        const std::string &end_word__ = ""
    ); ///< Receives the data. :

    //

    bool Recv2(
        std::vector<std::string> &data__,
        const int &n_recv__   = 1,
        const bool &use_etx__ = false
    ); ///< Receives the data. :

    /// @}
    /// @name  Sending.
    /// @{

    bool Send(const std::string &data__);     ///< Sends the data. :

    //

    bool Send_pwd(const std::string &data__); ///< Sends the password. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Parameters.
    /// @{

    std::string prompt_;                   ///< pronmt string.

    std::vector<char> cmd_end_;            ///< Characters of the command end.

    double timeout_sec_;                   ///< Time-out seconds.

    int n_retry_;                          ///< Retry no.

    int wait_usec_retry_;                  ///< The waiting time [micro soconds]

    int max_recv_size_;                    ///< The maximum receiving size [Byte]

    const int N_RETRY_C_ = 5;              ///< The number of retry. (default constant) :

    const int WAIT_USEC_RETRY_C_ = 200000; ///< The wait time for retry [micro sec.] (default constant) :

    const int MAX_RECV_SIZE_C_   = 16384;  ///< The maximum char array size for received packets. (default constant) :

    /// @}
    /// @name  Tools.
    /// @{

    MyAll::MyTcpClient client_; ///< For TCP connection.

    MyAll::MyString str_;       ///< For operating strings.

    /// @}

};

} // MyAll

#endif //_MYALL_MYTELNET_HPP
