// ================================================ //
///
/// @file   MyMsg.cpp
/// @brief  Implementation of MyMsg.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyMsg.hpp"

// #include <cstdint>

#include <iostream>

//

namespace MyAll {

//

using namespace std;

// using ufint = uint_fast32_t;


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyMsg::MyMsg(const std::string &msg__) {

    MyMsg::Init(msg__);

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyMsg::~MyMsg() {}


// ------------------------------------------------ //
///
/// @brief   This function initializes the header and messages.
/// @return  None.
///
// ------------------------------------------------ //


void MyMsg::Init(const std::string &msg__) {

    vector<string>().swap(msg_); // initialize

    head_ = "";

    if (msg__.size() > 0) {
        msg_.push_back(msg__);
    }

}


// ------------------------------------------------ //
///
/// @brief     This function sets the message header. @n
///            Get() attach this at the combined massage head,
///            while, GetData() attach this at each message component.
/// @param[in] head_msg__ : header string.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsg::Head(const std::string &head_msg__) {

    head_ = head_msg__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets a message.
/// @param[in] msg__ : message string.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsg::Set(const std::string &msg__) {

    msg_.push_back(msg__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets messages.
/// @param[in] msg__ : message strings.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsg::Set(const std::vector<std::string> &msg__) {

    for (const auto &it : msg__) { msg_.push_back(it); }

}


// ------------------------------------------------ //
///
/// @brief     This function sets a message with a component no of the message array. @n
///            Please use to change a message which already exists, basically.
///            If the number is over the array size,
///            the array number component becomes the message,
///            new components smaller than the specified number are padded with blanks.
/// @param[in] no__  : the number of the message array.
/// @param[in] msg__ : message string.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyMsg::Set(const int &no__, const std::string &msg__) {

    int msg_size = static_cast<int>(msg_.size());

    if (no__ > msg_size) {

        for (int i = msg_size; i < no__; i++) {
            msg_.push_back("");
        }

        msg_.push_back(msg__);

    }
    else if (no__ == msg_size) {

        msg_.push_back(msg__);

    }
    else if (no__ < 0) {

        cout << "[MyMsg::Set] Cannot set the message : " << msg__ << endl;
        return false;

    }
    else {

        msg_[no__] = msg__;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function joints the same words in the given messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsg::Organize_SUM() {

    vector<string>().swap(msg_organized_);

    msg_organized_ = msg_;

    bool find;

    int  i1_max = msg_organized_.size() - 1;

    for (int i1 = i1_max; i1 > 0; i1--) {

        find = false;

        for (int i2 = i1 - 1; i2 >= 0; i2--) {
            if (msg_organized_[i1] == msg_organized_[i2]) { find = true; }
        }

        if (find) { msg_organized_.erase(msg_organized_.begin() + i1); }

    }

}


// ------------------------------------------------ //
///
/// @brief     This function joints the same adjacent words in the given messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsg::Organize_ADJUST() {

    vector<string>().swap(msg_organized_);

    msg_organized_ = msg_;

    int i1_max = msg_organized_.size() - 1;

    for (int i1 = i1_max; i1 > 0; i1--) {

        if (msg_organized_[i1 - 1] == msg_organized_[i1]) {

            msg_organized_.erase(msg_organized_.begin() + i1);

        }

    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets the message which is combined. @n
///            The header word will be attached at the combined massage head.
/// @param[in] organize__  : Organize method.
///            @arg  "ALL"    = combined return. Irregular arguments will be set "ALL".
///            @arg  "SUM"    = combined return that the same words is jointed.
///            @arg  "ADJUST" = combined return that adjacent same words is jointed.
/// @return    Combined message string.
///
// ------------------------------------------------ //


string MyMsg::Get(const std::string &organize__) {

    // organization

    if (organize__ == "SUM") {
        MyMsg::Organize_SUM();
    }
    else if (organize__ == "ADJUST") {
        MyMsg::Organize_ADJUST();
    }
    else {
        vector<string>().swap(msg_organized_);
        msg_organized_ = msg_;
    }

    if (msg_organized_.size() == 0) { return ""; }

    // remove blank components.

    auto it = msg_organized_.begin();
    while (it != msg_organized_.end()) {
        if ((*it).size() == 0) {
            it = msg_organized_.erase(it);
        }
        else {
            it++;
        }
    }

    // make result string

    string result;

    for (const auto &it : msg_organized_) { result += " " + it; }

    if (head_.size() > 0) { result = head_ + " " + result; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the messages. @n
///            The header will be attached all components.
/// @param[in] organize__ : Organize method.
///            @arg  "ALL"    = non-organized messages. Irregular arguments will be set "ALL".
///            @arg  "SUM"    = messages that the same words is jointed.
///            @arg  "ADJUST" = messages that adjacent same words is jointed.
/// @return    message strings.
///
// ------------------------------------------------ //


vector<string> MyMsg::GetData(const std::string &organize__) {

    // organization

    vector<string> result;

    if (organize__ == "SUM") {
        MyMsg::Organize_SUM();
        result = msg_organized_;
    }
    else if (organize__ == "ADJUST") {
        MyMsg::Organize_ADJUST();
        result = msg_organized_;
    }
    else {
        result = msg_;
    }

    // 空文字要素を削除

    auto it = result.begin();
    while (it != result.end()) {
        if ((*it).size() == 0) {
            it = result.erase(it);
        }
        else {
            it++;
        }
    }

    // make result strings

    if (head_.size() > 0) {
        for (auto &&it : result) { it = head_ + " " + it; }
    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function prints the message which is combined.
/// @param[in] organize__ : Organize method.
///            @arg  "ALL"    = combined return. Irregular arguments will be set "ALL".
///            @arg  "SUM"    = combined return that the same words is jointed.
///            @arg  "ADJUST" = combined return that adjacent same words is jointed.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsg::Print(const std::string &organize__) {

    cout << MyMsg::Get(organize__) << endl;

}


// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyMsgS::MyMsgS() {
}


// ------------------------------------------------ //
///
/// @brief   End process.
///
// ------------------------------------------------ //


MyMsgS::~MyMsgS() {
}


// ------------------------------------------------ //
///
/// @brief   This function clears all messages and headers.
/// @return  None.
///
// ------------------------------------------------ //


void MyMsgS::Init() {

    map<string, vector<string> >().swap(msg_);

    map<string, string>().swap(head_);

}


// ------------------------------------------------ //
///
/// @brief   This function sets a new message and a blank header.
/// @return  None.
///
// ------------------------------------------------ //


void MyMsgS::Init(const std::string &msg_name__, const std::string &msg__) {

    head_[msg_name__] = "";

    vector<string>().swap(msg_[msg_name__]); // initialize

    msg_[msg_name__].push_back(msg__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets a message header. @n
///            Get() attach this at the combined massage head,
///            while, GetData() attach this at each message component.
/// @param[in] msg_name__ : message name.
/// @param[in] head_msg__ : header string.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsgS::Head(const std::string &msg_name__, const std::string &head_msg__) {

    head_[msg_name__] = head_msg__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets a message.
/// @param[in] msg_name__ : message name.
/// @param[in] msg__ : message string.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsgS::Set(const std::string &msg_name__, const std::string &msg__) {

    msg_[msg_name__].push_back(msg__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets messages.
/// @param[in] msg_name__ : message name.
/// @param[in] msg__ : message strings.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsgS::Set(const std::string &msg_name__, const std::vector<std::string> &msg__) {

    for (const auto &it : msg__) { msg_[msg_name__].push_back(it); }

}


// ------------------------------------------------ //
///
/// @brief     This function sets a message with array no. @n
///            Please use to change a message which already exists, basically.
///            If the number is over the array size,
///            the array number component becomes the message,
///            new components smaller than the specified number are padded with blanks.
/// @param[in] msg_name__ : message name.
/// @param[in] no__  : the number of the message array.
/// @param[in] msg__ : message string.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyMsgS::Set(const std::string &msg_name__, const int &no__, const std::string &msg__) {

    int msg_size = static_cast<int>( msg_[msg_name__].size());

    if (no__ > msg_size) {

        for (int i = msg_size; i < no__; i++) {
            msg_[msg_name__].push_back("");
        }

        msg_[msg_name__].push_back(msg__);

    }
    else if (no__ == msg_size) {

        msg_[msg_name__].push_back(msg__);

    }
    else if (no__ < 0) {

        cout << "[MyMsg::Set] Cannot set the message : " << msg__ << endl;
        return false;

    }
    else {

        msg_[msg_name__][no__] = msg__;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function joints the same words in the given messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsgS::Organize_SUM(const std::string &msg_name__) {

    vector<string>().swap(msg_organized_);

    msg_organized_ = msg_[msg_name__];

    bool find;

    int  i1_max = msg_organized_.size() - 1;

    for (int i1 = i1_max; i1 > 0; i1--) {

        find = false;

        for (int i2 = i1 - 1; i2 >= 0; i2--) {
            if (msg_organized_[i1] == msg_organized_[i2]) { find = true; }
        }

        if (find) { msg_organized_.erase(msg_organized_.begin() + i1); }

    }

}


// ------------------------------------------------ //
///
/// @brief     This function joints the same adjacent words in the given messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsgS::Organize_ADJUST(const std::string &msg_name__) {

    vector<string>().swap(msg_organized_);

    msg_organized_ = msg_[msg_name__];

    int i1_max = msg_organized_.size() - 1;

    for (int i1 = i1_max; i1 > 0; i1--) {

        if (msg_organized_[i1 - 1] == msg_organized_[i1]) {

            msg_organized_.erase(msg_organized_.begin() + i1);

        }

    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets the message which is combined. @n
///            The header word will be attached at the combined massage head.
/// @param[in] msg_name__ : message name.
/// @param[in] organize__ : Organize method.
///            @arg  "ALL"    = combined return. Irregular arguments will be set "ALL".
///            @arg  "SUM"    = combined return that the same words is jointed.
///            @arg  "ADJUST" = combined return that adjacent same words is jointed.
/// @return    Combined message string.
///
// ------------------------------------------------ //


string MyMsgS::Get(const std::string &msg_name__, const std::string &organize__) {

    // organization

    if (organize__ == "SUM") {
        MyMsgS::Organize_SUM(msg_name__);
    }
    else if (organize__ == "ADJUST") {
        MyMsgS::Organize_ADJUST(msg_name__);
    }
    else {
        vector<string>().swap(msg_organized_);
        msg_organized_ = msg_[msg_name__];
    }

    if (msg_organized_.size() == 0) { return ""; }

    // 空文字要素を削除

    auto it = msg_organized_.begin();
    while (it != msg_organized_.end()) {
        if ((*it).size() == 0) {
            it = msg_organized_.erase(it);
        }
        else {
            it++;
        }
    }

    // make result string

    string result;

    for (const auto &it : msg_organized_) { result += " " + it; }

    if (head_[msg_name__].size() > 0) {
        result = head_[msg_name__] + " " + result;
    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the messages. @n
///            The header will be attached all components.
/// @param[in] msg_name__ : message name.
/// @param[in] organize__ : Organize method.
///            @arg  "ALL"    = non-organized messages. Irregular arguments will be set "ALL".
///            @arg  "SUM"    = messages that the same words is jointed.
///            @arg  "ADJUST" = messages that adjacent same words is jointed.
/// @return    Combined message string.
///
// ------------------------------------------------ //


vector<string> MyMsgS::GetData(const std::string &msg_name__, const std::string &organize__) {

    // organization

    vector<string> result;

    if (organize__ == "SUM") {
        MyMsgS::Organize_SUM(msg_name__);
        result = msg_organized_;
    }
    else if (organize__ == "ADJUST") {
        MyMsgS::Organize_ADJUST(msg_name__);
        result = msg_organized_;
    }
    else {
        result = msg_[msg_name__];
    }

    // 空文字要素を削除

    auto it = result.begin();
    while (it != result.end()) {
        if ((*it).size() == 0) {
            it = result.erase(it);
        }
        else {
            it++;
        }
    }

    // make result strings

    if (head_[msg_name__].size() > 0) {
        for (auto &&it : result) { it = head_[msg_name__] + " " + it; }
    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function prints the message which is combined.
/// @param[in] msg_name__ : message name.
/// @param[in] organize__ : Organize method.
///            @arg  "ALL"    = combined return. Irregular arguments will be set "ALL".
///            @arg  "SUM"    = combined return that the same words is jointed.
///            @arg  "ADJUST" = combined return that adjacent same words is jointed.
/// @return    None.
///
// ------------------------------------------------ //


void MyMsgS::Print(const std::string &msg_name__, const std::string &organize__) {

    cout << MyMsgS::Get(msg_name__, organize__) << endl;

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
