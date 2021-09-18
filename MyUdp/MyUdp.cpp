// ================================================ //
///
/// @file   MyUdp.cpp
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyUdp.hpp"

//

#include <cstring>

#include <unistd.h>

#include <arpa/inet.h>

#include <errno.h>

#include <sys/socket.h>

#include <sys/time.h>

//

#include <chrono>

#include <iostream>

//

namespace MyAll {

//

using namespace std;

// ================================================ //
///
/// @brief  Implementation of MyUdpMaster.
///
// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MyUdpMaster::MyUdpMaster() {

    // ip_ = "";

    partner_ip_   = ""; // IP address for the taeget partner.
    partner_port_ = -1; // port for the taeget partner.

    sender_ip_    = ""; // Sender IP address of arrival packets
    sender_port_  = -1; // Sender port of arrival packets

    self_ip_      = ""; // Self IP address
    self_port_    = -1; // Self port.

    useSock_      = 0;

    // isConnect_ = false; // Initialized by InitRecv() or InitSend()

    isInit_      = false; // Initialized or not.

    use_timeout_ = false;

    timeout_val_.tv_sec  = 0;
    timeout_val_.tv_usec = 0;

    n_retry_ = 10;             // the number of retrying.

    wait_usec_retry_ = 200000; // [micro seconds]

    max_recv_size_   = 16384;  // max receivable size.

