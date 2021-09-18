//
// If executing this program on cygwin,
// you might need to run cygserver as an administrator.
// > cygrunsrv -S cygserver
//

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include <vector>
#include <string>

#include "MyShmS.hpp"

using namespace std;


int main() {

    cout << "TEST OF [MyShm]" << endl << endl;

    MyAll::MyShmS shm;

    cout << "Byte size :" << endl;
    cout << "bool  (Byte) = " << shm.ByteSize("bool") << endl;
    cout << "char  (Byte) = " << shm.ByteSize("char") << endl;
    cout << "int   (Byte) = " << shm.ByteSize("int") << endl;
    cout << "double(Byte) = " << shm.ByteSize("double") << endl;

    //

    cout << "Test of Init :" << endl;

    string str_1 = "INIT TEST 1";
    string str_2 = "INIT TEST 2";
    shm.Write("init_1", str_1, 11);
    shm.Write("init_2", str_2, 11);

    cout << "[before]" << endl;

    shm.Print();

    cout << "[clear]" << endl;

    shm.Clear();

    cout << "[after]" << endl;

    shm.Print();

    cout << endl;

    //

    cout << "Test of [bool] :" << endl;

    bool b = true;
    shm.Write("shm_bool", b);
    shm.Read("shm_bool", b);
    if (b) { cout << " shm_bool = TRUE" << endl; }

    cout << " [change]" << endl;

    shm.Write("shm_bool", false);
    shm.Read("shm_bool", b);
    if (!b) { cout << " shm_bool = FALSE" << endl; }

    cout << endl;

    //

    cout << "Test of [vec bool] :" << endl;

    vector<bool> vb = { true, true };
    vector<bool> vb_2;
    shm.Write("shm_vbool", vb);
    shm.Read("shm_vbool", vb_2);
    if ((vb_2[0]) && (vb_2[1])) { cout << " shm_vbool = TRUE" << endl; }

    cout << " [change]" << endl;

    for (auto &&it : vb) { it = false; }
    shm.Write("shm_vbool", vb);
    shm.Read("shm_vbool", vb_2);
    if ((!vb_2[0]) && (!vb_2[1])) { cout << " shm_vbool = FALSE" << endl; }

    cout << endl;

    //

    cout << "Test of [char] : " << endl;

    char c = '\x2a'; // '*'
    char c_2;

    shm.Write("shm_char", c);
    shm.Read("shm_char", c_2);

    cout << " IN  = " << c << endl;
    cout << " OUT = " << c_2 << endl << endl;

    //

    cout << "Test of [vec char] : " << endl;

    vector<char> vc = { '\x41', '\x42', '\x43' }; // 'A' 'B' 'C'
    vector<char> vc_2;

    shm.Write("shm_vchar", vc);
    shm.Read("shm_vchar", vc_2);

    cout << " IN  = ";
    for (const auto &it : vc) { cout << it << " "; }
    cout << endl;
    cout << " OUT = ";
    for (const auto &it : vc_2) { cout << it << " "; }
    cout << endl << endl;

    //
    // Preparing a multi-processing test
    //

    int n = 0;

    shm.Write("shm_int",     -1);
    shm.Write("shm_int",     n);

    vector<int> vint = { 10, 20, 30 };
    shm.Write("shm_vint",    vint);

    vector<double> vdouble = { 11.111, 22.222, 33.333 };
    shm.Write("shm_vdouble", vdouble);

    string write_data = "test: " + to_string(n);
    shm.Write("shm_string",  "", 30);
    shm.Write("shm_string",  write_data);

    vector<string> vstring = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    shm.Write("shm_vstring", vstring, 10);

    //
    // Output setting file
    //

    shm.WriteFile("./MyShmS.sample.shm"); // 外部に共有メモリ設定をはきだし

    //
    // Set semaphore manually
    //

    for (int i = 0; i < 2; i++) { // 2 つのマルチプロセスを起動させる

        if (fork() == 0) {        // マルチプロセス

            shm.Clear_shm_info(); // このサンプルのテスト用関数，普段使うな！

            if (!shm.ReadFile("./MyShmS.sample.shm")) { exit(0); } // 共有メモリ設定の読み取り

            shm.Print();

            if (i >= 1) { sleep(i); } // i 番目の子プロセスは i 秒待ってからスタート

            string read_data;

            while (1) {

                if (n >= 5) { break; }

                cout << "[process " << i << " has started.]" << endl;
                sleep(3);

                read_data = "";
                shm.Read("shm_string", read_data);
                cout << " process " << i << " : Read = " << read_data << endl;

                shm.Read("shm_int",    n);
                n++;
                shm.Write("shm_int", n);

                string vint_str = "";
                shm.Read("shm_vint", vint);
                // cout << "[sample] vint size 1 = " << vint.size() << endl;
                for (auto &it : vint) {
                    it       += 1;
                    vint_str += to_string(it) + ",";
                }
                // cout << "[sample] vint size 2 = " << vint.size() << endl;
                shm.Write("shm_vint", vint);

                string vdouble_str = "";
                shm.Read("shm_vdouble", vdouble);
                for (auto &it : vdouble) {
                    // for(uint i=0; i<vdouble.size(); i++) {
                    it += 0.0001;
                    vdouble_str += to_string(it) + ",";
                }
                shm.Write("shm_vdouble", vdouble);

                string vstring_str = "";
                shm.Read("shm_vstring", vstring);
                vstring_str = vstring[n] + ".day";

                write_data  = "test: " + to_string(n) + " : " + vint_str + " : " + vdouble_str + " : " + vstring_str;
                shm.Write("shm_string", write_data);
                shm.Read("shm_string", read_data);
                cout << " process " << i << " : Write= " << read_data << endl;

                sleep(1);

                cout << "[process " << i << " end loop, waiting unlock...]" << endl;

                if (n > 5) { break; }

            }

            cout << "[child process " << i << " end]" << endl;

            exit(EXIT_SUCCESS); // 子プロセスは exit で終了.

        }

    }

    for (int i = 0; i < 2; i++) {
        wait(NULL); // 子プロセスの終了を待つ
    }

    // shm.Close(); // デストラクタがやってくれるので不要．

    cout << "[sample end]" << endl;

    return 0;

}
