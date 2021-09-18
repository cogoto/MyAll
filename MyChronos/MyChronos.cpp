// ================================================ //
///
/// @file   MyChronos.cpp
/// @brief  Implementation of MyChronos.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //


#include "MyChronos.hpp"

//

#include <cmath>

#include <ctime>

//

#include <chrono>

#include <iostream>

//

namespace MyAll {

//

using namespace std;

using namespace std::chrono;

// using uint = uint_fast32_t;
// using uint = unsigned int;


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         The default time mode is set to "UT".
///
// ------------------------------------------------ //


MyChronos::MyChronos() {

    // mode_ = "UT";

    isdst_     = 0;

    time_diff_ = 0;

    SetMode("UT");

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyChronos::~MyChronos() {}


// ------------------------------------------------ //
///
/// @brief     This function sets time-zone mode.
/// @param[in] mode__ : Specifys the time zone.
///            @arg UT = UTC
///            @arg LT = the local time (depends on the environment in use.)
/// @return    None.
///
// ------------------------------------------------ //


void MyChronos::SetMode(const string &mode__) {

    mode_ = mode__;

    if ((mode__ != "UT") && (mode__ != "LT")) {

        cout << "[MyChronos::SetMode] Not UT/LT => Deal as UT" << endl;

        mode_ = "UT";

    }

    //
    // time difference between UTC and local time.
    //

    time_t     time_t_0 = 0;

    struct tm *tm_gm, *tm_local;

    time_t     time_t_gm, time_t_local;

    tm_gm        = gmtime(&time_t_0);

    time_t_gm    = mktime(tm_gm);

    tm_local     = localtime(&time_t_0);

    time_t_local = mktime(tm_local);

    time_diff_   = time_t_local - time_t_gm;

    // printf("time difference between UT & LT = %d hour\n",time_diff_/3600);


}


// ------------------------------------------------ //
///
/// @brief     This function gets time-zone mode.
/// @return    the time-zone mode.
///
// ------------------------------------------------ //


string MyChronos::GetMode() {

    return mode_;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the current date.
/// @param[in] name__ : A label of the date.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::SetCurrent(const string &name__) {

    // auto now = system_clock::now() ;

    auto        now      = high_resolution_clock::now();

    seconds     now_sec  = duration_cast<seconds>(now.time_since_epoch());

    nanoseconds now_nsec = duration_cast<nanoseconds>(now.time_since_epoch() - now_sec);

    time_t_current_  = now_sec.count();

    decimal_current_ = now_nsec.count() * 1.0e-9;
    // nsec_current_   = now_nsec.count();

    if (name__.size() > 0) {

        time_t_[name__]  = time_t_current_;

        decimal_[name__] = decimal_current_;
        // nsec_[name__]   = nsec_current_;

    }

    if (time_t_current_ <= 0) {return false;}

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "month & day" format.
/// @param[in] name__    : A label of the date.
/// @param[in] year__    : Year.
/// @param[in] mon__     : Month.
/// @param[in] mday__    : Days in each month.
/// @param[in] hour__    : Hour.
/// @param[in] min__     : Minutes.
/// @param[in] sec__     : Seconds.
/// @param[in] decimal__ : The decimal part of the seconds.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set(const string &name__, const int &year__, const int &mon__, const int &mday__, const int &hour__, const int &min__, const int &sec__, const double &decimal__) {

    if (name__.size() <= 0) {return false;}

    struct tm tm_0;

    tm_0.tm_year  = year__ - 1900;
    tm_0.tm_mon   = mon__ - 1;
    tm_0.tm_mday  = mday__;
    tm_0.tm_yday  = 0;
    tm_0.tm_wday  = 0;
    tm_0.tm_hour  = hour__;
    tm_0.tm_min   = min__;
    tm_0.tm_sec   = sec__;
    tm_0.tm_isdst = isdst_;

    return SetTM(name__, &tm_0, decimal__);

    /*
        if( !CheckTM(&tm_0) || decimal__ > 1 || decimal__ < 0 ) {
            cerr << "[MyChronos::Set] Irregular of input date" << endl;
            return false;
        };

        time_t_[name__] = mktime(&tm_0); // seconds from 1970/01/01 00:00:00 LT

        if(mode_ == "UT") { time_t_[name__] += time_diff_; }; // convert on UTC

        if(time_t_[name__] == -1) { return false; };

        decimal_[name__] = decimal__;

        if( !Check(name__) ) {

            time_t_.erase( time_t_.find(name__) );
            //auto itr1 = time_t_.find(name__);
            //if( itr1 != time_t_.end() ) { time_t_.erase(itr1); };

            decimal_.erase( decimal_.find(name__) );
            //auto itr2 = decimal_.find(name__);
            //if( itr2 != decimal_.end() ) { decimal_.erase(itr1); };

            return false;

        }

        return true;
     */
}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "month & day" format.
/// @param[in] name__    : A label of the date.
/// @param[in] year__    : Year.
/// @param[in] mon__     : Month.
/// @param[in] mday__    : Days in each month.
/// @param[in] hour__    : Hour.
/// @param[in] min__     : Minutes.
/// @param[in] sec__     : Seconds.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set(const string &name__, const int &year__, const int &mon__, const int &mday__, const int &hour__, const int &min__, const double &sec__) {

    int    sec_int     = static_cast<int>( sec__ );
    double sec_decimal = sec__ - sec_int;

    return Set(name__, year__, mon__, mday__, hour__, min__, sec_int, sec_decimal);

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "month & day" format.
/// @param[in] name__    : A label of the date.
/// @param[in] date__    : A date array.
///            @arg [0] = Year.
///            @arg [1] = Month.
///            @arg [2] = Days in each month.
///            @arg [3] = Hour.
///            @arg [4] = Minutes.
///            @arg [5] = Seconds.
/// @param[in] decimal__ : The decimal part of the seconds.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set(const string &name__, const array<int, 6> &date__, const double &decimal__) {

    return Set(name__, date__[0], date__[1], date__[2], date__[3], date__[4], date__[5], decimal__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "month & day" format.
/// @param[in] name__ : A label of the date.
/// @param[in] date__ : A date number which is formated "YYYYMMDDHHMMSS.S"
///            ex. 2020/01/23 01:23:45.6789 -> 20200123012345.6789
/// @param[in] use_decimal__ : Uses the decimal part or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set(const string &name__, const double &date__, const bool &use_decimal__) {

    double date_0 = date__;
    int    date_i[7];
    date_i[0] = static_cast<int>(date_0 / 1.0e+10);
    date_0   -= date_i[0] * 1.0e+10;
    date_i[1] = static_cast<int>(date_0 / 1.0e+08);
    date_0   -= date_i[1] * 1.0e+08;
    date_i[2] = static_cast<int>(date_0 / 1.0e+06);
    date_0   -= date_i[2] * 1.0e+06;
    date_i[3] = static_cast<int>(date_0 / 1.0e+04);
    date_0   -= date_i[3] * 1.0e+04;
    date_i[4] = static_cast<int>(date_0 / 1.0e+02);
    date_0   -= date_i[4] * 1.0e+02;
    date_i[5] = static_cast<int>(date_0);
    date_0   -= date_i[5];
    double decimal_0 = date_0;

    if (use_decimal__) {
        return Set(name__, date_i[0], date_i[1], date_i[2], date_i[3], date_i[4], date_i[5], decimal_0);
    }
    else {
        return Set(name__, date_i[0], date_i[1], date_i[2], date_i[3], date_i[4], date_i[5], 0.0);
    }

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "days of year" format.
/// @param[in] name__    : A label of the date.
/// @param[in] year__    : Year.
/// @param[in] yday__    : Days of the year.
/// @param[in] hour__    : Hour.
/// @param[in] min__     : Minutes.
/// @param[in] sec__     : Seconds.
/// @param[in] decimal__ : The decimal part of the seconds.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set_doy(const string &name__, const int &year__, const int &yday__, const int &hour__, const int &min__, const int &sec__, const double &decimal__) {

    vector<int> ymd = YDOY2YMD(year__, yday__);

    return Set(name__, year__, ymd[1], ymd[2], hour__, min__, sec__, decimal__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "days of year" format.
/// @param[in] name__    : A label of the date.
/// @param[in] year__    : Year.
/// @param[in] yday__    : Days of the year.
/// @param[in] hour__    : Hour.
/// @param[in] min__     : Minutes.
/// @param[in] sec__     : Seconds.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set_doy(const string &name__, const int &year__, const int &yday__, const int &hour__, const int &min__, const double &sec__) {

    vector<int> ymd = YDOY2YMD(year__, yday__);

    return Set(name__, year__, ymd[1], ymd[2], hour__, min__, sec__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "days of year" format.
/// @param[in] name__    : A label of the date.
/// @param[in] date__    : A date array.
///            @arg [0] = Year.
///            @arg [1] = Days of the year.
///            @arg [2] = Hour.
///            @arg [3] = Minutes.
///            @arg [4] = Seconds.
/// @param[in] decimal__ : The decimal part of the seconds.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set_doy(const string &name__, const array<int, 5> &date__, const double &decimal__) {

    vector<int> ymd = YDOY2YMD(date__[0], date__[1]);

    return Set(name__, date__[0], ymd[1], ymd[2], date__[2], date__[3], date__[4], decimal__);

}


// ------------------------------------------------ //
///
/// @brief     This function sets date manually with the "days of year" format.
/// @param[in] name__    : A label of the date.
/// @param[in] date__ : A date number which is formated "YYYYDDDHHMMSS.S".
///            ex. 2020/123 doy 01:23:45.6789 -> 2020123012345.6789
/// @param[in] use_decimal__ : Uses the decimal part or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set_doy(const string &name__, const double &date__, const bool &use_decimal__) {

    // printf("[MyChronos::Set_doy] date__ = %13.0f\n",date__);

    double date_0 = date__;
    int    date_i[6];
    date_i[0] = static_cast<int>(date_0 / 1.0e+09);
    date_0   -= date_i[0] * 1.0e+09;
    date_i[1] = static_cast<int>(date_0 / 1.0e+06);
    date_0   -= date_i[1] * 1.0e+06;
    date_i[2] = static_cast<int>(date_0 / 1.0e+04);
    date_0   -= date_i[2] * 1.0e+04;
    date_i[3] = static_cast<int>(date_0 / 1.0e+02);
    date_0   -= date_i[3] * 1.0e+02;
    date_i[4] = static_cast<int>(date_0);
    date_0   -= date_i[4];
    double decimal_0 = date_0;

    if (use_decimal__) {
        return Set_doy(name__, date_i[0], date_i[1], date_i[2], date_i[3], date_i[4], decimal_0);
    }
    else {
        return Set_doy(name__, date_i[0], date_i[1], date_i[2], date_i[3], date_i[4], 0.0);
    }

}


// ------------------------------------------------ //
///
/// @brief     This function sets the date referting to the strings.
/// @param[in] name__    : A label of the date.
/// @param[in] date__ : A date string which is formated as below,
///
///            @arg [year]*[month]*[day]*[hour]*[min]*[sec] : the "month & day" format
///            @arg [year]*[doy]*[hour]*[min]*[sec]         : the "days of years" format
///            @arg YYYYMMDDHHMMSS.S : ex. 2020/01/23 01:23:45.6789   -> 20200123012345.6789
///            @arg YYYYDDDHHMMSS.S  : ex. 2020/123 doy 01:23:45.6789 -> 2020123012345.6789
///
///            In the above, "*" is a delimiter which is other than numbers & dots.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Set(const string &name__, const string &date__) {

    vector<string> date_0 = str_.Split(date__, R"(\.)");

    // cout << "[MyChronos::Set] " << date__ << " [0] " << date_0[0] << ", [1] " << date_0[1] << endl;

    string date_str_0 = str_.Replace(date_0[0], R"(\D)", "");
    string date_str_1 = "0";
    if (date_0.size() > 1) {
        date_str_1 = str_.Replace(date_0[1], R"(\D)", "");
    }

    if (date_str_0.size() == 13) {

        //
        // Date format : "YYYYDDDHHMMSS.S"
        //

        double d0 = str_.Str2d(date_str_0);

        double d1 = str_.Str2d(date_str_1) / pow(10, date_str_1.size());

        return Set_doy(name__, d0, d1);

    }
    else if (date_0[0].size() == 14) {

        //
        // Date format : "YYYYMMDDHHMMSS.S"
        //

        double d0 = str_.Str2d(date_str_0);

        double d1 = str_.Str2d(date_str_1) / pow(10, date_str_1.size());

        return Set(name__, d0, d1);

    }

    // ------------//

    vector<double> date_vd;
    vector<int>    date_vi;

    str_.Split_double(date_vd, date__);

    //
    // Date format : "[year]*[doy]*[hour]*[min]*[sec]"
    //

    if (date_vd.size() == 5) {

        for (const auto &it : date_vd) {
            date_vi.push_back(static_cast<int>(it));
        }

        return Set_doy(name__, date_vi[0], date_vi[1], date_vi[2], date_vi[3], date_vd[4]);

    }

    //
    // Date format : "[year]*[month]*[day]*[hour]*[min]*[sec]"
    //

    if (date_vd.size() == 6) {

        for (const auto &it : date_vd) {
            date_vi.push_back(static_cast<int>(it));
        }

        return Set(name__, date_vi[0], date_vi[1], date_vi[2], date_vi[3], date_vi[4], date_vd[5]);

    }

    cerr << "[MyChronos::Set] No match of input string" << endl;

    return false;

}


// ------------------------------------------------ //
///
/// @brief     This function gets date component.
/// @param[in] name__    : A label of the date.
///            If "", this uses the current time.
/// @param[in] type__ : A type string of date component you wanna get.
///            @arg "year"    : Years.
///            @arg "mon"     : Months.
///            @arg "mday"    : days og the month.
///            @arg "hour"    : Hours.
///            @arg "min"     : Minutes.
///            @arg "sec"     : Seconds.
///            @arg "decimal" : The decimal part of the soconds.
///            @arg "yday"    : Days of the year.
///            @arg "mode"    : time-zone mode. UT = 0, LT = 1, Others = 2.
///            @arg "wday"    : Days of the week. Sunday = 0, ..., Saturday = 6.
///            @arg "isdst"   : The flag of summer time. No use = 0, use = >0.
/// @return    The date component value.
///            If failed, return -1.
///
// ------------------------------------------------ //


double MyChronos::Get(const string &name__, const string &type__) {

    struct tm* tm_0;
    double     decimal_0;

    if (!MyChronos::GetTM(name__, tm_0, decimal_0)) {
        cerr << "[MyChronos::GetDate] No time : " << name__ << endl;
        return -1;
    }

    if (type__ == "year" || type__ == "Y") {
        return tm_0->tm_year + 1900.0;
    }
    else if (type__ == "mon" || type__ == "month" || type__ == "M") {
        return tm_0->tm_mon + 1.0;
    }
    else if (type__ == "mday" || type__ == "day" || type__ == "D") {
        return tm_0->tm_mday;
    }
    else if (type__ == "hour" || type__ == "h") {
        return tm_0->tm_hour;
    }
    else if (type__ == "min" || type__ == "m" || type__ == "minute") {
        return tm_0->tm_min;
    }
    else if (type__ == "sec" || type__ == "s" || type__ == "seconds") {
        return tm_0->tm_sec;
    }
    else if (type__ == "decimal" || type__ == "deci" || type__ == "dec") {
        return decimal_0;
    }
    else if (type__ == "yday" || type__ == "doy" || type__ == "YD") {
        return tm_0->tm_yday + 1.0;
    }
    else if (type__ == "mode") {
        if     (mode_ == "UT") {return 0.0;}
        else if (mode_ == "LT") {return 1.0;}
        else                   {return 2.0;}
    }
    else if (type__ == "wday" || type__ == "W" || type__ == "weekday" || type__ == "week") {
        return tm_0->tm_wday;
    }
    else if (type__ == "isdst") {
        return tm_0->tm_isdst;
    }
    else {
        return -1;
    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets date component.
/// @param[in] name__    : A label of the date.
///            If "", this uses the current time.
/// @param[in] type__ : A type number of date component you wanna get.
///            @arg 0  : Years.
///            @arg 1  : Months.
///            @arg 2  : days og the month.
///            @arg 3  : Hours.
///            @arg 4  : Minutes.
///            @arg 5  : Seconds.
///            @arg 6  : The decimal part of the soconds.
///            @arg 7  : Days of the year.
///            @arg 8  : time-zone mode. UT = 0, LT = 1, Others = 2.
///            @arg 9  : Days of the week. Sunday = 0, ..., Saturday = 6.
///            @arg 10 : The flag of summer time. No use = 0, use = >0.
/// @return    The date component value.
///            If failed, return -1.
///
// ------------------------------------------------ //


double MyChronos::Get(const string &name__, const int &type__) {

    string type_str = "";

    if     (type__ == 0) {type_str = "year";}
    else if (type__ == 1) {type_str = "mon";}
    else if (type__ == 2) {type_str = "mday";}
    else if (type__ == 3) {type_str = "hour";}
    else if (type__ == 4) {type_str = "min";}
    else if (type__ == 5) {type_str = "sec";}
    else if (type__ == 6) {type_str = "decimal";}
    else if (type__ == 7) {type_str = "yday";}
    else if (type__ == 8) {type_str = "mode";}
    else if (type__ == 9) {type_str = "wday";}
    else if (type__ == 10) {type_str = "isdst";}
    else {return -1;}

    return Get(name__, type_str);

}


// ------------------------------------------------ //
///
/// @brief     This function gets the date array.
/// @param[in] name__    : A label of the date.
///            If "", this gets the current time.
/// @return    The date array.
///            @arg [0]  : Years.
///            @arg [1]  : Months.
///            @arg [2]  : days og the month.
///            @arg [3]  : Hours.
///            @arg [4]  : Minutes.
///            @arg [5]  : Seconds.
///            @arg [6]  : The decimal part of the soconds.
///            @arg [7]  : Days of the year.
///            @arg [8]  : time-zone mode. UT = 0, LT = 1, Others = 2.
///            @arg [9]  : Days of the week. Sunday = 0, ..., Saturday = 6.
///            @arg [10] : The flag of summer time. No use = 0, use = >0.
///
// ------------------------------------------------ //


vector<double> MyChronos::GetDate(const string &name__) {

    vector<double> result;

    struct tm*     tm_0;

    if (!MyChronos::GetTM(name__, tm_0)) {
        cerr << "[MyChronos::GetDate] No time : " << name__ << endl;
        result.push_back(-1);
        return result;
    }

    result.push_back(tm_0->tm_year + 1900.0); // [0]
    result.push_back(tm_0->tm_mon + 1.0);     // [1]
    result.push_back(tm_0->tm_mday);          // [2]
    result.push_back(tm_0->tm_hour);          // [3]
    result.push_back(tm_0->tm_min);           // [4]
    result.push_back(tm_0->tm_sec);           // [5]

    result.push_back(decimal_[name__]);       // [6]

    result.push_back(tm_0->tm_yday + 1.0);    // [7]

    if (mode_ == "UT") {
        result.push_back(0.0);                // [8]
    }
    else if (mode_ == "LT") {
        result.push_back(1.0);
    }
    else {
        result.push_back(2.0);
    }

    result.push_back(tm_0->tm_wday);  // [9]
    result.push_back(tm_0->tm_isdst); // [10]

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function gets the date array.
/// @param[in]  name__ : A label of the date.
///             If "", this gets the current time.
/// @param[out] date__ : The date array.
///             @arg [0]  : Years.
///             @arg [1]  : Months.
///             @arg [2]  : days og the month.
///             @arg [3]  : Hours.
///             @arg [4]  : Minutes.
///             @arg [5]  : Seconds.
///             @arg [6]  : The decimal part of the soconds.
///             @arg [7]  : Days of the year.
///             @arg [8]  : time-zone mode. UT = 0, LT = 1, Others = 2.
///             @arg [9]  : Days of the week. Sunday = 0, ..., Saturday = 6.
///             @arg [10] : The flag of summer time. No use = 0, use = >0.
/// @return     None.
///
// ------------------------------------------------ //


bool MyChronos::GetDate(const string &name__, vector<double> &date__) {

    vector<double>().swap(date__);

    struct tm* tm_0;

    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::GetDate] No time : " << name__ << endl;
        date__.push_back(-1);
        return false;
    }

    date__.push_back(tm_0->tm_year + 1900.0); // [0]
    date__.push_back(tm_0->tm_mon + 1.0);     // [1]
    date__.push_back(tm_0->tm_mday);          // [2]
    date__.push_back(tm_0->tm_hour);          // [3]
    date__.push_back(tm_0->tm_min);           // [4]
    date__.push_back(tm_0->tm_sec);           // [5]

    date__.push_back(decimal_[name__]);       // [6]

    date__.push_back(tm_0->tm_yday + 1.0);    // [7]

    if (mode_ == "UT") {
        date__.push_back(0.0);                // [8]
    }
    else if (mode_ == "LT") {
        date__.push_back(1.0);
    }
    else {
        date__.push_back(2.0);
    }

    date__.push_back(tm_0->tm_wday);  // [9]
    date__.push_back(tm_0->tm_isdst); // [10]

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function sets date from an array data.
/// @param[in] name__ : A label of the date.
/// @param[in] date__ : The date array.
///             @arg [0]  : Years.
///             @arg [1]  : Months.
///             @arg [2]  : days og the month.
///             @arg [3]  : Hours.
///             @arg [4]  : Minutes.
///             @arg [5]  : Seconds.
///             @arg [6]  : The decimal part of the soconds.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::SetDate(const string &name__, const vector<double> &date__) {

    vector<int> date_vi;

    for (const auto &it : date__) {
        date_vi.push_back(static_cast<int>(it));
    }

    return Set(name__, date_vi[0], date_vi[1], date_vi[2], date_vi[3], date_vi[4], date_vi[5], date__[6]);

}


// ------------------------------------------------ //
///
/// @brief     This function sets date from an array data.
/// @param[in] name__ : A label of the date.
/// @param[in] date__ : The date array.
///             @arg [0]  : Years.
///             @arg [1]  : Months. (no use)
///             @arg [2]  : days og the month. (no use)
///             @arg [3]  : Hours.
///             @arg [4]  : Minutes.
///             @arg [5]  : Seconds.
///             @arg [6]  : The decimal part of the soconds.
///             @arg [7]  : Days of the year.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::SetDate_doy(const string &name__, const vector<double> &date__) {

    vector<int> date_vi;

    for (const auto &it : date__) {
        date_vi.push_back(static_cast<int>(it));
    }

    vector<int> ymd = YDOY2YMD(date_vi[0], date_vi[7]);

    return Set(name__, date_vi[0], ymd[1], ymd[2], date_vi[3], date_vi[4], date_vi[5], date__[6]);

}


// ------------------------------------------------ //
///
/// @brief      This function gets the tm structure.
/// @param[in]  name__ : A label of the date.
///             If "", this gets the current time.
/// @param[out] tm__ : The tm structure.
/// @param[out] decimal__ : The decimal part of the seconds.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::GetTM(const string &name__, struct tm* &tm__, double &decimal__) {

    if (name__.size() > 0) {

        if (time_t_.count(name__) <= 0) {
            cerr << "[MyChronos::GetTM] Cannot read time : " << name__ << endl;
            return false;
        }

        if (mode_ == "LT") {
            tm__ = localtime(&time_t_[name__]);
        }
        else {
            tm__ = gmtime(&time_t_[name__]);
        }

        decimal__ = decimal_[name__];

    }
    else {

        SetCurrent();

        if (mode_ == "LT") {
            tm__ = localtime(&time_t_current_);
        }
        else {
            tm__ = gmtime(&time_t_current_);
        }

        decimal__ = decimal_current_;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function gets the tm structure.
/// @param[in]  name__ : A label of the date.
///             If "", this gets the current time.
/// @param[out] tm__ : The tm structure.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::GetTM(const string &name__, struct tm* &tm__) {

    double decimal_tmp;

    return GetTM(name__, tm__, decimal_tmp);

}


// ------//

/*
   struct tm* MyChronos::GetTM( const string &name__ ) {

        struct tm* tm_0;

        if( name__.size() > 0 ) {

                if( time_t_.count(name__) <= 0 ) {
                        cerr << "[MyChronos::GetTM] Cannot read time : " << name__ << endl;
                        SetCurrent( name__ );
                };

                if( mode_ == "LT" ) {
                        //cout << "[MyChronos::GetTM] LT" << endl;
                        tm_0 = localtime(&time_t_[name__]);
                } else {
                        //cout << "[MyChronos::GetTM] UT" << endl;
                        tm_0 = gmtime(&time_t_[name__]);
                }

        } else {

                SetCurrent();

                if( mode_ == "LT" ) {
                        tm_0 = localtime(&time_t_current_);
                } else {
                        tm_0 = gmtime(&time_t_current_);
                }

        };

        return tm_0;

   };
 */


// ------------------------------------------------ //
///
/// @brief      This function gets the time_t structure.
/// @param[in]  name__ : A label of the date.
///             If "", this gets the current time.
/// @param[out] time_t__ : The time_t structure.
/// @param[out] decimal__ : The decimal part of the seconds.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::GetTIME_T(const string &name__, time_t &time_t__, double &decimal__) {

    if (name__.size() > 0) {

        if (time_t_.count(name__) <= 0) {
            cerr << "[MyChronos::GetTIME_T] Cannot read time : " << name__ << endl;
            return false;
        }

        time_t__  = time_t_[name__];
        decimal__ = decimal_[name__];

    }
    else {

        SetCurrent();

        time_t__  = time_t_current_;
        decimal__ = decimal_current_;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function gets the time_t structure.
/// @param[in]  name__ : A label of the date.
///             If "", this gets the current time.
/// @param[out] time_t__ : The time_t structure.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::GetTIME_T(const string &name__, time_t &time_t__) {

    double decimal_tmp;

    return GetTIME_T(name__, time_t__, decimal_tmp);

}


// ------------------------------------------------ //
///
/// @brief      This function sets the tm structure.
/// @param[in]  name__ : A label of the date.
/// @param[out] date__ : The tm structure.
/// @param[out] decimal__ : The decimal part of the seconds. The default is 0.0.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::SetTM(const string &name__, const struct tm* date__, const double &decimal__) {

    if (name__.size() <= 0) {return false;}

    if (!CheckTM(date__)) {

        cerr << "[MyChronos::SetTM] Irregular of input date : checking tm" << endl;
        return false;

    }
    else if (decimal__ > 1 || decimal__ < -1) {

        cerr << "[MyChronos::SetTM] Irregular of input date : decimal = " << decimal__ << endl;
        return false;

    }

    struct tm date_0 = *date__;

    time_t_[name__] = mktime(&date_0); // seconds from 1970/01/01 00:00:00 LT

    if (time_t_[name__] == -1) {

        time_t_.erase(time_t_.find(name__));

        return false;

    }

    if (mode_ == "UT") {time_t_[name__] += time_diff_;}  // convert on UTC

    decimal_[name__] = decimal__;

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function sets the time_t structure.
/// @param[in]  name__ : A label of the date.
/// @param[out] date__ : The time_t structure.
/// @param[out] decimal__ : The decimal part of the seconds. The default is 0.0.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::SetTIME_T(const string &name__, const time_t &date__, const double &decimal__) {

    if (decimal__ > 1 || decimal__ < 0) {return false;}

    time_t_[name__]  = date__;

    decimal_[name__] = decimal__;

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function shifts the registered date.
/// @param[in]  name__ : The label of the date that you wanna shift.
/// @param[in]  add_sec__ : The additional time [seconds].
/// @param[in]  new_name__ : If you set, the shifted date would be set with this new label.
///             If not, the original date would be overwritten.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Shift(const string &name__, const double &add_sec__, const string &new_name__) {

    if (name__.size() <= 0) {
        return false;
    }

    int    add_sec = static_cast<int>(add_sec__);
    double add_dec = add_sec__ - add_sec;
    // int add_nsec = static_cast<int>((add_sec__ - add_sec)*1.0e+9);

    double dec_new = decimal_[name__] + add_dec;
    // int nsec_new = nsec_[name__] + add_nsec;

    // cout << add_sec << "." << add_nsec << endl;
    // printf("%f",add_sec__);
    // cout << "::" << nsec_[name__] << "," << add_nsec << endl;

    time_t sec_new;

    if (!GetTIME_T(name__, sec_new)) {
        // cerr << "[MyChronos::Shift] No time : " << name__ << endl;
        return false;
    }

    sec_new += add_sec;

    if ((add_sec__ > 0) && (dec_new > 1)) {
        // if( (add_sec__ > 0) && (nsec_new > 1.0e+9) ) {
        // cout << "[MyChronos::Shift] + " << add_sec__ << "," << nsec_new << endl;
        sec_new += 1;
        dec_new -= 1;
    }

    if ((add_sec__ < 0) && (dec_new < 0)) {
        // if( (add_sec__ < 0) && (nsec_new < 0) ) {
        // cout << "[MyChronos::Shift] - " << sec_new << "." << nsec_new << endl;
        sec_new -= 1;
        dec_new += 1;
        // nsec_new += 1.0e+9;
        // cout << "[MyChronos::Shift] - " << sec_new << "." << nsec_new << endl;
    }


    if (new_name__.size() > 0) {

        time_t_[new_name__]  = sec_new;
        decimal_[new_name__] = dec_new;
        // nsec_[new_name__]   = nsec_new;
        return Check(new_name__);

    }
    else {

        time_t_[name__]  = sec_new;
        decimal_[name__] = dec_new;
        // nsec_[name__]   = nsec_new;
        return Check(name__);

    }

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the differential time between two dates.
/// @param[in]  name_0__ : The date of the origin point.
/// @param[in]  name_1__ : The date of the target.
///             If you set "", it is used the current date.
/// @return     the differential time [seconds].
///
// ------------------------------------------------ //


double MyChronos::Diff(const string &name_0__, const string &name_1__) {

    double dt = 0;

    time_t time_t_0, time_t_1;

    if (!GetTIME_T(name_0__, time_t_0)) {
        cerr << "[MyChronos::Diff] No time : " << name_0__ << endl;
        return dt;
    }

    if (name_1__.size() > 0) {

        if (!GetTIME_T(name_1__, time_t_1)) {
            cerr << "[MyChronos::Diff] No time : " << name_1__ << endl;
            return dt;
        }

        dt  = difftime(time_t_1, time_t_0);

        dt += decimal_[name_1__] - decimal_[name_0__];
        // dt += (nsec_[name_1__] - nsec_[name_0__]) * 1.0e-9;

    }
    else {

        SetCurrent();

        dt  = difftime(time_t_current_, time_t_0);

        dt += decimal_current_ - decimal_[name_0__];

    }

    return dt;

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the differential time between two dates.
/// @param[in]  name_0__ : The date of the origin point.
/// @param[in]  name_1__ : The date of the target.
///             If you set "", it is used the current date.
/// @param[out] sec__ : The differential time (time_t) [seconds].
/// @param[out] decimal__ : The decimal part of the differential time [seconds].
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyChronos::Diff(const string &name_0__, const string &name_1__, time_t &sec__, double &decimal__) {

    double dt_sec = 0;

    time_t time_t_0, time_t_1;

    if (!GetTIME_T(name_0__, time_t_0)) {
        cerr << "[MyChronos::Diff] No time : " << name_0__ << endl;
        sec__ = decimal__ = 0;
        return false;
    }

    if (name_1__.size() > 0) {

        if (!GetTIME_T(name_1__, time_t_1)) {
            cerr << "[MyChronos::Diff] No time : " << name_1__ << endl;
            sec__ = decimal__ = 0;
            return false;
        }

        dt_sec    = difftime(time_t_1, time_t_0);

        decimal__ = decimal_[name_1__] - decimal_[name_0__];

    }
    else {

        SetCurrent();

        dt_sec    = difftime(time_t_current_, time_t_0);

        decimal__ = decimal_current_ - decimal_[name_0__];

    }

    if ((dt_sec > 0) && (decimal__ < 0)) {
        dt_sec    -= 1;
        decimal__ += 1;
    }
    if ((dt_sec < 0) && (decimal__ > 0)) {
        dt_sec    += 1;
        decimal__ -= 1;
    }

    sec__ = static_cast<time_t>(dt_sec);

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function for waiting the sleeping time.
/// @param[in]  sec__ : a sleeping time [seconds]
/// @return     None.
///
// ------------------------------------------------ //


void MyChronos::Sleep(const double &sec__) {

    struct timespec ts;

    int sec = static_cast<int>(sec__);

    ts.tv_sec  = sec;
    ts.tv_nsec = (static_cast<long>(sec__) - static_cast<long>(sec)) * 1.0e+9;

    nanosleep(&ts, NULL);

    return;

}


// ------------------------------------------------ //
///
/// @brief      This function for waiting the sleeping time.
/// @param[in]  sec__ : a sleeping time [seconds]
/// @return     None.
///
// ------------------------------------------------ //


void MyChronos::Sleep(const int &sec__, const int &nsec__) {

    struct timespec ts;

    ts.tv_sec  = sec__;
    ts.tv_nsec = static_cast<long>(nsec__);

    nanosleep(&ts, NULL);

    return;

}


// ------------------------------------------------ //
///
/// @brief      This function waits until the tick time,
///             which is the specified time stamp with nsec after N cycles.
/// @param[in]  tick_nsec__ : a wake-up time stamp with nano second unit.
/// @return     None.
///
// ------------------------------------------------ //


bool MyChronos::TickWait_nsec(const int &tick_nsec__) {

    if(tick_nsec__ > 1000000000 || tick_nsec__ < 0) { return false; }

    //

    auto now_epoch = high_resolution_clock::now().time_since_epoch();

    int now_nsec = (duration_cast<nanoseconds>(now_epoch) - duration_cast<seconds>(now_epoch)).count();

    //

    struct timespec ts;

    ts.tv_sec  = 0;
    ts.tv_nsec = static_cast<long>((now_nsec / tick_nsec__ + 1) * tick_nsec__ - now_nsec);

    nanosleep(&ts, NULL);

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function checks whether the year is a leap year or not.
/// @param[in]  year__ : The year you wanna check.
/// @return     A leap year or not.
///
// ------------------------------------------------ //


bool MyChronos::IsLeapYear(const int &year__) {

    int leap_day = (1 / (year__ % 4 + 1)) * (1 - 1 / (year__ % 100 + 1)) + (1 / (year__ % 400 + 1));

    if (leap_day == 1) {return true;}

    return false;

}


// ------------------------------------------------ //
///
/// @brief      This function converts the format from "year/month/day" to "year/days of the year".
/// @param[in]  year__ : Year.
/// @param[in]  mon__  : Month.
/// @param[in]  mday__ : Days of the month.
/// @return     The converted date array.
///             @arg [0] : Year.
///             @arg [1] : Days of the year.
///
// ------------------------------------------------ //


vector<int> MyChronos::YMD2YDOY(const int &year__, const int &mon__, const int &mday__) {

    if (mon__  < 1 || mon__  >  12) {return {-1};}
    if (mday__ < 1 || mday__ >  31) {return {-1};}

    vector<int> ydoy = {year__, 0};

    for (int m = 1; m < mon__; m++) {

        ydoy[1] += MONTH_DAY[m - 1];

        if ((mon__ == 2) && (IsLeapYear(year__))) {
            ydoy[1] += 1;
        }
    }

    ydoy[1] += mday__;

    return ydoy;

}


// ------------------------------------------------ //
///
/// @brief      This function converts the format from "year/days of the year" to "year/month/day".
/// @param[in]  year__ : Year.
/// @param[in]  yday__ : Days of the year.
/// @return     The converted date array.
///             @arg [0] : Year.
///             @arg [1] : Month.
///             @arg [2] : Days of the month.
///
// ------------------------------------------------ //


vector<int> MyChronos::YDOY2YMD(const int &year__, const int &yday__) {

    if (yday__ < 1 || yday__ >  366) {return {};}

    vector<int> ymd = {year__, 0, yday__};

    for (int m = 0; m < 12; m++) {

        int month_day_0 = MONTH_DAY[m];

        if ((m == 1) && (IsLeapYear(year__))) {
            month_day_0 += 1;
        }

        if (ymd[2] <= month_day_0) {
            ymd[1] = m + 1;
            break;
        }

        ymd[2] -= month_day_0;

    }

    if (ymd[1] == 0) {return {-1};}

    return ymd;

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the Julian Day (JD).
/// @param[in]  name__ : A label of the date.
///             If you set "", it is used the current date.
/// @return     The Julian Day.
///
// ------------------------------------------------ //


double MyChronos::JD(const string &name__) {

    struct tm* tm_0;
    double     sec_decimal = 0;

    if (name__.size() > 0) {

        if (time_t_.count(name__) <= 0) {

            //cerr << "[MyChronos::JD] No time -> Set current time to " << name__ << endl;

            SetCurrent(name__);

        }

        tm_0 = gmtime(&time_t_[name__]);
        sec_decimal = decimal_[name__];

    }
    else {

        SetCurrent();

        tm_0 = gmtime(&time_t_current_);
        sec_decimal = decimal_current_;

    }

    int year_jd = tm_0->tm_year + 1900;
    int mon_jd  = tm_0->tm_mon  - 1;

    if (tm_0->tm_mon == 0) {mon_jd = 11; year_jd -= 1;}
    if (tm_0->tm_mon == 1) {mon_jd = 12; year_jd -= 1;}

    double JD = 0;

    JD  = static_cast<int>(year_jd * 365.25) + static_cast<int>(year_jd / 400) - static_cast<int>(year_jd / 100);
    JD += static_cast<int>(30.59 * (mon_jd));
    JD += tm_0->tm_mday + 1721088.5;
    JD += static_cast<double>(tm_0->tm_hour) / 24 + static_cast<double>(tm_0->tm_min) / 1440;
    JD += (static_cast<double>(tm_0->tm_sec) + sec_decimal) / 86400;

    return JD;

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the Modified Julian Day (MJD).
/// @param[in]  name__ : A label of the date.
///             If you set "", it is used the current date.
/// @return     The Modified Julian Day.
///
// ------------------------------------------------ //


double MyChronos::MJD(const string &name__) {

    return JD(name__) - 2400000.5;

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the Truncated Julian Day (TJD).
/// @param[in]  name__ : A label of the date.
///             If you set "", it is used the current date.
/// @return     The Truncated Julian Day.
///
// ------------------------------------------------ //


double MyChronos::TJD(const string &name__) {

    return JD(name__) - 2440000.5;

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the Sidereal Time(ST).
/// @param[in]  name__ : A label of the date.
///             If "", it is used the current time.
/// @return     The Sidereal Time [hour].
///
// ------------------------------------------------ //


double MyChronos::ST(const string &name__) {

    double st_double =  0.671262 + 1.0027379094 * TJD(name__);

    int    st_int    = static_cast<int>(st_double);

    return 24 * (st_double - st_int);

}


// ------------------------------------------------ //
///
/// @brief      This function calculates the Local Sidereal Time(LST)
/// @param[in]  name__ : A label of the date.
///             If "", it is used the current time.
/// @param[in]  longitude__ : A longitude of the site, west longitude = >0.
/// @return     The Local Sidereal Time [hour].
///
// ------------------------------------------------ //


double MyChronos::LST(const string &name__, const double &longitude__) {

    double lst = ST(name__) - longitude__ / 15;

    while (lst < 0) {
        lst += 24;
        if (lst >= 0) {break;}
    }

    while (lst >= 24) {
        lst -= 24;
        if (lst < 24) {break;}
    }

    return lst;

}


// ------------------------------------------------ //
///
/// @brief      This function converts a real number to a "day/hour/minute/second" unit.
/// @param[in]  date__ : A real number.
/// @param[in]  date_unit__ : The unit of the real number :
///             @arg day  : ex. date__ = 12.5    -> 12 day 12:00:00.0
///             @arg hour : ex. date__ = 10.5    ->  0 day 10:30:00.0
///             @arg min  : ex. date__ = 55.5    ->  0 day 00:55:30.0
///             @arg sec  : ex. date__ = 86411.1 ->  1 day 00:00:11.1
/// @return     The date array.
///             @arg [0] : Day
///             @arg [1] : Hour
///             @arg [2] : Minute
///             @arg [3] : Second
///             @arg [4] : The decimal part of the second [second]
///             If failed, this would return {-1}.
///
// ------------------------------------------------ //


vector<double> MyChronos::D2DHMS(const double &date__, const string &date_unit__) {

    double date_0;

    if (date_unit__ == "day") {
        date_0 = date__ * 86400;
    }
    else if (date_unit__ == "hour") {
        date_0 = date__ * 3600;
    }
    else if (date_unit__ == "min") {
        date_0 = date__ * 60;
    }
    else if (date_unit__ == "sec") {
        date_0 = date__;
    }
    else {
        return {-1};
    }

    vector<double> date_v;

    int date_tmp = static_cast<int>( date_0 / 86400 );
    date_v.push_back(date_tmp); // [0] day

    date_0  -= 86400 * date_tmp;
    date_tmp = static_cast<int>( date_0 / 3600 );
    date_v.push_back(date_tmp); // [1] hour

    date_0  -= 3600 * date_tmp;
    date_tmp = static_cast<int>( date_0 / 60 );
    date_v.push_back(date_tmp); // [2] min

    date_0  -= 60 * date_tmp;
    date_tmp = static_cast<int>( date_0 );
    date_v.push_back(date_tmp); // [3] sec

    date_0 -= date_tmp;
    date_v.push_back(date_0);   // [4] decimal part (sec)

    return date_v;

}


// ------------------------------------------------ //
///
/// @brief      This function converts a "day/hour/minute/second" unit to a real number.
/// @param[in]  date__ : The date array.
///             @arg [0] : Day
///             @arg [1] : Hour
///             @arg [2] : Minute
///             @arg [3] : Second
///             @arg [4] : The decimal part of the second [second]
/// @param[in]  date_unit__ : The unit of the converted real number :
///             @arg day
///             @arg hour
///             @arg min
///             @arg sec
/// @return     A real number converted from the date array.
///             If failed, this would return -1.
///
// ------------------------------------------------ //


double MyChronos::DHMS2D(const vector<double> &date__, const string &date_unit__) {

    if (date__.size() != 5) {
        cout << "[MyChronos::DHMS2D] No match size of input." << endl;
        return -1;
    }

    vector<double> coeff;

    if (date_unit__ == "day") {
        coeff = {1.0, 1.0 / 24.0, 1.0 / 1440.0, 1.0 / 86400.0, 1.0 / 86400.0};
    }
    else if (date_unit__ == "hour") {
        coeff = {24.0, 1.0, 1.0 / 60.0, 1.0 / 3600.0, 1.0 / 3600.0};
    }
    else if (date_unit__ == "min") {
        coeff = {1440.0, 60.0, 1.0, 1.0 / 60.0, 1.0 / 60.0};
    }
    else if (date_unit__ == "sec") {
        coeff = {86400.0, 3600.0, 60.0, 1.0, 1.0};
    }
    else {
        cout << "[MyChronos::DHMS2D] No match date unit." << endl;
        return -1;
    }

    double result = 0;
    int    i      = 0;

    for (const auto &it : date__) {
        // cout << it << " * " << coeff[i] << endl;
        result += coeff[i] * it;
        i++;
    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function checks all the time data associated with the label.
/// @param[in] name__    : A label of the date.
/// @return    Normal or not.
///
// ------------------------------------------------ //

bool MyChronos::Check(const string &name__) {

    struct tm* tm_0;

    if (!GetTM(name__, tm_0)) {return false;}

    if (!CheckTM(tm_0)) {return false;}

    if (decimal_[name__] < 0 || decimal_[name__] >= 1) {return false;}
    // if ( nsec_[name__] < 0.0 || nsec_[name__] >= 1000000000.0 ) { return false; };

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function checks the tm structure associated with the label.
/// @param[in] name__    : A label of the date.
/// @return    Normal or not.
///
// ------------------------------------------------ //


bool MyChronos::CheckTM(const struct tm* tm__) {

    if (tm__->tm_mon  < 0 || tm__->tm_mon  >  11) {return false;}
    if (tm__->tm_mday < 1 || tm__->tm_mday >  31) {return false;}
    if (tm__->tm_yday < 0 || tm__->tm_yday > 366) {return false;}
    if (tm__->tm_wday < 0 || tm__->tm_wday >   6) {return false;}
    if (tm__->tm_hour < 0 || tm__->tm_hour >  23) {return false;}
    if (tm__->tm_min  < 0 || tm__->tm_min  >  59) {return false;}
    if (tm__->tm_sec  < 0 || tm__->tm_sec  >  59) {return false;}

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function prints the date.
/// @param[in] name__    : A label of the date.
/// @param[in] type__    : The format type.
///            @arg 1      = "[year]/[month]/[mday] [wday] [hour]:[minute]:[second] [time mode] ( [yday] doy) ; [name label]"
///            @arg Others = "[year] [month] [mday] [hour] [minute] [second] [time mode] [yday] [wday] [isdst] ; [name label]"
/// @return    None.
///
// ------------------------------------------------ //


void MyChronos::Print(const string &name__, const int &type__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::Print] Cannot print : " << name__ << endl;
        return;
    }

    double sec_print;

    if (name__.size() > 0) {
        sec_print = tm_0->tm_sec + decimal_[name__];
    }
    else {
        sec_print = tm_0->tm_sec + decimal_current_;
    }

    if (type__ == 1) {

        printf("%04d/%02d/%02d %s %02d:%02d:%015.12f %s ( %03d doy ) ; %s",
               tm_0->tm_year + 1900, tm_0->tm_mon + 1,  tm_0->tm_mday,
               WDAY_NAME[tm_0->tm_wday].c_str(),
               tm_0->tm_hour,      tm_0->tm_min,    sec_print,
               mode_.c_str(),      tm_0->tm_yday + 1, name__.c_str());

        if (isdst_ <= 0) {
            printf("\n");
        }
        else {
            printf(" : summer time\n");
        }

    }
    else {

        printf("%04d %02d %02d %02d %02d %015.12f %s %03d %s %d ; %s\n",
               tm_0->tm_year + 1900, tm_0->tm_mon + 1, tm_0->tm_mday,
               tm_0->tm_hour,      tm_0->tm_min,   sec_print,
               mode_.c_str(),      tm_0->tm_yday + 1,
               WDAY_NAME[tm_0->tm_wday].c_str(),
               isdst_,
               name__.c_str());

    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : year, month, day, hour, minute & second. @n
///            ex. 2020/12/31 01:23:45 & delimiter is ":" -> "2020:12:31:01:23:45"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::YMDHMS(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::YMDHMS] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    str_.Sprintf(date_str, "%04d%s%02d%s%02d%s%02d%s%02d%s%02d",
                 tm_0->tm_year + 1900, delimiter__.c_str(), tm_0->tm_mon + 1, delimiter__.c_str(),
                 tm_0->tm_mday,      delimiter__.c_str(), tm_0->tm_hour,  delimiter__.c_str(),
                 tm_0->tm_min,       delimiter__.c_str(), tm_0->tm_sec);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : year, day of year, hour, minute & second. @n
///            ex. 2020/001 01:23:45 & delimiter is ":" -> "2020:001:01:23:45"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::YDOYHMS(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::YDOYHMS] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    str_.Sprintf(date_str, "%04d%s%03d%s%02d%s%02d%s%02d",
                 tm_0->tm_year + 1900, delimiter__.c_str(), tm_0->tm_yday + 1, delimiter__.c_str(),
                 tm_0->tm_hour,      delimiter__.c_str(), tm_0->tm_min,    delimiter__.c_str(), tm_0->tm_sec);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : year & day of year. @n
///            ex. 2020/123 & delimiter is "/" -> "2020/123"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::YDOY(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::YDOY] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    str_.Sprintf(date_str, "%04d%s%03d", tm_0->tm_year + 1900, delimiter__.c_str(), tm_0->tm_yday + 1);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : year, month & day. @n
///            ex. 2020/12/31 & delimiter is "" -> "20201231"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::YMD(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::YMD] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    str_.Sprintf(date_str, "%04d%s%02d%s%02d", tm_0->tm_year + 1900, delimiter__.c_str(), tm_0->tm_mon + 1, delimiter__.c_str(), tm_0->tm_mday);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : hour, minute & second. @n
///            ex. 01:23:45 & delimiter is "" -> "012345"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::HMS(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::HMS] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    str_.Sprintf(date_str, "%02d%s%02d%s%02d",
                 tm_0->tm_hour, delimiter__.c_str(), tm_0->tm_min, delimiter__.c_str(), tm_0->tm_sec);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : hour, minute & second(milli-seconds). @n
///            ex. 01:23:45.67 & delimiter is ":" -> "01:23:45.670"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::HMS_msec(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::HMS_msec] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    double sec_print;

    if (name__.size() > 0) {
        sec_print = tm_0->tm_sec + decimal_[name__];
    }
    else {
        sec_print = tm_0->tm_sec + decimal_current_;
    }

    str_.Sprintf(date_str, "%02d%s%02d%s%05.3f",
                 tm_0->tm_hour, delimiter__.c_str(), tm_0->tm_min, delimiter__.c_str(), sec_print);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : hour, minute & second(micro-seconds). @n
///            ex. 01:23:45.6789 & delimiter is ":" -> "01:23:45.678900"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::HMS_usec(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::HMS_usec] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    double sec_print;

    if (name__.size() > 0) {
        sec_print = tm_0->tm_sec + decimal_[name__];
    }
    else {
        sec_print = tm_0->tm_sec + decimal_current_;
    }

    str_.Sprintf(date_str, "%02d%s%02d%s%09.6f",
                 tm_0->tm_hour, delimiter__.c_str(), tm_0->tm_min, delimiter__.c_str(), sec_print);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets the formated date string : hour, minute & second(nano-seconds). @n
///            ex. 01:23:45.6789 & delimiter is ":" -> "01:23:45.678900000"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @param[in] delimiter__ : The delimiter between components.
///            The default value is "".
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::HMS_nsec(const string &name__, const string &delimiter__) {

    struct tm* tm_0;
    if (!GetTM(name__, tm_0)) {
        cerr << "[MyChronos::HMS_nsec] Cannot get string : " << name__ << endl;
        return "";
    }

    string date_str;

    double sec_print;

    if (name__.size() > 0) {
        sec_print = tm_0->tm_sec + decimal_[name__];
    }
    else {
        sec_print = tm_0->tm_sec + decimal_current_;
    }

    str_.Sprintf(date_str, "%02d%s%02d%s%012.9f",
                 tm_0->tm_hour, delimiter__.c_str(), tm_0->tm_min, delimiter__.c_str(), sec_print);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets only the decimal part of which order is milli-second. @n
///            ex. 2020/01/01 00:00:00.12345 -> "123"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::MSEC(const string &name__) {

    string date_str;

    double dec_print;

    if (name__.size() > 0) {
        dec_print = decimal_[name__];
    }
    else {
        dec_print = decimal_current_;
    }

    str_.Sprintf(date_str, "%03.0f", dec_print * 1.0e+3);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets only the decimal part of which order is micro-second. @n
///            ex. 2020/01/01 00:00:00.12345 -> "123450"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::USEC(const string &name__) {

    string date_str;

    double dec_print;

    if (name__.size() > 0) {
        dec_print = decimal_[name__];
    }
    else {
        dec_print = decimal_current_;
    }

    str_.Sprintf(date_str, "%06.0f", dec_print * 1.0e+6);

    return date_str;

}


// ------------------------------------------------ //
///
/// @brief     This function gets only the decimal part of which order is nano-second. @n
///            ex. 2020/01/01 00:00:00.123456789 -> "123456789"
/// @param[in] name__      : A label of the date.
///            If "", the current time is set.
/// @return    The formated date string.
///
// ------------------------------------------------ //


string MyChronos::NSEC(const string &name__) {

    string date_str;

    double dec_print;

    if (name__.size() > 0) {
        dec_print = decimal_[name__];
    }
    else {
        dec_print = decimal_current_;
    }

    str_.Sprintf(date_str, "%09.0f", dec_print * 1.0e+9);

    return date_str;

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
