// command example
//
// ./MyLog.sample.exe -lpd
//

#include <iostream>
#include <string>
#include <vector>

#include "MyMsg.hpp"

using namespace std;


int main(int argc, char* argv[]) {

    MyAll::MyMsg  msg;

    MyAll::MyMsgS msgs;

    cout << "TEST OF [MyMsg]" << endl;

    msg.Init("msg1");
    msg.Set("msg2");
    msg.Set("msg");
    msg.Set("msg");
    msg.Set("");
    msg.Set("msg3");

    msg.Set(4, "msg");
    msg.Set(7, "msg");

    cout << "[MyMsg] ALL     : ";
    msg.Print("ALL");

    cout << "[MyMsg] SUM     : ";
    msg.Print("SUM");

    cout << "[MyMsg] ADJUST : ";
    msg.Print("ADJUST");

    vector<string> result = msg.GetData("ALL");

    cout << "[MyMsg] GetData ALL : ";
    for (auto it : result) {
        cout << it << ", ";
    }
    cout << endl;

    ///

    cout << "TEST OF [MyMsg] with Head" << endl;

    msg.Head("<HEAD_MyMsg>");

    cout << "[MyMsg] ADJUST : ";
    msg.Print("ADJUST");

    result = msg.GetData("SUM");

    cout << "[MyMsg] GetData SUM : ";
    for (auto it : result) {
        cout << it << ", ";
    }
    cout << endl;

    ///

    cout << "TEST OF [MyMsgS]" << endl;

    msgs.Init("key1", "key1_msg1");
    msgs.Set("key1", "key1_msg2");
    msgs.Set("key1", "key1_msg");
    msgs.Set("key1", "key1_msg");

    msgs.Init("key2", "key2_msg1");
    msgs.Set("key2", "key2_msg2");
    msgs.Set("key2", "key2_msg");
    msgs.Set("key2", "key2_msg");

    ///msgs.Init(__FUNCTION__, "func_name_1:" + string(__FUNCTION__) );
    ///msgs.Set(__FUNCTION__, "func_name_0");
    ///msgs.Set(__FUNCTION__, "func_name_2");
    ///msgs.Set(__FUNCTION__, "func_name_0");

    cout << "[MyMsgs] ALL key1 : ";
    msgs.Print("key1", "ALL");
    cout << "[MyMsgs] ALL key2 : ";
    msgs.Print("key2", "ALL");
    ///cout << "[MyMsgs] ALL func_name : ";
    ///msgs.Print( __FUNCTION__, "ALL" );

    cout << "[MyMsgs] SUM key1 : ";
    msgs.Print("key1", "SUM");
    cout << "[MyMsgs] SUM key2 : ";
    msgs.Print("key2", "SUM");
    ///cout << "[MyMsgs] SUM func_name : ";
    ///msgs.Print( __FUNCTION__, "SUM" );

    cout << "[MyMsgs] ADJUST key1 : ";
    msgs.Print("key1", "ADJUST");
    cout << "[MyMsgs] ADJUST key2 : ";
    msgs.Print("key2", "ADJUST");
    ///cout << "[MyMsgs] ADJUST func_name : ";
    ///msgs.Print( __FUNCTION__, "ADJUST" );

    vector<string> result2 = msgs.GetData("key1", "ALL");

    cout << "[MyMsgs] GetData ALL key1 : ";
    for (auto it : result2) {
        cout << it << ", ";
    }
    cout << endl;

    ///

    cout << "TEST OF [MyMsgS] with Head" << endl;

    msgs.Head("key1", "<HEAD_MyMsgS>");

    cout << "[MyMsgs] ADJUST key1 : ";
    msgs.Print("key1", "ADJUST");

    result2 = msgs.GetData("key1", "SUM");

    cout << "[MyMsgs] GetData SUM key1 : ";
    for (auto it : result2) {
        cout << it << ", ";
    }
    cout << endl;

    ///

    return 0;

}
