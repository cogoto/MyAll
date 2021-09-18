// ================================================ //
///
/// @file   MyOption.cpp
/// @brief  Implementation of MyOption.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyOption.hpp"

// #include <cstdint>

#include <cerrno>

#include <iostream>

//

namespace MyAll {

//

using namespace std;

// using uint = uint_fast32_t;


// ------------------------------------------------ //
///
/// @brief  First process.  @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MyOption::MyOption() {

    exist_help_string_ = false;
    exist_help_file_   = false;
    use_help_at_blank_ = false;

    help_opt_.push_back("help");

    map_args_.clear();
    args_.clear();
    opts_.clear();
    single_opts_.clear();

    n_args_ = 0;
    n_opts_ = 0;

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyOption::~MyOption() { }


// ------------------------------------------------ //
///
/// @brief     This function sets help options words.
/// @param[in] help_opt__ : help option strings. ex. {"h", "help"}
/// @return    None.
///
// ------------------------------------------------ //


void MyOption::SetHelp_opt(const std::vector<std::string> &help_opt__) {

    help_opt_ = help_opt__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets help string.
///            This string will be called
///            when the help printing is requred.
/// @param[in] help_string__ : help string.
/// @return    None.
///
// ------------------------------------------------ //


void MyOption::SetHelp_string(const std::string help_string__) {

    exist_help_string_ = true;

    help_string_       = help_string__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets file which was written help strings.
///            These strings will be called
///            when the help printing is requred.
/// @param[in] help_file__ : help string file.
/// @return    None.
///
// ------------------------------------------------ //


void MyOption::SetHelp_file(const std::string help_file__) {

    exist_help_file_ = true;

    cat_help_file_   = "cat " + help_file__;

}


// ------------------------------------------------ //
///
/// @brief     This function sets whether to call help strings
///            in the case of options & arguments are blank.
/// @return    None.
///
// ------------------------------------------------ //


void MyOption::UseHelp_blank() {

    use_help_at_blank_ = true;

}


// ------------------------------------------------ //
///
/// @brief     This function sets single-options which have no argument.
///            These strings will be called
///            when the help print is requred.
/// @param[in] single_opts__ : single-option strings.
/// @return    None.
///
// ------------------------------------------------ //


void MyOption::SetSingleOpt(const std::vector<std::string> &single_opts__) {

    vector<string>().swap(single_opts_);

    for (const auto &it : single_opts__) {
        single_opts_.push_back(it);
    }

    for (const auto &it : help_opt_) {
        single_opts_.push_back(it);
    }

}


// ------------------------------------------------ //
///
/// @brief     This function sets options which have arguments.
/// @param[in] arg_opts__ : options to put before the arguments.
/// @return    None.
///
// ------------------------------------------------ //


void MyOption::SetArgOpt(const std::vector<std::string> &arg_opts__) {

    vector<string>().swap(arg_opts_);

    for (const auto &it : arg_opts__) {
        arg_opts_.push_back(it);
    }

}


// ------------------------------------------------ //
//
//  @brief  : This function reads all arguments & options as member variables ("map_args_" & "args_").
//
//  @arg    :
//     argc_ ... a number of arguments.
//     argv_[] ... arguments
//
// ------------------------------------------------ //

void MyOption::ReadOpt(const int &argc__, char* argv__[]) {

    program_name_ = argv__[0];

    int  argv_type;

    bool match_single_opts;
    bool match_arg_opts;

    for (int i = 1; i < argc__; i++) {

        string argv_now, argv_now_0;
        argv_now   = argv__[i];
        argv_now_0 = argv_now;

        string argv_next = "";
        if ((i + 1) < argc__) {
            argv_next = argv__[i + 1];
        }

        argv_type = 0;

        if (argv_now.find("-") == 0) {
            argv_now.replace(0, 1, ""); // remove "-"
            argv_type = 1;              // detected option word by "-"
        }

        if (argv_now.find("-") == 0) {
            argv_now.replace(0, 1, ""); // remove "--"
            argv_type = 2;              // detected option word by "--"
        }

        if (argv_now.size() >= 1) {

            match_single_opts = false;
            match_arg_opts    = false;

            if (argv_type == 1) {

                for (const auto &it : argv_now) {

                    string argv_now_str { it };

                    for (const auto &it : single_opts_) {
                        if (it == argv_now_str) {
                            opts_.push_back(argv_now_str);
                            map_args_[argv_now_str] = "yes";
                            match_single_opts       = true;
                        }
                    }

                    for (const auto &it : arg_opts_) {
                        if (it == argv_now_str) {
                            opts_.push_back(argv_now_str);
                            map_args_[argv_now_str] = argv_next;
                            match_arg_opts = true;
                        }
                    }

                }

            }

            if (argv_type == 2) {

                opts_.push_back(argv_now);

                for (const auto &it : single_opts_) {
                    if (it == argv_now) {
                        map_args_[argv_now] = "yes";
                        match_single_opts   = true;
                    }
                }

                for (const auto &it : arg_opts_) {
                    if (it == argv_now) {
                        map_args_[argv_now] = argv_next;
                        match_arg_opts      = true;
                    }
                }

            }

            if (match_arg_opts) {
                i++;
                continue;
            }

            if (!match_single_opts && !match_arg_opts) {

                args_.push_back(argv_now_0);

            }

        }

    }

    n_opts_ = static_cast<int>(opts_.size());
    n_args_ = static_cast<int>(args_.size());

    //
    // help process
    //
    if (exist_help_string_ || exist_help_file_) {
        for (const auto &it : help_opt_) {
            if (map_args_[it] == "yes") {
                if (exist_help_string_) { cout << help_string_ << endl; }
                if (exist_help_file_) { system(cat_help_file_.c_str()); }
                exit(0);
            }
        }
        if (argc__ == 1 && use_help_at_blank_) {
            if (exist_help_string_) { cout << help_string_ << endl; }
            if (exist_help_file_) { system(cat_help_file_.c_str()); }
            exit(0);
        }
    }

}


/*
   void MyOption::ReadOpt_org(int argc__, char* argv__[]) {

    program_name_ = argv__[0];

    int argv_type;

    // cout << "argc : " << argc_ << " : " << program_name_ << endl;

    for (int i = 1; i < argc__; i++) {

        string argv_now, argv_now_0;
        argv_now   = argv__[i];
        argv_now_0 = argv_now;

        argv_type = 0;

        string argv_next = "";
        if( (i+1) < argc__ ) {
            argv_next  = argv__[i+1];
        }

        //cout << argv_now << " ; " << argv_next << endl;

        if (argv_now.find("-") == 0) { // arguments with options

            //
            // "-" での option 指定
            //
            argv_now.replace(0,1,""); // "-" 消去
            argv_type = 1;

            //
            // "--" での option 指定
            //
            if (argv_now.find("-") == 0) {
                argv_now.replace(0,1,"");
                argv_type = 2;
            }; // 上記と合わせて "--" 消去

            // cout << "option = " << argv_now << endl;

            if(argv_now.size() >= 1) {

                if(argv_type == 1) {

                    for(const auto &it : argv_now) {
                    //for(uint j=0; j < argv_now.size(); j++) {

                        string argv_now_str { it };
                        //string argv_now_str { argv_now[j] };
                        opts_.push_back(argv_now_str);
                        map_args_[argv_now_str] = "yes";

                    }

                    if(argv_next.size() > 0) {
                        //if(argv_next.find("-") != 0 && argv_next.size() > 0) {

                        bool match_single_opts;
                        bool is_match_single_opts = true;


                        for(const auto &it1 : argv_now) {
                        //for(uint j=0; j < argv_now.size(); j++) {
                            string argv_now_str { it1 };
                            //string argv_now_str { argv_now[j] };
                            match_single_opts = false;
                            for(auto it2 : single_opts_) {
                                if(it2 == argv_now_str) {
                                    match_single_opts = true;
                                    continue;
                                }
                            }
                            if(!match_single_opts) {
                                map_args_[argv_now_str] = argv_next;
                                is_match_single_opts = false;
                            }
                        }
                        if(!is_match_single_opts) { // single option でなければ次の引数を飛ばす．
                            i++;
                            continue;
                        }

                    };

                };

                if(argv_type == 2) {
                    opts_.push_back(argv_now);
                    map_args_[argv_now] = "yes";
                    if(argv_next.find("-") != 0 && argv_next.size() > 0) {
                        bool match_single_opts = false;
                        for(auto it : single_opts_) {
                            if(it == argv_now) {
                                match_single_opts = true;
                                continue;
                            }
                        }
                        if(!match_single_opts) {
                            map_args_[argv_now] = argv_next;
                            i++;
                            continue;
                        }
                    }
                };

            }

        } else { // command-line arguments

            args_.push_back(argv_now);

        }

    }

    n_opts_ = opts_.size();
    n_args_ = args_.size();

    //
    // help process
    //
    if(exist_help_string_ || exist_help_file_) {
        for(const auto &it : help_opt_) {
        //for(uint i=0; i<help_opt_.size(); i++) {
            if(map_args_[it] == "yes") {
                if(exist_help_string_) { cout << help_string_ << endl; };
                if(exist_help_file_)  { system(cat_help_file_.c_str()); };
                exit(0);
            }
        }
        if(argc__ == 1 && use_help_at_blank_) {
            if(exist_help_string_) { cout << help_string_ << endl; };
            if(exist_help_file_)  { system(cat_help_file_.c_str()); };
            exit(0);
        };
    }

   };
 */


// ------------------------------------------------ //
///
/// @brief     This function gets arguments with the options as map.
/// @return    arguments mapped with the options.
///
// ------------------------------------------------ //


map<string, string> MyOption::GetOpt() {

    return map_args_;

}


// ------------------------------------------------ //
///
/// @brief     This function gets option's phrases.
/// @return    options.
///
// ------------------------------------------------ //


vector<string> MyOption::GetOptPhrase() {

    return opts_;

}


// ------------------------------------------------ //
///
/// @brief     This function checks there is the option?
/// @param[in] opt__ : option you wanna know the existance.
/// @return    exist or not.
///
// ------------------------------------------------ //


bool MyOption::GetOpt_bool(const std::string &opt__) {

    if (map_args_[opt__].size() > 0) {
        return true;
    }
    else {
        return false;
    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets an argument with the option as integer.
/// @param[in] opt__ : the option.
/// @param[in] arg_int__ : argument integer of the option.
/// @return    the converted integer.
///
// ------------------------------------------------ //


bool MyOption::GetOpt_int(const std::string &opt__, int &arg_int__) {

    char* extra;

    arg_int__ = strtol(map_args_[opt__].c_str(), &extra, 10);

    if (extra == map_args_[opt__]) {
        cerr << "[ERROR][MyOption::GetOpt_int] cannot convert to integer : [" << opt__ << "] " << extra << endl;
        return false;
    }

    if (errno == ERANGE) {
        cerr << "[ERROR][MyOption::GetOpt_int] range error. [" << opt__ << "] " << map_args_[opt__] << endl;
        errno = 0;
        return false;
    }

    return true;

    // return atoi(map_args_[opt__].c_str()); // pattern [1]

}


// ------------------------------------------------ //
///
/// @brief     This function gets an argument with the option as integer.
/// @param[in] opt__ : the option.
/// @return    the converted integer.
///
// ------------------------------------------------ //


int MyOption::GetOpt_int(const std::string &opt__) {

    int result;

    MyOption::GetOpt_int(opt__, result);

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function gets an argument with the option as double float type.
/// @param[in] opt__ : the option.
/// @param[in] arg_double__ : argument double of the option.
/// @return    the converted double.
///
// ------------------------------------------------ //


bool MyOption::GetOpt_double(const std::string &opt__, double &arg_double__) {

    char* extra;

    arg_double__ = strtod(map_args_[opt__].c_str(), &extra);

    if (extra == map_args_[opt__]) {
        cerr << "[ERROR][MyOption::GetOpt_double] cannot convert to double : [" << opt__ << "] " << extra << endl;
        return false;
    }

    if (errno == ERANGE) {
        cerr << "[ERROR][MyOption::GetOpt_double] range error. [" << opt__ << "] " << map_args_[opt__] << endl;
        errno = 0;
        return false;
    }

    return true;

    // return stod(map_args_[opt__]); // pattern [1]

}


// ------------------------------------------------ //
///
/// @brief     This function gets an argument with the option as double float type.
/// @param[in] opt__ : the option.
/// @return    the converted double.
///
// ------------------------------------------------ //


double MyOption::GetOpt_double(const std::string &opt__) {

    double result;

    MyOption::GetOpt_double(opt__, result);

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function gets an argument with the option as string.
/// @param[in] opt__ : the option.
/// @return    the string.
///
// ------------------------------------------------ //


string MyOption::GetOpt_string(const std::string &opt__) {

    return map_args_[opt__];

}


// ------------------------------------------------ //
///
/// @brief     This function gets the number of options.
/// @return    the number of options.
///
// ------------------------------------------------ //


int MyOption::GetOpt_num() {

    return n_opts_;

}


// ------------------------------------------------ //
///
/// @brief     This function gets arguments which is not associated with options.
/// @return    the arguments.
///
// ------------------------------------------------ //


vector<string> MyOption::GetArg() {

    return args_;

}


// ------------------------------------------------ //
///
/// @brief     This function gets arguments without options as integer.
/// @param[in] num__ : argument no.
/// @param[in] arg_int__ : integer converted from argument.
/// @return    the integer.
///
// ------------------------------------------------ //


bool MyOption::GetArg_int(const int &num__, int &arg_int__) {

    if (num__ >= 0) {

        char* extra;

        arg_int__ = strtol(args_[num__].c_str(), &extra, 10);

        if (extra == args_[num__]) {
            cerr << "[ERROR][MyOption::GetArg_int] cannot convert to integer : [" << num__ << "] " << extra << endl;
            return false;
        }

        if (errno == ERANGE) {
            cerr << "[ERROR][MyOption::GetArg_int] range error. [" << num__ << "] " << args_[num__] << endl;
            errno = 0;
            return false;
        }

        return true;

        // return atoi(args_[num__].c_str());

    }
    else {

        cerr << "[ERROR][MyOption::GetArg_int] irregular index : [" << num__ << "]" << endl;
        exit(0);

    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets arguments without options as integer.
/// @param[in] num__ : argument no.
/// @return    the integer.
///
// ------------------------------------------------ //


int MyOption::GetArg_int(const int &num__) {

    int result;

    MyOption::GetArg_int(num__, result);

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function gets arguments without options as double float type.
/// @param[in] num__ : argument no.
/// @param[in] arg_double__ : double converted from argument.
/// @return    the double.
///
// ------------------------------------------------ //


bool MyOption::GetArg_double(const int &num__, double &arg_double__) {

    if (num__ >= 0) {

        char* extra;

        arg_double__ = strtod(args_[num__].c_str(), &extra);

        if (extra == args_[num__]) {
            cerr << "[ERROR][MyOption::GetArg_double] cannot convert to double : [" << num__ << "] " << extra << endl;
            return false;
        }

        if (errno == ERANGE) {
            cerr << "[ERROR][MyOption::GetArg_double] range error. [" << num__ << "] " << args_[num__] << endl;
            errno = 0;
            return false;
        }

        return true;

    }
    else {

        cerr << "[ERROR][MyOption::GetArg_double] irregular index : " << num__ << endl;
        exit(0);

    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets arguments without options as double float type.
/// @param[in] num__ : argument no.
/// @return    the double.
///
// ------------------------------------------------ //


double MyOption::GetArg_double(const int &num__) {

    double result;

    MyOption::GetArg_double(num__, result);

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function gets arguments without options as string.
/// @param[in] num__ : argument no.
/// @return    the string.
///
// ------------------------------------------------ //


string MyOption::GetArg_string(const int &num__) {

    if (num__ >= 0) {
        return args_[num__];
    }
    else {
        cout << "[MyOption::GetArg_string] irregular index : " << num__ << endl;
        exit(0);
    }

}


// ------------------------------------------------ //
///
/// @brief     This function gets the number of arguments
///            which is not associated with options.
/// @return    the number of arguments.
///
// ------------------------------------------------ //


int MyOption::GetArg_num() {

    return n_args_;

}


// ------------------------------------------------ //
///
/// @brief     This function gets executing program name.
/// @return    the name.
///
// ------------------------------------------------ //


string MyOption::Name() {

    return program_name_;

}


// ------------------------------------------------ //

}  // MyAll

// ================================================ //
