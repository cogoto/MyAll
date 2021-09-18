// Display options.
//
// Exec sample
//
//  ./MyOption.sample.exe -a test_a --b 1.1 -c --cde 2.22 arg1 arg2 3.33 -ef -gg test_b --ijk -lw test_c --xy "arg3 arg4" -z arg5
//
// If you wanna display help words, execute below
//
// ./MyOption.sample.exe -help (or -h)
//
// ./MyOption.sample.exe
//

//
// Include Library
//

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;


//
// Include My Library
//

#include "MyOption.hpp"


//
//  MAIN
//
int main(int argc, char* argv[]) {

    // using uint = unsigned int;

    MyAll::MyOption opt;

    /// help

    vector<string> help_opt__ = { "h", "help" };

    opt.SetHelp_opt(help_opt__);                   // help に使うオプション文字の指定

    opt.SetHelp_string("You called help string."); // help の文

    opt.SetHelp_file("./MyOption.sample.help");    // ここで指定した help file の内容が表示される

    opt.UseHelp_blank();                           // オプションが何もないときに help を表示

    /// Reading options

    //
    // * SetSingleOpt(), SetArgOpt() は必ず ReadOpt() の前に行う !!!
    //
    vector<string> single_opt__ = { "w", "xy", "z" };
    vector<string> arg_opt__    = { "a", "b", "cde" };

    opt.SetSingleOpt(single_opt__);                    // オプション引数をとりたくないオプションを指定する．

    opt.SetArgOpt(arg_opt__);                          // オプション引数をとりたいオプションを指定する．

    opt.ReadOpt(argc, argv);                           // オプションを読む

    vector<string>      keyword  = opt.GetOptPhrase(); // オプション文字を読み込む

    map<string, string> map_args = opt.GetOpt();       // オプション引数を map 配列として読む

    cout << "This program : " << opt.Name() << endl;   // この実行プログラムの名前

    cout << endl << "Options with arguments  : ";
    for (auto it: arg_opt__) {
        cout << it << ", ";
    }
    cout << endl;

    cout << endl << "Options without arguments  : ";
    for (auto it: single_opt__) {
        cout << it << ", ";
    }
    cout << endl;

    cout << endl;

    cout << "#option : argument" << endl << endl;

    for (int i = 0; i < opt.GetOpt_num(); i++) {
        cout << keyword[i] << " : " << map_args[keyword[i]] << endl;
    }

    cout << endl;

    for (int i = 0; i < opt.GetOpt_num(); i++) {

        int    result_opt_int    = opt.GetOpt_int(keyword[i]);
        double result_opt_double = opt.GetOpt_double(keyword[i]);

        cout << " bool   : " << keyword[i] << " : ";
        if (opt.GetOpt_bool(keyword[i])) { cout << "true"; } // オプションが存在するかどうかの判定
        else { cout << "false"; }
        cout << endl;

        cout << " string : " << keyword[i] << " : " << opt.GetOpt_string(keyword[i]) << endl; // オプション引数を string として読む
        cout << " int    : " << keyword[i] << " : " << result_opt_int << endl;                // オプション引数 int として読む
        cout << " double : " << keyword[i] << " : " << result_opt_double << endl;             // オプション引数を double として読む

        cout << endl;

    }

    vector<string> args = opt.GetArg(); // 引数を vector として読む

    cout << "#argument" << endl << endl;

    for (const auto &it : args) {
        cout << it << endl;
    }

    cout << endl;

    for (int i = 0; i < opt.GetArg_num(); i++) {

        int    result_arg_int    = opt.GetArg_int(i);
        double result_arg_double = opt.GetArg_double(i);

        cout << " string : " << opt.GetArg_string(i) << endl; // 引数を string として読む
        cout << " int    : " << result_arg_int    << endl;    // 引数を int として読む
        cout << " double : " << result_arg_double << endl;    // 引数を double として読む
        cout << endl;

    }

} // main
