// ================================================ //
///
/// @file   MyLog.cpp
/// @brief  Implementation of MyLog.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyLog.hpp"

//

#include <iostream>

#include <thread>

//

namespace MyAll {

//

using namespace std;

// using uint = unsigned int;


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         The default is set that the object works only logging to a file.
///
// ------------------------------------------------ //


MyLog::MyLog() {

    MyLog::Init();

}


// ------------------------------------------------ //
///
/// @brief  First process (overload). @n
///         The default is set that the object works only logging to a file.
///
// ------------------------------------------------ //


MyLog::MyLog(const std::string &log_file__) {

    MyLog::Init();

    MyLog::SetLogFile(log_file__);

}

// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyLog::~MyLog() {

    if(use_demon_) {

        MyLog::UseDeMon(false, "");

        //cout << "[Mylog] Wait to stop DeMon." << flush;

        struct timespec ts;
        ts.tv_sec  = demon_sec_ + 1; // timespec will replace the "demon_sec" address.
        ts.tv_nsec = 0;

        nanosleep(&ts, NULL);

        //cout << " > End." << endl;

    }

}


// ------------------------------------------------ //
///
/// @brief  Initializes parameters of mamber valiables.
///
// ------------------------------------------------ //


void MyLog::Init() {

    use_log_       = true;
    use_log_print_ = false;
    use_print_     = false;
    use_debug_     = false;
    use_demon_     = false;

    demon_file_       = ";";
    demon_file_no_    = 7;
    demon_sec_        = 10;
    same_debug_count_ = 10;
    demon_regular_debug_count_ = 600;

    use_time_ = 0;
    time_mode_ = "UT";

    debug_stamp_ = "[DEBUG]";

    curr_log_str_   = "";
    curr_print_str_ = "";
    curr_debug_str_ = "";

    return;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the log file.
/// @param[in] log_file__ : the log file name.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::SetLogFile(const std::string &log_file__) {

    log_.ReOpen(log_file__);
    log_.Close();

}


// ------------------------------------------------ //
///
/// @brief     This function sets whether to log to file. @n
///            If this is false, no standard output of log data would be done
///            even in the case of that use_log_print__ is true.
/// @param[in] use_log__ : Uses or not.
/// @param[in] use_log_print__ : Prints log data or not.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::UseLog(const bool &use_log__, const bool &use_log_print__) {

    use_log_ = use_log__;

    use_log_print_ = use_log_print__;

    if (!use_log_) { use_log_print_ = false; }

}

/*
// ------------------------------------------------ //
///
/// @brief     This function sets whether to print log data. @n
///            This applies in the case of that data is loging to the file.
/// @param[in] use_log__ : Uses or not.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::UseLogPrint(const bool &use_log_print__) {

    use_log_print_ = use_log_print__;

}
*/

// ------------------------------------------------ //
///
/// @brief     This function sets whether to print regular messages. @n
///            These messages are not logging to the file.
///            These are useful for informing temporal data, like a current status.
/// @param[in] use_print__ : Uses or not.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::UsePrint(const bool &use_print__) {

    use_print_ = use_print__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets whether to print the debug messages. @n
