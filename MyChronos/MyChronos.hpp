/// ================================================ //
///
/// @file   MyChronos.hpp
/// @brief  Class libraly of MyChronos.
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

#ifndef _MYALL_MYCHRONOS_HPP_
#define _MYALL_MYCHRONOS_HPP_

//

#include <array>

#include <map>

#include <string>

#include <vector>

//

#include "MyString.hpp"

//

namespace MyAll {

// ================================================ //
///
/// @class   MyChronos
/// @brief   My class for dealing with time.
/// @details This class can calculate things related to time and
///          convert to various date formats.
/// @date    2020/01/01
///
// ================================================ //

class MyChronos {

 public:

    MyChronos();  ///< Constractor :

    ~MyChronos(); ///< Destractor :

    ///
    /// @name  Time zone setting.
    /// @{

    void        SetMode(const std::string &mode__ = "UT"); ///< Sets the time mode; UT, LT or Zone name. :

    std::string GetMode();                                 ///< Gets the corrent time mode.

    /// @}
    /// @name  Setting the current date.
    /// @{

    bool SetCurrent(const std::string &name__ = ""); ///< Sets the current time with the name label. :

    /// @}
    /// @name  Setting the date(based the "month & day" format) manually.
    /// @{

    bool Set(
        const std::string &name__,
        const int &        year__,
        const int &        mon__,
        const int &        mday__,
        const int &        hour__,
        const int &        min__,
        const int &        sec__,
        const double &     decimal__
    ); ///< Sets the date manually. :

    bool Set(
        const std::string &name__,
        const int &        year__,
        const int &        mon__,
        const int &        mday__,
        const int &        hour__,
        const int &        min__,
        const double &     sec__
    ); ///< Sets the date manually. :

    bool Set(
        const std::string &name__,
        const std::array<int, 6> &date__,
        const double &decimal__ = 0.0
    ); ///< Sets the date manually. :

    bool Set(
        const std::string &name__,
        const double &     date__,
        const bool &       use_decimal__ = true
    ); ///< Sets the date manually. :

    /// @}
    /// @name  Setting the date(based "days of year" format) manually.
    /// @{

    bool Set_doy(
        const std::string &name__,
        const int &        year__,
        const int &        yday__,
        const int &        hour__,
        const int &        min__,
        const int &        sec__,
        const double &     decimal__
    ); ///< Sets the date manually. :

    bool Set_doy(
        const std::string &name__,
        const int &        year__,
        const int &        yday__,
        const int &        hour__,
        const int &        min__,
        const double &     sec__
    ); ///< Sets the date manually. :

    bool Set_doy(
        const std::string &name__,
        const std::array<int, 5> &date__,
        const double &decimal__ = 0.0
    ); ///< Sets the date manually. :

    bool Set_doy(
        const std::string &name__,
        const double &     date__,
        const bool &       use_decimal__ = true
    ); ///< Sets the date manually. :

    /// @}
    /// @name  Setting the date from a string.
    /// @{

    bool Set(const std::string &name__, const std::string &date__); ///< Sets the date with strings. :

    /// @}
    /// @name  Getting the date component as a double number
    /// @{

    double Get(const std::string &name__, const std::string &type__); ///< Gets the component. :

    double Get(const std::string &name__, const int &type__);         ///< Gets the component. :

    /// @}
    /// @name  Setting the date array.
    /// @{

    bool SetDate(const std::string &name__, const std::vector<double> &date__);     ///< Sets the date by an array. :

    bool SetDate_doy(const std::string &name__, const std::vector<double> &date__); ///< Sets the date by an array. :

    /// @}
    /// @name  Getting the date as an array.
    /// @{

    std::vector<double> GetDate(const std::string &name__ = "");                         ///< Gets the date as an array. :

    bool                GetDate(const std::string &name__, std::vector<double> &date__); ///< Gets the date as an array. :

    /// @}
    /// @name  Setting the date structure (time_t, struct tm).
    /// @{

    bool SetTM(const std::string &name__, const struct tm* date__, const double &decimal__  = 0.0); ///< Sets a tm structure. :

    bool SetTIME_T(const std::string &name__, const time_t &date__, const double &decimal__ = 0.0); ///< Sets a time_t structure. :

    /// @}
    /// @name  Getting the date structure (time_t, struct tm).
    /// @{

    bool GetTM(const std::string &name__, struct tm* &tm__, double &decimal__);     ///< Gets a tm structure. :

    bool GetTM(const std::string &name__, struct tm* &tm__);                        ///< Gets a tm structure. :

    bool GetTIME_T(const std::string &name__, time_t &time_t__, double &decimal__); ///< Gets a time_t structure. :

    bool GetTIME_T(const std::string &name__, time_t &time_t__);                    ///< Gets a time_t structure. :

    /// @}
    /// @name  Shifting the registered date.
    /// @{

