#include <iostream>
#include <string>
#include <vector>

#include "MyString.hpp"

using namespace std;


int main() {

    string text_ = "I have {+1.2} apple pens & -3.4 pineapple pens. !!!!!!";

    cout << endl << "TEXT : " << text_ << endl;

    MyAll::MyString my;

    int j;

    //

    cout << endl << "TEST OF [ Split( const string &text__, const vector<string> &delimiter__) ]" << endl;
    vector<string> delimiter_split = { R"(\s+)", R"(have)", R"(e)", R"(\+)", R"(\{)", R"(\})" };

    // cout << endl << "TEST OF [ Split( const string &text__, const string &delimiter__) ]" << endl;
    // string delimiter_split = R"(\s+)";
    // string delimiter_split = R"(have)";

    for (auto it : delimiter_split) {
        cout << "pattern = " << it << endl;
    }

    vector<string> result;
    result = my.Split(text_, delimiter_split);
    // my.Split( result, text_, delimiter_split );

    j = 0;
    for (auto it : result) {
        cout << " result[" << j << "] =" << it.c_str() << endl;
        j++;
    }

    cout << endl << "TEST OF [ Split_match( string const &text, string const &match_pattern ) ]" << endl;

    string pattern_split = "(\\w)+"; /// word
    // string pattern_split = "(p)+";
    // string pattern_split = "(\\S)+"; /// without blanks
    // pattern_split = "(\\d)+"; /// positive Integer only ([0-9]+)

    cout << "pattern = " << pattern_split.c_str() << endl;
    result = my.Split_match(text_, pattern_split);
    j      = 0;
    for (auto it : result) {
        cout << " result[" << j << "] = " << it.c_str() << endl;
        j++;
    }

    //

    pattern_split = "(\\d)+"; /// positive Integer only ([0-9]+)

    cout << "pattern = " << pattern_split.c_str() << endl;
    result = my.Split_match(text_, pattern_split);
    j      = 0;
    for (auto it : result) {
        cout << " result[" << j << "] = " << it.c_str() << endl;
        j++;
    }

    //

    cout << "pattern is without blanks ( (\\S)+: default )" << endl;

    result = my.Split_match(text_);

    j      = 0;
    for (auto it : result) {
        cout << " result[" << j << "] = " << it.c_str() << endl;
        j++;
    }

    //

    cout << endl << "TEST OF [ Replace_directly( string &text, const vector<string> &pattern_before, const string &pattern_after ) ]" << endl;

    vector<string> pattern_before { "(!)+$", "pens", "^I", "^apple", "have$" };

    string pattern_after = "?";

    string text_2 = text_;

    my.Replace_directly(text_2, pattern_before, pattern_after);

    for (auto it : pattern_before) {
        cout << " pattern_before    = " << it << endl;
    }
    cout << " pattern_after     = " << pattern_after.c_str() << endl;
    cout << " origin text       = " << text_ << endl;
    cout << " result            = " << text_2 << endl;

    //

    cout << endl << "TEST OF [ Replace_directly( string &text, const string &pattern_before, const string &pattern_after ) ]" << endl;

    string pattern_before2 = "apple";

    string pattern_after2  = "berry";

    string text_3 = text_;

    my.Replace_directly(text_3, pattern_before2, pattern_after2);

    cout << " pattern_before = " << pattern_before2.c_str() << endl;
    cout << " pattern_after  = " << pattern_after2.c_str() << endl;
    cout << " result         = " << text_3.c_str() << endl;

    //

    cout << endl << "TEST OF [ Replace( const string &text, const vector<string> &pattern_before, const string &pattern_after ) ]" << endl;

    string result_replace = my.Replace(text_, pattern_before, pattern_after);

    for (auto it : pattern_before) {
        cout << " pattern_before    = " << it << endl;
    }
    cout << " pattern_after     = " << pattern_after.c_str() << endl;
    cout << " result            = " << result_replace.c_str() << endl;


    cout << endl << "TEST OF [ Replace( const string &text, const string &pattern_before, const string &pattern_after ) ]" << endl;

    string result_replace2 = my.Replace(text_, pattern_before2, pattern_after2);

    cout << " pattern_before = " << pattern_before2.c_str() << endl;
    cout << " pattern_after  = " << pattern_after2.c_str() << endl;
    cout << " result         = " << result_replace2.c_str() << endl;

    //

    cout << endl << "TEST OF [ Replace_seq( const string &text, const vector<string> &pattern_before, const string &pattern_after ) ]" << endl;

    string text_4 = "memtodk1 2 2048 1 4 59 0 29045 /mnt/raid1/r20063a_miz_S1_ 2 4 59 0 29045 /mnt/raid2/r20063a_miz_S2_ -d";

    vector<string> pattern_before_4 { "nohup", "memtodk1", "-d", R"(^\s+)", R"(\s+$)" };

    cout << " origin text   = " << text_4 << endl;
    cout << " Replace()     = " << my.Replace(text_4, pattern_before_4, "") << endl;
    cout << " Replace_seq() = " << my.Replace_seq(text_4, pattern_before_4, "") << endl;


    //

    cout << endl << "TEST OF [ Match( const string &text__, const string &pattern__ ) ]" << endl;

    string text_ip    = "192.168.0.1";
    string pattern_ip = R"([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)";

    cout << " input text  = " << text_ip.c_str() << endl;
    cout << " pattern     = " << pattern_ip.c_str() << endl;

    if (my.Match(text_ip, pattern_ip)) {
        cout << " -> Pattern matched!" << endl;
    }
    else {
        cout << " -> No pattern matched..." << endl;
    }

    string text_match_1    = "I have an apple pen.";
    string pattern_match_1 = R"(.*app.*)";

    cout << " input text  = " << text_match_1 << endl;
    cout << " pattern     = " << pattern_match_1 << endl;

    if (my.Match(text_match_1, pattern_match_1)) {
        cout << " -> Pattern matched!" << endl;
    }
    else {
        cout << " -> No pattern matched..." << endl;
    }

    string text_match_2    = "SET START PRM";
    string pattern_match_2 = R"(^SET\s*\S+.*)";

    cout << " input text  = " << text_match_2 << endl;
    cout << " pattern     = " << pattern_match_2 << endl;

    if (my.Match(text_match_2, pattern_match_2)) {
        cout << " -> Pattern matched!" << endl;
    }
    else {
        cout << " -> No pattern matched..." << endl;
    }

    string text_match_3    = "!UTC?0:2019227091822:2019227091822;\n";
    string pattern_match_3 = "^\\!UTC\\?0:.*;\\n*$";
    // string pattern_match_3 = R"(^\!UTC\?0:.*;\n*$)";

    cout << " input text  = " << text_match_3 << endl;
    cout << " pattern     = " << pattern_match_3 << endl;

    if (my.Match(text_match_3, pattern_match_3)) {
        cout << " -> Pattern matched!" << endl;
    }
    else {
        cout << " -> No pattern matched..." << endl;
    }

    cout << endl;

    //

    cout << endl << "TEST OF [ Str2d( string const &in_ ) ]" << endl;

    string Str_01 = "3.0-e02"; /// = 3.0e-02
    string Str_02 = "2.1E+02"; /// = 2.1e+02
    string Str_03 = "Xff";     // = 0xff
    string Str_04 = "abc4.3+e01d";
    string Str_05 = "xyz0x02";
    string Str_06 = "4.567E+00 ";
    string Str_07 = "0x2f      ";
    string Str_08 = "0x03D09000";
    string Str_09 = "1000000";

    cout << Str_01 << " -> " << my.Str2d(Str_01, false) << endl;
    cout << Str_02 << " -> " << my.Str2d(Str_02, false) << endl;
    cout << Str_03 << " -> " << my.Str2d(Str_03, false) << endl;
    cout << Str_04 << " -> " << my.Str2d(Str_04, false) << endl;
    cout << Str_05 << " -> " << my.Str2d(Str_05, false) << endl;
    cout << Str_06 << " -> " << my.Str2d(Str_06, false) << endl;
    cout << Str_07 << " -> " << my.Str2d(Str_07, false) << endl;
    cout << Str_08 << " -> " << my.Str2d(Str_08, false) << endl;
    cout << Str_09 << " -> " << my.Str2d(Str_09, false) << endl;

    //

    cout << endl << "TEST OF [ Str2i( string const &in_ ) ]" << endl;

    cout << Str_01 << " -> " << my.Str2i(Str_01, false) << endl;
    cout << Str_02 << " -> " << my.Str2i(Str_02, false) << endl;
    cout << Str_03 << " -> " << my.Str2i(Str_03, false) << endl;
    cout << Str_04 << " -> " << my.Str2i(Str_04, false) << endl;
    cout << Str_05 << " -> " << my.Str2i(Str_05, false) << endl;
    cout << Str_06 << " -> " << my.Str2i(Str_06, false) << endl;
    cout << Str_07 << " -> " << my.Str2i(Str_07, false) << endl;
    cout << Str_08 << " -> " << my.Str2i(Str_08, false) << endl;
    cout << Str_09 << " -> " << my.Str2i(Str_09, false) << endl;

    //

    cout << endl << "TEST OF [ Str2b( string const &in_ ) ]" << endl;

    vector<string> Str_bool = { "", "0", "f", "F", "FaLsE", "N", "nO", "true", "fal" };

    for (auto it : Str_bool) {
        cout << it << " -> ";
        if (my.Str2b(it)) { cout << "true"  << endl; }
        else                { cout << "false" << endl; }
    }

    //

    cout << endl << "TEST OF [ VecStr2d( vector<string> const &in_, vector<double> &out_ ) ]" << endl;

    vector<string> v1 { "1.1e+00", "2.2e+01", "3.3+e02", "4.4+e03", "0x03D09000", "AAA" };

    vector<double> v1_result;

    my.VecStr2d(v1, v1_result);

    j = 0;
    for (auto it : v1) {
        cout << " [" << j << "] = " << it.c_str() << " -> " << v1_result[j] << endl;
        j++;
    }

    //

    cout << endl << "TEST OF [ VecStr2d( vector<vector<string> > const &in_, vector<vector<double> > &out_ ) ]" << endl;

    vector<vector<string> > v2 { { "1.1e-01", "2.2e-02" }, { "3.3-e03", "abc" } };

    vector<vector<double> > v2_result;

    my.VecStr2d(v2, v2_result);

    cout << "[0][0] : " << v2[0][0].c_str() << " -> " << v2_result[0][0] << endl;
    cout << "[0][1] : " << v2[0][1].c_str() << " -> " << v2_result[0][1] << endl;
    cout << "[1][0] : " << v2[1][0].c_str() << " -> " << v2_result[1][0] << endl;
    cout << "[1][1] : " << v2[1][1].c_str() << " -> " << v2_result[1][1] << endl;

    //

    cout << endl << "TEST OF [ StrHex2c( const string &in__, char &out__ ) ]" << endl;

    string Strhex_01 = "0x31   "; /// = 1
    string Strhex_02 = "64     "; /// = @
    string Strhex_03 = "6.5+e01"; /// = A
    string Strhex_04 = "0x0a   "; /// = LF
    string Strhex_05 = "0x7a   "; /// = z

    unsigned char Strhex_out_c;

    my.Hex2c(Strhex_01, Strhex_out_c);
    cout << Strhex_01 << " -> " << Strhex_out_c << endl;
    my.Hex2c(Strhex_02, Strhex_out_c);
    cout << Strhex_02 << " -> " << Strhex_out_c << endl;
    my.Hex2c(Strhex_03, Strhex_out_c);
    cout << Strhex_03 << " -> " << Strhex_out_c << endl;
    my.Hex2c(Strhex_04, Strhex_out_c);
    cout << Strhex_04 << " -> " << Strhex_out_c << endl;
    my.Hex2c(Strhex_05, Strhex_out_c);
    cout << Strhex_05 << " -> " << Strhex_out_c << endl;

    //

    char char_01 = '\x31'; // 49
    char char_02 = '\xe3'; // -29 or 227
    char char_03 = '\xff'; //  -1 or 255
    char char_04 = '\x00'; //  0

    cout << endl << "TEST OF [ Char2int( const char &in__ ) ]" << endl;

    cout << "0x31 = " << my.Char2int(char_01) << endl;
    cout << "0xe3 = " << my.Char2int(char_02) << endl;
    cout << "0xff = " << my.Char2int(char_03) << endl;
    cout << "0x00 = " << my.Char2int(char_04) << endl;

    cout << endl << "TEST OF [ Char2uint( const char &in__ ) ]" << endl;

    cout << "0x31 = " << my.Char2uint(char_01) << endl;
    cout << "0xe3 = " << my.Char2uint(char_02) << endl;
    cout << "0xff = " << my.Char2uint(char_03) << endl;
    cout << "0x00 = " << my.Char2uint(char_04) << endl;

    //

    /*
        cout << endl << "TEST OF [ Str2hex( const string &in__, int &out__ ) ]" << endl;

        string Strhex_11 = "0x2f      "; /// = 47
        string Strhex_12 = "0x03D09000"; /// = 64000000

        unsigned int Strhex_out_i;

        my.Str2hex( Strhex_11, Strhex_out_i );
        cout << Strhex_11 << " -> " << Strhex_out_i << endl;
        my.Str2hex( Strhex_12, Strhex_out_i );
        cout << Strhex_12 << " -> " << Strhex_out_i << endl;
     */

    //

    cout << endl << "TEST OF [ Sprintf( string str, const char* format, ... ) ]" << endl;

    string str_;

    int    in_01   = 1;
    int    in_02   = -22;
    float  in_03   = 33.3;
    double in_04   = -44.44;
    char   in_05[] = "char_test";
    string in_06   = "string_test";

    my.Sprintf(str_, "%02d %d %05.2f %.3f %s %s\n", in_01, in_02, in_03, in_04, in_05, in_06.c_str());

    cout << str_ << endl;

    //

    cout << endl << "TEST OF [ Split_double( string const &in_, vector<double> &out_ ) ]" << endl;

    string text2_ = "I have -10.57 + 0.57 - 2 apple-pens  !!! . .. ... #$%&@=*:;_\?+,+\"\'^~`|(){}[]<> desu.";

    vector<double> split_double_out;

    my.Split_double(split_double_out, text2_);

    cout << "text : " << text2_.c_str() << endl;
    cout << "size : " << split_double_out.size() << endl;

    j = 0;
    for (auto it : split_double_out) {
        cout << " [" << j << "] = " << it << endl;
        j++;
    }

    //

    cout << endl << "TEST OF [ ToUpperCase / ToLowerCase ( string const &in_ ) ]" << endl;

    string text_UL       = "I will wait for Ms.Alice until 2:00 PM.";

    string text_UL_upper = text_UL;
    string text_UL_lower = text_UL;

    my.ToUpperCase(text_UL_upper);
    my.ToLowerCase(text_UL_lower);

    cout << "text           : " << text_UL << endl;
    cout << " -> upper case : " << text_UL_upper << endl;
    cout << " -> lower case : " << text_UL_lower << endl;

    //

    return 0;

}  // main