///            These messages should be used only for the debugging or development of your software.
///            These messages are not logging to the file.
/// @param[in] use_debug__ : Uses or not.
/// @param[in] debug_stamp__ : A header string of debug print (the default is "[DEBUG]").
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::UseDebug(
    const bool &use_debug__,
    const std::string debug_stamp__
) {

    use_debug_   = use_debug__;

    debug_stamp_ = debug_stamp__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets whether to use the debug monitoring with the temporaly files,
///            that file name will be "demon_file__.[0-9]+".
/// @param[in] use_demon__ : Uses or not.
/// @param[in] demon_file__ : The header part of the result file of debug monitoring,
/// @param[in] demon_file_no__ : The number of result files. (default = 7)
/// @param[in] demon_sec__ : The period of debug status monitoring. (default = 10)
/// @param[in] same_debug_count__ : Whenever the number of the same debugs is a multiple of this number,
///            logging to the debug monitoring file again. (default = 10)
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::UseDeMon(
    const bool &use_demon__,
    const string &demon_file__,
    const int  &demon_file_no__,
    const int  &demon_sec__,
    const int  &same_debug_count__,
    const int  &demon_regular_debug_count__
) {

    use_demon_ = use_demon__;

    demon_file_ = demon_file__;

    demon_file_no_ = demon_file_no_;

    demon_sec_ = demon_sec__;

    same_debug_count_ = same_debug_count__;

    demon_regular_debug_count_ = demon_regular_debug_count__;

    //

    if(use_demon_) { MakeDeMon(); }

}


// ------------------------------------------------ //


void MyLog::MakeDeMon() {

    thread th;

    th = thread(&MyLog::DeMon, this);

    /*
    th = thread(
        &MyLog::DeMon,
        this,
        std::ref(use_demon_),
        std::ref(demon_file_),
        std::ref(demon_file_no_),
        std::ref(demon_sec_),
        std::ref(curr_debug_str_),
        std::ref(same_debug_count_),
        std::ref(time_)
    );
    */

    th.detach();

}


// ------------------------------------------------ //

void MyLog::DeMon() {

    struct timespec ts;
    ts.tv_sec  = demon_sec_;
    ts.tv_nsec = 0;

    //

    int yday_now = static_cast<int>(time_.Get("", "yday"));
    int yday_pre = yday_now;

    string pre_debug_str = curr_debug_str_;

    int file_no = 1;
    string debug_file = demon_file_ + "." + to_string(file_no);

    debug_.Blank(debug_file);

    int same_debug_no = 0;

    int cnt = 0;

    string demon_out_str;

    while(1) {

        if(! use_demon_) { break; }

        nanosleep(&ts, NULL);

        //

        yday_now = static_cast<int>(time_.Get("", "yday"));

        if(yday_pre != yday_now) {
            file_no += 1;
            if(file_no > demon_file_no_) { file_no = 1; }
            debug_file = demon_file_ + "." + to_string(file_no);
            debug_.Blank(debug_file);
        }

        //

        if(time_.Diff("debug") > demon_sec_) {
            if(pre_debug_str == curr_debug_str_) {
                if(same_debug_no == 0) {
                    demon_out_str = time_.YMDHMS() + " : " + curr_debug_str_ + "\n";
                    debug_.Out(debug_file, demon_out_str);
                }

                same_debug_no++;

                if(same_debug_no % same_debug_count_ == 0) {
                    demon_out_str = time_.YMDHMS() + " : " + curr_debug_str_ + " : " + to_string(same_debug_no*demon_sec_) + " sec.\n";
                    debug_.Out(debug_file, demon_out_str);
                }

            }
            else {
                same_debug_no = 0;
            }
        }
        else {
            same_debug_no = 0;
        }

        //

        if(cnt > demon_regular_debug_count_) {
            demon_out_str = time_.YMDHMS() + " [DeMon] current debug : " + curr_debug_str_ + "\n";
            debug_.Out(debug_file, demon_out_str);
            cnt = 0;
        }

        //

        yday_pre = yday_now;

        pre_debug_str = curr_debug_str_;

        cnt++;

    }

    return;

}

/*
void MyLog::DeMon(
    const bool &use_demon__,
    const string &demon_file__,
    const int &demon_file_no__,
    const double &demon_sec__,
    const string &curr_debug_str__,
    const int &same_debug_count__,
    MyChronos &time__
) {

    cout << "use_demon__        : " << &use_demon__ << endl;
    cout << "demon_file__       : " << &demon_file__ << endl;
    cout << "demon_file_no__    : " << &demon_file_no__ << endl;
    cout << "demon_sec__        : " << &demon_sec__ << endl;
    cout << "curr_debug_str__   : " << &curr_debug_str__ << endl;
    cout << "same_debug_count__ : " << &same_debug_count__ << endl;

    cout << "use_demon_        : " << &use_demon_ << endl;
    cout << "demon_file_       : " << &demon_file_ << endl;
    cout << "demon_file_no_    : " << &demon_file_no_ << endl;
    cout << "demon_sec_        : " << &demon_sec_ << endl;
    cout << "curr_debug_str_   : " << &curr_debug_str_ << endl;
    cout << "same_debug_count_ : " << &same_debug_count_ << endl;

    double demon_sec = demon_sec__;

    struct timespec ts;
    ts.tv_sec  = demon_sec; // timespec will replace the "demon_sec" address.
    ts.tv_nsec = 0;

    //

    int yday_now = static_cast<int>(time__.JD());
    int yday_pre = yday_now;

    string pre_debug_str = curr_debug_str__;

    int file_no = 1;
    string debug_file = demon_file__ + "." + to_string(file_no);

    debug_.Blank(debug_file);

    int same_debug_no = 0;

    string demon_out_str;

    while(1) {

        cout << "[DeMon]" << endl;

        if(! use_demon__) { break; }

        nanosleep(&ts, NULL);

        yday_now = static_cast<int>(time__.JD());

        //cout << "[DeMon] JD = " << yday_now << endl;

        if(yday_pre != yday_now) {
            file_no += 1;
            if(file_no > demon_file_no__) { file_no = 1; }
            debug_file = demon_file__ + "." + to_string(file_no);
            debug_.Blank(debug_file);
        }

        if(time__.Diff("debug") > demon_sec__) {
            cout << "[DeMon] 01 : " << pre_debug_str << " : " << curr_debug_str__ << endl;
            if(pre_debug_str == curr_debug_str__) {
                if(same_debug_no == 0) {
                    demon_out_str = time__.YMDHMS() + " : " + curr_debug_str__ + "\n";
                    debug_.Out(debug_file, demon_out_str);
                    cout << "[DeMon] 02 : " << demon_out_str << endl;
                }

                same_debug_no++;

                if(same_debug_no % same_debug_count__ == 0) {
                    demon_out_str = time__.YMDHMS() + " : " + curr_debug_str__ + " : " + to_string(same_debug_no*demon_sec__) + " sec.\n";
                    debug_.Out(debug_file, demon_out_str);
                    cout << "[DeMon] 03 : " << demon_out_str << endl;
                }

            }
            else {
                cout << "[DeMon] 04" << endl;
                same_debug_no = 0;
            }
        }
        else {
            same_debug_no = 0;
        }

        yday_pre = yday_now;

        pre_debug_str = curr_debug_str__;

    }

    cout << "[DeMon] end." << endl;

    return;

}
*/

// ------------------------------------------------ //
///
/// @brief     This function sets whether to put the time stamp
///            in the header of data.
/// @param[in] use_time__ : Select a type of the time stamp.
///            @arg 1      : YMDHMS (2020/01/23 01:23:45 -> 20200123012345)
///            @arg 2      : YDOYHMS (2020/365 doy 01:23:45 -> 2020365012345)
///            @arg 3      : HMS (01:23:45 -> 012345)
///            @arg others : no use.
/// @param[in] mode__ : Sets a time zone. ("UT" or "LT")
///             The default is "UT".
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::UseTime(const int &use_time__, const std::string &mode__) {

    use_time_  = use_time__;
    time_mode_ = mode__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the log string. @n
///            If the standard output of the log data is set,
///            the log string would be printed to the display.
/// @param[in] in__ : The log string.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Log(const std::string &in__) {

    if (in__.size() <= 0) { return; }

    string in__2 = in__;

    if (use_time_ > 0) {
        in__2 = MyLog::GetTime() + " " + in__2;
    }

    if (use_log_) {

        lock_guard<recursive_mutex> lock(mtx_main_);

        log_.Out(in__2 + "\n");

        if (use_log_print_) {
            cout << in__2 << endl;
        }

        curr_log_str_ = in__2;

    }

}


// ------------------------------------------------ //
///
/// @brief     This function sets the log strings. @n
///            If the standard output of the log data is set,
///            the log strings would be printed to the display.
/// @param[in] in__ : The log strings.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Log(const std::vector<std::string> &in__) {

    if (!in__.empty()) {
        for (const auto &it : in__) {
            MyLog::Log(it);
        }
    }

}