    bool Shift(const std::string &name__, const double &add_sec__, const std::string &new_name__ = ""); ///< shifts the date. :

    /// @}
    /// @name  The differential time between two registered date.
    /// @{

    double Diff(const std::string &name_0__, const std::string &name_1__ = "");                              ///< Gets the differential time. :

    bool   Diff(const std::string &name_0__, const std::string &name_1__, time_t &sec__, double &decimal__); ///< Gets the differential time. :

    /// @}
    /// @name  Sleep functions.
    /// @{

    void Sleep(const double &sec__);

    void Sleep(const int &sec__, const int &nsec__);

    bool TickWait_nsec(const int &tick_nsec__);

    //bool TickWait_sec(const int &tick_sec__);

    //bool TickWait(const int &tick_nsec__, const int &n_tick__ = 0);

    /// @}
    /// @name  Converting between the "day of year" & "month & day" format.
    /// @{

    std::vector<int> YDOY2YMD(const int &year__, const int &yday__);                   ///< Converts the date. :

    std::vector<int> YMD2YDOY(const int &year__, const int &mon__, const int &mday__); ///< Converts the date. :

    /// @}
    /// @name  Checking a leap year.
    /// @{

    bool IsLeapYear(const int &year__); ///< Confirms a leap year. :

    /// @}
    /// @name  Calculating JDs
    /// @{

    double JD(const std::string &name__  = ""); ///< Calculates the Julian Day (JD). :

    double MJD(const std::string &name__ = ""); ///< Calculates the Modified Julian Day (MJD). :

    double TJD(const std::string &name__ = ""); ///< Calculates the Truncated Julian Day (TJD). :

    /// @}
    /// @name  Calculating a Sidereal Time(ST), Local Sidereal Time(LST)
    /// @{

    double ST(const std::string &name__ = "");                        ///< Calculates the ST. :

    double LST(const std::string &name__, const double &longitude__); ///< Calculates the LST. :

    /// @}
    /// @name  Converting between the real number and DHMS format.
    /// @{

    std::vector<double> D2DHMS(const double &date__, const std::string &date_unit__ = "day");              ///< Converts the format. :

    double              DHMS2D(const std::vector<double> &date__, const std::string &date_unit__ = "day"); ///< Converts the format. :

    /// @}
    /// @name  Printing the date.
    /// @{

    void Print(const std::string &name__ = "", const int &type__ = 1); ///< Print the date to the display. :

    /// @}
    /// @name  Formating the date.
    /// @{

    std::string YMDHMS(const std::string &name__   = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string YDOYHMS(const std::string &name__  = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string YDOY(const std::string &name__     = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string YMD(const std::string &name__      = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string HMS(const std::string &name__      = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string HMS_msec(const std::string &name__ = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string HMS_usec(const std::string &name__ = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    std::string HMS_nsec(const std::string &name__ = "", const std::string &delimiter__ = ""); ///< Gets the formatted date string. :

    //

    std::string MSEC(const std::string &name__ = ""); ///< Gets the decimal part which is formatted. :

    std::string USEC(const std::string &name__ = ""); ///< Gets the decimal part which is formatted. :

    std::string NSEC(const std::string &name__ = ""); ///< Gets the decimal part which is formatted. :

    /// @}

 private:

    /// @}
    /// @name  Checking date values.
    /// @{

    bool Check(const std::string &name__); ///< Checks all date values associated with the name. :

    bool CheckTM(const struct tm* tm__);   ///< Checks values of the tm structure. :

    /// @}
    /// @name  Date variables.
    /// @{

    std::map<std::string, time_t> time_t_;  ///< time [sec] : The origin is 1970/1/1 00:00:00 UT [sec]

    std::map<std::string, double> decimal_; ///< decimal parts of the time [sec]

    time_t time_t_current_;                 ///< The current time [sec]

    double decimal_current_;                ///< The decimal part of the current time [sec]

    int time_diff_;                         ///< Time difference calcurated by a function.

    /// @}
    /// @name  Control parameters.
    /// @{

    int isdst_;        ///< Flag of the summer time

    std::string mode_; ///< A time mode : "UT" = UTC(GMT), "LT" = localtime

    /// @}
    /// @name  Constants.
    /// @{

    const std::string WDAY_NAME[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};                                    ///< days of the week.

    const std::string MON_NAME[12] = {"Jan", "Feb", "Mar", "Apl", "May", "Jun", "Jul", "Arg", "Sep", "Oct", "Nov", "Dec"}; ///< Month words.

    const int MONTH_DAY[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};                                            ///< Days in each month.

    /// @}
    /// @name  Tools
    /// @{

    MyAll::MyString str_; ///< String manipulation object.

    /// @}

};

}  // MyAll

#endif //_MYALL_MYCHRONOS_HPP_
