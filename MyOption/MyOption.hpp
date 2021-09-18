// ================================================ //
///
/// @file   MyOption.hpp
/// @brief  Class libraly of MyOption.
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

#ifndef _MYALL_MYOPTION_HPP_
#define _MYALL_MYOPTION_HPP_

//

#include <map>

#include <string>

#include <vector>

//

namespace MyAll {

// ================================================ //
///
/// @class   MyOption
/// @brief   My class for reading argumaens from the command line.
/// @details You could use option, argument and help functions
///          when your programs are executed in the command line,
/// @date    2017/05/26  ver. 1.1
///
// ================================================ //

class MyOption {

 public:

    MyOption();  ///< Constractor :

    ~MyOption(); ///< Destractor :

    ///
    /// @name  Setting help options and strings.
    /// @{

    void SetHelp_opt(const std::vector<std::string> &help_opt__); ///< Sets options for help. :

    void SetHelp_string(const std::string help_string__);         ///< Sets a help argument. :

    void SetHelp_file(const std::string help_file__);             ///< Sets file which is written help strings. :

    void UseHelp_blank();                                         ///< Whether to use help in the case of options & arguments are blank. :

    /// @}
    /// @name  Setting about single-options.
    /// @{

    void SetSingleOpt(const std::vector<std::string> &single_opts__); ///< Sets single-options which have no arguments. :

    /// @}
    /// @name  Setting options which have arguments.
    /// @{

    void SetArgOpt(const std::vector<std::string> &arg_opts__); ///< Sets options with arguments. :

    /// @}
    /// @name  Reading & analyzing arguments & options from the command line.
    /// @{

    void ReadOpt(const int &argc__, char* argv__[]); ///< Reads & analyzes all arguments & options. :

    // void ReadOpt_org(const int &argc__, const char* argv__[]); ///< Read & analyze all arguments & options. :

    /// @}
    /// @name  Getting arguments which is associated with options.
    /// @{

    std::map<std::string, std::string> GetOpt();       ///< Gets arguments with the options as map. :

    std::vector<std::string>           GetOptPhrase(); ///< Gets option's phrase. :

    //

    bool        GetOpt_bool(const std::string &opt__);                         ///< Checks existence of the option. :

    bool        GetOpt_int(const std::string &opt__, int &arg_int__);          ///< Gets an argument with the option as integer. :

    int         GetOpt_int(const std::string &opt__);                          ///< Gets an argument with the option as integer. :

    bool        GetOpt_double(const std::string &opt__, double &arg_double__); ///< Gets an argument with the option as double. :

    double      GetOpt_double(const std::string &opt__);                       ///< Gets an argument with the option as double. :

    std::string GetOpt_string(const std::string &opt__);                       ///< Gets an argument with the option as string. :

    int         GetOpt_num();                                                  ///< Gets the number of options. :

    /// @}
    /// @name  Arguments which is not concerned with options.
    /// @{

    std::vector<std::string> GetArg(); ///< Get command-line arguments as vector. :

    //

    bool        GetArg_int(const int &num__, int &arg_int__);          ///< Gets a command-line argument as integer. :

    int         GetArg_int(const int &num__);                          ///< Gets a command-line argument as integer. :

    bool        GetArg_double(const int &num__, double &arg_double__); ///< Gets a command-line argument as double. :

    double      GetArg_double(const int &num__);                       ///< Gets a command-line argument as double. :

    std::string GetArg_string(const int &num__);                       ///< Gets a command-line argument as string. :

    int         GetArg_num();                                          ///< Gets a number of arguments. :

    /// @}
    /// @name  Your program Name.
    /// @{

    std::string Name(); ///< Executed program name. :

    /// @}

 private:

    ///
    /// @name  Containers.
    /// @{

    std::string program_name_;                    ///< The program name. :

    std::vector<std::string> args_;               ///< Arguments without the options. :

    std::map<std::string, std::string> map_args_; ///< Arguments with the option. :

    std::vector<std::string> opts_;               ///< Options. :

    std::vector<std::string> single_opts_;        ///< Single options. :

    std::vector<std::string> arg_opts_;           ///< Options which have arguments. :

    std::vector<std::string> help_opt_;           ///< Help options. :

    std::string help_string_;                     ///< Help string. :

    std::string cat_help_file_;                   ///< Help file name. :

    /// @}
    /// @name  The number of options and arguments.
    /// @{

    int n_opts_; ///< The number of options :

    int n_args_; ///< The number of arguments :

    /// @}
    /// @name  Control parameters.
    /// @{

    bool exist_help_string_; ///< The existence of the help string. :

    bool exist_help_file_;   ///< The existence of the help file. :

    bool use_help_at_blank_; ///< Whether to use help when options & arguments are blank. :

    /// @}

};

}  // MyAll

#endif ///_MYALL_MYOPTION_HPP_