// ------------------------------------------------ //
///
/// @brief     This function prints the regular message.
/// @param[in] in__ : The message.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Print(const std::string &in__) {

    if (in__.size() <= 0) { return; } // size = 0 は無視

    string in__2 = in__;

    if (use_time_) {
        in__2 = MyLog::GetTime() + " " + in__2;
    }

    if (use_print_) {

        lock_guard<recursive_mutex> lock(mtx_main_);

        cout << in__2 << endl;

        curr_print_str_ = in__2;

    }

}


// ------------------------------------------------ //
///
/// @brief     This function prints the regular messages.
/// @param[in] in__ : The messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Print(const std::vector<std::string> &in__) {

    if (!in__.empty()) {
        for (const auto &it : in__) {
            MyLog::Print(it);
        }
    }

}


// ------------------------------------------------ //
///
/// @brief     This function prints the debug message.
/// @param[in] in__ : The debug message.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Debug(const std::string &in__) {

    if (in__.size() <= 0) { return; } // size = 0 は無視

    string in__2 = in__;

    if (debug_stamp_.size() > 0) {
        in__2 = debug_stamp_ + " " + in__2;
    }

    if (use_time_) {
        in__2 = MyLog::GetTime() + " " + in__2;
    }

    {
        lock_guard<recursive_mutex> lock(mtx_main_);

        if (use_debug_) {
            cout << in__2 << endl;
        }

        if(use_demon_) {
            time_.SetCurrent("debug");
        }

        curr_debug_str_ = in__2;
    }

}


