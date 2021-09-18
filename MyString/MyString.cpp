// ================================================ //
///
/// @file   MyString.cpp
/// @brief  Implementation of MyString.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyString.hpp"

//

#include <cstdint>

#include <cstdarg>

#include <iostream>

//

#include <regex>

#include <string>

// #include <boost/regex.hpp>

//

namespace MyAll {

//

using namespace std;

// using uint = uint_fast32_t;

// using namespace boost;


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyString::MyString() {

    sprintf_n_max_ = 8192;

    // Init for Str2d()

    str2num_before_.resize(3);
    str2num_before_[0] = vector<string> { R"(\+[eE])", R"(E\+)" };
    str2num_before_[1] = vector<string> { R"(-[eE])",  R"(E-)" };
    str2num_before_[2] = vector<string> { R"(^[xX])",  R"(^0X)" };

    str2num_after_.resize(3);
    str2num_after_[0] = "e+";
    str2num_after_[1] = "e-";
    str2num_after_[2] = "0x";

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyString::~MyString() {}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with a delimiter.
/// @param[out] result__ : A splited string array.
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  delimiter__ : A delimiting Pattern.
///             The default delimiter is blank.
///             The pattern is described by the regular expression (ECMAScript),
///             the examples are as belows,
///             @arg R"(\s+)" ... Splits by blanks.
///             @arg R"(\d+)" ... Splits by integers ([0-9]+) only.
///             @arg R"(\w+)" ... Splits by words.
///             @arg R"(abc)" ... Splits by "abc".
///             @arg R"(\+)"  ... Splits by "+" character.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Split(std::vector<std::string> &result__, const std::string &text__, const std::string &delimiter__) {

    vector<string>().swap(result__);

    if (text__.empty()) { return; }

    if (delimiter__.size() == 0) {
        result__.push_back(text__);
        // cout << "[MyString::Split] no delimiter" << endl;
        return;
    }

    regex delimiter_2(delimiter__);

    sregex_token_iterator first(text__.begin(), text__.end(), delimiter_2, -1);
    sregex_token_iterator last;

    result__.resize(distance(first, last));

    // copy(first, last, result__.begin());

    int i = 0;

    for_each(first, last, [&result__, &i](const string &it) { result__[i] = it; i++; });

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with delimiters.
/// @param[out] result__ : A splited string array.
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  delimiter__ : Delimiting Patterns.
///             The lower number has more higher priority.
///             Refer the other Split() explanation for the regex patterns.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Split(std::vector<std::string> &result__, const std::string &text__, const std::vector<std::string> &delimiter__) {

    vector<string>().swap(result__);

    if (text__.empty()) { return; }

    ///
    /// In internal, all delimiters are replaced to "\n[0-100]*" (ex. "\n0123"),
    /// which string is not in the given one.
    /// If there is no replaceable delimiters,
    /// we use MyString::Split_slow().
    ///

    string delimiter_replace = "\n";

    bool   decide_delimiter  = false;

    for (int i = 0; i <= 100; i++) {

        if (text__.find(delimiter_replace) != std::string::npos) {
            delimiter_replace += to_string(i);
            continue;
        }
        else {
            decide_delimiter = true;
            break;
        }

    }

    if (!decide_delimiter) {
        MyString::Split_slow(result__, text__, delimiter__);
        return;
    }

    string text_new = text__;

    MyString::Replace_directly(text_new, delimiter__, delimiter_replace);

    MyString::Split(result__, text_new, "(" + delimiter_replace + ")+");

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with delimiters.
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  delimiter__ : A delimiting Pattern.
///             This function will be a little slower than void Split().
///             The default delimiter is blank.
///             Refer the other Split() explanation for the regex patterns.
/// @return     A splited string array.
///
// ------------------------------------------------ //


std::vector<std::string> MyString::Split(const std::string &text__, const std::string &delimiter__) {

    vector<string> result;

    MyString::Split(result, text__, delimiter__);

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with delimiters.
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  delimiter__ : Delimiting Patterns.
///             This function will be a little slower than void Split().
///             The lower number has more higher priority.
///             Refer the other Split() explanation for the regex patterns.
/// @return     A splited string array.
///
// ------------------------------------------------ //


std::vector<std::string> MyString::Split(const std::string &text__, const std::vector<std::string> &delimiter__) {

    vector<string> result;

    MyString::Split(result, text__, delimiter__);

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with delimiters. (slower)
///             This function will be used when Split() cannot find an internal replaceable delimiter.
/// @param[out] result__ : Splited string array.
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  delimiter__ : Delimiting Patterns.
///             The lower number has more higher priority.
///             Refer the other Split() explanation for the regex patterns.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Split_slow(std::vector<std::string> &result__, const std::string &text__, const std::vector<std::string> &delimiter__) {

    if (text__.empty()) { return; }

    vector<string> result_2;
    vector<string> result_3;

    vector<string>().swap(result__);

    result__.push_back(text__);

    if (delimiter__.size() <= 0) { return; }

    for (const auto &it_delimiter : delimiter__) {

        if (it_delimiter.empty()) { continue; }

        vector<string>().swap(result_3);

        for (const auto &it_result : result__) {

            if (it_result.empty()) { continue; }

            vector<string>().swap(result_2);

            MyString::Split(result_2, it_result, it_delimiter);

            if (result_2.empty()) { continue; }

            if (result_2[0].empty()) {
                copy(result_2.begin() + 1, result_2.end(), back_inserter(result_3));
            }
            else {
                copy(result_2.begin(), result_2.end(), back_inserter(result_3));
            }

        }

        result__ = result_3;

    }

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with a matched pattern.
/// @param[out] result__ : splited string array.
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  match_pattern__ : A pick-up pattern.
///             The pattern is described by the regular expression (ECMAScript),
///             the examples are as belows,
//              @arg R"(\S)+" ... Pick up without blanks. : default
//              @arg R"(\d)+" ... Pick up positive integers ([0-9]+) only.
//              @arg R"(\w)+" ... Pick up words.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Split_match(std::vector<std::string> &result__, const std::string &text__, const std::string &match_pattern__) {

    // vector<string> result;

    regex  pattern__2(match_pattern__);

    auto   it  = text__.cbegin();
    auto   end = text__.cend();

    smatch match_results;

    while (regex_search(it, end, match_results, pattern__2)) {
        result__.push_back(match_results.str());
        it = match_results[0].second;
    }

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string with a matched pattern. (a little slower)
/// @param[in]  text__ : A string you wanna split.
/// @param[in]  match_pattern__ : A pick-up pattern.
///             The pattern is described by the regular expression (ECMAScript),
///             Refer the other Split_match() explanation for the regex patterns.
/// @return     splited string array.
///
// ------------------------------------------------ //


std::vector<std::string> MyString::Split_match(const std::string &text__, const std::string &match_pattern__) {

    vector<string> result;

    MyString::Split_match(result, text__, match_pattern__);

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function splits a string to real numbers.
/// @param[in]  text__ : String you wanna split.
/// @param[out] result__ : Converted real numbers.
/// @return     None.
///
// ------------------------------------------------ //


bool MyString::Split_double(std::vector<double> &result__, const std::string &text__) {

    //
    // pattern for removing words without real-number.
    //
    vector<string> string_pattern = {
        "([!#$%&@=*:;_\\?+,\"\'^/~`|])+",
        "([{}<>()])+",
        "([[]])+",
        "([A-Z])+",
        "([a-z])+",
        "([-])+([A-Z])+",
        "([-])+([a-z])+",
        "([-])([\\s])+",
        "([.])([.])+",
        "([.])+([A-Z\\s])+",
        "([A-Z\\s])+([.])+"
    };

    string in__2;

    MyString::Replace(in__2, text__, string_pattern, " ");

    vector<string> in__3;

    MyString::Split_match(in__3, in__2);

    return MyString::VecStr2d(in__3, result__);

}


// ------------------------------------------------ //
///
/// @brief         This function directly replaces a string using a matching pattern.
/// @param[in,out] text__ : The target string.
/// @param[in]     pattern_before__ : A pattern you wanna replace.
///                The pattern could be described by the regular expression (ECMAScript).
/// @param[in]     pattern_after__ : A string after the replacement.
/// @return        None.
///
// ------------------------------------------------ //


void MyString::Replace_directly(std::string &text__, const std::string &pattern_before__, const std::string &pattern_after__) {

    text__ = MyString::Replace(text__, pattern_before__, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief         This function directly replaces a string using matching patterns at once.
/// @param[in,out] text__ : The target string.
/// @param[in]     pattern_before__ : Patterns you wanna replace.
///                The patterns could be described by the regular expression (ECMAScript).
///                These are applied at once as a pattern of "(A|B|C|...)".
/// @param[in]     pattern_after__ : A string after the replacement.
/// @return        None.
///
// ------------------------------------------------ //


void MyString::Replace_directly(std::string &text__, const std::vector<std::string> &pattern_before__, const std::string &pattern_after__) {

    text__ = MyString::Replace(text__, pattern_before__, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief      This function replaces a string using a matching pattern.
/// @param[out] result__ : The replaced string.
/// @param[in]  text__ : A string you wanna replace.
/// @param[in]  pattern_before__ : A pattern for replacement.
///             The pattern could be described by the regular expression (ECMAScript).
/// @param[in]  pattern_after__ : A string after the replacement.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Replace(std::string &result__, const std::string &text__, const std::string &pattern_before__, const std::string &pattern_after__) {

    result__ = MyString::Replace(text__, pattern_before__, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief      This function replaces a string using matching patterns at once.
/// @param[out] result__ : The replaced string.
/// @param[in]  text__ : A string you wanna replace.
/// @param[in]  pattern_before__ : Patterns for replacement.
///             The patterns are described by the regular expression (ECMAScript).
///             These are applied at once as a pattern of "(A|B|C|...)".
/// @param[in]  pattern_after__ : A string after the replacement.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Replace(std::string &result__, const std::string &text__, const std::vector<std::string> &pattern_before__, const std::string &pattern_after__) {

    result__ = MyString::Replace(text__, pattern_before__, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief      This function replaces a string using a matching pattern.
/// @param[in]  text__ : A string you wanna replace.
/// @param[in]  pattern_before__ : A pattern for replacement.
///             The pattern is described by the regular expression (ECMAScript).
/// @param[in]  pattern_after__ : A string after the replacement.
/// @return     The replaced string.
///
// ------------------------------------------------ //


std::string MyString::Replace(const std::string &text__, const std::string &pattern_before__, const std::string &pattern_after__) {

    if (pattern_before__.empty()) { return text__; }

    regex pattern_before__2(pattern_before__);

    return regex_replace(text__, pattern_before__2, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief      This function replaces a string using matching patterns at once.
/// @param[in]  text__ : A string you wanna replace.
/// @param[in]  pattern_before__ : patterns for replacement.
///             The patterns are described by the regular expression (ECMAScript).
///             These are applied at once as a pattern of "(A|B|C|...)".
/// @param[in]  pattern_after__ : A string after the replacement.
/// @return     The replaced string.
///
// ------------------------------------------------ //


std::string MyString::Replace(const std::string &text__, const std::vector<std::string> &pattern_before__, const std::string &pattern_after__) {

    if (pattern_before__.empty()) { return text__; }

    string pattern_before_2 = "(";

    for (const auto &it : pattern_before__) {
        pattern_before_2 += it + "|";
    }

    pattern_before_2.back() = ')';

    //cout << pattern_before_2 << endl;

    regex pattern_before_3(pattern_before_2);

    return regex_replace(text__, pattern_before_3, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief      This function replaces a string using matching patterns sequentially.
/// @param[out] result__ : The replaced string.
/// @param[in]  text__ : A string you wanna replace.
/// @param[in]  pattern_before__ : patterns for replacement.
///             The patterns are described by the regular expression (ECMAScript).
///             These patterns are applied in order from the front.
/// @param[in]  pattern_after__ : A string after the replacement.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Replace_seq(std::string &result__, const std::string &text__, const std::vector<std::string> &pattern_before__, const std::string &pattern_after__) {

    result__ = MyString::Replace_seq(text__, pattern_before__, pattern_after__);

}


// ------------------------------------------------ //
///
/// @brief      This function replaces a string using matching patterns sequentially.
/// @param[in]  text__ : A string you wanna replace.
/// @param[in]  pattern_before__ : patterns for replacement.
///             The patterns are described by the regular expression (ECMAScript).
///             These patterns are applied in order from the front.
/// @param[in]  pattern_after__ : A string after the replacement.
/// @return     The replaced string.
///
// ------------------------------------------------ //


std::string MyString::Replace_seq(const std::string &text__, const std::vector<std::string> &pattern_before__, const std::string &pattern_after__) {

    if (pattern_before__.empty()) { return text__; }

    string text_replaced = text__;

    for (const auto &it : pattern_before__) {
        regex pattern_before_regex(it);
        text_replaced = regex_replace(text_replaced, pattern_before_regex, pattern_after__);
    }

    return text_replaced;

}


// ------------------------------------------------ //
///
/// @brief      This function searches the matching Pattern in a string.
/// @param[in]  text__ : A string you wanna check.
/// @param[in]  pattern__ : A matching pattern.
///             The pattern is described by the regular expression (ECMAScript),
/// @return     Matched or not.
///
// ------------------------------------------------ //


bool MyString::Match(const std::string &text__, const std::string &pattern__) {

    regex pattern__2(pattern__);

    return regex_match(text__, pattern__2);

}


// ------------------------------------------------ //
///
/// @brief      This function searches the number of matched strings with a pattern.
/// @param[in]  text__ : A string you wanna check.
/// @param[in]  pattern__ : A matching pattern.
///             The pattern is described by the regular expression (ECMAScript),
/// @return     The number of matched strings.
///
// ------------------------------------------------ //


int MyString::SearchNum(const std::string &text__, const std::string &pattern__) {

    vector<string> result;

    MyString::Split_match(result, text__, pattern__);

    return static_cast<int>( result.size());

}


// ------------------------------------------------ //
///
/// @brief      This function converts a string to the real number.
///             This is a wrapper of "stod(strtod)".
/// @param[in]  in__ : A string you wanna convert to the number.
/// @param[out] out__ : The convertd double.
/// @return     Success to convert or not.
///
// ------------------------------------------------ //

bool MyString::Str2d(const std::string &in__, double &out__) {

    vector<string>().swap(err_msg_);

    bool   result  = true;

    string out_str = in__;

    int    i       = 0;

    for (const auto &it : str2num_before_) {
        MyString::Replace_directly(out_str, it, str2num_after_[i]);
        i++;
    }

    try {
        out__ = stod(out_str);
        // out__ = stod(in__);
    } catch (const invalid_argument &e) {
        err_msg_.push_back("[WARN][MyString::Str2d] invalid argument. : " + in__ + " : " + e.what() + " -> set 0.0");
        // cerr << "[WARN][MyString::Str2d] invalid argument. : " << in__ << " : " << e.what() << " -> set 0.0" << endl;
        result = false;
        out__  = 0.0;
    } catch (const out_of_range &e) {
        err_msg_.push_back("[WARN][MyString::Str2d] out of range. : " + in__ + " : " + e.what() + " -> set 0.0");
        // cerr << "[WARN][MyString::Str2d] out of range. : " << in__ << " : " << e.what() << " -> set 0.0" << endl;
        result = false;
        out__  = 0.0;
    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function converts a string to the real number.
///             This is a wrapper of "stod(strtod)".
/// @param[in]  in__ : A string you wanna convert to the number.
/// @param[in]  exit_irregular__ : If this is true & the string is irregular,
///             exit the process. default is true.
/// @return     The convertd double.
///
// ------------------------------------------------ //


double MyString::Str2d(const std::string &in__, const bool &exit_irregular__) {

    double out;

    if (!MyString::Str2d(in__, out)) {
        if (exit_irregular__) {
            // err_msg_.push_back("[ERROR][MyString::Str2d] Irregular string : Exit.");
            cerr << "[ERROR][MyString::Str2d] Irregular string : Exit." << endl;
            exit(0);
        }
    }

    return out;

    /*
       string out = in__;

       int i = 0;

       for(const auto &it : str2num_before_) {
        Replace_directly( out, it, str2num_after_[i] );
        i++;
       }

       return strtod( out.c_str(), &strtod_extra_ );
     */
}


// ------------------------------------------------ //
///
/// @brief      Strings to the real numbers (1 dimensional array).
///             This function is a wrapper of "stod(strtod)".
/// @param[in]  in__ : Strings you wanna convert to the number.
/// @param[out] out__ : The convertd real numbers.
/// @return     Success to convert or not.
///
// ------------------------------------------------ //


bool MyString::VecStr2d(const std::vector<std::string> &in__, std::vector<double> &out__) {

    vector<string> err_msg_tmp;

    bool result = true;

    vector<double>().swap(out__);
    out__.resize(in__.size());

    // out__ = vector<double> (in__.size(), 0.0);

    int i = 0;

    for (const auto &it : in__) {

        if (!MyString::Str2d(it, out__[i])) {
            result = false;
            for (const auto &it : err_msg_) { err_msg_tmp.push_back(it); }
        }

        i++;

    }

    err_msg_ = err_msg_tmp;

    return result;

    /*
       vector<double>().swap(out__);

       out__.reserve( in__.size() );

       for(const auto &it : in__) {
        out__.push_back( stod(it) );
        //out__.push_back( Str2d(it) );
       }
     */
}


// ------------------------------------------------ //
///
/// @brief      This function converts strings to the real numbers (2 dimensional array).
///             This function is a wrapper of "stod(strtod)".
/// @param[in]  in__ : Strings you wanna convert to the number (2d array).
/// @param[out] out__ : The convertd real numbers (2d array).
/// @return     Success to convert or not.
///
// ------------------------------------------------ //


bool MyString::VecStr2d(const std::vector<std::vector<std::string> > &in__, std::vector<std::vector<double> > &out__) {

    vector<string> err_msg_tmp;

    bool result = true;

    vector<vector<double> > ().swap(out__);

    out__.resize(in__.size());

    int i = 0;

    for (const auto &it : in__) {

        out__[i].resize(it.size());

        if (!MyString::VecStr2d(it, out__[i])) {
            result = false;
            for (const auto &it : err_msg_) { err_msg_tmp.push_back(it); }
        }

        i++;

    }

    err_msg_ = err_msg_tmp;

    return result;

    /*
       vector<vector<double>> ().swap(out__);

       out__.resize(in__.size());

       uint i1 = 0;

       for(const auto &it1 : in__) {

        out__[i1].reserve( it1.size() );

        for(const auto &it2 : it1) {
            out__[i1].push_back( stod(it2) );
            //out__[i1].push_back( Str2d(it2) );
        }

        i1++;

       }
     */

    /*
       uint N1 = in__.size();

       out__.resize( N1 );

       for ( uint i1 = 0; i1 < N1; i1++ ) {

        uint N2 = in__[i1].size();

        out__[i1].resize( N2 );

        for ( uint i2 = 0; i2 < N2; i2++ ) {

            out__[i1][i2] = Str2d( in__[i1][i2] );

        }

       }
     */
}


// ------------------------------------------------ //
///
/// @brief      This function converts a string to the integer number.
///             Internally, this casts the result of Str2d().
/// @param[in]  in__ : A string you wanna convert to the number.
/// @param[out] out__ : The convertd integer number.
/// @return     Success to convert or not.
///
// ------------------------------------------------ //


bool MyString::Str2i(const std::string &in__, int &out__) {

    double out_d;

    if (!MyString::Str2d(in__, out_d)) {

        for (auto &&it : err_msg_) {
            MyString::Replace_directly(it, "[MyString::Str2d]", "[MyString::Str2i]");
        }

        out__ = 0;
        return false;

    }

    out__ = static_cast<int>(out_d);

    return true;

}


/*
   bool MyString::Str2i(const std::string &in__, int &out__, const int &base__) {

    vector<string>().swap(err_msg_);

    bool result = true;

    try {
        out__ = stoi(in__, nullptr, base__);
    } catch (const invalid_argument &e) {
        err_msg_.push_back("[WARN][MyString::Str2i] invalid argument. : " + in__ + " : " + e.what() + " -> set 0");
        // cout << "[WARN] [MyString::Str2i] invalid argument. : " << in__ << " : " << e.what() << endl;
        result = false;
        out__  = 0;
    } catch (const out_of_range &e) {
        err_msg_.push_back("[WARN][MyString::Str2i] out of range. : " + in__ + " : " + e.what() + " -> set 0");
        // cout << "[WARN] [MyString::Str2i] out of range. : " << in__ << " : " << e.what() << endl;
        result = false;
        out__  = 0;
    }

    return result;

    // out__ = MyString::Str2i( in__ );

   }
 */


// ------------------------------------------------ //
///
/// @brief      This function converts a string to the integer number.
///             Internally, this casts the result of Str2d().
/// @param[in]  in__ : A string you wanna convert to the number.
/// @param[in]  exit_irregular__ : If this is true & the string is irregular,
///             exit the process.
///             If false & the string is irregular, the return value is 0.
///             default is true.
/// @return     The convertd integer number or exit().
///
// ------------------------------------------------ //


int MyString::Str2i(const std::string &in__, const bool &exit_irregular__) {

    double out_d;

    if (!MyString::Str2d(in__, out_d)) {

        if (exit_irregular__) { exit(0); }

        return 0;

    }

    return static_cast<int>(out_d);


    /*
       int out;

       if (!MyString::Str2i(in__, out)) {
        if (exit_irregular__) {
            cerr << "[ERROR][MyString::Str2i] Irregular string : Exit." << endl;
            exit(0);
        }
       }

       return out;
     */

    /*
       double out_tmp;

       MyString::Str2d(in__, out_tmp);

       return static_cast<int>(out_tmp);
     */
}


// ------//

/*
   bool MyString::Str2i_round( const string &in__, int &out__ ){

    double out_tmp;

    if( !MyString::Str2d(in__, out_tmp) ) {
        out__ = 0;
        return false;
    }

    out_tmp += 0.5; // rounding

    out__ = static_cast<int>(out_tmp);

    return true;

    //out__ = MyString::Str2i_round( in__ );

   };

   //------//

   int  MyString::Str2i_round( const string &in__, const bool &exit_irregular__ ){

    int out;

    if( !MyString::Str2i_round(in__, out) ) {
        if(exit_irregular__) {
            cout << "[MyString::Str2i_round] Exit." << endl;
            exit(0);
        }
    }

    return out;

   };
 */


// ------------------------------------------------ //
///
/// @brief      This function converts a string to the boolean.
/// @param[in]  in__ : A string you wanna convert to the boolean.
///             Below strings are false examples,
///             (regardless of the uppper / lower case)
///             @arg  false
///             @arg  f
///             @arg  no
///             @arg  n
///             @arg  0
///             Other patterns will be assumed "true".
/// @return     The convertd boolean.
///
// ------------------------------------------------ //


bool MyString::Str2b(const std::string &in__) {

    string pattern1 = R"([Ff][Aa][Ll][Ss][Ee])";
    string pattern2 = R"([Ff])";
    string pattern3 = R"([Nn][Oo])";
    string pattern4 = R"([Nn])";

    if (in__ == "" || in__ == "0" || Match(in__, pattern1) || Match(in__, pattern2) || Match(in__, pattern3) || Match(in__, pattern4)) {
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function converts a string to the boolean.
/// @param[in]  in__ : A string you wanna convert to the boolean.
///             See explanation of the other Str2b() for pattern examples.
/// @param[out] out__ : The convertd boolean.
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Str2b(const std::string &in__, bool &out__) {

    out__ = MyString::Str2b(in__);

}


// ------------------------------------------------ //
///
/// @brief      This function converts a hex-style string to the char-type number.
/// @param[in]  in__ : A hex format string.
/// @param[out] out__ : The convertd char number.
/// @return     None.
///
// ------------------------------------------------ //


bool MyString::Hex2c(const std::string &in__, unsigned char &out__) {

    double out;

    bool   result = MyString::Str2d(in__, out);

    // out__ = static_cast<uint_fast32_t>(out);
    out__ = static_cast<unsigned char>(out);

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function converts a hex-style string to the char-type number.
/// @param[in]  in__ : A hex format string.
/// @return     The convertd char number.
///
// ------------------------------------------------ //


unsigned char MyString::Hex2c(const std::string &in__) {

    unsigned char out_tmp;

    if (!MyString::Hex2c(in__, out_tmp)) {
        cerr << "[ERROR][MyString::Hex2c] Irregular string : Exit." << endl;
        exit(0);
    }

    return out_tmp;

}


// ------------------------------------------------ //
///
/// @brief      This function converts a char-type to the integer.
/// @brief      Convert a char to the integer.
/// @param[in]  in__ : A character.
/// @return     The convertd integer.
///
// ------------------------------------------------ //


int MyString::Char2int(const char &in__) {

    return static_cast<int>(in__);

}


// ------------------------------------------------ //
///
/// @brief      This function converts a char-type to the unsigned integer.
/// @param[in]  in__ : A character.
/// @param[in]  exit_irregular__ : If this is true and the string is irregular,
///             the process would exit.
///             The default is true.
/// @return     The convertd integer.
///
// ------------------------------------------------ //


int MyString::Char2uint(const char &in__, const bool &exit_irregular__) {

    // char in[4];
    char in[5];
    sprintf(in, "0x%02x", in__ & 0xff);

    return MyString::Str2i(in, exit_irregular__);
    // return MyString::Str2i(in, 16, exit_irregular__);

}


// ------------------------------------------------ //
///
/// @brief      This function converts a char-type to the unsigned integer.
/// @param[in]  in__ : A character.
/// @param[out] out__ : The convertd unsigned integer number.
/// @return     Success convert or not.
///
// ------------------------------------------------ //


bool MyString::Char2uint(const char &in__, int &out__) {

    char in[5];
    sprintf(in, "0x%02x", in__ & 0xff);

    return MyString::Str2i(in, out__);
    // return MyString::Str2i(in, out__, 16);

}


// ------------------------------------------------ //
///
/// @brief      This function prints formatted strings.
///             This could be used like "sprintf" in c standard.
/// @details    Note that output string size is limited < sprintf_n_max_ (default=8192).
/// @param[out] str__ : The formated string.
/// @param[in]  format__ : The regex format style.
/// @param[in]  ... : Valiables to format (va_list).
/// @return     None.
///
// ------------------------------------------------ //


bool MyString::Sprintf(std::string &str__, const char* format__, ...) {

    vector<string>().swap(err_msg_);

    va_list arg;
    char    str_c[sprintf_n_max_];

    va_start(arg, format__);
    vsprintf(str_c, format__, arg);
    va_end(arg);

    str__ = str_c;

    if (static_cast<int>(str__.size()) > sprintf_n_max_) {
        err_msg_.push_back("[ERROR][MyString::Sprintf] Over the prepared array size. : >" + to_string(sprintf_n_max_));
        // cerr << "[ERROR][MyString::Sprintf] FLOWING SIZE (>" << sprintf_n_max_ << ")" << endl;
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function prints formatted strings.
///             This could be used like "sprintf" in c standard.
/// @details    Note that output string size is limited < sprintf_n_max_ (default=8192).
/// @param[in]  format__ : The regex format style.
/// @param[in]  ... : Valiables to format (va_list).
/// @return     The formated string.
///
// ------------------------------------------------ //


std::string MyString::Sprintf_out(const char* format__, ...) {

    vector<string>().swap(err_msg_);

    va_list arg;
    char    str_c[sprintf_n_max_];

    va_start(arg, format__);
    vsprintf(str_c, format__, arg);
    va_end(arg);

    string str_tmp = str_c;

    if (static_cast<int>(str_tmp.size()) > sprintf_n_max_) {
        err_msg_.push_back("[ERROR][MyString::Sprintf_out] Over the prepared array size. : >" + to_string(sprintf_n_max_));
        cerr << err_msg_.back() << endl;
    }

    return str_tmp;

}


// ------------------------------------------------ //
///
/// @brief      This function sets the maximum string size in using "MyString::Sprintf".
/// @param[in]  maxsize__ : The maximum string size of "MyString::Sprintf".
/// @return     None.
///
// ------------------------------------------------ //


void MyString::Sprintf_maxsize(int maxsize__) {

    if (maxsize__ > 0) {
        sprintf_n_max_ = maxsize__;
    }
    else {
        sprintf_n_max_ = 8192;
    }

}


// ------------------------------------------------ //
///
/// @brief          This function converts a string to the upper case one directly.
/// @param[in,out]  in__ : A string to convert.
/// @return         None.
///
// ------------------------------------------------ //


void MyString::ToUpperCase(std::string &in__) {

    transform(in__.begin(), in__.end(), in__.begin(), ::toupper);

}


// ------------------------------------------------ //
///
/// @brief          This function converts a string to the lower case one directly.
/// @param[in,out]  in__ : A string to convert.
/// @return         None.
///
// ------------------------------------------------ //


void MyString::ToLowerCase(std::string &in__) {

    transform(in__.begin(), in__.end(), in__.begin(), ::tolower);

}


// ------------------------------------------------ //
///
/// @brief          This function returns the current error messages.
///                 These error messages are issued by
///                 @li Str2d()
///                 @li VecStr2d()
///                 @li Str2i()
///                 @li Sprintf()
///                 @li Sprintf_out()
/// @return         Current error messages.
///
// ------------------------------------------------ //


std::vector<std::string> MyString::ErrMsg() {

    return err_msg_;

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