    error_msg_       = "";

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyUdpMaster::~MyUdpMaster() { }


// ------------------------------------------------ //
///
/// @brief   This creates the socket of UDP which is used in servers & clients. @n
///          When this is failed, if immediately after it happens,
///          you can get error status with ErrMsg().
/// @return  Success the creation or not.
///
// ------------------------------------------------ //


bool MyUdpMaster::CreateSocket(void) {

    //
    // socket function :
    //
    // [0] : address family : AF_INET = IPv4
    // [1] : socket type : SOCK_DGRAM = UDP
    // [2] : protocol : 0
    //
    // The return is the socket number.
    // If errors occur, it will be -1.
    //

    useSock_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (useSock_ < 0) {

        error_msg_ = "[ERROR] Cannot create socket : " + string(strerror(errno));

        cerr << "[MyUdp:MyUdpMaster::CreateSocket] " << error_msg_ << endl;

        // perror("CreateSocket");

        return false;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief   This confirm that the initialization were operated.
/// @return  Initialized or not.
///
// ------------------------------------------------ //


bool MyUdpMaster::IsInit() {

    return isInit_;

}


// ------------------------------------------------ //
///
/// @brief     This sets the time-out interval.
/// @param[in] sec__ : The time-out interval [seconds]
///            If <0, no time-out execution.
/// @return    None.
///
// ------------------------------------------------ //


void MyUdpMaster::SetTimeOut(const double &sec__) {

    if (sec__ < 0) {

        use_timeout_ = false;

        timeout_val_.tv_sec  = 0;
        timeout_val_.tv_usec = 0;

    }
    else {

        use_timeout_ = true;

        int sec_int  = static_cast<int>( sec__ );
        int usec_int = static_cast<int>((sec__ - static_cast<double>(sec_int)) * 1000000 );

        timeout_val_.tv_sec  = static_cast<time_t>(sec_int);
        timeout_val_.tv_usec = static_cast<time_t>(usec_int);

    }

}


// ------------------------------------------------ //
///
/// @brief     This gets the time-out interval.
/// @return    The time-out interval. [seconds]
///
// ------------------------------------------------ //


double MyUdpMaster::GetTimeOut() {

    double timeout_sec = static_cast<double>(timeout_val_.tv_sec);

    timeout_sec += static_cast<double>(timeout_val_.tv_usec) / 1000000;

    return timeout_sec;

}


// --------------------------------------------------//


/*
   void MyUdpMaster::SetTimeOut(const int &sec__, const int &usec__){

    if((sec__ < 0)||(usec__ < 0)) {

        use_timeout_ = false;

        timeout_val_.tv_sec  = 0;
        timeout_val_.tv_usec = 0;

    } else {

        use_timeout_ = true;

        timeout_val_.tv_sec  = static_cast<time_t>(sec__);
        timeout_val_.tv_usec = static_cast<time_t>(usec__);

    }

   }
 */


// ------------------------------------------------ //
///
/// @brief     This sets the retry parameters, which is used in Wait() & Connect().
/// @param[in] n_retry__ : Retry numbers.
///            If <0, N_RETRY_C_ is taken on.
/// @param[in] wait_usec_retry__ : The wait time between retries. [micro seconds]
///            If <0, WAIT_USEC_RETRY_C_ is taken on.
/// @return    None.
///
// ------------------------------------------------ //


void MyUdpMaster::SetRetry(const int &n_retry__, const int &wait_usec_retry__) {

    if (n_retry__ >= 0) {
        n_retry_ = n_retry__;
    }
    else {
        n_retry_ = N_RETRY_C_;
    }

    if (wait_usec_retry__ >= 0) {
        wait_usec_retry_ = wait_usec_retry__;
    }
    else {
        wait_usec_retry_ = WAIT_USEC_RETRY_C_;
    }

}


// ------------------------------------------------ //
///
/// @brief     This sets the maximum size of receiving buffers.
/// @param[in] max_recv_size__ : The maximum char size.
///            If <0, MAX_RECV_SIZE_C_ is taken on.
/// @return    None.
///
// ------------------------------------------------ //


void MyUdpMaster::SetMaxRecvSize(const int &max_recv_size__) {

    if (max_recv_size__ >= 0) {
        max_recv_size_ = max_recv_size__;
    }
    else {
        max_recv_size_ = MAX_RECV_SIZE_C_;
    }

    // max_recv_size_ = max_recv_size__;

}


// ------------------------------------------------ //
///
/// @brief     This function prepares the self network information
///            for receiving and returns it.
/// @return    The data structure of the server.
///
// ------------------------------------------------ //


sockaddr_in MyUdpMaster::GetSockAddr_self() {

    sockaddr_in myself_in;

    bzero((char *) &myself_in, sizeof(myself_in));

    myself_in.sin_family      = AF_INET;
    myself_in.sin_port        = htons(self_port_); // Self port number
    myself_in.sin_addr.s_addr = htonl(INADDR_ANY); // Accepts all IP address, the packet is selected after receiving.

    return myself_in;

}


// ------------------------------------------------ //
///
/// @brief     This function prepares the self network information
///            for sending and returns it.
/// @return    The data structure of the server.
///
// ------------------------------------------------ //


sockaddr_in MyUdpMaster::GetSockAddr_partner() {

    sockaddr_in partner_in;

    bzero((char *) &partner_in, sizeof(partner_in));

    partner_in.sin_family      = AF_INET;
    partner_in.sin_port        = htons(partner_port_);
    partner_in.sin_addr.s_addr = inet_addr(partner_ip_.c_str());

    return partner_in;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving with UDP protocol.
///            This will accept all packets, so you might need to select these packets.
///            If the return value(n_read_) is >= char array size, you should repeat the receiving processes.
///            If the return value(n_read_) is <0, the error is occured.
///            Error codes are as belows,
///
///            @li  0 : Non received data (possibility of non connections, already)
///            @li -1 : Receiving error.
///            @li -2 : Non initialized error.
///            @li -3 : Connection error before the receiving process. (without time-out setting)
///            @li -4 : Connection error before the receiving process. (with time-out setting)
///            @li -5 : Time-out error before the receiving process.
///            @li -6 : There is no readable data in the socket before the receiving process.
///            @li -7 : Disconnection detection before the receiving process.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] data__ : A char array (char pointer) to store received data.
///            Please set an char array after the declaration like "char data[N]".
///            This array should be initialized using memset by yourself.
/// @param[in] sizeof_data__ : The maximum byte size to store received data.
///            you should set "sizeof(data__)".
///            (data__ is the first argument.)
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpMaster::Recv(char* data__, const int &sizeof_data__, const int &useSock__) {

    error_msg_ = "";

    if (isInit_) {

        int useSock;

        if (useSock__ == -1) {
            useSock = useSock_;
        }
        else {
            useSock = useSock__;
        }

        sockaddr_in sender_info_in;
        socklen_t   sender_info_in_len = sizeof(sender_info_in);

        //

        if (CheckReceivable(useSock)) {

            n_read_ = recvfrom(useSock, data__, sizeof_data__ - 1, 0, (struct sockaddr *) &sender_info_in, &sender_info_in_len); // 接続先情報を受け取る
            // n_read_ = recvfrom(useSock, data__, sizeof_data__ - 1, 0, NULL, NULL); // 接続先情報を受け取らない

            if (n_read_ > 0) {

                return n_read_;

            }
            else if (n_read_ == 0) {

                error_msg_ = "[ERROR] The connection already closed.";

                n_read_    = -7;

            }
            else if (n_read_ == -1) {

                error_msg_ = "[ERROR] Failed to receive.";

            }
            else {

                error_msg_ = "[ERROR] Iregullar error.";
                n_read_    = -1; // set to receiving error.

            }

            sender_ip_   = inet_ntoa(sender_info_in.sin_addr);
            sender_port_ = ntohs(sender_info_in.sin_port);

        }

    }
    else {

        error_msg_ = "[ERROR] Not initialized UDP connection before receiving.";

        n_read_    = -2;

    }

    //
    // error information
    //

    if (n_read_ < 0) {

        error_msg_ += " : " + string(strerror(errno));
        error_msg_ += " : sender(" + sender_ip_ + "," + to_string(sender_port_) + ")";
        error_msg_ += " : self("   + self_ip_   + "," + to_string(self_port_)   + ")";

        cerr << "[MyUdpMaster::Recv] " << error_msg_ << endl;

        // perror("recvfrom");

    }

    return n_read_;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving with UDP protocol.
///            This will accept all packets, so you might need to select these packets.
///            If the return value(n_read_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] data__ : A char vector to store received data.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpMaster::Recv(vector<char> &data__, const int &useSock__) {

    vector<char>().swap(data__);

    char data_c[max_recv_size_];

    while (1) {

        memset(data_c, '\0', max_recv_size_);

        if (MyUdpMaster::Recv(data_c, sizeof(data_c), useSock__) > 0) {

            for (int i = 0; i < n_read_; i++) { data__.push_back(data_c[i]); }

            if (n_read_ >= static_cast<int>(max_recv_size_)) { continue; }

            return n_read_;

        }

        return n_read_;

    }


}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving with UDP protocol.
///            This will accept all packets, so you might need to select these packets.
///            If the return value(n_read_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] data__ : A string to store received data.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpMaster::Recv(string &data__, const int &useSock__) {

    data__ = "";

    char data_c[max_recv_size_];

    while (1) {

        memset(data_c, '\0', max_recv_size_);

        if (MyUdpMaster::Recv(data_c, sizeof(data_c), useSock__) > 0) {

            data__ += data_c;
            // for ( int i=0; i<n_read_; i++) { data__.push_back( data_c[i] ); }

            if (n_read_ == static_cast<int>(max_recv_size_)) { continue; }

            return n_read_;

        }

        return n_read_;

    }

}


// ------------------------------------------------ //
///
/// @brief     This checks the connection before receiving.
///            If it is receivable & there is a time-out setting,
///            the waiting process will be started.
/// @param[in] useSock__ : The socket for receiving packets.
/// @return    Be receivable or not.
///
// ------------------------------------------------ //


bool MyUdpMaster::CheckReceivable(const int &useSock__) {

    //
    // Please take care of the rewriting. (this function is already speed-critical.)
    //

    FD_ZERO(&chk_fd_);
    FD_SET(useSock__, &chk_fd_);

    if (use_timeout_) {

        int result_select;

        struct timeval timeout_val = timeout_val_; // Make copy. Because the timeval value will be overwrited by the select function.

        if ((result_select = select(useSock__ + 1, &chk_fd_, NULL, NULL, &timeout_val)) > 0) {

            if (FD_ISSET(useSock__, (fd_set *) &chk_fd_)) {

                return true;

            }
            else {

                error_msg_ = "[ERROR] There is no readable data in the socket.";
                n_read_    = -6;

                return false;

            }

        }
        else if (result_select == 0) {

            error_msg_ = "[ERROR] Time out of receiving-check : > (" + to_string(timeout_val_.tv_sec) + "sec," + to_string(timeout_val_.tv_usec) + "usec) : " + string(strerror(errno));
            n_read_    = -5;

            return false;

        }
        else {

            error_msg_ = "[ERROR] Cannot connect in receiving-check : " + string(strerror(errno));
            n_read_    = -4;

            return false;

        }

    }
    else {

        if (select(useSock__ + 1, &chk_fd_, NULL, NULL, NULL) != -1) {

            if (FD_ISSET(useSock__, (fd_set *) &chk_fd_)) {

                return true;

            }
            else {

                error_msg_ = "[ERROR] There is no readable data in the socket.";
                n_read_    = -6;

                return false;

            }

        }
        else {

            error_msg_ = "[ERROR] Cannot connect in receiving-check (without time out setting) : " + string(strerror(errno));
            n_read_    = -3;

            return false;

        }

    }

}


// ------------------------------------------------ //
///
/// @brief     This is the function for sending with UDP protocol.
///            If the return value(n_write_) is <0, the error is occured.
///            error codes are as belows,
///
///            @li  0 : Non sent data (possibility of non connections, already)
///            @li -1 : Sending error.
///            @li -2 : Non initialized error.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] data__ : A char array (char pointer) to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @param[in] n_byte__ : The byte size to send.
///            If you can accept, it should be set "sizeof(data__)".
///            If the size is under "sizeof(data__)",
///            you have to loop this function for sending all.
/// @param[in] useSock__ : A socket number you wanna receive. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. default is true.
/// @return    The byte size of sent data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpMaster::Send(const char* data__, const int &n_byte__, const int &useSock__) {

    error_msg_ = "";

    if (isInit_) {

        //
        // socket 選択
        //

        int useSock;

        if (useSock__ == -1) { useSock = useSock_;  }
        else                { useSock = useSock__; }

        //
        // Sending
        //

        n_write_ = sendto(useSock, data__, n_byte__, 0, (struct sockaddr *) &partner_sockaddr_in_, sizeof(partner_sockaddr_in_));

        if (n_write_ >= 0) {

            return n_write_;

        }
        else {

            error_msg_ = "[ERROR] Failsed to send.";

        }

    }
    else {

        error_msg_ = "[ERROR] Not initialized UDP connection before sending.";

        n_write_   = -2;

    }

    //
    // エラー情報処理
    //

    if (n_write_ < 0) {

        error_msg_ += " : " + string(strerror(errno));
        error_msg_ += " : partner(" + partner_ip_ + "," + to_string(partner_port_) + ")";
        error_msg_ += " : self("    + self_ip_    + "," + to_string(self_port_)    + ") : ";

        cerr << "[MyUdpMaster::Send] " << error_msg_ << endl;

        // perror("sendto");

    }

    return n_write_;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for sending with UDP protocol.
///            If the return value(n_write_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] data__ : A char vector to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            if this is -1, useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. default is true.
/// @return    The byte size of sent data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpMaster::Send(const vector<char> &data__, const int &useSock__) {

    int  n_data = data__.size();

    char data_c[n_data];

    memset(data_c, '\0', n_data);

    memcpy(data_c, &data__[0], n_data);

    return MyUdpMaster::Send(data_c, n_data, useSock__);

}


// ------------------------------------------------ //
///
/// @brief     This is the function for sending with TCP protocol.
///            If the return value(n_write_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these error codes.
/// @param[in] data__ : data string to send.
/// @param[in] useSock__ : the socket number. (file descriptor)
///            if this is -1, useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. default is true.
/// @return    The byte size of sent data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpMaster::Send(const string &data__, const int &useSock__) {

    // int n_data = data__.size();

    // char data_c[n_data];

    // data__.copy(data_c, data__.size());

    // return MyUdpMaster::Send(data_c, n_data, useSock__);

    return MyUdpMaster::Send(data__.c_str(), data__.size(), useSock__);

}


// ------------------------------------------------ //
///
/// @brief     This function returns the current error massage.
/// @return    The current error massage.
///
// ------------------------------------------------ //


string MyUdpMaster::ErrMsg() {

    return error_msg_;

}


// ------------------------------------------------ //
///
/// @brief     This function formats the given packets and
///            displays these if you need.
/// @param[in] data__ : the packets which need to format.
/// @param[in] std_out__ : standard output in your display?
/// @return    The array of formated packets.
///            @li [0] : The number of packets
///            @li [1] : The formated string as hexadecimal numbers.
///            @li [2] : The formated string as integer numbers.
///            @li [3] : The formated string as char type.
///
// ------------------------------------------------ //


vector<string> MyUdpMaster::DisplayPacket(const vector<char> &data__, const bool std_out__) {

    int n_data = data__.size();

    if (n_data < 0) { return { }; }

    vector<string> data_str = { "N    : ", "HEX  : ", "INT  : ", "CHAR : " };

    // N_data

    data_str[0] += to_string(n_data);

    //

    for (int ip = 0; ip < n_data; ip++) {

        if (ip % 4 == 0 && ip != 0) { // Separate every 4 Byte
            data_str[1] += ", ";      // HEX
            data_str[2] += ", ";      // INT
            data_str[3] += ", ";      // CHAR
        }

        char char_tmp[32];

        sprintf(char_tmp, "%02x   ", data__[ip] & 0xff); // HEX
        data_str[1] += char_tmp;
        sprintf(char_tmp, "%-4d ",   data__[ip]);        // INT
        data_str[2] += char_tmp;
        sprintf(char_tmp, "%c    ",  data__[ip]);        // CHAR
        data_str[3] += char_tmp;

    }

    if (std_out__) {

        for (const auto &it : data_str) { cout << it << endl; }

    }

    return data_str;

}


// ------------------------------------------------ //
///
/// @brief     This function formats the given packet string and
///            displays this if you need.
/// @param[in] data__ : the packet string which need to format.
/// @param[in] std_out__ : standard output in your display?
/// @return    The array of the formated packet string.
///            @li [0] : The number of string characters.
///            @li [1] : The formated string as hexadecimal numbers.
///            @li [2] : The formated string as integer numbers.
///            @li [3] : The formated string as char type.
///
// ------------------------------------------------ //


vector<string> MyUdpMaster::DisplayPacket(const string &data__, const bool std_out__) {

    int n_data = data__.size();

    if (n_data < 0) { return { }; }

    char data_c[n_data];
    sprintf(data_c, "%s", data__.c_str());

    vector<char> data_vc;

    for (int i = 0; i < n_data; i++) {
        data_vc.push_back(data_c[i]);
    }

    return DisplayPacket(data_vc, std_out__);

}


// ------------------------------------------------ //
///
/// @brief     This function closes the socket.
/// @return    Closed or not.
///
// ------------------------------------------------ //


bool MyUdpMaster::Close(const int &useSock__) {

    error_msg_ = "";

    isInit_    = false;

    if (useSock__ >= 0) {

        if (close(useSock__) < 0) {
            error_msg_ = "[ERROR] Failed to close : " + to_string(useSock__) + " : " + string(strerror(errno));
            return false;
        }

        return true;

    }
    else if (useSock_ > 0) {

        if (close(useSock_) < 0) {
            error_msg_ = "[ERROR] Failed to close : " + to_string(useSock_) + " : " + string(strerror(errno));
            return false;
        }

        useSock_ = 0;

        return true;

    }
    else {

        error_msg_ = "[ERROR] No socket to close.";
        return false;

    }

}


// ------------------------------------------------ //
///
/// @brief     This function returns the IP address of the partner.
///            Please execute immediately after to get the precise information.
/// @return    The current IP address of the partner.
///
// ------------------------------------------------ //


string MyUdpMaster::GetIp_partner() {

    return partner_ip_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the self IP address.
/// @return    The current self IP address.
///
// ------------------------------------------------ //


string MyUdpMaster::GetIp_self() {

    return self_ip_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the port number of the partner.
///            Please execute immediately after to get the precise information.
/// @return    The current port number of the partner.
///
// ------------------------------------------------ //


int MyUdpMaster::GetPort_partner() {

    return partner_port_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the self port number.
/// @return    The current self port number.
///
// ------------------------------------------------ //


int MyUdpMaster::GetPort_self() {

    return self_port_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the number of the received packets.
///            If it is a negative number, it means an error code.
/// @return    The number of the received packets [Byte] or error codes.
///
// ------------------------------------------------ //


int MyUdpMaster::GetN_recv() {

    return n_read_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the number of the sent packets.
///            If it is a negative number, it means an error code.
/// @return    The number of the sent packets [Byte] or error codes.
///
// ------------------------------------------------ //


int MyUdpMaster::GetN_send() {

    return n_write_;

}


// --------------------------------------------------//


// ================================================ //
///
/// @brief  Implementation of MyUdpComm.
///
// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process. Initialize member variables.
///
// ------------------------------------------------ //


MyUdpComm::MyUdpComm() {

    isInit_ = false;

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyUdpComm::~MyUdpComm() {

    Close();

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this client object.
/// @param[in] partner_ip__ : The partner ip address.
/// @param[in] partner_port__ : The partner port number.
/// @param[in] self_port__ : The self ip port (int).
/// @return    Initialized or not.
///
// ------------------------------------------------ //


bool MyUdpComm::Init(const string partner_ip__, const int partner_port__, const int self_port__) {

    partner_ip_   = partner_ip__;
    partner_port_ = partner_port__;
    self_port_    = self_port__;

    if (isInit_) {
        if (!Close()) { return false; }
    }

    if (!CreateSocket()) { return false; }

    self_sockaddr_in_    = GetSockAddr_self();
    partner_sockaddr_in_ = GetSockAddr_partner();

    if (bind(useSock_, (sockaddr *) &self_sockaddr_in_, sizeof(self_sockaddr_in_)) != 0) {

        error_msg_ = "[ERROR] Bind Failed : " + string(strerror(errno));
        cerr << "[MyUdp:MyUdpComm::Init] " << error_msg_ << endl;
        // perror("Bind");

        return false;

    }

    isInit_ = true;

    return isInit_;

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this client object.
///            In this process, the self port number becomes the same as the partner's one.
/// @param[in] partner_ip__ : The partner ip address.
/// @param[in] partner_port__ : The partner port number.
/// @return    Initialized or not.
///
// ------------------------------------------------ //


bool MyUdpComm::Init(const string ip__, const int partner_port__) {

    int self_port = partner_port__;

    return MyUdpComm::Init(ip__, partner_port__, self_port);

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving from only the current partner with UDP protocol.
///            If the return value(n_read_) is <0, the error is occured.
///            Error codes are as belows,
///
///            @li  0 : Non received data (possibility of non connections, already)
///            @li -1 : Receiving error.
///            @li -2 : Non initialized error.
///            @li -3 : Connection error before the receiving process. (without time-out setting)
///            @li -4 : Connection error before the receiving process. (with time-out setting)
///            @li -5 : Time-out error before the receiving process.
///            @li -6 : There is no readable data in the socket before the receiving process.
///            @li -7 : Disconnection detection before the receiving process.
///            @li -8 : Time-out error of no connections.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] data__ : The char vector to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpComm::Recv_partner(vector<char> &data__) {

    double wait_time  = static_cast<double>(timeout_val_.tv_sec) + static_cast<double>(timeout_val_.tv_usec) / 1000000;

    auto   begin_time = chrono::system_clock::now();

    while (1) {

        if (MyUdpMaster::Recv(data__) < 0) {
            return n_read_;
        }

        if (partner_ip_ == sender_ip_) { return n_read_; }

        auto   curr_time    = chrono::system_clock::now();

        double process_time = chrono::duration_cast<chrono::milliseconds>(curr_time - begin_time).count();

        if (process_time > wait_time) {

            error_msg_ = "[ERROR] time out (no partner connection)";

            return -8;

        }

    }

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving from only the current partner with UDP protocol.
///            If the return value(n_read_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these error codes.
/// @param[in] data__ : The data string to send.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpComm::Recv_partner(string &data__) {

    double wait_time  = static_cast<double>(timeout_val_.tv_sec) + static_cast<double>(timeout_val_.tv_usec) / 1000000;

    auto   begin_time = chrono::system_clock::now();

    while (1) {

        if (MyUdpMaster::Recv(data__) < 0) {
            return n_read_;
        }

        cout << "p:: " << partner_ip_ << ":" << partner_port_ << endl;
        cout << "s:: " << sender_ip_ << ":" << sender_port_ << endl;

        if (partner_ip_ == sender_ip_ and partner_port_ == sender_port_) { return n_read_; }

        auto   curr_time    = chrono::system_clock::now();

        double process_time = chrono::duration_cast<chrono::milliseconds>(curr_time - begin_time).count();

        if (process_time > wait_time) {

            error_msg_ = "[ERROR] time out (no partner connection)";

            return -8;

        }

    }

}


// --------------------------------------------------//


// ================================================ //
///
/// @brief  Implementation of MyUdpServer.
///
// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MyUdpServer::MyUdpServer() {

    isInit_    = false;

    isPartner_ = false;

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyUdpServer::~MyUdpServer() {

    Close();

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this server object.
/// @param[in] self_port__ : The self(server) port number.
/// @return    Initialized or not.
///
// ------------------------------------------------ //


bool MyUdpServer::Init(const int self_port__) {

    self_port_ = self_port__;

    if (isInit_) {
        if (!Close()) { return false; }
    }

    if (!CreateSocket()) { return false; }

    self_sockaddr_in_ = GetSockAddr_self();

    if (bind(useSock_, (sockaddr *) &self_sockaddr_in_, sizeof(self_sockaddr_in_)) != 0) {

        error_msg_ = "[ERROR] Bind Failed : " + string(strerror(errno));
        cerr << "[MyUdp:UdpRecv::InitRecv] " << error_msg_ << endl;
        // perror("Bind");

        return false;

    }

    isInit_ = true;

    return isInit_;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving from
///            only the current partner(client) with UDP protocol.
///            If the return value(n_read_) is <0, the error is occured.
///            error codes are as belows,
///
///            @li  0 : Non received data (possibility of non connections, already)
///            @li -1 : Receiving error.
///            @li -2 : Non initialized error.
///            @li -3 : Connection error before the receiving process. (without time-out setting)
///            @li -4 : Connection error before the receiving process. (with time-out setting)
///            @li -5 : Time-out error before the receiving process.
///            @li -6 : There is no readable data in the socket before the receiving process.
///            @li -7 : Disconnection detection before the receiving process.
///            @li -8 : Time-out error of no connections.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] data__ : The char vector to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpServer::Recv_partner(vector<char> &data__) {

    if (isPartner_) {

        double wait_time  = static_cast<double>(timeout_val_.tv_sec) + static_cast<double>(timeout_val_.tv_usec) / 1000000;

        auto   begin_time = chrono::system_clock::now();

        while (1) {

            if (MyUdpMaster::Recv(data__) < 0) { return n_read_; }

            if (partner_ip_ == sender_ip_) { return n_read_; }

            auto   curr_time    = chrono::system_clock::now();

            double process_time = chrono::duration_cast<chrono::milliseconds>(curr_time - begin_time).count();

            if (process_time > wait_time) {

                error_msg_ = "[ERROR] time out (no partner connection)";

                return -8;

            }

        }

    }

    if (MyUdpMaster::Recv(data__) < 0) { return n_read_; }

    //
    // Sets a sender to the partner.
    //

    partner_ip_   = sender_ip_;
    partner_port_ = sender_port_;

    partner_sockaddr_in_ = GetSockAddr_partner();

    isPartner_ = true;

    return n_read_;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving from
///            only the current partner(client) with UDP protocol.
///            If the return value(n_read_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these error codes.
/// @param[in] data__ : The data string to send.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyUdpServer::Recv_partner(string &data__) {

    if (isPartner_) {

        double wait_time  = static_cast<double>(timeout_val_.tv_sec) + static_cast<double>(timeout_val_.tv_usec) / 1000000;

        auto   begin_time = chrono::system_clock::now();

        while (1) {

            if (MyUdpMaster::Recv(data__) < 0) { return n_read_; }

            if (partner_ip_ == sender_ip_) { return n_read_; }

            auto   curr_time    = chrono::system_clock::now();

            double process_time = chrono::duration_cast<chrono::milliseconds>(curr_time - begin_time).count();

            if (process_time > wait_time) {

                error_msg_ = "[ERROR] time out (no partner connection)";

                return -8;

            }

        }

    }

    if (MyUdpMaster::Recv(data__) < 0) { return n_read_; }

    //
    // Sets a sender to the partner.
    //

    partner_ip_   = sender_ip_;
    partner_port_ = sender_port_;

    partner_sockaddr_in_ = GetSockAddr_partner();

    isPartner_ = true;

    return n_read_;

}


// ------------------------------------------------ //
///
/// @brief     This function reset the current partner information.
/// @return    None.
///
// ------------------------------------------------ //


void MyUdpServer::Reset() {

    isPartner_    = false;

    partner_ip_   = "";
    partner_port_ = 0;

    partner_sockaddr_in_ = GetSockAddr_partner();

}


// ------------------------------------------------ //
///
/// @brief      This function closes sockets.
/// @return     Closed or not.
///
// ------------------------------------------------ //


bool MyUdpServer::Close() {

    MyUdpServer::Reset();

    return MyUdpMaster::Close();

}


// --------------------------------------------------//


}  // MyAll


// ================================================ //
