// ================================================ //
///
/// @file   MyTelnet.cpp
/// @brief  Implementation of MyTelnet.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyTelnet.hpp"

//

#include <unistd.h>

//

#include <string>

#include <iostream>

//

namespace MyAll {

//

using namespace std;

// using uint = unsigned int;


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyTelnet::MyTelnet() {

    prompt_ = "";

    SetTimeOut();

    SetRetry();

    SetMaxRecvSize();

    SetCmdEnd();

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyTelnet::~MyTelnet() {

    if (client_.IsInit()) { Close(); }

}


// ------------------------------------------------ //
///
/// @brief      This function initializes the telnet connection.
///             The prompt string would be set automatically.
/// @param[in]  id_request_word__ : A string When requesting the user ID.
/// @param[in]  id__ : The user ID
/// @param[in]  pwd_request_word__ : A string When requesting the password.
/// @param[in]  pwd__ : The password
/// @param[in]  server_ip__ : The server ip address.
/// @param[in]  server_port__ : The server ip port.
/// @param[in]  wait_usec_init__ : The wait time after the TCP initialization.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Init(
    const string &id_request_word__,
    const string &id__,
    const string &pwd_request_word__,
    const string &pwd__,
    const string &server_ip__,
    const int &server_port__,
    const int &wait_usec_init__
) {

    if (client_.IsInit()) { Close(); }

    client_.Init(server_ip__, server_port__);

    usleep(wait_usec_init__);

    vector<string> retmp;

    //
    // ID
    //

    // cout << "[MyTelnet::Init] ID" << endl;

    if (!Recv(retmp, id_request_word__)) { return false; }

    if (!Send(id__)) { return false; }

    //
    // Password
    //

    // cout << "[MyTelnet::Init] PWD" << endl;

    if (!Recv(retmp, pwd_request_word__)) { return false; }

    if (!Send_pwd(pwd__)) { return false; }

    //
    // Get prompt
    //

    // cout << "[MyTelnet::Init] GET PROMPT" << endl;

    if (!Recv2(retmp, 1)) { return false; }

    vector<string> retmp2 = str_.Split(retmp.back(), { '\x0d', '\x0a' });

    prompt_ = retmp2.back(); // 一番最後のものがプロンプト

    // PrintPrompt();

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function initializes the telnet connection.
///            This can use in the case
///            when the string of requesting the information is as below,
///            @li String of ID request : "login:"
///            @li String of password request : "Password:"
///            @li Port No. : 23
///            The wait time after the initialization is set to 500000 microseconds.
///            The prompt string would be set automatically.
/// @param[in] id__ : The user ID
/// @param[in] pwd__ : The password
/// @param[in] server_ip__ : The server ip address.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Init(const string &id__, const string &pwd__, const string &server_ip__) {

    string id_request_word  = "login:";
    string pwd_request_word = "Password:";
    int    server_port      = 23;
    int    wait_usec_init   = 500000;

    return Init(id_request_word, id__, pwd_request_word, pwd__, server_ip__, server_port, wait_usec_init);

}


// ------------------------------------------------ //
///
/// @brief   This function confirms whether the initialization was done.
/// @return  Initialized or not.
///
// ------------------------------------------------ //


bool MyTelnet::IsInit() {

    return client_.IsInit();

}


// ------------------------------------------------ //
///
/// @brief     This function sets the time-out period.
/// @param[in] sec__ : The time-out period.
///            If you set <0, no time-out setting would be used.
/// @return    None.
///
// ------------------------------------------------ //


void MyTelnet::SetTimeOut(const double &sec__) {

    timeout_sec_ = sec__;

    client_.SetTimeOut(sec__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets retry parameters
/// @param[in] n_retry__ : The number of retrying.
/// @param[in] wait_usec_retry__ : The wait time between retrying [microseconds].
/// @return    None.
///
// ------------------------------------------------ //


void MyTelnet::SetRetry(const int &n_retry__, const int &wait_usec_retry__) {

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

    client_.SetRetry(n_retry_, wait_usec_retry_);

}


// ------------------------------------------------ //
///
/// @brief     This function sets the maximum receiving size.
/// @param[in] max_recv_size__ : The maximum receiving size.
/// @return    None.
///
// ------------------------------------------------ //


void MyTelnet::SetMaxRecvSize(const int &max_recv_size__) {

    if (max_recv_size__ >= 0) {
        max_recv_size_ = max_recv_size__;
    }
    else {
        max_recv_size_ = MAX_RECV_SIZE_C_;
    }

    client_.SetMaxRecvSize(max_recv_size_);

}


// ------------------------------------------------ //
///
/// @brief     This function sets characters at the command end.
///            These characters would be set to your command tail.
/// @param[in] cmd_end__ : Characters which would be add in the command tail.
/// @return    None.
///
// ------------------------------------------------ //


void MyTelnet::SetCmdEnd(const vector<char> &cmd_end__) {

    cmd_end_ = cmd_end__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the prompt string manually.
///            This string would be used to discriminate the telnet prompt.
///            Please set after the initialization.
/// @param[in] prompt__ : The prompt string.
/// @return    None.
///
// ------------------------------------------------ //


void MyTelnet::SetPrompt(const string &prompt__) {

    prompt_ = prompt__;

}


// ------------------------------------------------ //
///
/// @brief     This function prints the prompt string.
/// @return    None.
///
// ------------------------------------------------ //


void MyTelnet::PrintPrompt() {

    cout << "PROMPT = " << prompt_ << endl;

}


// ------------------------------------------------ //
///
/// @brief      This function receives the telnet packets.
///             This would be finished when the end string is found.
/// @param[out] data__ : The received data.
/// @param[in]  end_word__ : The end string of the received data.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Recv(vector<string> &data__, const string &end_word__) {

    int n_retry = 0;

    vector<string>().swap(data__);

    string buff_recv = "";

    string end_word  = end_word__;

    if (end_word__.size() <= 0) {
        if (prompt_.size() > 0) {
            end_word = prompt_;
        }
        else {
            return false;
        }
    }

    while (1) {

        if (client_.Recv(buff_recv) > 0) {

            vector<string> buff_recv_split = str_.Split(buff_recv, { '\x0d', '\x0a' });

            for (const auto &it : buff_recv_split) {
                data__.push_back(it);
            }

            if (buff_recv.find(end_word) != std::string::npos) { return true; }

            continue;

        }

        n_retry++;

        usleep(wait_usec_retry_);

        if (n_retry >= n_retry_) { return false; }

    }

}


// ------------------------------------------------ //
///
/// @brief      This function receives the telnet packets.
///             This would finish when the prompt string is returned.
/// @param[out] data__ : The received data.
/// @param[in]  n_recv__ : The maximum number of received data.
/// @param[in]  use_etx__ : Whether the function send an end command (Ctrl-c)
///             when the prompt string is not returned.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Recv2(vector<string> &data__, const int &n_recv__, const bool &use_etx__) {

    int n_retry = 0;

    vector<string>().swap(data__);

    string buff_recv = "";

    while (1) {

        if (client_.Recv(buff_recv) > 0) {

            vector<string> buff_recv_split = str_.Split(buff_recv, { '\x0d', '\x0a' });

            for (const auto &it : buff_recv_split) {
                data__.push_back(it);
            }

            if (static_cast<int>(data__.size()) >= n_recv__) { break; }

            if (prompt_.size() > 0 and buff_recv.find(prompt_) != std::string::npos) { return true; }

            continue;

        }

        n_retry++;
        usleep(wait_usec_retry_);

        if (n_retry >= n_retry_) { return false; }

    }

    if (use_etx__) {
        if (client_.Send(vector<char> { '\x03' }) <= 0) { return false; } // send "end of text (Ctrl-c)"
    }

    vector<string> retmp;

    if (prompt_.size() > 0) {
        return MyTelnet::Recv(retmp, prompt_); // confirmation whether the prompt is returned.
    }
    else {
        return true;
    }

}


// ------------------------------------------------ //
///
/// @brief      This function sends the telnet packets.
/// @param[in]  send_data__ : The send data.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Send(const string &send_data__) {

    string msg = send_data__;

    for (const auto &it : cmd_end_) { msg += it; }

    string buff_recv = "";

    int    n_retry   = 0;

    int    i = 0;

    int    msg_size = static_cast<int>(msg.size());

    while (i < msg_size) {

        // cout << "[MyTelnet::Send] " << msg[i] << endl;

        if (client_.Send(vector<char> { msg[i] }) == 1) {

            if (client_.Recv(buff_recv) > 0) {

                if (buff_recv[0] == msg[i]) {
                    // cout << "[MSG] " << buff_recv << endl;
                    i++;
                    continue;
                }

            }

        }

        n_retry++;
        usleep(wait_usec_retry_);

        if (n_retry >= n_retry_) { return false; }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function sends the password of telnet.
/// @param[in]  data__ : The password.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Send_pwd(const string &data__) {

    for (const auto &it : data__) {

        if (client_.Send(vector<char> { it }) != 1) { return false; } // sends each character one by one.

        usleep(wait_usec_retry_);

    }

    for (const auto &it : cmd_end_) {

        if (client_.Send(vector<char> { it }) != 1) { return false; }

        usleep(wait_usec_retry_);

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function sends and receives the telnet packets.
///             This would be finished when the end string is found in the received data.
/// @param[in]  send_data__ : The send data.
/// @param[out] recv_data__ : The received data.
/// @param[in]  end_word__  : The end string of the received data.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Cmd(const string &send_data__, vector<string> &recv_data__, const string &end_word__) {

    if (!MyTelnet::Send(send_data__)) { return false; }

    return MyTelnet::Recv(recv_data__, end_word__);

}


// ------------------------------------------------ //
///
/// @brief      This function sends and receives the telnet packets.
///             This would finish when the prompt string is returned.
/// @param[in]  send_data__ : The send data.
/// @param[out] recv_data__ : The received data.
/// @param[in]  n_recv__    : The maximum number of received data.
/// @param[in]  use_etx__   : Whether the function send an end command (Ctrl-c)
///             when the prompt string is not returned.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Cmd2(const string &send_data__, vector<string> &recv_data__, const int &n_recv__, const bool &use_etx__) {

    if (!MyTelnet::Send(send_data__)) { return false; }

    return MyTelnet::Recv2(recv_data__, n_recv__, use_etx__);

}


// ------------------------------------------------ //
///
/// @brief      This function gets the current error message.
/// @return     The current error message.
///
// ------------------------------------------------ //


string MyTelnet::ErrMsg() {

    return client_.ErrMsg();

}


// ------------------------------------------------ //
///
/// @brief      This function gets the server IP address.
/// @return     The server IP address.
///
// ------------------------------------------------ //


string MyTelnet::GetIp_server() {

    return client_.GetIp_server();

}


// ------------------------------------------------ //
///
/// @brief      This function gets the server port number.
/// @return     The server port number.
///
// ------------------------------------------------ //


int MyTelnet::GetPort_server() {

    return client_.GetPort_server();

}


// ------------------------------------------------ //
///
/// @brief      This function gets the number of current received packets.
/// @return     The number of current received packets.
///
// ------------------------------------------------ //


int MyTelnet::GetN_recv() {

    return client_.GetN_recv();

}


// ------------------------------------------------ //
///
/// @brief      This function closes the telnet connection.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyTelnet::Close() {

    string cmd_exit = "exit";

    if (!Send(cmd_exit)) { return false; }

    return client_.Close();

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
