// ================================================ //
///
/// @file   MySysCmd.cpp
/// @brief  Implementation of MySysCmd.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MySysCmd.hpp"

#include <iostream>

#include <thread>

#include <unistd.h>

//

namespace MyAll {

//

using namespace std;


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MySysCmd::MySysCmd() {

    term_color_define_ = false;

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MySysCmd::~MySysCmd() { }


// ------------------------------------------------ //
///
/// @brief     This function orders the command to the system.
/// @param[in] command__ : command string.
/// @param[in] print__ : print replies? : default = true.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MySysCmd::System(const std::string &command__, const bool &print__) {

    vector<string> return_str;

    if (!MySysCmd::System(command__, return_str)) { return false; }

    if (print__) {
        for (const auto &it : return_str) {
            cout << it.c_str() << endl;
        }
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function orders the command to the system & receives the replies.
/// @param[in] command__ : command string.
/// @param[in] return__  : command replies.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MySysCmd::System(const std::string &command__, std::vector<std::string> &return__) {

    vector<string>().swap(return__);

    FILE *fp;

    if ((fp = popen(command__.c_str(), "r")) == NULL) {
        return false;
    }

    const int BUFF_MAX = 8192;
    char      buff[BUFF_MAX];

    while (fgets(buff, BUFF_MAX, fp) != NULL) {

        for (int i = 0; i < BUFF_MAX; i++) {
            if (*(buff + i) == '\n' || *(buff + i) == '\r') {
                *(buff + i) = '\0';
                break;
            }
        }

        return__.push_back(buff);

    }

    (void) pclose(fp);

    // for (auto it : return__) {
    //    cout << it.c_str() << endl;
    // };

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function executes beep sounds.
/// @details   In order not to block up main process,
///            this work is executed in the other threaded process.
/// @param[in] count__ : The number of the beep pulses.
/// @return    None.
///
// ------------------------------------------------ //


void MySysCmd::BeepSound(const int &count__) {

    int count = count__;
    if (count < 0) { count = 1; }

    thread th(&MySysCmd::BeepSound_org, this, count);
    // th.join();
    th.detach();

}


// ------------------------------------------------ //
///
/// @brief     The threaded function of beep sounds.
/// @param[in] count__ : The number of the beep pulses.
/// @return    None.
///
// ------------------------------------------------ //


void MySysCmd::BeepSound_org(const int &count__) {

    for (int i = 0; i < count__; i++) {
        cout << "\007" << flush; // execute beep!
        ///cout << "\a" << flush;
        usleep(250000);
    }

}


// ------------------------------------------------ //
///
/// @brief     The standard output with colors (charactor & background),
///            bold size & under line.
/// @details   Colors could select as belows,
///            @li black @li red     @li green @li yellow
///            @li blue  @li magenta @li cyan  @li white
/// @param[in] str__ : String you wanna print.
/// @param[in] char_color__ : Charactor color.
/// @param[in] back_color__ : Back-graund color.
/// @param[in] bold__ : Do you wanna be the string bold size? (true/false)
/// @param[in] underscore__ : Do you wanna use an underscore? (true/false)
/// @return    None.
///
// ------------------------------------------------ //


void MySysCmd::Print(const std::string &str__, const std::string char_color__, const std::string back_color__, const bool bold__, const bool underscore__) {

    if (!term_color_define_) {
        char_color_list_["black"]   = "30";
        char_color_list_["red"]     = "31";
        char_color_list_["green"]   = "32";
        char_color_list_["yellow"]  = "33";
        char_color_list_["blue"]    = "34";
        char_color_list_["magenta"] = "35";
        char_color_list_["cyan"]    = "36";
        char_color_list_["white"]   = "37";

        back_color_list_["black"]   = ";40";
        back_color_list_["red"]     = ";41";
        back_color_list_["green"]   = ";42";
        back_color_list_["yellow"]  = ";43";
        back_color_list_["blue"]    = ";44";
        back_color_list_["magenta"] = ";45";
        back_color_list_["cyan"]    = ";46";
        back_color_list_["white"]   = ";47";

        term_color_define_ = true;
    }

    string output = "\e[";

    if (bold__) { output += "1;"; }

    if (underscore__) { output += "4;"; }

    output += char_color_list_[char_color__];

    if (back_color__ != "") { output += back_color_list_[back_color__]; }

    output += "m" + str__ + "\e[m";

    cout << output.c_str() << endl;

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
