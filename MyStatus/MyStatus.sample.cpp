#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <unistd.h>
#include <sys/wait.h>


#include "MyStatus.hpp"

using namespace std;


int main() {

    cout << "# Mystatus sample program : " << flush;

    MyAll::MyStatus my_;

    cout << "start!" << endl;

    //
    // Command
    //

    my_.SetCmd("UTC1", "UTC CHECK?: $$1",                               "!UTC CHECK?: $$1 $$2");
    my_.SetCmd("UTC2", R"(UTC CHECK?: \\$$1 \$1 \$\$1 \\$1 \\$$1 $$2)", "!UTC CHECK?: 2 $$2");
    my_.SetCmd("UTC3", "UTC CHECK?: 3");

    my_.SetMacro("TEMP_ALL", "TEMP CHECK?: 1");
    my_.SetMacro("TEMP_ALL", "TEMP CHECK?: 2");
    my_.SetMacro("TEMP_ALL", "TEMP CHECK?: 3");

    // sleep(1);

    //
    // Status
    //

    my_.DefSts_norm("NORMAL_DOY 1", true,  true,  {{"NORM","[MSG] normal 1"}, {"INFO","[MSG] info 1"}});
    my_.DefSts_norm("NORMAL_DOY 2", false, true,  {{"NORM","[MSG] normal 2"}});
    my_.DefSts_norm("NORMAL_DOY 3", false, false, {{"NORM","[MSG] normal 3"}});

    my_.DefSts_norm("NORMAL_MDAY", false, true, {{"NORM","[MSG] days of month"}});

    my_.SetSts_norm("NORMAL_DOY 2", true);
    my_.SetSts_norm("NORMAL_DOY 3", true);

    my_.DefSts_err("ERR_ABS_DIFF", false, false, {{"ERR","[ERROR] absolute UTC difference"}});
    my_.DefSts_err("ERR_REL_DIFF", false, false, {{"ERR","[ERROR] relative UTC difference"}});

    my_.DefSts_err("ERR_TEMP",     false, false, {{"ERR","[ERROR] over range of temperatures"}});

    my_.DefSts_err("ERR_PLACE",    false, false, {{"WARN","[WARN] NO COUNTRY"}});

    my_.DefSts_err("ERR_TIME",     false, false, {{"ERR","[ERROR] NO LOCAL TIME"}});

    my_.DefSts_warn("WARN_NOISE",  false, false, {{"WARN","[WARN] NOISY"}});

    //
    // Values
    //

    my_.DefVal_double("DOY 1", 101,   true);
    my_.DefVal_double("DOY 2", 102.0, false);
    my_.DefVal_double("DOY 3", 103.5);

    my_.DefVal_double("dDOY",  fabs(my_.GetVal_double("DOY 1") - my_.GetVal_double("DOY 2")));

    my_.DefVal_int("MDAY", 15);

    my_.DefVal_int("TEMP", { 20, 21, 22, 23, 24 });

    my_.DefVal_char("BAND", { 'C', 'X', 0x4b, 0x51 }); // 0x4b = K, 0x51 = Q

    my_.DefVal_bool("SWITCH", { true, false, false }, true);

    my_.DefVal_string("PLACE", "MIZUSAWA", true, 10);

    my_.DefVal_string("TIME",  "UT");

    my_.DefVal_double("NOISE", {0.0, 0.1, 0.2});

    //
    // Connect Status & values
    //

    my_.Connect("NORMAL_DOY 1", "DOY 1", ">= 1", "<= 366");
    my_.Connect("NORMAL_DOY 2", "DOY 2", ">= 1", "<= 366");
    // my_.Connect("NORMAL_DOY 3", "DOY 3", ">= 1", "<= 366");

    my_.Connect("ERR_REL_DIFF", "dDOY",  "< -1", "> 1");

    my_.Connect("NORMAL_MDAY", "MDAY", "<= 31", "");

    my_.Connect("ERR_ABS_DIFF", "DOY 1", "< 1", "> 366");
    my_.Connect("ERR_ABS_DIFF", "DOY 2", "< 1", "> 366");
    //  my_.Connect("ERR_ABS_DIFF", "DOY 3", "< 1", "> 366");

    my_.Connect("ERR_TEMP",     "TEMP",  "< 0", ">= 30");

    my_.Connect("ERR_PLACE",    "PLACE", "!= (JAPAN|USA)", "");

    my_.Connect("ERR_TIME",     "TIME",  "exc T", "");

    my_.Connect("WARN_NOISE",   "NOISE",  "> 1.0", "");

    my_.WriteShmFile("./MyStatus.sample.shm");

    //

    cout << endl << "--- FIRST SETTING ---------------------" << endl << endl;

    my_.Print();

    cout << endl << "--- GET* ------------------------------" << endl << endl;

    map<string, string> cmd = my_.GetCmd();
    cout << "[cmd]" << endl;
    for (auto it = cmd.begin(); it != cmd.end(); ++it) {
        cout << "[" << it->first << "]" << it->second << endl;
    }

    map<string, string> ret = my_.GetCmd_return();
    cout << "[cmd return]" << endl;
    for (auto it = ret.begin(); it != ret.end(); ++it) {
        cout << "[" << it->first << "]" << it->second << endl;
    }

    cout << endl;
    cout << "[cmd] UTC1 $$1 = A, $$2 = B" << endl;
    cout << "[command] " << my_.GetCmd("UTC1", { "A" }) << endl;
    cout << "[return]  " << my_.GetCmd_return("UTC1", { "A", "B" }) << endl;
    cout << endl;

    cout << endl;
    cout << "[cmd] UTC2 $$1 = A, $$2 = B" << endl;
    cout << "[command] " << my_.GetCmd("UTC2", { "A", "B" }) << endl;
    cout << "[return]  " << my_.GetCmd_return("UTC2", { "A", "B" }) << endl;
    cout << endl;

    cout << "[cmd] get only \"UTC1\"" << endl;
    cout << "[UTC1]" << my_.GetCmd("UTC1") << endl;
    cout << endl;

    vector<string> macro = my_.GetMacro("TEMP_ALL");
    cout << "[macro]" << endl << "TEMP_ALL :" << endl;
    for (auto it : macro) {
        cout << " " << it << endl;
    }
    cout << endl;

    vector<string> sts_name = my_.GetStsName();
    cout << "[status name ; message]" << endl;
    for (const auto &it : sts_name) {
        cout << it << endl;
        //cout << it << " ;\n" << my_.GetStsMsg(it) << flush;
    }
    cout << endl;

    vector<string> normal_sts_name = my_.GetStsName_norm();
    cout << "all  normal status name :" << endl;
    for (const auto &it : normal_sts_name) { cout << it << " : [MSG] " <<  my_.GetStsMsg(it,"NORM") << endl; }
    //for (const auto &it : normal_sts_name) { cout << it << "   "; }
    cout << endl;

    vector<string> warn_sts_name = my_.GetStsName_warn();
    cout << "all warning status name :" << endl;
    for (const auto &it : warn_sts_name) { cout << it << " : [MSG] " <<  my_.GetStsMsg(it,"WARN") << endl; }
    //for (auto it : warn_sts_name) { cout << it << "   "; }
    cout << endl;

    vector<string> err_sts_name = my_.GetStsName_err();
    cout << "all error   status name :" << endl;
    for (const auto &it : err_sts_name) { cout << it << " : [MSG] " <<  my_.GetStsMsg(it,"ERR") << endl; }
    //for (auto it : err_sts_name) { cout << it << "   "; }
    cout << endl;

    cout << "[Get status]" << endl;
    vector<string>    sts_name_1;
    map<string, bool> sts_bool_1;
    my_.GetSts(sts_name_1, sts_bool_1);
    for (auto it : sts_name_1) {
        cout << it;
        if (sts_bool_1[it]) { cout << " : true" << endl; }
        if (!sts_bool_1[it]) { cout << " : false" << endl; }
    }
    // cout << "-> Only NORMAL_DOY 3 is no controled by values." << endl;
    cout << endl;

    cout << "[Get values]" << endl;
    cout << "DOY 1      : " << my_.GetVal_double("DOY 1") << endl;
    cout << "DOY 2      : " << my_.GetVal_double("DOY 2") << endl;
    cout << "DOY 3      : " << my_.GetVal_double("DOY 3") << endl;
    cout << "dDOY       : " << my_.GetVal_double("dDOY")  << endl;

    cout << "TEMP       : ";
    for (auto it : my_.GetVal_ints("TEMP")) { cout << it << "  "; }
    cout << endl;
    cout << "TEMP[1]    : " << my_.GetVal_int("TEMP", 1) << endl;

    cout << "BAND       : ";
    for (auto it : my_.GetVal_chars("BAND")) { cout << it << "  "; }
    cout << endl;
    cout << "BAND[2]    : " << my_.GetVal_char("BAND", 2) << endl;

    cout << "SWITCH     : ";
    for (auto it : my_.GetVal_bools("SWITCH")) {
        if (it) { cout << "TRUE  "; }
        if (!it) { cout << "FALSE  "; }
    }
    cout << endl;
    cout << "SWITCH[0]  : ";
    if (my_.GetVal_bool("SWITCH", 0)) { cout << "TRUE" << endl; }
    else { cout << "FALSE" << endl; }

    cout << "PLACE      : " << my_.GetVal_string("PLACE") << endl;

    cout << endl;


    cout << endl << "--- CHANGE* 1 -------------------------" << endl << endl;

    cout << "> DOY 2    set to 400" << endl;
    my_.SetVal_double("DOY 2", 400.0);

    cout << "> dDOY     set" << endl;
    my_.SetVal_double("dDOY",  fabs(my_.GetVal_double("DOY 1") - my_.GetVal_double("DOY 2")));

    cout << "> TEMP[10] set to 30" << endl;
    my_.SetVal_int("TEMP", 30, 10);

    cout << "> PLACE    set to JAPAN" << endl;
    my_.SetVal_string("PLACE", "JAPAN");

    cout << "> TIME     set to JST" << endl;
    my_.SetVal_string("TIME",  "JST");

    cout << "> BAND[1]  set to S" << endl;
    my_.SetVal_char("BAND", 'S', 1);

    cout << endl;

    my_.PrintSts_detail();

    my_.PrintVal();

    if (my_.CheckSts_norm()) {
        vector<string> norm_name1 = my_.GetSts_norm(true);
        cout << "Current normal status : ";
        for (auto it : norm_name1) { cout << it << "   "; }
    }
    cout << endl << endl;

    if (my_.CheckSts_warn()) {
        vector<string> warn_name1 = my_.GetSts_warn(true);
        cout << "Current warnig status : ";
        for (auto it : warn_name1) { cout << it << "   "; }
    }
    cout << endl << endl;

    if (my_.CheckSts_err()) {
        vector<string> err_name1 = my_.GetSts_err(true);
        cout << "Current error  status : ";
        for (auto it : err_name1) { cout << it << "   "; }
    }
    cout << endl << endl;


    cout << endl << "--- CHANGE* 2 -------------------------" << endl << endl;

    cout << "> DOY 1    set to 300" << endl;
    my_.SetVal_double("DOY 1", 300.0);

    cout << "> DOY 2    set to 300" << endl;
    my_.SetVal_double("DOY 2", 300.0);

    cout << "> DOY 3    set to 300" << endl;
    my_.SetVal_double("DOY 3", 300.0);

    cout << "> NORMAL_DOY 1 set to false." << endl;
    my_.SetSts_norm("NORMAL_DOY 1", false);

    cout << "> NORMAL_DOY 2 set to false." << endl;
    my_.SetSts_norm("NORMAL_DOY 2", false);

    cout << "> NORMAL_DOY 3 set to false." << endl;
    my_.SetSts_norm("NORMAL_DOY 3", false);

    cout << "> TEMP[11] set to 1" << endl;
    my_.SetVal_int("TEMP", 1, 11);

    cout << "> TIME     set to XXX" << endl;
    my_.SetVal_string("TIME", "XXX");

    cout << "> dDOY     set" << endl;
    my_.SetVal_double("dDOY", my_.GetVal_double("DOY 1") - my_.GetVal_double("DOY 2"));

    cout << "> NOISE    set to 0.5, 1.0, 2.0" << endl;
    my_.SetVal_double("NOISE", {0.5, 1.0, 2.0});

    cout << endl;

    cout << " -> NORMAL_DOY 1 & 2 were controled by the values." << endl << endl;


    my_.PrintSts_detail();

    my_.PrintVal();

    if (my_.CheckSts_norm()) {
        vector<string> norm_name1 = my_.GetSts_norm(true);
        cout << "Current normal status : ";
        for (auto it : norm_name1) { cout << it << "   "; }
    }
    cout << endl << endl;

    if (my_.CheckSts_warn()) {
        vector<string> warn_name1 = my_.GetSts_warn(true);
        cout << "Current warnig status : ";
        for (auto it : warn_name1) { cout << it << "   "; }
    }
    cout << endl << endl;

    if (my_.CheckSts_err()) {
        vector<string> err_name1 = my_.GetSts_err(true);
        cout << "Current error  status : ";
        for (auto it : err_name1) { cout << it << "   "; }
    }
    cout << endl << endl;

    /*
    if (my_.CheckSts("ERR_TEMP")) {
        cout << "[ERR_TEMP] true." << endl;
    } else {
        cout << "[ERR_TEMP] false." << endl;
    }
    */

    cout << endl << "--- TEST GetVal -----------------------" << endl << endl;

    vector<string> result_getstss = my_.GetStsS();
    for (auto it : result_getstss) {
        cout << it << endl;
    }

    vector<string> result_getvals = my_.GetValS();
    for (auto it : result_getvals) {
        cout << it << endl;
    }

    cout << endl << "------------------------------------------" << endl << endl;

    // my_.Print();


    return 0;

}
