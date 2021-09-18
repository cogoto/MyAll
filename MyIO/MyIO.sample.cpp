#include <complex>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "MyIO.hpp"

using namespace std;

/// to string for Cygwin

/*
   template < typename T > std::string to_string( const T& n ) {

        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;

   }
 */
int main() {

    using uint = unsigned int;

    cout << "TEST OF [MyI]" << endl;

    MyAll::MyI *in = new MyAll::MyI("./MyIO.sample.txt");

    vector<vector<string> > data;
    vector<string> delimiter       = { ",", "=", R"(\s+)" };
    vector<string> delimiter_blank = {};
    string comment_out = ";";

    cout << "reading of [MyI::ReadData]" << endl;

    // in->ReadData( data );
    // in->ReadData( data, delimiter );
    in->ReadData(data, delimiter, comment_out);

    cout << "reading of [MyI::ReadDouble]" << endl;

    vector<vector<double> > data2;
    in->ReadDouble(data2, delimiter, comment_out);

    cout << "reading of [MyI::ReadParam] <string>" << endl;

    map<string, string> prm1;
    in->ReadParam(prm1, delimiter, comment_out);
    vector<string>      key;
    in->GetKey(key); /// ReadParam() を走らせた直後しかそのときのキーワードは取得できないので注意(ここではprm1の)

    cout << "reading of [MyI::ReadParam] <double>" << endl;

    map<string, double> prm2;
    in->ReadParam(prm2, delimiter, comment_out);

    cout << "reading of [MyI::ReadParam] vector<double>" << endl;

    map<string, vector<double>> prm6;
    in->ReadParam(prm6, delimiter, comment_out);

    cout << "reading of [MyI::ReadSection]" << endl;

    map<string, map<int, string> > prm3;
    in->ReadSection(prm3, "[", {}, comment_out); // delimiter なしで，一行全部パラメータとして読む
    // in->ReadSection( prm3, "[", vector<string>(), comment_out );
    // in->ReadSection( prm3, "[", delimiter_blank, comment_out );

    cout << "reading of [MyI::ReadSection]" << endl;

    map<string, map<string, string> > prm4;
    in->ReadSection(prm4, "[", delimiter, comment_out);
    vector<string> session1;
    in->GetSection(session1);
    map<string, map<int, string> > key4;
    in->GetKey(key4); /// ReadSection() を走らせた直後しかそのときのキーワードは取得できないので注意．(ここではprm4の)

    cout << "reading of [MyI::ReadParam] <string><int>" << endl;

    map<string, map<int, string> > prm5;
    in->ReadParam(prm5, delimiter, comment_out);
    // vector<string> key5;
    // in->GetKey(key5); /// ReadParam() を走らせた直後しかそのときのキーワードは取得できないので注意(ここではprm1の)

    cout << endl << "Test of file existance" << endl << endl;

    string no_exist_file = "no_exist_file.txt";
    if (!in->CheckExist(no_exist_file)) {
        cout << no_exist_file << " do not existed" << endl;
    }

    delete in;

    //

    cout << endl << "Result of [MyI::ReadData]" << endl;
    for (uint i = 0; i < data.size(); i++) {
        for (uint j = 0; j < data[i].size(); j++) {
            cout << data[i][j] << " : ";
        }
        cout << endl;
    }
    // cout << endl;

    cout << endl << "Result of [MyI::ReadDouble]" << endl;
    for (uint i = 0; i < data2.size(); i++) {
        for (uint j = 0; j < data2[i].size(); j++) {

            // string form = "%08.2f ";
            // cout << format("%04.1f ") % data2[i][j];
            // cout << format(form) % data2[i][j];
            printf("%08.2f ", data2[i][j]);

            // cout << str << " : ";
            // cout << data2[i][j] + data2[i][j] << " : ";

        }
        cout << endl;
    }
    // cout << endl;

    cout << endl << "Result of [MyI::ReadParam] <string>" << endl;
    cout << "[ip1] " << prm1["ip1"] << endl;
    cout << "[ip2] " << prm1["ip2"] << endl;
    cout << "[port1] " << prm1["port1"] << endl;
    cout << "[port2] " << prm1["port2"] << endl;

    cout << endl << "Result of [MyI::GetKey]" << endl;
    cout << " SIZE = " << key.size() << endl;

    cout << endl << "Result of [MyI::ReadParam] <string><int>" << endl;
    cout << "[prm1]  ";
    for (uint i = 0; i < prm5["prm1"].size(); i++) {
        if (i != 0) { cout << " : "; }
        cout << prm5["prm1"][i];
    }
    cout << endl;
    cout << "[ip1]   " << prm5["ip1"][0] << " : " << prm5["ip1"][1] << endl;
    cout << "[ip2]   " << prm5["ip2"][0] << " : " << prm5["ip2"][1] << endl;
    cout << "[port1] " << prm5["port1"][0] << endl;
    cout << "[port2] " << prm5["port2"][0] << endl;

    cout << endl << "Result of [MyI::ReadParam] <double>" << endl;
    cout << "[ip1] " << prm2["ip1"] << endl;
    cout << "[ip2] " << prm2["ip2"] << endl;
    cout << "[port1] " << prm2["port1"] << endl;
    cout << "[port2] " << prm2["port2"] << endl;

    cout << endl << "Result of [MyI::ReadParam] vector<double>" << endl;
    cout << "[prm1][0] " << prm6["prm1"][0] << endl;
    cout << "[prm1][1] " << prm6["prm1"][1] << endl;
    cout << "[prm2][0] " << prm6["prm2"][0] << endl;
    cout << "[prm2][1] " << prm6["prm2"][1] << endl;
    cout << "[ip1][0]  " << prm6["ip1"][0] << endl;
    cout << "[ip1][1]  " << prm6["ip1"][1] << endl;
    cout << "[ip2][0]  " << prm6["ip2"][0] << endl;
    cout << "[ip2][1]  " << prm6["ip2"][1] << endl;
    cout << "[prm1] size = " << prm6["prm1"].size() << endl;
    cout << "[prm2] size = " << prm6["prm2"].size() << endl;
    cout << "[ip1]  size = " << prm6["ip1"].size() << endl;
    cout << "[ip2]  size = " << prm6["ip2"].size() << endl;

    cout << endl << "Result of [MyI::ReadSection]" << endl << endl;
    cout << "[SECTION_1][0] " << prm3["SECTION_1"][0] << endl;
    cout << "[SECTION_1][1] " << prm3["SECTION_1"][1] << endl;
    cout << "[SECTION_2][0] " << prm3["SECTION_2"][0] << endl;
    cout << "[SECTION_2][1] " << prm3["SECTION_2"][1] << endl << endl;
    cout << "[SECTION_1][sec_1] " << prm4["SECTION_1"]["sec_1"] << endl;
    cout << "[SECTION_1][sec_2] " << prm4["SECTION_1"]["sec_2"] << endl;
    cout << "[SECTION_2][sec_3] " << prm4["SECTION_2"]["sec_3"] << endl;
    cout << "[SECTION_2][sec_4] " << prm4["SECTION_2"]["sec_4"] << endl;

    cout << endl << "Result of [MyI::GetSection]" << endl << endl;
    cout << "[0] " << session1[0] << endl;
    cout << "[1] " << session1[1] << endl;

    cout << endl << "Result of [MyI::GetKey]" << endl << endl;
    cout << session1[0] << " SIZE = " << key4[session1[0]].size() << endl;
    cout << session1[0] << " [0] " << key4[session1[0]][0].c_str() << endl;
    cout << session1[0] << " [1] " << key4[session1[0]][1] << endl;
    cout << session1[1] << " SIZE = " << key4[session1[1]].size() << endl;
    cout << session1[1] << " [0] " << key4[session1[1]][0] << endl;
    cout << session1[1] << " [1] " << key4[session1[1]][1] << endl;
    cout << session1[1] << " [2] " << key4[session1[1]][2] << endl;

    /*
       cout << endl << "Test of file existance" << endl << endl;
       string     no_exist_file = "no_exist_file.txt";
       MyAll::MyI in2(no_exist_file);
       cout << "-> " << flush;
       if (!in2.ReadData(data, delimiter, comment_out)) {
        cout << no_exist_file << " do not existed" << endl;
       }
     */

    //

    cout << endl << "Result of [MyI::ReadComplexDouble]" << endl << endl;

    unique_ptr<MyAll::MyI> in3(new MyAll::MyI("./MyIO.sample2.txt"));

    delimiter = { R"(\()", R"(\))", R"(\s+)" };
    string complex_delimiter = ",";

    vector<vector<complex<double> > > complex_data;
    in3->ReadComplexDouble(complex_data, complex_delimiter, delimiter, comment_out, false);

    for (const auto &it1 : complex_data) {
        for (const auto &it2 : it1) {
            cout << it2 << " ";
        }
        cout << endl;
    }


    ///
    ///
    ///

    cout << endl << "TEST OF [MyO]" << endl;

    MyAll::MyO *   out = new MyAll::MyO("./MyIO.sample.out");

    vector<string> out_data1 { "#No.1", "No.2", "No.3", "No.4" };
    vector<vector<string> > out_data2;
    vector<double> out_data3 { 11.11, 22.22, 33.33, 44.44 };

    vector<string> out_data2_tmp;

    for (int i = 0; i < 6; i++) {
        out_data2_tmp.clear();
        vector<string>().swap(out_data2_tmp);
        for (int j = 0; j < 4; j++) {
            out_data2_tmp.push_back(to_string(i * j));
            // out_data2[i][j] = "s";
            // string out_data4 = to_string(1);
            // cout << to_string(j) << endl;
            // out_data2[i][j] = "a";
        }
        out_data2.push_back(out_data2_tmp);
    }

    if (!out->Close()) { cout << "[MyO] Failed to close. : No.0" << endl; }
    if (!out->ReOpen()) { cout << "[MyO] Failed to re-open." << endl; }

    string out_data0 = "##";
    if (!out->Write(out_data0)) { cout << "[MyO] Failed to Write()." << endl; }
    out->Write(" This is a sample file of [MyO]\n");
    out->Write(out_data1);
    out->Write(out_data2, ", ");
    out->Write("\n\n");

    if (!out->Writef("%s %s %s %s\n", out_data1[0].c_str(), out_data1[1].c_str(), out_data1[2].c_str(), out_data1[3].c_str())) { cout << "[MyO] Failed Writef()." << endl; }
    out->Writef("%8.3f %8.3f %8.3f %8.3f\n", out_data3[0],                   out_data3[1],                   out_data3[2],                   out_data3[3]);
    out->Writef("%05d %05d %05d %05d\n",     static_cast<int>(out_data3[0]), static_cast<int>(out_data3[1]), static_cast<int>(out_data3[2]), static_cast<int>(out_data3[3]));

    if (!out->Close()) { cout << "[MyO] Failed to close. : No.1" << endl; }

    if (!out->Out("\ntest : [MyO::Out]\n")) { cout << "[MyO] Failed to write." << endl; }
    out->Out(out_data1);
    out->Outf("%.4f %.4f %.4f %.4f\n", out_data3[0], out_data3[1], out_data3[2], out_data3[3]);

    delete out;

    cout << "[MyO] Check Output : MyIO.sample.out" << endl;

    //

    return 0;

}