// ------------------------------------------------ //
///
/// @brief     This function prints the debug messages.
/// @param[in] in__ : The debug messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Debug(const std::vector<std::string> &in__) {

    if (!in__.empty()) {
        for (const auto &it : in__) {
            MyLog::Debug(it);
        }
    }

}


// ------------------------------------------------ //
///
/// @brief     This function prints the debug message with the source file & line information. @n
///            The arguments should be set as below explanation.
///            This would output like @n
///            @em "[time stamp] [debug header] [file name]([line no.]) [message]"
/// @param[in] file__ : Please set as "__FILE__".
/// @param[in] line__ : Please set as "__LINE__".
/// @param[in] in__ : The debug message.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Debug(const std::string &file__, const int &line__, const std::string &in__) {

    string in__2 = file__ + "(" + to_string(line__) + ") " + in__;

    MyLog::Debug(in__2);

}


// ------------------------------------------------ //
///
/// @brief     This function prints the debug messages with the source file & line information. @n
///            The arguments should be set as below explanation.
///            This would output like @n
///            @em "[time stamp] [debug header] [file name]([line no.]) [message]"
/// @param[in] file__ : Please set as "__FILE__".
/// @param[in] line__ : Please set as "__LINE__".
/// @param[in] in__ : The debug messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyLog::Debug(const std::string &file__, const int &line__, const std::vector<std::string> &in__) {

    for (const auto &it : in__) { MyLog::Debug(file__, line__, it); }

}


// ------------------------------------------------ //
///
/// @brief     This function gets the time stamp.
/// @return    The time stamp.
///
// ------------------------------------------------ //


string MyLog::GetTime() {

    if (use_time_ == 1) { return time_.YMDHMS(); }

    if (use_time_ == 2) { return time_.YDOYHMS(); }

    if (use_time_ == 3) { return time_.HMS(); }

    return "";

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
