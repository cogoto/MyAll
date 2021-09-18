// command example
//
// ./MyLog.sample.exe -l -p -d -D
//

#include <unistd.h>

#include <string>
#include <vector>
#include <iostream>

#include "MyLog.hpp"
#include "../include/MyOption.hpp"

using namespace std;


int main(int argc, char* argv[]) {

    MyAll::MyOption opt;

    vector<string> single_opt__ = { "n", "l", "p", "d", "D" };

    opt.SetSingleOpt(single_opt__);

    opt.ReadOpt(argc, argv); // オプションを読む

    //

    cout << "TEST OF [MyLog]" << endl;

    MyAll::MyLog my("./MyLog.sample.log");

    my.UseLog(!opt.GetOpt_bool("n"), opt.GetOpt_bool("l")); // "n" ... logを記録しない＆標準出力しない（UseLogPrint()を無効にする）
    //my.UseLogPrint(opt.GetOpt_bool("l")); // "l" ... logの内容を標準出力する
    my.UsePrint(opt.GetOpt_bool("p"));    // "p" ... Print()で指定した文字列を出力する
    my.UseDebug(opt.GetOpt_bool("d"));    // "d" ... Debug()で指定した文字列を出力する

    string text_1 = "This is log data.";
    string text_2 = "This is print data.";
    string text_3 = "This is defebug data.";
    string text_4 = "This is debug-Log data.";

    cout << endl << "[MyLog] TIME STAMP = 0" << endl;

    my.UseTime(0); // no time stamp

    my.Log(text_1);
    my.Print(text_2);
    my.Debug(text_3);
    my.Debug(__FILE__, __LINE__, "Debug 1");

    sleep(1);

    cout << endl << "[MyLog] TIME STAMP = 1" << endl;

    my.UseTime(1); // YYYYMMDDHHMMSS

    my.Log(text_1);
    my.Print(text_2);
    my.Debug(text_3);
    my.Debug(__FILE__, __LINE__, "Debug 2");

    sleep(1);

    cout << endl << "[MyLog] TIME STAMP = 2" << endl;

    my.UseTime(2); // YYYYDOYHHMMSS

    my.Log(text_1);
    my.Print(text_2);
    my.Debug(text_3);
    my.Debug(__FILE__, __LINE__);

    sleep(1);

    cout << endl << "[MyLog] TIME STAMP = 3" << endl;

    my.UseTime(3); // HHMMSS

    my.UseDebug(opt.GetOpt_bool("d"), "[DEBUG] >");

    my.Log(text_1);
    my.Print(text_2);
    my.Debug(text_3);
    my.Debug(__FILE__, __LINE__);

    //
    // debug log
    //

    cout << endl << "[MyLog] Debug monitoring test." << endl;

    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 0;

    my.UseTime(1); // YYYYMMDDHHMMSS

    my.UseDeMon(opt.GetOpt_bool("D"), "./MyLog.sample.debug", 3, 2, 3, 10);

    for(int i=1;i<=12;i++) {

        my.Debug(text_4 + to_string(i));

        ts.tv_sec  = i;
        nanosleep(&ts, NULL);

    }

    ts.tv_sec  = 1;
    nanosleep(&ts, NULL);

    return 0;

}
