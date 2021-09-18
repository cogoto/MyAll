// ================================================ //
///
/// @file   MyString.hpp
/// @brief  Class libraly of MyString.
/// @author cogoto
///
/// @note   Document style is for Doxygen.
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#ifndef _MYALL_MYSTRING_HPP_
#define _MYALL_MYSTRING_HPP_

//

#include <string>

#include <vector>

//

namespace MyAll {

// ================================================ //
///
/// @class    MyString
/// @brief    My class for dealing string.
/// @details  This class operates string as below,
///           @li Splitting string with regular explessions.
///           @li Replacing string with regular explessions.
///           @li String(s) to value(s)
///           etc...
/// @date     2017/06/02 ver. 2.0
///           2019/12/24 ver. 2.1
///
// ================================================ //

class MyString {

 public:

    MyString();  ///< Constractor :

    ~MyString(); ///< Destractor :

    ///
    /// @name  Splitting by delimiters (regular expression).
    /// @{

    void Split(
        std::vector<std::string> &result__,
        const std::string &text__,
        const std::string &delimiter__ = R"(\s+)"
    ); ///< String splitting with a delimiter. :

    //

    void Split(
        std::vector<std::string> &result__,
        const std::string &text__,
        const std::vector<std::string> &delimiter__
    ); ///< String splitting with a delimiter. :

    //

    std::vector<std::string> Split(
        const std::string &text__,
        const std::string &delimiter__ = R"(\s+)"
    ); ///< String splitting with a delimiter. (lower speed) :

    //

    std::vector<std::string> Split(
        const std::string &text__,
        const std::vector<std::string> &delimiter__
    ); ///< String splitting with a delimiter. (lower speed) :

    /// @}
    /// @name  Splitting with matching patterns (regular expression).
    /// @{

    void Split_match(
        std::vector<std::string> &result__,
        const std::string &text__,
        const std::string &match_pattern__ = R"(\S+)"
    ); ///< String splitting with a matched pattern. :

    //

    std::vector<std::string> Split_match(
        const std::string &text__,
        const std::string &match_pattern__ = R"(\S+)"
    ); ///< String splitting with a matched pattern. (lower speed) :

    //

    bool Split_double(
        std::vector<double> &result__,
        const std::string &text__
    ); ///< Converts a string line to the real number array.

    /// @}
    /// @name  Replacements using the regular expressions.
    /// @{

    void Replace_directly(
        std::string &text__,
        const std::string &pattern_before__,
        const std::string &pattern_after__
    ); ///< A direct string replacement with a pattern. :

    //

    void Replace_directly(
        std::string &text__,
        const std::vector<std::string> &pattern_before__,
        const std::string &pattern_after__
    ); ///< Direct string replacements with patterns. :

    //

    void Replace(
        std::string &result__,
        const std::string &text__,
        const std::string &pattern_before__,
        const std::string &pattern_after__
    ); ///< A string replacement with a pattern. :

    //

    void Replace(
        std::string &result__,
        const std::string &text__,
        const std::vector<std::string> &pattern_before__,
        const std::string &pattern_after__
    ); ///< String replacement with patterns. :

    //

    std::string Replace(
        const std::string &text__,
        const std::string &pattern_before__,
        const std::string &pattern_after__
    ); ///< A string replacement with a pattern. (lower speed) :

    //

    std::string Replace(
        const std::string &text__,
        const std::vector<std::string> &pattern_before__,
        const std::string &pattern_after__
    ); ///< String replacements with patterns. (lower speed) :

    //

    void Replace_seq(
        std::string &result__,
        const std::string &text__,
        const std::vector<std::string> &pattern_before__,
        const std::string &pattern_after__
    ); ///< String replacements with patterns sequentially. :

    //

    std::string Replace_seq(
        const std::string &text__,
        const std::vector<std::string> &pattern_before__,
        const std::string &pattern_after__
    ); ///< String replacements with patterns sequentially. :

    /// @}
    /// @name  Searching a matched string using the regular expression.
    /// @{

    bool Match(const std::string &text__, const std::string &pattern__); ///< Searches the matched pattern. :

