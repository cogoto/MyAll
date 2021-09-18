#include <iostream>

#include <unistd.h> // for sleep

#include "MyChronos.hpp"

using namespace std;


int main() {

    MyAll::MyChronos time_;

    cout << endl << "Set start time!!" << endl;

    time_.SetCurrent("start_time");

    time_.Print("start_time");

    cout << endl;

    ///

    cout << "TEST of Set():" << endl;

    cout << "[Set] : 2001/01/24     00:11:22.345678901" << endl << "     => " << flush;

    // time_.Set("ymd1", 2019, 01, 24, 00, 11, 22, 345678901);
    time_.Set("ymd1", 2001, 01, 24, 00, 11, 22.345678901);

    time_.Print("ymd1");

    cout << endl;

    ///

    cout << "[Set] : 2003/02/01     12:34:56.789000000" << endl << "     => " << flush;

    array<int, 6> date_hms  = { 2003, 02, 01, 12, 34, 56 };
    double date_hms_decimal = 0.789000000;

    time_.Set("ymd2", date_hms, date_hms_decimal);

    time_.Print("ymd2");

    cout << endl;

    ///

    cout << "[Set] : 2002/03/01     12:34:56.789000000" << endl << "     => " << flush;

    // int    YMDHMS_3 = 20020301123456;
    // double YMDHMS_dec_3 = 0.789;
    time_.Set("ymd3", 20020301123456.0, 0.789);

    time_.Print("ymd3");

    cout << endl;

    ///

    cout << "TEST of Set_doy():" << endl;

    cout << "[Set] : 2018/100doy    12:34:56.789000000" << endl << "     => " << flush;

    // time_.Set_doy("ydoy1", 2018, 100, 12, 34, 56, 789000000);
    time_.Set_doy("ydoy1", 2018, 100, 12, 34, 56.789000000);

    time_.Print("ydoy1");

    cout << endl;

    ///

    cout << "[Set] : 2018/200doy    12:34:56.789000000" << endl << "     => " << flush;

    array<int, 5> date_doy  = { 2003, 200, 12, 34, 56 };
    double date_doy_decimal = 0.789000000;

    time_.Set_doy("ydoy2", date_doy, date_doy_decimal);

    time_.Print("ydoy2");

    cout << endl;

    ///

    cout << "[Set] : 2018/300doy    12:34:56.789000000" << endl << "     => " << flush;

    // int    YDOYHMS_3 = 2018300123456;
    // double YDOYHMS_dec_3 = 0.789;
    time_.Set_doy("ydoy3", 2018300123456.0, 0.789);

    time_.Print("ydoy3");

    cout << endl;

    ///

    cout << "TEST of Set() with string :" << endl;

    string set_str_1 = "*2020/04/01_12:34:56.789012;";

    cout << "[Set] : " << set_str_1 << endl << "     => " << flush;

    time_.Set("set_str_1", set_str_1);

    time_.Print("set_str_1");

    cout << endl;

    ///

    string set_str_2 = "2021001223344.0123***456789012345";

    cout << "[Set] : " << set_str_2 << endl << "     => " << flush;

    time_.Set("set_str_2", set_str_2);

    time_.Print("set_str_2");

    cout << endl;

    ///

    string set_str_3 = "2022y 1M 2D 3h 4m 5.6789s";

    cout << "[Set] : " << set_str_3 << endl << "     => " << flush;

    time_.Set("set_str_3", set_str_3);

    time_.Print("set_str_3");

    cout << endl;

    ///

    cout << "TEST of leap year :" << endl;
    for (int i = 2018; i <= 2021; i++) {
        if (time_.IsLeapYear(i)) { cout << i << " is a leap year." << endl; }
        else { cout << i << " is not..." << endl; }
    }

    cout << endl;

    ///

    sleep(1);

    ///

    cout << "TEST of GetDate:" << endl;

    time_.SetCurrent("start_time");

    cout << "[start_time_1]" << flush;
    vector<double> start_time_1 = time_.GetDate("start_time");
    for (auto it : start_time_1) {
        cout << " " << it << flush;
    }
    cout << endl;

    cout << "[start_time_2]" << flush;
    vector<double> start_time_2;
    time_.GetDate("start_time", start_time_2);
    for (auto it : start_time_2) {
        cout << " " << it << flush;
    }
    cout << endl;

    cout << endl;

    ///

    cout << "TEST of SetDate:" << endl;

    vector<double> set_date_1 = { 2018, 01, 02, 03, 04, 05, 0.6, 0 };
    cout << "[set_date_1]" << endl;
    for (auto it : set_date_1) { cout << " " << it << flush; }
    cout << endl;
    time_.SetDate("set_date_1", set_date_1);
    time_.Print("set_date_1");

    vector<double> set_date_2 = { 2019, 0, 0, 01, 02, 03, 0.4, 25 };
    cout << "[set_date_2]" << endl;
    for (auto it : set_date_2) { cout << " " << it << flush; }
    cout << endl;
    if (!time_.SetDate_doy("set_date_2", set_date_2)) {
        cout << "Invalid input!" << endl;
    }
    time_.Print("set_date_2");

    cout << endl;

    ///

    cout << "TEST of Shift" << endl;

    // double shift_sec = -86400 * 365 * 2 + 0.1;
    double shift_sec = -7200.5;
    cout << shift_sec << " sec time shift" << endl;

    time_.SetCurrent("current_time");
    time_.Print("current_time");

    if (!time_.Shift("current_time", shift_sec, "shift_time")) {
        cout << "Failed Shift()" << endl;
    }
    time_.Print("shift_time");

    double shift_sec_2 = -86400 * 365 * 2 - 0.05;
    cout << "Add " << shift_sec_2 << " sec time shift over again" << endl;

    if (!time_.Shift("shift_time", shift_sec_2)) {
        cout << "Failed Shift()" << endl;
    }
    time_.Print("shift_time");


    cout << endl;

    ///

    cout << "TEST of Diff :" << endl;

    double dt = time_.Diff("current_time", "shift_time"); // [sec]

    printf("[shift_time] - [current_time] = %f sec (= %f + %f)\n", dt, shift_sec, shift_sec_2);

    time_t dt_sec;
    double dt_decimal;

    time_.Diff("current_time", "shift_time", dt_sec, dt_decimal); // [sec]

    cout << "[shift_time] - [current_time] = " << dt_sec << " sec + "  << dt_decimal << " nsec" << endl;

    cout << "[shift_time]   = " << flush;
    time_.Print("shift_time");
    cout << "[current_time] = " << flush;
    time_.Print("current_time");

    cout << endl;

    ///

    cout << "TEST of JDs, ST, LST :" << endl;

    printf("current JD  = %f [day]\n",   time_.JD());
    printf("current MJD = %f [day]\n",   time_.MJD());
    printf("current TJD = %f [day]\n",   time_.TJD());

    double st_double  = time_.ST("");
    printf("current ST  = %f [h]\n", st_double);
    vector<double> ST = time_.D2DHMS(st_double, "hour");
    cout << "->";    for (auto it : ST) { cout << " " << it; }  cout << " [DHMS]" << endl;

    double lst_double   = time_.LST("", -141.8);
    vector<double> LST  = time_.D2DHMS(lst_double, "hour");
    double lst_double_1 = time_.DHMS2D(LST, "hour");
    printf("current LST = %f [h] @ East longitude 141.8 deg\n", lst_double);
    cout << "->"; for (auto it : LST) { cout << " " << it; }  cout << " [DHMS]" << endl;
    cout << "-> reconverted : " << lst_double_1 << " [h]" << endl;

    cout << endl;

    ///

    cout << "TEST of Get String :" << endl;

    cout << "-> now  : ";
    cout << "YMDHMS  : " << time_.YMDHMS() << endl;
    cout << "YDOYHMS : " << time_.YDOYHMS() << endl;
    cout << "YDOY    : " << time_.YDOY() << endl;
    cout << "YMD     : " << time_.YMD() << endl;
    cout << "HMS     : " << time_.HMS() << endl;

    cout << "-> against [start_time] : ";
    time_.Print("start_time");
    cout << "HMS_msec : " << time_.HMS_msec("start_time", ":") << endl;
    cout << "HMS.MSEC : " << time_.HMS("start_time", ":") << "." << time_.MSEC("start_time") << endl;
    cout << "HMS_usec : " << time_.HMS_usec("start_time", ":") << endl;
    cout << "HMS.USEC : " << time_.HMS("start_time", ":") << "." << time_.USEC("start_time") << endl;
    cout << "HMS_nsec : " << time_.HMS_nsec("start_time", ":") << endl;
    cout << "HMS.NSEC : " << time_.HMS("start_time", ":") << "." << time_.NSEC("start_time") << endl;

    cout << endl;

    ///

    cout << "TEST of Get :" << endl;
    cout << "YEAR : " << time_.Get("", "year") << endl;
    cout << "MON  : " << time_.Get("", "month") << endl;
    cout << "MDAY : " << time_.Get("", "D") << endl;
    cout << "HOUR : " << time_.Get("", "h") << endl;
    cout << "MIN  : " << time_.Get("", "minute") << endl;
    cout << "SEC  : " << time_.Get("", "sec") << endl;
    cout << "DECIMAL : " << time_.Get("", "deci") << endl;
    cout << "YDAY : " << time_.Get("", "doy") << endl;
    cout << "MODE : " << time_.Get("", "mode") << endl;
    cout << "WDAY : " << time_.Get("", "W") << endl;
    cout << "ISDST : " << time_.Get("", "isdst") << endl;

    cout << endl;

    cout << "-> against [start_time] : " << endl;
    cout << "YEAR : " << time_.Get("start_time", 0) << endl;
    cout << "MON  : " << time_.Get("start_time", 1) << endl;
    cout << "MDAY : " << time_.Get("start_time", 2) << endl;

    cout << endl;

    //cout << "TEST of TickWait_nsec : 2,000,000 nsec tick" << endl;
    //time_.TickWait_nsec(2000000);


    return 0;

}
