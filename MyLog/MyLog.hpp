// ================================================ //
///
/// @file   MyLog.hpp
/// @brief  Class libraly of MyLog.
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

#ifndef _MYALL_MYLOG_HPP_
#define _MYALL_MYLOG_HPP_

//

#include <string>

#include <mutex>

//

#include "MyChronos.hpp"

#include "MyIO.hpp"

//

namespace MyAll {

// ================================================ //
///
/// @class   MyLog
/// @brief   My class for logging, printing and debugging.
/// @details This class have three kinds of methods like as below,
///          @li "Log"   : log to the file, print the log to your display.
///          @li "Print" : print regular messages(status etc.) to your display. Not to log.
///          @li "Debug" : print debug messages to your display. Not to log.
///          Each function of these methods can be switch to the opposite (print <-> no print).
/// @date    2020/01/01
///
// ================================================ //

class MyLog {

 public:

    MyLog();                              ///< Constractor :

    MyLog(const std::string &log_file__); ///< Constractor with setting a log file. :

    ~MyLog();                             ///< Destractor :

    ///
    /// @name  Preparing.
    /// @{

    void SetLogFile(const std::string &log_file__); ///< Sets a log file. :

    //

    void UseLog(
        const bool &use_log__,
        const bool &use_log_print__=false
    ); ///< Logging to the file or not. :

    //

    //void UseLogPrint(const bool &use_log_print__); ///< Prints log data to your display or not. :

    //

    void UsePrint(const bool &use_print__); ///< Prints the regular messages or not. :

    //

    void UseDebug(
        const bool &use_debug__,
        const std::string debug_stamp__ = "[DEBUG]"
    ); ///< Prints the debug messages or not. :

    //

    void UseDeMon(
        const bool &use_demon__,
        const std::string &demon_file__,
        const int  &demon_file_no__ = 7,
        const int  &demon_sec__ = 10,
        const int  &same_debug_count__ = 60,
        const int  &demon_regular_debug_count__ = 360
    ); ///< Logging the debug messages or not. :

    //

    void UseTime(
        const int &use_time__     = 0,   // 0 = no use, 1 = YMDHMS, 2 = YDOYHMS, 3 = HMS.
        const std::string &mode__ = "UT" // "UT" or "LT".
    );                                   ///< Uses the time stamp or not. :

    /// @}
    /// @name  Log.
    /// @{

    void Log(const std::string &in__); ///< Logs the string to the file. :

    //

    void Log(const std::vector<std::string> &in__); ///< Logs the strings to the file. :

    /// @}
    /// @name  Print.
    /// @{

    void Print(const std::string &in__); ///< The standard output. :

    //

    void Print(const std::vector<std::string> &in__); ///< The standard output. :

    /// @}
    /// @name  Debug.
    /// @{

    void Debug(const std::string &in__); ///< The standard output of the debug string. :

    //

    void Debug(const std::vector<std::string> &in__); ///< The standard output of the debug string. :

    //

    void Debug(
        const std::string &file__, // Please set as "__FILE__"
        const int &line__,         // Please set as "__LINE__"
        const std::string &in__ = ""
    );                             ///< The standard output of the debug string with labels of the file information. :

    //

    void Debug(
        const std::string &file__, // Please set as "__FILE__"
        const int &line__,         // Please set as "__LINE__"
        const std::vector<std::string> &in__
    );                             ///< The standard output of the debug string with labels of the file information. :

    /// @}

 private:

    ///
    /// @name  Initialization.
    /// @{

    void Init(); ///< Initializes parameters. :

    /// @}
    /// @name  Gets time.
    /// @{

    std::string GetTime();  ///< Gets time stamp. :

    /// @}
    /// @name  Debug monitoring.
    /// @{

    void MakeDeMon();

    //

    void DeMon();

    /*
    void DeMon(
        const bool &use_demon__,
        const std::string &demon_file__,
        const int &demon_file_no__,
        const double &demon_sec__,
        const std::string &curr_debug_str__,
        const int &same_debug_count__,
        MyAll::MyChronos &time__
    );
    */

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Parameters.
    /// @{

    bool use_log_;            ///< Uses logging to the file or not.

    bool use_log_print_;      ///< Prints logging data or not.

    //

    bool use_print_;          ///< Prints regular messages or not.

    //

    bool use_debug_;          ///< Prints debug messages or not.

    //

    bool use_demon_;     ///< Uses debug monitoring & logging or not.

    std::string demon_file_;  ///<

    int demon_file_no_;       ///<

    int demon_sec_;   ///< Monitor timing of debug.

    int same_debug_count_;

    int demon_regular_debug_count_;

    //

    int use_time_;            ///< Uses time stamp or not.

    std::string time_mode_;   ///< Time-zone mode.

    //

    std::string debug_stamp_; ///< The header string of debug messages.

    //

    std::string curr_log_str_;

    std::string curr_print_str_;

    std::string curr_debug_str_;

    //

    std::recursive_mutex mtx_main_;  ///<

    /// @}
    /// @name  Tools.
    /// @{

    MyAll::MyO log_;        ///< For writing to the log file.

    MyAll::MyO debug_;        ///< For writing to the log file.

    MyAll::MyChronos time_; ///< For dealing time.

    /// @}


};

} // MyAll

#endif // _MYALL_MYLOG_HPP_