    /// @}
    /// @name  The number of matched patterns.
    /// @{

    int SearchNum(const std::string &text__, const std::string &pattern__); ///< Searches the number of matched strings :

    /// @}
    /// @name  Strings to real numbers.
    /// @{

    bool Str2d(const std::string &in__, double &out__); ///< A string to the real number. :

    //

    double Str2d(const std::string &in__, const bool &exit_irregular__ = true); ///< A string to the real number. :

    //

    bool VecStr2d(
        const std::vector<std::string> &in__,
        std::vector<double> &out__
    ); ///< Strings to real numbers. :

    //

    bool VecStr2d(
        const std::vector<std::vector<std::string> > &in__,
        std::vector<std::vector<double> > &out__
    ); ///< Strings to real numbers. :

    /// @}
    /// @name  Strings to integers.
    /// @{

    bool Str2i(
        const std::string &in__,
        int &out__
    ); ///< a string to the integer. :

    //

    int Str2i(
        const std::string &in__,
        const bool &exit_irregular__ = true
    ); ///< A String to the integer. :

    // bool Str2i_round( const std::string &in__, int &out__ ); // String to integer with rounding.

    // int  Str2i_round( const std::string &in__, const bool &exit_irregular__=true ); // String to integer with rounding.

    /// @}
    /// @name  A string to the boolean.
    /// @{

    void Str2b(const std::string &in__, bool &out__); ///< A string to the boolean. :

    //

    bool Str2b(const std::string &in__);              ///< A string to the boolean. :

    /// @}
    /// @name  A hex string to the char type.
    /// @{

    bool Hex2c(const std::string &in__, unsigned char &out__); ///< A hex string to the char-type. :

    //

    unsigned char Hex2c(const std::string &in__); ///< A hex string to the unsigned char-type. :

    // void Str2hex( const std::string &in__, unsigned char &out__ ); // String to hex (char).

    // void Str2hex( const std::string &in__, uint_fast32_t &out__ ); // String to hex (Integer).

    // uint_fast32_t Str2hex( const std::string &in__ ); // String to hex (Integer).

    /// @}
    /// @name  A char type to the integer.
    /// @{

    int Char2int(const char &in__); ///< A char-type to the integer. :

    //

    int Char2uint(const char &in__, const bool &exit_irregular__ = true);  ///< A char-type to the (unsigned) integer.

    //

    bool Char2uint(const char &in__, int &out__); ///< A char-type to the (unsigned) integer.

    /// @}
    /// @name  Printing formatted strings.
    /// @{

    bool Sprintf(std::string &str__, const char* format__, ...); ///< "sprintf" with string. :

    //

    std::string Sprintf_out(const char* format__, ...); ///< "sprintf" with string. :

    //

    void Sprintf_maxsize(int maxsize__ = 8192); ///< The maximum string size of "MyString::Sprintf". :

    /// @}
    /// @name  Converting to the upper/lower case.
    /// @{

    void ToUpperCase(std::string &in__); ///< Converts to the upper case. :

    //

    void ToLowerCase(std::string &in__); ///< Converts to the lower case. :

    /// @}
    /// @name  Getting error messages.
    /// @{

    std::vector<std::string> ErrMsg(); ///< Gets current error messages.

    /// @}

 private:

    ///
    /// @name  Splitting (slow)
    /// @{

    void Split_slow(
        std::vector<std::string> &result__,
        const std::string &text__,
        const std::vector<std::string> &delimiter__
    ); ///< String splitting with delimiters. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Parameters
    /// @{

    int sprintf_n_max_;                                     ///< Maximum string size of the output in "MyString::Sprintf".

    std::vector<std::vector<std::string> > str2num_before_; ///< Regex patterns using before converting strings to numbers.

    std::vector<std::string> str2num_after_;                ///< Strings corresponding to str2d_before_.

    std::vector<std::string> err_msg_;                      ///< Current error messages.

    // char* strtod_extra_;

    /// @}

};

}  // MyAll

#endif /// _MYALL_MYSTRING_HPP_
