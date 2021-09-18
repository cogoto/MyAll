// ================================================ //
///
/// @file   MyTcp.cpp
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyTcp.hpp"

//

// #include <cstdio>

#include <cstring>

#include <unistd.h>

#include <arpa/inet.h>

#include <errno.h>

#include <sys/select.h>

#include <sys/socket.h>

//

#include <iostream>

//

namespace MyAll {

//

using namespace std;

//

//
// In BSD system, it need to set the SIGPIPE signal.
// Otherwise, the forced termination would be occured in a send process or others.
//

// #include <signal.h>
// signal(SIGPIPE, SIG_IGN);

// ================================================ //
///
/// @brief  Implementation of MyTcpMaster.
///
// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process.
///         Initialize member variables.
///
// ------------------------------------------------ //


MyTcpMaster::MyTcpMaster() {

    selfSock_ = -1;    // the self socket
    useSock_  = -1;    // the socket for communicating.

    isInit_   = false; // Initialized or not.
    // isConnect_ = false; // Connected partner?

    server_ip_   = "";
    client_ip_   = "";
    server_port_ = -1;
    client_port_ = -1;

    SetTimeOut();

    SetRetry();

    SetMaxRecvSize();

    error_msg_ = "";

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyTcpMaster::~MyTcpMaster() { }


// ------------------------------------------------ //
///
/// @brief   This creates the socket of TCP/IP which is used in servers & clients. @n
///          When this is failed, if immediately after it happens,
///          you can get error status with ErrMsg().
/// @return  Success the creation or not.
///
// ------------------------------------------------ //


bool MyTcpMaster::CreateSocket() {

    //
    // socket function : like a telephone body.
    //
    // [0] : address family : AF_INET = IPv4
    // [1] : socket type : SOCK_STREAM = TCP
    // [2] : protocol : 0
    //
    // The return is the socket number.
    // If errors occur, it will be -1.
    //

    selfSock_ = socket(AF_INET, SOCK_STREAM, 0);

    // selfSock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (selfSock_ == -1) {

        error_msg_ = "[ERROR] Cannot create socket : " + string(strerror(errno));

        cerr << "[MyTcp:MyTcpMaster::CreateSocket] " << error_msg_ << endl;

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


bool MyTcpMaster::IsInit() {

    return isInit_;

}


/*
   //--------------------------------------------------//
   //
   //  @brief  : server に接続できたかどうか? / client から接続があったかどうか?
   //  @arg    : -
   //  @return : bool
   //
   //--------------------------------------------------//


   bool MyTcpMaster::IsConnect() {

        return isConnect_;

   };
 */


// ------------------------------------------------ //
///
/// @brief     This sets the time-out interval.
/// @param[in] sec__ : The time-out interval [seconds]
///            If <0, no time-out execution.
/// @return    None.
///
// ------------------------------------------------ //


void MyTcpMaster::SetTimeOut(const double &sec__) {

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


double MyTcpMaster::GetTimeOut() {

    double timeout_sec = static_cast<double>(timeout_val_.tv_sec);

    timeout_sec += static_cast<double>(timeout_val_.tv_usec) / 1000000;

    return timeout_sec;

}


// ------------------------------------------------ //
///
/// @brief     This sets the retry parameters.
/// @param[in] n_retry__ : retry numbers.
///            If <0, N_RETRY_C_ is taken on.
/// @param[in] wait_usec_retry__ : A wait time between retries. [micro seconds]
///            If <0, WAIT_USEC_RETRY_C_ is taken on.
/// @return    None.
///
// ------------------------------------------------ //


void MyTcpMaster::SetRetry(const int &n_retry__, const int &wait_usec_retry__) {

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
/// @brief     This sets the maximum size of reveiving buffers.
/// @param[in] max_recv_size__ : The maximum char size.
///            If <0, MAX_RECV_SIZE_C_ is taken on.
/// @return    None.
///
// ------------------------------------------------ //


void MyTcpMaster::SetMaxRecvSize(const int &max_recv_size__) {

    if (max_recv_size__ >= 0) {
        max_recv_size_ = max_recv_size__;
    }
    else {
        max_recv_size_ = MAX_RECV_SIZE_C_;
    }

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving with TCP protocol. @n
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
///            if this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::Recv(char* data__, const int &sizeof_data__, const int &useSock__) {

    error_msg_ = "";

    if (isInit_) {

        int useSock;

        if (useSock__ == -1) {
            useSock = useSock_;
        }
        else {
            useSock = useSock__;
        }

        //

        if (PreRecv(useSock) > 0) {

            n_read_ = recv(useSock, data__, sizeof_data__, 0);
            // n_read_ = read( useSock, data__, sizeof_data__ ); // receive

            if (n_read_ > 0) {

                return n_read_;

            }
            else if (n_read_ == 0) {

                error_msg_ = "[WARN] received data is empty.";

            }
            else if (n_read_ == -1) {

                error_msg_ = "[ERROR] Failed to receive.";

            }
            else {

                error_msg_ = "[ERROR] Iregullar error.";
                n_read_    = -1; // set to receiving error.

            }

        }

    }
    else {

        error_msg_ = "[ERROR] Not initialized TCP connection before receiving.";

        n_read_    = -2;

    }

    //
    // error information
    //

    if (n_read_ <= 0) {

        string str_errno = string(strerror(errno));
        if (str_errno.size() > 0) { error_msg_ += " : " + str_errno; }

        error_msg_ += " : server(" + server_ip_ + "," + to_string(server_port_) + "), client(" + client_ip_ + "," + to_string(client_port_) + ")";

        if (n_read_ < 0) { cerr << "[MyTcpMaster::Recv] " << error_msg_ << endl; }

        // perror("read");

    }

    return n_read_;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving with TCP protocol. @n
///            If the return value(n_read_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] data__ : A char vector to store received data.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            if this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::Recv(std::vector<char> &data__, const int &useSock__) {

    vector<char>().swap(data__);

    char data_c[max_recv_size_];

    while (1) {

        memset(data_c, '\0', max_recv_size_);

        if (MyTcpMaster::Recv(data_c, sizeof(data_c), useSock__) > 0) {

            for (int i = 0; i < n_read_; i++) { data__.push_back(data_c[i]); }

            if (n_read_ >= static_cast<int>(max_recv_size_)) { continue; }

            return n_read_;

        }

        return n_read_;

    }


}


// ------------------------------------------------ //
///
/// @brief     This is the function for receiving with TCP protocol. @n
///            If the return value(n_read_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] data__ : A string to store received data.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::Recv(std::string &data__, const int &useSock__) {

    data__ = "";

    char data_c[max_recv_size_];

    while (1) {

        memset(data_c, '\0', max_recv_size_);

        if (MyTcpMaster::Recv(data_c, sizeof(data_c), useSock__) > 0) {

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
/// @brief     This is the preparing function before receiving process. @n
///            The function checks the receiving status and
///            wait connections when the time-out function is set.
///            If the return value(n_read_) is <0, the error is occured.
///            Error codes are as belows,
///
///            @li -3 : Connection error before the receiving process. (without time-out setting)
///            @li -4 : Connection error before the receiving process. (with time-out setting)
///            @li -5 : Time-out error before the receiving process.
///            @li -6 : There is no readable data in the socket before the receiving process.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    the receivable code(=1) or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::PreRecv(const int &useSock__) {

    int useSock;

    if (useSock__ == -1) { useSock = useSock_;  }
    else                { useSock = useSock__; }

    FD_ZERO(&chk_fd_);
    FD_SET(useSock, &chk_fd_);

    if (use_timeout_) {

        int result_select;

        struct timeval tv = timeout_val_; // timeval は select により書き換えられるのでコピーを用意．

        if ((result_select = select(useSock + 1, &chk_fd_, NULL, NULL, &tv)) > 0) {

            if (FD_ISSET(useSock, (fd_set *) &chk_fd_)) {

                return 1;

            }

            error_msg_ = "[ERROR] There is no readable data in the socket.";
            n_read_    = -6;

            return n_read_;

        }
        else if (result_select == 0) {

            double timeout_val_double = static_cast<double>(timeout_val_.tv_sec) + static_cast<double>(timeout_val_.tv_usec) / 1000000;
            error_msg_ = "[ERROR] Time out in receiving-check : > " + to_string(timeout_val_double);
            n_read_    = -5;

            return n_read_;

        }
        else {

            error_msg_ = "[ERROR] Cannot connect in receiving-check.";
            n_read_    = -4;

            return n_read_;

        }

    }
    else {

        if (select(useSock + 1, &chk_fd_, NULL, NULL, NULL) != -1) {

            if (FD_ISSET(useSock, (fd_set *) &chk_fd_)) {

                return 1;

            }

            error_msg_ = "[ERROR] There is no readable data in the socket.";
            n_read_    = -6;

            return n_read_;

        }
        else {

            error_msg_ = "[ERROR] Cannot connect in receiving-check(without time out setting).";
            n_read_    = -3;

            return false;

        }

    }

}


// ------------------------------------------------ //
///
/// @brief     This is the function for sending with TCP protocol. @n
///            If the return value(n_write_) is <0, the error is occured.
///            Error codes are as belows,
///
///            @li  0 : Non sent data (possibility of non connections, already)
///            @li -1 : Sending error.
///            @li -2 : Non initialized error.
///            @li -3 : Connection error before the sending process. (without time-out setting)
///            @li -4 : Connection error before the sending process. (with time-out setting)
///            @li -5 : Time-out error before the sending process.
///            @li -6 : Disconnection detection before the sending process.
///            @li -7 : There is no sendable file descriptor before the sending process.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] data__ : A char array (char pointer) to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @param[in] n_byte__ : A Byte size to send.
///            If you can accept, it should be set "sizeof(data__)".
///            If the size is under "sizeof(data__)",
///            you have to loop this function for sending all.
/// @param[in] useSock__ : A socket number you wanna receive. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. default is true.
/// @return    The byte size of sent data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::Send(const char* data__, const int &n_byte__, const int &useSock__, const bool &pre_check__) {

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

        if (PreSend(useSock) > 0) {

            //
            // 送信前チェックでの切断検知
            //

            if (pre_check__) {

                if (!CheckConnect(useSock)) { return n_write_; }

            }

            n_write_ = send(useSock, data__, n_byte__, MSG_NOSIGNAL);

            if (n_write_ > 0) {

                return n_write_;

            }
            else if (n_write_ == 0) {

                error_msg_ = "[WARN] sent data is empty.";

            }
            else {

                error_msg_ = "[ERROR] Failsed to send.";

            }

        }

    }
    else {

        error_msg_ = "[ERROR] Not initialized TCP connection before sending.";

        n_write_   = -2;

    }

    //
    // エラー情報処理
    //

    if (n_write_ <= 0) {

        string str_errno = string(strerror(errno));
        if (str_errno.size() > 0) { error_msg_ += " : " + str_errno; }

        error_msg_ += " : server(" + server_ip_ + "," + to_string(server_port_) + "), client(" + client_ip_ + "," + to_string(client_port_) + ")";

        if (n_write_ < 0) { cerr << "[MyTcpMaster::Send] " << error_msg_ << endl; }

        // perror("write");

    }

    return n_write_;

}


// ------------------------------------------------ //
///
/// @brief     This is the function for sending with TCP protocol. @n
///            If the return value(n_write_) is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] data__ : A char vector to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is -1, useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. The default is true.
/// @return    The byte size of sent data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::Send(const std::vector<char> &data__, const int &useSock__, const bool &pre_check__) {

    int  n_data = data__.size();

    char data_c[n_data];                       // 送信バッファ

    memset(data_c, '\0', n_data);              // 初期化

    memcpy(data_c, &data__[0], data__.size()); // 送信バッファにコピー ポインタを使用

    return MyTcpMaster::Send(data_c, n_data, useSock__, pre_check__);

}


/*
   int MyTcpMaster::Send(const vector<char> &data__, const int &useSock__) {

    error_msg_ = "";

    if ( isInit_ ) {

        int n_data = data__.size(); // 送信データを char に

        char Send_Buff[n_data]; //送信バッファ

        memset(Send_Buff, '\0', n_data); // 初期化

        memcpy(Send_Buff, &data__[0], n_data); //送信バッファにコピー ポインタを使用

        //
        // socket 選択
        //

        int useSock;

        if(useSock__ == -1) { useSock = useSock_;  }
        else                { useSock = useSock__; }

        //int check_sendable = PreSend(useSock);

        if( PreSend(useSock) ) {

            n_write_ = write( useSock, Send_Buff, n_data );

            if( n_write_ >= 0 ) {

                return n_write_;

            } else {

                error_msg_ = "[ERROR] Failsed to send.";

            }

        }

    } else {

        error_msg_  = "[ERROR] Not initialized TCP connection before sending.";

        n_write_ = -2;

    }


    //
    // エラー情報処理
    //

    if(n_write_ < 0) {

        error_msg_ += " : server(" + server_ip_ + "," + to_string(server_port_) + ")";
        error_msg_ += " : client(" + client_ip_ + "," + to_string(client_port_) + ")";

        cerr << "[MyTcpMaster::Send] " << error_msg_ << endl;

        perror("write");

    }

    return n_write_;

   };
 */


// ------------------------------------------------ //
///
/// @brief     This is the function for sending with TCP protocol. @n
///            If the return value(n_write_) is <0, the error is occured.
///            Please refer the explanation of the based function
///            about these error codes.
/// @param[in] data__ : A data string to send.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is -1, useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. default is true.
/// @return    The byte size of sent data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::Send(const std::string &data__, const int &useSock__, const bool &pre_check__) {

    // int n_data = data__.size();

    // char data_c[n_data]; // send buffer

    // data__.copy(data_c, data__.size());

    // return MyTcpMaster::Send(data_c, n_data, useSock__);

    return MyTcpMaster::Send(data__.c_str(), data__.size(), useSock__, pre_check__);

}


// ------------------------------------------------ //
///
/// @brief     This is the preparing function before sending process. @n
///            The function checks the sending status and
///            wait when the time-out function is set.
///            Note that this function should be used with Send().
///            If the return value(n_write_) is <0, the error is occured.
///            Error codes are as belows,
///
///            @li -3 : Connection error before the sending process. (without time-out setting)
///            @li -4 : Connection error before the sending process. (with time-out setting)
///            @li -5 : Time-out error before the sending process.
///            @li -6 : Disconnection detection before the sending process. (This is checked CheckConnect().)
///            @li -7 : There is no sendable file descriptor before the sending process.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
///            Please note finely when you wanna re-write this,
///            because this code is designed for speed.
/// @param[in] useSock__ : A socket number you wanna receive. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @param[in] pre_check__ : Connection check before sending. default is true.
/// @return    The sendable code(=1) or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::PreSend(const int &useSock__) {

    int useSock;

    if (useSock__ == -1) {
        useSock = useSock_;
    }
    else {
        useSock = useSock__;
    }

    FD_ZERO(&chk_fd_);
    FD_SET(useSock, &chk_fd_);

    if (use_timeout_) {

        int result_select;

        struct timeval tv = timeout_val_; // Make copy. Because the timeval value will be overwrited by the select function.

        if ((result_select = select(useSock + 1, NULL, &chk_fd_, NULL, &tv)) > 0) {

            if (FD_ISSET(useSock, &chk_fd_) != 0) {

                return 1;

            }

            error_msg_ = "[ERROR] There is no file descriptor of the socket.";
            n_write_   = -7;

            return n_write_;

        }
        else if (result_select == 0) {

            double timeout_val_double = static_cast<double>(timeout_val_.tv_sec) + static_cast<double>(timeout_val_.tv_usec) / 1000000;
            error_msg_ = "[ERROR] Time out in sending-check : > " + to_string(timeout_val_double);
            n_write_   = -5;

            return n_write_;

        }
        else {

            error_msg_ = "[ERROR] Cannot connect in sending-check.";
            n_write_   = -4;

            return n_write_;

        }

    }
    else {

        if (select(useSock + 1, NULL, &chk_fd_, NULL, NULL) != -1) {

            if (FD_ISSET(useSock, &chk_fd_) != 0) {

                return 1;

            }

            error_msg_ = "[ERROR] There is no file descriptor of the socket.";
            n_write_   = -7;

            return n_write_;

        }
        else {

            error_msg_ = "[ERROR] Cannot connect in sending-check(without time out setting).";
            n_write_   = -3;

            return n_write_;

        }

    }

}


// ------------------------------------------------ //
///
/// @brief     This checks the connection before sending. @n
///            If the received session of the partner is not continued,
///            this function returns false.
///            If the error occurs, n_write_ is set -6.
/// @param[in] useSock__ : A socket number you wanna receive. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    connected or not.
///
// ------------------------------------------------ //


bool MyTcpMaster::CheckConnect(const int &useSock__) {

    int useSock;

    if (useSock__ == -1) { useSock = useSock_;  }
    else                { useSock = useSock__; }

    // char* rbuf;
    // rbuf[0] = '\0';

    char rbuf[1];
    memset(rbuf, '\0', sizeof(rbuf));

    int retmp = recv(useSock, rbuf, sizeof(rbuf), MSG_DONTWAIT); // check the connection with receive

    if (!((retmp == -1) && (errno == EAGAIN))) {
        // if( !((recv(useSock,rbuf,sizeof(rbuf),MSG_DONTWAIT) == -1)&&(errno == EAGAIN)) ) {

        error_msg_  = "[ERROR] Connection already closed before sending. : ";
        error_msg_ += "N=" + to_string(retmp) + ",errno=" + to_string(errno);

        n_write_    = -6;

        return false;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function send a message, and wait to receive the replies immediately. @n
///            If the return value is <0, the error is occured.
///            Error codes are as belows,
///
///            @li  -1 : Sending error.
///            @li  -2 : Non initialized error.
///            @li  -3 : Connection error before the sending process. (without time-out setting)
///            @li  -4 : Connection error before the sending process. (with time-out setting)
///            @li  -5 : Time-out error before the sending process.
///            @li  -6 : Disconnection detection before the sending process.
///            @li -11 : Receiving error.
///            @li -12 : Connection error before the receiving process. (without time-out setting)
///            @li -13 : Connection error before the receiving process. (with time-out setting)
///            @li -14 : Time-out error before the receiving process.
///            @li -15 : There is no readable data in the socket before the receiving process.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] send_data__ : A char vector to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @param[in] recv_data__ : A char vector to store received data.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::SR(const std::vector<char> &send_data__, std::vector<char> &recv_data__, const int &useSock__) {

    error_msg_ = "";

    vector<char>().swap(recv_data__);

    int n_err = 0;

    if (isInit_) {

        int  n_send_data = send_data__.size();

        char Send_Buff[n_send_data];

        memset(Send_Buff, '\0', n_send_data);

        memcpy(Send_Buff, &send_data__[0], n_send_data);

        char Recv_Buff[max_recv_size_];

        memset(Recv_Buff, '\0', max_recv_size_);

        //

        int useSock;

        if (useSock__ == -1) { useSock = useSock_;  }
        else                { useSock = useSock__; }

        if (PreSend(useSock) > 0) {

            n_write_ = send(useSock, Send_Buff, n_send_data, MSG_NOSIGNAL);

            if (n_write_ >= 0) {

                if (PreRecv(useSock) > 0) {

                    n_read_ = recv(useSock, Recv_Buff, sizeof(Recv_Buff), 0);
                    // n_read_ = read( useSock, Recv_Buff, sizeof(Recv_Buff) );

                    if (n_read_ >= 0) {

                        for (int i = 0; i < n_read_; i++) { recv_data__.push_back(Recv_Buff[i]); }

                        return n_read_;

                    }
                    else if (n_read_ == -1) {

                        error_msg_ = "[ERROR] Failed to receive.";
                        n_err      = n_read_ - 10;

                    }
                    else {

                        error_msg_ = "[ERROR] Iregullar error.";
                        n_read_    = -1; // set to receiving error.
                        n_err      = n_read_ - 10;

                    }

                }
                else {

                    n_err = n_read_ - 10;

                }

            }
            else {

                error_msg_ = "[ERROR] Failsed to send.";
                n_err      = n_write_;

            }

        }

    }
    else {

        error_msg_ = "[ERROR] Not initialized TCP connection before command(send->recv).";
        n_err      = -2;
        n_read_    = n_err;
        n_write_   = n_err;

    }

    //
    // error information.
    //

    if (n_err < 0) {

        string str_errno = string(strerror(errno));
        if (str_errno.size() > 0) { error_msg_ += " : " + str_errno; }

        error_msg_ += " : server(" + server_ip_ + "," + to_string(server_port_) + "), client(" + client_ip_ + "," + to_string(client_port_) + ")";

        cerr << "[MyTcpMaster::SR] " << error_msg_ << endl;

    }

    return n_err;

}


// ------------------------------------------------ //
///
/// @brief     This function send a message, and wait to receive the replies immediately. @n
///            If the return value is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] send_data__ : A string to send.
/// @param[in] recv_data__ : A string to store received data.
/// @param[in] useSock__ : the socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::SR(const std::string &send_data__, std::string &recv_data__, const int &useSock__) {

    recv_data__ = "";

    vector<char> send_data_vchar(send_data__.begin(), send_data__.end());

    vector<char> recv_data_vchar;

    int result = MyTcpMaster::SR(send_data_vchar, recv_data_vchar, useSock__);

    if (result > 0) {

        for (const auto &it : recv_data_vchar) { recv_data__.push_back(it); }

    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function receive a message, and send the reply. @n
///            If the return value is <0, the error is occured.
///            Error codes are as belows,
///
///            @li  -1 : Sending error.
///            @li  -2 : Non initialized error.
///            @li  -3 : Connection error before the sending process. (without time-out setting)
///            @li  -4 : Connection error before the sending process. (with time-out setting)
///            @li  -5 : Time-out error before the sending process.
///            @li -11 : Receiving error.
///            @li -12 : Connection error before the receiving process. (without time-out setting)
///            @li -13 : Connection error before the receiving process. (with time-out setting)
///            @li -14 : Time-out error before the receiving process.
///            @li -15 : There is no readable data in the socket before the receiving process.
///
///            If immediately after these happen,
///            you can get error messages with ErrMsg().
/// @param[in] recv_data__ : A char vector to store received data.
/// @param[in] send_data__ : A char vector to send.
///            Please set NULL ('\0') in the tail component if you need.
/// @param[in] wait_usec__ : A wait time between the receive and send process. [micro seconds]
///            The default value is 0.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            If this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::RS(std::vector<char> &recv_data__, const std::vector<char> &send_data__, const int wait_usec__, const int &useSock__) {

    error_msg_ = "";

    vector<char>().swap(recv_data__);

    int n_err = 0;

    if (isInit_) {

        int  n_send_data = send_data__.size();

        char Send_Buff[n_send_data];

        memset(Send_Buff, '\0', n_send_data);

        memcpy(Send_Buff, &send_data__[0], n_send_data);

        char Recv_Buff[max_recv_size_];

        memset(Recv_Buff, '\0', max_recv_size_);

        //

        int useSock;

        if (useSock__ == -1) { useSock = useSock_;  }
        else                { useSock = useSock__; }

        if (PreRecv(useSock) > 0) {

            n_read_ = recv(useSock, Recv_Buff, sizeof(Recv_Buff), 0);
            // n_read_ = read( useSock, Recv_Buff, sizeof(Recv_Buff) );

            if (n_read_ >= 0) {

                usleep(wait_usec__);

                if (PreSend(useSock) > 0) {

                    n_write_ = send(useSock, Send_Buff, n_send_data, MSG_NOSIGNAL);

                    if (n_write_ >= 0) {

                        for (int i = 0; i < n_read_; i++) { recv_data__.push_back(Recv_Buff[i]); } // 出力データ処理

                        return n_read_;

                    }
                    else {

                        error_msg_ = "[ERROR] Failsed to send.";
                        n_err      = n_write_;

                    }

                }

            }
            else if (n_read_ == -1) {

                error_msg_ = "[ERROR] Failed to receive.";
                n_err      = n_read_ - 10;

            }
            else {

                error_msg_ = "[ERROR] Iregullar error.";
                n_read_    = -1; // set to receiving error
                n_err      = n_read_ - 10;
            }

        }
        else {

            n_err = n_read_ - 10;

        }

    }
    else {

        error_msg_ = "[ERROR] Not initialized TCP connection before command(recv->send).";
        n_err      = -2;
        n_read_    = n_err;
        n_write_   = n_err;

    }

    //
    // error information
    //

    if (n_err < 0) {

        string str_errno = string(strerror(errno));
        if (str_errno.size() > 0) { error_msg_ += " : " + str_errno; }

        error_msg_ += " : server(" + server_ip_ + "," + to_string(server_port_) + "), client(" + client_ip_ + "," + to_string(client_port_) + ")";

        cerr << "[MyTcpMaster::RS] " << error_msg_ << endl;

    }

    return n_err;

}


// ------------------------------------------------ //
///
/// @brief     This function receive a message, and send the reply. @n
///            If the return value is <0, the error is occured.
///            Please refer the explanation of the based function of overloads
///            about these errors.
/// @param[in] recv_data__ : A string to store received data.
/// @param[in] send_data__ : A string to send.
/// @param[in] wait_usec__ : A wait time between the receiving and sending process. [micro seconds]
///            The default value is 0.
/// @param[in] useSock__ : A socket number. (file descriptor)
///            if this is default(=-1), useSock_ is taken on.
/// @return    The byte size of received data or error code (<0).
///
// ------------------------------------------------ //


int MyTcpMaster::RS(std::string &recv_data__, const std::string &send_data__, const int wait_usec__, const int &useSock__) {

    recv_data__ = "";

    vector<char> send_data_vchar(send_data__.begin(), send_data__.end());

    vector<char> recv_data_vchar;

    int result = MyTcpMaster::RS(recv_data_vchar, send_data_vchar, useSock__);

    if (result > 0) {

        for (const auto &it : recv_data_vchar) { recv_data__.push_back(it); }

    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the current error massage.
/// @return    The current error massage.
///
// ------------------------------------------------ //


string MyTcpMaster::ErrMsg() {

    return error_msg_;

}


// ------------------------------------------------ //
///
/// @brief     This function formats the given packets and
///            displays these if you need.
/// @param[in] data__ : The packets which need to format.
/// @param[in] std_out__ : Standard output in your display?
/// @return    The array of formated packets.
///            @li [0] : The number of packets
///            @li [1] : The formated string as hexadecimal numbers.
///            @li [2] : The formated string as integer numbers.
///            @li [3] : The formated string as char type.
///
// ------------------------------------------------ //


std::vector<std::string> MyTcpMaster::DisplayPacket(const std::vector<char> &data__, const bool std_out__) {

    int n_data = data__.size();

    if (n_data < 0) { return { }; }

    vector<string> data_str = { "N    : ", "HEX  : ", "INT  : ", "CHAR : " };

    data_str[0] += to_string(n_data); // N

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
/// @param[in] data__ : The packet string which need to format.
/// @param[in] std_out__ : Standard output in your display?
/// @return    The array of the formated packet string.
///            @li [0] : The number of string characters.
///            @li [1] : The formated string as hexadecimal numbers.
///            @li [2] : The formated string as integer numbers.
///            @li [3] : The formated string as char type.
///
// ------------------------------------------------ //


std::vector<std::string> MyTcpMaster::DisplayPacket(const std::string &data__, const bool std_out__) {

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
/// @brief     This function returns the IP address of the server.
/// @return    The current IP address of the server.
///
// ------------------------------------------------ //


std::string MyTcpMaster::GetIp_server() {

    return server_ip_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the IP address of the client.
/// @return    The current IP address of the client.
///
// ------------------------------------------------ //


std::string MyTcpMaster::GetIp_client() {

    return client_ip_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the port number of the server.
/// @return    The current port numper of the server.
///
// ------------------------------------------------ //


int MyTcpMaster::GetPort_server() {

    return server_port_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the port number of the client.
/// @return    The current port numper of the client.
///
// ------------------------------------------------ //


int MyTcpMaster::GetPort_client() {

    return client_port_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the number of the received packets.
///            If it is a negative number, it means an error code.
/// @return    the number of the received packets [Byte] or error codes.
///
// ------------------------------------------------ //


int MyTcpMaster::GetN_recv() {

    return n_read_;

}


// ------------------------------------------------ //
///
/// @brief     This function returns the number of the sent packets.
///            If it is a negative number, it means an error code.
/// @return    the number of the sent packets [Byte] or error codes.
///
// ------------------------------------------------ //


int MyTcpMaster::GetN_send() {

    return n_write_;

}


// ------------------------------------------------ //
///
/// @fn        MyTcpMaster::useSock_
/// @brief     This member variable is the socket number
///            using to send and receive messages.
///            In the client case, this is equal to "selfSock_".
///            In the server case, this would be the socket which is given by accept function
///            when a client connect to this server.
///
// ------------------------------------------------ //


// --------------------------------------------------//
//
// @brief  :
//
// --------------------------------------------------//

/*
   void MyTcpMaster::SetStatusOut(const bool true_or_false__){

        status_out_ = true_or_false__;

   };
 */

// --------------------------------------------------//
//
// @brief  :
// @return : (bool)
//
// --------------------------------------------------//

/*
   bool MyTcpMaster::IsTimeOut(){

        return isTimeOut_;

   };
 */


// --------------------------------------------------//


// ================================================ //
///
/// @brief  Implementation of MyTcpClient.
///
// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MyTcpClient::MyTcpClient() { }


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyTcpClient::~MyTcpClient() {

    Close();

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this client object.
/// @param[in] server_ip__   : The server ip address.
/// @param[in] server_port__ : The server port number.
/// @param[in] client_ip__   : The self(client) ip address. default is "", corresponding to "127.0.0.1".
/// @param[in] client_port__ : The self(client) ip port (int).
///            If you do not wanna specify it, please set -1 (default argument).
/// @return    Initialized or not.
///
// ------------------------------------------------ //


bool MyTcpClient::Init(const std::string &server_ip__, const int &server_port__, const std::string &client_ip__, const int &client_port__) {

    return MyTcpClient::InitClient(server_ip__, server_port__, client_ip__, client_port__);

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this client object.
///            the client IP addres is set to "127.0.0.1".
/// @param[in] server_ip__   : The server ip address.
/// @param[in] server_port__ : The server port number.
/// @param[in] client_port__ : The self(client) ip port (int).
///            If you do not wanna specify it, please set -1 (* need to set this argument).
/// @return    Initialized or not.
///
// ------------------------------------------------ //


bool MyTcpClient::Init(const std::string &server_ip__, const int &server_port__, const int &client_port__) {

    return MyTcpClient::InitClient(server_ip__, server_port__, "", client_port__);

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this client object.
///            This is the based function of MyTcpClient::Init().
/// @param[in] server_ip__   : The server ip address.
/// @param[in] server_port__ : The server port number.
/// @param[in] client_ip__   : The self(client) ip address. default is "", corresponding to "127.0.0.1".
/// @param[in] client_port__ : The self(client) ip port (int).
///            If you do not wanna specify it, please set -1 (default argument).
/// @return    Initialized or not.
///
// ------------------------------------------------ //


bool MyTcpClient::InitClient(const std::string &server_ip__, const int &server_port__, const std::string &client_ip__, const int &client_port__) {

    isInit_      = false;

    server_ip_   = server_ip__;    // IP address of the partner server

    server_port_ = server_port__;  // port of the partner server

    if (client_ip__.size() > 0) {
        client_ip_ = client_ip__;
    }
    else {
        client_ip_ = "127.0.0.1";
    }

    client_port_ = client_port__; // self port

    if (CreateSocket()) {
        useSock_ = selfSock_;     // The client use the socket created by the socket function for communications.
    }
    else {
        return false;
    }

    //
    // If you wanna set the port number, you need to execute "bind" about the client.
    // If you don't set the port number, the connect function will bind self port properly.
    //

    if (client_port__ > 0) {

        //
        // Sets a data structure of the client.
        // (like registering own telephone number.
        //  If you don't execute "bind", it is like a calling from a public phone,
        //  that is, the client would not care about own telephone number.)
        //

        sockaddr_in client_in = GetSockAddr_client();

        if (bind(useSock_, (sockaddr *) &client_in, sizeof(client_in)) != 0) {

            error_msg_ = "[ERROR] Bind Failed : " + string(strerror(errno));
            cout << "[MyTcp:MyTcpClient::InitClient] " << error_msg_ << endl;
            perror("bind");

            return false;

        }

    }

    //
    // Sets a data structure of the partner server and connects.
    // (like registering the telephone number of your friend and ring just once at the first time.)
    //

    sockaddr_in server_in = GetSockAddr_server();

    if (!Connect(server_in)) {

        return false;

    }

    isInit_ = true;

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function connects the server.
/// @param[in] server_in__ : The data structure of the server.
/// @return    Connected or not.
///
// ------------------------------------------------ //


bool MyTcpClient::Connect(const sockaddr_in &server_in__) {

    for (int i = 0; i <= n_retry_; i++) {

        if (connect(selfSock_, (struct sockaddr *) &server_in__, sizeof(server_in__)) == 0) {

            // isConnect_ = true;

            return true;

        }
        else {

            usleep(wait_usec_retry_);

        }

    }

    error_msg_ = "[ERROR] Failed to open TCP Connection with (" + server_ip_ + "," + to_string(server_port_) + ") : " + string(strerror(errno));

    cerr << "[MyTcp:MyTcpClient::Connect] " << error_msg_ << endl;

    // perror("Connect");

    // isConnect_ = false;

    return false;

}


// ------------------------------------------------ //
///
/// @brief     This function prepares the information of the server
///            and returns it.
/// @return    The data structure of the server.
///
// ------------------------------------------------ //


sockaddr_in MyTcpClient::GetSockAddr_server() {

    sockaddr_in server_in;

    bzero((char *) &server_in, sizeof(server_in));

    server_in.sin_family      = AF_INET;
    server_in.sin_port        = htons(server_port_);           // htons : convert endianness
    server_in.sin_addr.s_addr = inet_addr(server_ip_.c_str()); // convert char pointer address to binaly.

    return server_in;

}


// ------------------------------------------------ //
///
/// @brief     This function prepares the information of the client (itself)
///            and returns it.
/// @return    The data structure of the client.
///
// ------------------------------------------------ //


sockaddr_in MyTcpClient::GetSockAddr_client() {

    // string localhost_ip = "127.0.0.1";

    sockaddr_in client_in;

    bzero((char *) &client_in, sizeof(client_in));

    client_in.sin_family      = AF_INET;
    client_in.sin_port        = htons(client_port_);
    client_in.sin_addr.s_addr = inet_addr(client_ip_.c_str());
    // client_in.sin_addr.s_addr = inet_addr( localhost_ip.c_str() );

    return client_in;

}


// ------------------------------------------------ //
///
/// @brief     This function closes the socket.
/// @return    Closed or not.
///
// ------------------------------------------------ //


bool MyTcpClient::Close() {

    //
    // Since useSock_ is selfSock_ in the client case,
    // you should close selfSock_ only.
    //

    isInit_ = false;
    // isConnect_ = false;

    if (selfSock_ != -1) {

        if (close(selfSock_) == -1) {

            error_msg_ = "[ERROR] Failed to close client socket : " + string(strerror(errno));
            cerr << "[MyTcpClient::Close] " << error_msg_ << endl;

            return false;

        }
        else {

            selfSock_ = -1;
            useSock_  = -1;

        }

    }

    return true;

}


// --------------------------------------------------//


// ================================================ //
///
/// @brief  Implementation of MyTcpServer.
///
// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MyTcpServer::MyTcpServer() {

    SetListenNo();

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyTcpServer::~MyTcpServer() {

    Close();

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this server object. @n
///            You can begin immediately to wait client connections
///            after initializing, if you need.
/// @param[in] server_port__ : The self(server) port number.
/// @param[in] wait_client__ : Begining to wait after the initialization?
/// @return    Initialized or not.
///            If it is set to wait, this will returns accept client status.
///
// ------------------------------------------------ //


bool MyTcpServer::Init(const int &server_port__, const bool &wait_client__) {

    return InitServer("", server_port__, wait_client__);

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this server object. @n
///            You can begin immediately to wait client connections
///            after initializing, if you need.
/// @param[in] server_ip__   : The self(server) IP address.
/// @param[in] server_port__ : The self(server) port number.
/// @param[in] wait_client__ : Begining to wait after the initialization?
/// @return    Initialized or not.
///            If it is set to wait, this will returns accept client status.
///
// ------------------------------------------------ //


bool MyTcpServer::Init(const std::string &server_ip__, const int &server_port__, const bool &wait_client__) {

    return InitServer(server_ip__, server_port__, wait_client__);

}


// ------------------------------------------------ //
///
/// @brief     This function Initializes this server object. @n
///            This is the based function of MyTcpServer::Init().
/// @param[in] server_ip__   : The self(server) IP address.
/// @param[in] server_port__ : The self(server) port number.
/// @param[in] wait_client__ : Begining to wait after the initialization?
/// @return    Initialized or not.
///            If it is set to wait, this will returns accept client status.
///
// ------------------------------------------------ //


bool MyTcpServer::InitServer(const std::string &server_ip__, const int &server_port__, const bool &wait_client__) {

    isInit_      = false;

    server_ip_   = server_ip__;

    server_port_ = server_port__;

    client_ip_   = "";

    client_port_ = -1;

    //
    // Create the client reception socket
    // (like a phone body installation.)
    //

    if (!CreateSocket()) { return false; }

    //

    sockaddr_in server_in = GetSockAddr_server(); // like preparing a phone number

    //
    // The charm for no wonder time-waiting after kill the servers.
    //

    int yes = 1;
    setsockopt(selfSock_, SOL_SOCKET, SO_REUSEADDR, (const char *) &yes, sizeof(yes));

    //
    // Bind process.
    // (like a telephone connection to the line.)
    //

    if (bind(selfSock_, (struct sockaddr *) &server_in, sizeof(server_in)) != 0) {

        error_msg_ = "[ERROR] Bind failed : " + string(strerror(errno));
        cout << "[MyTcp:MyTcpServer::InitServer] " << error_msg_ << endl;
        perror("bind");

        return false;

    }

    //
    // Set the server connectable.
    // The listen number is about 5 to 10, usually.
    // (this is like the maximum register number of voicemails.)
    //

    if (listen(selfSock_, n_listen_) != 0) {

        error_msg_ = "[ERROR] Listen failed : " + string(strerror(errno));
        cout << "[MyTcp:MyTcpClient::InitServer] " << error_msg_ << endl;
        perror("listen");

        return false;

    }

    //

    isInit_ = true;

    if (wait_client__) { return Wait(); } // like a waiting for someone's phone.

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function prepares the information of the server
///            and returns it.
/// @return    The data structure of the server.
///
// ------------------------------------------------ //


sockaddr_in MyTcpServer::GetSockAddr_server() {

    sockaddr_in server_in;

    bzero((char *) &server_in, sizeof(server_in));

    server_in.sin_family = AF_INET;
    server_in.sin_port   = htons(server_port_);

    if (server_ip_.size() == 0) {
        server_in.sin_addr.s_addr = INADDR_ANY; // Allowing connections from all clients.
    }
    else {
        server_in.sin_addr.s_addr = inet_addr(server_ip_.c_str());
    }

    return server_in;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the listen number.
/// @param[in] n_listen__ : The listen number.
/// @return    None.
///
// ------------------------------------------------ //


void MyTcpServer::SetListenNo(const int &n_listen__) {

    if (n_listen__ > 0) {
        n_listen_ = n_listen__;
    }
    else {
        n_listen_ = 5;
    }

}


// ------------------------------------------------ //
///
/// @brief     This function begins to wait client connections.
/// @return    Connected or not.
///
// ------------------------------------------------ //


bool MyTcpServer::Wait() {

    string client_ip;
    int    client_port;

    return MyTcpServer::Wait(client_ip, client_port);
    // return MyTcpServer::WaitClientConnection( client_ip, client_port );

}


// ------------------------------------------------ //
///
/// @brief      This function begins to wait client connections.
/// @param[out] client_ip__ : The accepted client IP address.
/// @param[out] client_port__ : The accepted client port numper.
/// @return     Accepted or not.
///
// ------------------------------------------------ //


bool MyTcpServer::Wait(std::string &client_ip__, int &client_port__) {

    // StdOut("[MyTcp:MyTcpServer::Wait] Waiting Client....");

    struct sockaddr_in client;
    unsigned int       length = sizeof(client);

    for (int i = 0; i <= n_retry_; i++) {

        useSock_      = accept(selfSock_, (struct sockaddr *) &client, (socklen_t *) &length);

        client_ip_    = inet_ntoa(client.sin_addr);
        client_port_  = ntohs(client.sin_port);

        client_ip__   = client_ip_;
        client_port__ = client_port_;

        if (useSock_ == -1) {         // failed the acception

            usleep(wait_usec_retry_); // waiting before re-try.

        }
        else {

            return true;

        }

    }

    error_msg_ = "[ERROR] Accept Failed : " + string(strerror(errno));
    cerr << "[MyTcp:MyTcpServer::Wait] " << error_msg_ << endl;

    // perror("accept");

    // isConnect_ = false; // accept detection

    return false;

}


// ------------------------------------------------ //
///
/// @brief      This function begins to wait client connections.
///             This function will be used in the case of a multi-threaded process
///             which shold have many sockets at the same time.
/// @param[out] useSock__ : The socket of the accepted client.
///             This socket will be used to communicate the client.
/// @return     Accepted or not.
///
// ------------------------------------------------ //


bool MyTcpServer::Wait(int &useSock__) {

    string client_ip;
    int    client_port;

    return MyTcpServer::Wait(useSock__, client_ip, client_port);
    // return MyTcpServer::WaitClientConnection(useSock__, client_ip, client_port);

}


// ------------------------------------------------ //
///
/// @brief      This function begins to wait client connections.
///             This function will be used in the case of a multi-threaded process
///             which shold have many sockets at the same time.
/// @param[out] useSock__ : The socket of the accepted client.
///             This socket will be used to communicate the client.
/// @param[out] client_ip__ : The accepted client IP address.
/// @param[out] client_port__ : The accepted client port numper.
/// @return     Accepted or not.
///
// ------------------------------------------------ //


bool MyTcpServer::Wait(int &useSock__, std::string &client_ip__, int &client_port__) {

    // StdOut("[MyTcp:MyTcpServer::Wait] Waiting Client....");

    struct sockaddr_in client;
    unsigned int       length = sizeof(client);

    for (int i = 0; i <= n_retry_; i++) {

        useSock__     = accept(selfSock_, (struct sockaddr *) &client, (socklen_t *) &length);

        client_ip_    = inet_ntoa(client.sin_addr);
        client_port_  = ntohs(client.sin_port);

        client_ip__   = client_ip_;
        client_port__ = client_port_;

        if (useSock__ < 0) {

            usleep(wait_usec_retry_); // waiting before re-try.

        }
        else {

            return true;

        }

    }

    error_msg_ = "[ERROR] Accept Failed : "  + string(strerror(errno)) + " : " + to_string(useSock_);
    cerr << "[MyTcp:MyTcpServer::Wait] " << error_msg_ << endl;
    perror("accept");

    // isConnect_ = false; // accept detection

    return false;

}


// ------------------------------------------------ //
///
/// @brief      This function closes the server socket of client contacts.
/// @return     Closed or not.
///
// ------------------------------------------------ //


bool MyTcpServer::CloseServer() {

    isInit_ = false;

    if (selfSock_ != -1) {

        if (close(selfSock_) == -1) {

            error_msg_ = "[ERROR] Failed to close server socket : " + string(strerror(errno));
            cerr << "[MyTcpServer::CloseServer] " << error_msg_ << endl;

            return false;

        }
        else {

            selfSock_ = -1;

        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function closes the socket for communicating with the client.
/// @return     Closed or not.
///
// ------------------------------------------------ //


bool MyTcpServer::CloseClient() {

    // isConnect_ = false;

    if (useSock_ != -1) {

        if (close(useSock_) == -1) {

            error_msg_ = "[ERROR] Failed to close client connection : " + string(strerror(errno));
            cerr << "[MyTcpServer::CloseClient] " << error_msg_ << endl;

            return false;

        }
        else {

            useSock_ = -1;

        }

    }
    else {

        error_msg_ = "[ERROR] Invalid socket with client : " + to_string(useSock_);
        cerr << "[MyTcpServer::CloseClient] " << error_msg_ << endl;

        return false;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief         This function closes the socket which was used to communicate with clients.
/// @param[in,out] useSock__ : The socket which was used to communicate with the client.
///                If it is successed, this will change to -1.
/// @return        Closed or not.
///
// ------------------------------------------------ //


bool MyTcpServer::CloseClient(int &useSock__) {

    // isConnect_ = false; // accept detection

    if (useSock__ >= 0) {

        if (close(useSock__) == -1) {

            error_msg_ = "[ERROR] Failed to close client connection : " + string(strerror(errno));
            cerr << "[MyTcpServer::CloseClient] " << error_msg_ << endl;

            return false;

        }
        else {

            useSock__ = -1;

        }

        return true;

    }
    else {

        error_msg_ = "[ERROR] Invalid socket with client : " + to_string(useSock__);
        cerr << "[MyTcpServer::CloseClient] " << error_msg_ << endl;

        return false;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief         This function closes all sockets (the server and client one).
/// @return        Closed or not.
///
// ------------------------------------------------ //


bool MyTcpServer::Close() {

    bool   result    = true;

    string error_msg = "";

    if (!MyTcpServer::CloseClient()) {
        error_msg += error_msg_;
        result     = false;
    }

    if (!MyTcpServer::CloseServer()) {
        if (!result) { error_msg += "; "; }
        error_msg += error_msg_;
        result     = false;
    }

    if (!result) { error_msg_ = error_msg; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief         This function closes the server socket and the given client one.
/// @param[in,out] useSock__ : The socket which was used to communicate with the client.
///                If it is successed, this will change to -1.
/// @return        Closed or not.
///
// ------------------------------------------------ //


bool MyTcpServer::Close(int &useSock__) {

    bool   result    = true;

    string error_msg = "";

    if (!MyTcpServer::CloseClient(useSock__)) {
        error_msg += error_msg_;
        result     = false;
    }

    if (!MyTcpServer::CloseServer()) {
        if (!result) { error_msg += "; "; }
        error_msg += error_msg_;
        result     = false;
    }

    if (!result) { error_msg_ = error_msg; }

    return result;

}


// --------------------------------------------------//


}  // MyAll


// ================================================ //
