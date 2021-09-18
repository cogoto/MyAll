// ================================================ //
///
/// @file   MyStatus.cpp
/// @brief  Implementation of MyStatus.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyStatus.hpp"

//

#include <iostream>

//

namespace MyAll {

//

using namespace std;

//using uint = unsigned int;


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyStatus::MyStatus(){

    close_shm_at_end_ = true;

};


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyStatus::~MyStatus(){

    if( close_shm_at_end_ ) { MyStatus::CloseShm(); }

};


// ------------------------------------------------ //
///
/// @brief     This function sets a command information.
///            The command statement & return word can use variables
///            which will be replaced in getter functions.
/// @param[in] cmd_name__   : The command name.
/// @param[in] cmd__        : The command statement.
///            The variables are expressed $$[0-9]+, like "$$1"(the first), "$$2" ...
/// @param[in] cmd_return__ : (A part of) the return string.
/// @param[in] wait__       : A wait time after the command execution.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetCmd(
    const string &cmd_name__,
    const string &cmd__,
    const string &cmd_return__,
    const double &wait__
) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    cmd_[cmd_name__]        = cmd__;

    cmd_return_[cmd_name__] = cmd_return__;

    cmd_wait_[cmd_name__]   = wait__;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all registered command statements.
/// @return    Commands.
///
// ------------------------------------------------ //


map<string, string>  MyStatus::GetCmd(){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all registered return strings.
/// @return    Return strings.
///
// ------------------------------------------------ //


map<string, string>  MyStatus::GetCmd_return(){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_return_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all wait times.
/// @return    Wait times.
///
// ------------------------------------------------ //


map<string, double>  MyStatus::GetCmd_wait(){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_wait_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets the command statement.
/// @param[in] cmd_name__ : The command name.
/// @param[in] replace__  : The replacement words.
///            The "$$(n+1)" in the command statement is replaced to the [n] componet of this array.
///            (For example, "$$1" will be replaced to replace__[0].)
/// @return    Command.
///
// ------------------------------------------------ //


string MyStatus::GetCmd( const string &cmd_name__, const vector<string> &replace__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    string result = cmd_[cmd_name__];

    if(replace__.size()>0) {
        for( int i=replace__.size(); i>0; i-- ) {
            string before = R"(\$\$)" + to_string(i);
            str_.Replace_directly( result, before, replace__[i-1] );
        }
    };

    //str_.Replace_directly(result, R"(\\$)", "$");

    return result;

};


// ------------------------------------------------ //
///
/// @brief     This function gets the return string.
/// @param[in] cmd_name__ : The command name.
/// @param[in] replace__  : The replacement words.
///            The "$$(n+1)" in the return string is replaced to the [n] componet of this array.
///            (For example, "$$1" will be replaced to replace__[0].)
/// @return    return string.
///
// ------------------------------------------------ //


string MyStatus::GetCmd_return( const string &cmd_name__, const vector<string> &replace__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    string result = cmd_return_[cmd_name__];

    if(replace__.size()>0) {
        for( int i=replace__.size(); i>0; i-- ) {
            string before = R"(\$\$)" + to_string(i);
            str_.Replace_directly( result, before, replace__[i-1] );
        }
    };

    //str_.Replace_directly(result, R"(\\$)", "$");

    return result;

};


// ------------------------------------------------ //
///
/// @brief     This function gets the wait time.
/// @param[in] cmd_name__ : The command name.
/// @return    wait time.
///
// ------------------------------------------------ //


double MyStatus::GetCmd_wait( const string &cmd_name__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_wait_[cmd_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function sets a binary command information.
/// @param[in] cmd_name__   : The command name.
/// @param[in] cmd__        : The binary command data.
/// @param[in] cmd_return__ : The return binary data.
/// @param[in] wait__       : A wait time after the command execution.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetCmdC(
    const string &cmd_name__,
    const vector<char> &cmd__,
    const vector<char> &cmd_return__,
    const double &wait__
) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    cmd_c_[cmd_name__]        = cmd__;

    cmd_c_return_[cmd_name__] = cmd_return__;

    cmd_c_wait_[cmd_name__]   = wait__;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all registered binary command data.
/// @return    Commands.
///
// ------------------------------------------------ //


map<string, vector<char>>  MyStatus::GetCmdC(){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_c_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all registered binary return data.
/// @return    Return data.
///
// ------------------------------------------------ //


map<string, vector<char>>  MyStatus::GetCmdC_return(){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_c_return_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all wait times.
/// @return    Wait times.
///
// ------------------------------------------------ //


map<string, double>  MyStatus::GetCmdC_wait(){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_c_wait_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets the binary command data.
/// @param[in] cmd_name__ : The command name.
/// @return    commands.
///
// ------------------------------------------------ //


vector<char> MyStatus::GetCmdC( const string &cmd_name__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_c_[cmd_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets the binary return data.
/// @param[in] cmd_name__ : The command name.
/// @return    return data.
///
// ------------------------------------------------ //


vector<char> MyStatus::GetCmdC_return( const string &cmd_name__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_c_return_[cmd_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets the wait time.
/// @param[in] cmd_name__ : The command name.
/// @return    The wait time.
///
// ------------------------------------------------ //


double MyStatus::GetCmdC_wait( const string &cmd_name__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    return cmd_wait_[cmd_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function sets a macro command line one by one. @n
///            If it is need to set all components of your macro command,
///            please use a loop process or write continuously.
/// @param[in] macro_name__ : A macro command name.
/// @param[in] cmd__        : A command statement in a macro command.
/// @param[in] cmd_return__ : (A part of) the return string corresponding to the command statement.
/// @param[in] wait__       : A wait time after the command execution.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetMacro(
    const string &macro_name__,
    const string &macro__,
    const string &cmd_return__,
    const double &wait__
) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    macro_[macro_name__].push_back(macro__);

    macro_return_[macro_name__].push_back(cmd_return__);

    macro_wait_[macro_name__].push_back(wait__);

};


// ------------------------------------------------ //
///
/// @brief     This function gets the macro command.
/// @param[in] macro_name__ : A macro command name.
/// @return    Components of the macro command.
///
// ------------------------------------------------ //


vector<string>  MyStatus::GetMacro( const string &macro_name__ ){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    if( macro_[macro_name__].size() > 0 ) {

        return macro_[macro_name__];

    } else {

        //vector<string> rtn = {};
        return {};

    }

};


// ------------------------------------------------ //
///
/// @brief     This function gets return strings of the macro command.
/// @param[in] macro_name__ : A macro command name.
/// @return    Return strings of the macro command.
///
// ------------------------------------------------ //


vector<string>  MyStatus::GetMacro_return( const string &macro_name__ ){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    if( macro_return_[macro_name__].size() > 0 ) {

        return macro_return_[macro_name__];

    } else {

        //vector<string> rtn = {};
        return {};

    }

};


// ------------------------------------------------ //
///
/// @brief     This function gets wait times of the macro command.
/// @param[in] macro_name__ : A macro command name.
/// @return    Wait times of the macro command.
///
// ------------------------------------------------ //


vector<double>  MyStatus::GetMacro_wait( const string &macro_name__ ){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    if( macro_wait_[macro_name__].size() > 0 ) {

        return macro_wait_[macro_name__];

    } else {

        //vector<double> rtn = {};
        return {};

    }

};


// ------------------------------------------------ //
///
/// @brief     This function defines the message.
///            (Note that these messages are not related to status onees.)
/// @param[in] msg_name__ : A message name.
/// @param[in] msg__ : A message.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::DefMsg( const string &msg_name__, const string &msg__ ) {

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    if( ExistMsg(msg_name__) ) {
        cerr << "[MyStatus::DefMsg] the message \"" << msg_name__ << "\" already exists." << endl;
        exit(1); // すでに定義されているなら終了
    }

    msg_name_.push_back(msg_name__);

    msg_msg_[msg_name__]  = msg__;

};


// ------------------------------------------------ //
///
/// @brief     This function gets the message.
/// @param[in] msg_name__ : A message name.
/// @param[in] replace__  : The replacement words.
///            The "$$(n+1)" string in the message is replaced to the [n] componet of this array.
///            (For example, "$$1" will be replaced to replace__[0].)
/// @return    The message.
///
// ------------------------------------------------ //


string MyStatus::GetMsg( const string &msg_name__, const vector<string> &replace__ ){

    lock_guard<recursive_mutex> lock(mtx_cmd_);

    string result = msg_msg_[msg_name__];

    if(replace__.size()>0) {
        for( int i=replace__.size(); i>0; i-- ) {
            string before = R"(\$\$)" + to_string(i);
            str_.Replace_directly( result, before, replace__[i-1] );
        }
    };

    return result;

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status.
/// @param[in] sts_name__ : A status name.
/// @param[in] sts_bool__ : A default status.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @param[in] sts_msg__  : Status messages.
/// @param[in] kind__     : The kind of the status. ("normal", "warnig" and "error")
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::DefSts(
    const string &sts_name__,
    const bool &sts_bool__,
    const bool &use_shm__,
    std::map<std::string, std::string> &sts_msg__,
    const string &kind__
) {

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( ExistSts(sts_name__) ) {
        cerr << "[MyStatus::DefSts] the status \"" << sts_name__ << "\" already exists." << endl;
        exit(1);
    }

    if(use_shm__) { MyStatus::SetShm( sts_name__ ); }

    sts_name_.push_back(sts_name__);

    sts_msg_[sts_name__]  = sts_msg__;

    //sts_msg_T_[sts_name__]  = sts_msg_T__;

    //sts_msg_F_[sts_name__]  = sts_msg_F__;

    sts_bool_[sts_name__] = sts_bool__;

    if( shm_use_[sts_name__] ) { shm_.Write( sts_name__, sts_bool__ ); };

    sts_kind_[sts_name__] = kind__;

    /*
    if(err_flg__) {
        sts_kind_[sts_name__] = "error";
    } else {
        sts_kind_[sts_name__] = "normal";
    }
    */

};


// ------------------------------------------------ //
///
/// @brief     This function defines a normal status.
/// @param[in] sts_name__ : A status name.
/// @param[in] sts_bool__ : A default status.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @param[in] sts_msg__  : Status messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::DefSts_norm(
    const string &sts_name__,
    const bool &sts_bool__,
    const bool &use_shm__,
    std::map<std::string, std::string> sts_msg__
) {

    MyStatus::DefSts( sts_name__, sts_bool__, use_shm__, sts_msg__, "normal" );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a warning status.
/// @param[in] sts_name__ : A status name.
/// @param[in] sts_bool__ : A default status.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @param[in] sts_msg__  : Status messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::DefSts_warn(
    const string &sts_name__,
    const bool &sts_bool__,
    const bool &use_shm__,
    std::map<std::string, std::string> sts_msg__
) {

    MyStatus::DefSts( sts_name__, sts_bool__, use_shm__, sts_msg__, "warning" );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a error status.
/// @param[in] sts_name__ : A status name.
/// @param[in] sts_bool__ : A default status.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @param[in] sts_msg__  : Status messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::DefSts_err(
    const string &sts_name__,
    const bool &sts_bool__,
    const bool &use_shm__,
    std::map<std::string, std::string> sts_msg__
) {

    MyStatus::DefSts( sts_name__, sts_bool__, use_shm__, sts_msg__, "error" );

};


// ------------------------------------------------ //
///
/// @brief     This function sets the status.
/// @param[in] sts_name__ : The status name.
/// @param[in] sts_bool__ : A status.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetSts( const string &sts_name__, const bool &sts_bool__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistSts(sts_name__) ) {
        cerr << "[MyStatus::SetSts] status \"" << sts_name__ << "\" does not exist." << endl;
        exit(1);
    }; // ステータスが定義されていなければ終了

    sts_bool_[sts_name__] = sts_bool__;

    Update(); // 値がステータスに関連付けられている場合，そちらを優先する．

    if( shm_use_[sts_name__] ) { shm_.Write( sts_name__, sts_bool_[sts_name__] ); };

};


// ------------------------------------------------ //
///
/// @brief     This function sets the normal status.
/// @param[in] sts_name__ : The status name.
/// @param[in] sts_bool__ : The status.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetSts_norm( const string &sts_name__, const bool &sts_bool__ ){

    MyStatus::SetSts( sts_name__, sts_bool__ );

};


// ------------------------------------------------ //
///
/// @brief     This function sets the warning status.
/// @param[in] sts_name__ : The status name.
/// @param[in] sts_bool__ : The status.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetSts_warn( const string &sts_name__, const bool &sts_bool__ ){

    MyStatus::SetSts( sts_name__, sts_bool__ );

};


// ------------------------------------------------ //
///
/// @brief     This function sets the error status.
/// @param[in] sts_name__ : The status name.
/// @param[in] sts_bool__ : The status.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetSts_err( const string &sts_name__, const bool &sts_bool__ ){

    MyStatus::SetSts( sts_name__, sts_bool__ );

};


// ------------------------------------------------ //
///
/// @brief     This function gets status names.
/// @param[in] kind__ : The kind of status.
///            If it is "", all of the kind would be selected.
/// @return    status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetStsName(const std::string &kind__){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if(kind__.empty()) {

        if( sts_name_.size() < 1 ) { return {""}; };

        return sts_name_;

    }
    else {

        vector<string> sts_name;

        for(const auto &it : sts_kind_) {
            if(it.second == kind__) {
                sts_name.push_back(it.first);
            }
        };

        if( sts_name.size() < 1 ) { return {""}; };

        return sts_name;

    }

};


// ------------------------------------------------ //
///
/// @brief     This function gets normal status names.
/// @return    normal status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetStsName_norm(){

    return MyStatus::GetStsName("normal");

}


// ------------------------------------------------ //
///
/// @brief     This function gets warning status names.
/// @return    warning status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetStsName_warn(){

    return MyStatus::GetStsName("warning");

}


// ------------------------------------------------ //
///
/// @brief     This function gets error status names.
/// @return    error status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetStsName_err(){

    return MyStatus::GetStsName("error");

}


// ------------------------------------------------ //
///
/// @brief     This function gets status names that are the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @param[in] kind__     : The kind of status.
///            If it is "", all of the kind would be selected.
/// @return    Status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetSts(const bool &sts_bool__, const std::string &kind__){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<string> sts_match;

    if(kind__.empty()) {

        for(const auto &it : sts_name_) {
            if(sts_bool_[it] == sts_bool__) {
                sts_match.push_back(it);
            }
        }

    }
    else {

        for(const auto &it : sts_name_) {
            if(sts_kind_[it] == kind__) {
                if(sts_bool_[it] == sts_bool__) {
                    sts_match.push_back(it);
                }
            }
        }

    }

    if(sts_match.empty()) { return {""}; };

    return sts_match;

};


// ------------------------------------------------ //
///
/// @brief     This function gets normal status names being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @return    Status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetSts_norm( const bool &sts_bool__ ){

    return MyStatus::GetSts(sts_bool__, "normal");

}


// ------------------------------------------------ //
///
/// @brief     This function gets warning status names being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @return    Status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetSts_warn( const bool &sts_bool__ ){

    return MyStatus::GetSts(sts_bool__, "warning");

}


// ------------------------------------------------ //
///
/// @brief     This function gets error status names being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @return    Status names.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetSts_err( const bool &sts_bool__ ){

    return MyStatus::GetSts(sts_bool__, "error");

}


// ------------------------------------------------ //
///
/// @brief     This function gets a status that is specified by the name.
/// @param[in] sts_name__ : The status name.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::GetSts( const string &sts_name__ ) {

    lock_guard<recursive_mutex> lock(mtx_sts_);

    return sts_bool_[sts_name__];

}


// ------------------------------------------------ //
///
/// @brief     This function gets all statuses.
/// @return    Statuses.
///
// ------------------------------------------------ //


map<string, bool> MyStatus::GetSts(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    return sts_bool_;

};


// ------------------------------------------------ //
///
/// @brief      This function gets all statuses.
/// @param[out] sts_name__ : Status names.
/// @param[out] sts_bool__ : Statuses.
/// @return     None.
///
// ------------------------------------------------ //


void MyStatus::GetSts( vector<string> &sts_name__, map<string, bool> &sts_bool__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<string>().swap(sts_name__);

    sts_bool__.clear();

    sts_name__ = GetStsName();

    sts_bool__ = GetSts();

};


// ------------------------------------------------ //
///
/// @brief     This function gets detailed-statuses that is specified by the name. @n
///            The detailed-statuses are associated with each status-value that is connected.
/// @param[in] sts_name__ : The status name.
/// @return    The detailed-status array.
///
// ------------------------------------------------ //


vector<bool> MyStatus::GetSts_detail( const string &sts_name__ ) {

    lock_guard<recursive_mutex> lock(mtx_sts_);

    return sts_val_bool_[sts_name__];

}


// ------------------------------------------------ //
///
/// @brief     This function gets all detailed-statuses. @n
///            The detailed-statuses are associated with each status-value that is connected.
/// @return    All detailed-status arrays.
///
// ------------------------------------------------ //


map<string, vector<bool>> MyStatus::GetSts_detail(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    return sts_val_bool_;

};


// ------------------------------------------------ //
///
/// @brief     This function gets all status names and detailed-statuses.
/// @param[in] sts_name__ : The status names.
/// @param[in] sts_val_bool__ : The detailed-statuses that are associated with each status-value.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::GetSts_detail( vector<string> &sts_name__, map<string, vector<bool>> &sts_val_bool__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<string>().swap(sts_name__);

    sts_val_bool__.clear();

    sts_name__ = GetStsName();

    sts_val_bool__ = GetSts_detail();

};


// ------------------------------------------------ //
///
/// @brief     This function gets all statuses as strings.
/// @return    Status strings.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetStsS(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( sts_name_.size() < 1 ) { return {""}; };

    vector<string> result;

    for(const auto &it : sts_name_) {
        if( sts_bool_[it] ) {
            result.push_back( it + " [STS] = T" );
        } else {
            result.push_back( it + " [STS] = F" );
        }
    }

    return result;

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existence of normal statuses being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @return    Exists or not.
///
// ------------------------------------------------ //


bool MyStatus::CheckSts_norm(const bool &sts_bool__){

    return MyStatus::CheckSts(sts_bool__, "normal");

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existence of warning statuses being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @return    Exists or not.
///
// ------------------------------------------------ //


bool MyStatus::CheckSts_warn(const bool &sts_bool__){

    return MyStatus::CheckSts(sts_bool__, "warning");

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existence of error statuses being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @return    Exists or not.
///
// ------------------------------------------------ //


bool MyStatus::CheckSts_err(const bool &sts_bool__){

    return MyStatus::CheckSts(sts_bool__, "error");

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existence of statuses being the specified state.
/// @param[in] sts_bool__ : The status to specify.
/// @param[in] kind__     : The kind of status.
///            If it is "", all of the kind would be selected.
/// @return    Exists or not.
///
// ------------------------------------------------ //


bool MyStatus::CheckSts(const bool &sts_bool__, const std::string &kind__){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    bool exist_sts = false;

    if(kind__.empty()) {

        for(const auto &it : sts_name_) {
            if(sts_bool_[it] == sts_bool__) {
                exist_sts = true;
                break;
            }
        }

    }
    else {

        for(const auto &it : sts_name_) {
            if(sts_kind_[it] == kind__) {
                if(sts_bool_[it] == sts_bool__) {
                    exist_sts = true;
                    break;
                }
            }
        }

    }

    return exist_sts;

};


// ------------------------------------------------ //
///
/// @brief     This is the overload function for distinguishing boolean & string
///            (Both are char pointer, Internally).
///            GetSts() has the same function, I recommend using it.
/// @param[in] sts_name__ : The status name.
/// @return    Exit.
///
// ------------------------------------------------ //



bool MyStatus::CheckSts( const string &sts_name__ ){

    cerr << "[MyStatus::CheckSts] Please use GetSts()!" << endl;
    exit(0);

    //lock_guard<recursive_mutex> lock(mtx_sts_);
    //return sts_bool_[sts_name__];

};


// ------------------------------------------------ //
///
/// @brief     This is the overload function for distinguishing boolean & char*
///            (Both are char pointer, Internally).
///            GetSts() has the same implementation, I recommend using it.
/// @param[in] sts_name__ : The status name.
/// @return    Exit.
///
// ------------------------------------------------ //



bool MyStatus::CheckSts( const char* sts_name__ ){

    cerr << "[MyStatus::CheckSts] Please use GetSts()!" << endl;
    exit(0);

    //lock_guard<recursive_mutex> lock(mtx_sts_);
    //return sts_bool_[sts_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets the registered status message with the keyword.
/// @param[in] sts_name__ : The status name.
/// @param[in] msg_key__  : The message keyword.
/// @param[in] replace__  : The replacement words.
///            The "$$(n+1)" in the message is replaced to the [n] componet of this array.
///            (For example, "$$1" will be replaced to replace__[0].)
/// @return    The status message.
///
// ------------------------------------------------ //


string MyStatus::GetStsMsg(
    const string &sts_name__,
    const string &msg_key__,
    const vector<string> &replace__
){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    string result = "";

    if(msg_key__.empty()) {
        for(const auto &it : sts_msg_[sts_name__]) {
            result += "[" + it.first + "] " + it.second + "\n";
        }
    }
    else {
        result = sts_msg_[sts_name__][msg_key__];
    }

    if(replace__.size() > 0) {
        for( int i=replace__.size(); i>0; i-- ) {
            string before = R"(\$\$)" + to_string(i);
            str_.Replace_directly( result, before, replace__[i-1] );
        }
    };

    //str_.Replace_directly(result, R"(\\$)", "$");

    return result;

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value (a boolean value).
///            This value will be the first component of a status-value array.
///            (Internally, each status-value is implemented as an array.)
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_bool  ( const string &val_name__, const bool &val__, const bool &use_shm__ ){

    vector<bool> val = {val__};

    return MyStatus::DefVal_bool( val_name__, val, use_shm__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value (a char-type value).
///            This value will be the first component of a status-value array.
///            (Internally, each status-value is implemented as an array.)
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_char  ( const string &val_name__, const char &val__, const bool &use_shm__ ){

    vector<char> val = {val__};

    return MyStatus::DefVal_char( val_name__, val, use_shm__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value (a integer).
///            This value will be the first component of a status-value array.
///            (Internally, each status-value is implemented as an array.)
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_int   ( const string &val_name__, const int &val__, const bool &use_shm__ ){

    vector<int> val = {val__};

    return MyStatus::DefVal_int( val_name__, val, use_shm__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value (a real number).
///            This value will be the first component of a status-value array.
///            (Internally, each status-value is implemented as an array.)
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_double( const string &val_name__, const double &val__, const bool &use_shm__ ){

    vector<double> val = {val__};

    return MyStatus::DefVal_double( val_name__, val, use_shm__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value (a string).
///            This value will be the first component of a status-value array.
///            (Internally, each status-value is implemented as an array.)
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value.
/// @param[in] use_shm__  : Uses shared memory or not.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_string( const string &val_name__, const string &val__, const bool &use_shm__, const int &str_byte_size__ ){

    vector<string> val = {val__};

    return MyStatus::DefVal_string( val_name__, val, use_shm__, str_byte_size__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value array (boolean values).
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value array.
/// @param[in] use_shm__  : Uses shared memory or not.
///            Note that an array size of the status-values cannot change
///            in the case of using shared memory.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_bool( const string &val_name__, const vector<bool> &val__, const bool &use_shm__ ){

    if( !ExistVal(val_name__) ) {
        lock_guard<recursive_mutex> lock(mtx_sts_);
        val_name_.push_back(val_name__);
    } else {
        cerr << "[MyStatus::SetVal_bool] the status value \"" << val_name__ << "\" already exists." << endl;
        return false;
        //exit(1);
    }

    if( use_shm__ ) { MyStatus::SetShm( val_name__, val__.size(), 0 ); }

    return MyStatus::SetVal_bool( val_name__, val__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value array (char-type values).
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-value.
/// @param[in] use_shm__  : Uses shared memory or not.
///            Note that an array size of the status-values cannot change
///            in the case of using shared memory.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_char( const string &val_name__, const vector<char> &val__, const bool &use_shm__ ){

    if( !ExistVal(val_name__) ) {
        lock_guard<recursive_mutex> lock(mtx_sts_);
        val_name_.push_back(val_name__);
    } else {
        cerr << "[MyStatus::DefVal_char] the status value \"" << val_name__ << "\" already exists." << endl;
        return false;
        //exit(1);
    }

    if( use_shm__ ) { MyStatus::SetShm( val_name__, val__.size(), 0 ); }

    return MyStatus::SetVal_char( val_name__, val__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value array (integers).
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values.
/// @param[in] use_shm__  : Uses shared memory or not.
///            Note that an array size of the status-values cannot change
///            in the case of using shared memory.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_int( const string &val_name__, const vector<int> &val__, const bool &use_shm__ ){

    if( !ExistVal(val_name__) ) {
        lock_guard<recursive_mutex> lock(mtx_sts_);
        val_name_.push_back(val_name__);
    } else {
        cerr << "[MyStatus::DefVal_int] the status value \"" << val_name__ << "\" already exists." << endl;
        return false;
        //exit(1);
    }

    if( use_shm__ ) { MyStatus::SetShm( val_name__, val__.size(), 0 ); }

    return MyStatus::SetVal_int( val_name__, val__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value array (real numbers).
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values.
/// @param[in] use_shm__  : Uses shared memory or not.
///            Note that an array size of the status-values cannot change
///            in the case of using shared memory.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_double( const string &val_name__, const vector<double> &val__, const bool &use_shm__ ){

    if( !ExistVal(val_name__) ) {
        lock_guard<recursive_mutex> lock(mtx_sts_);
        val_name_.push_back(val_name__);
    } else {
        cerr << "[MyStatus::DefVal_double] the status value \"" << val_name__ << "\" already exists." << endl;
        return false;
        //exit(1);
    }

    if( use_shm__ ) { MyStatus::SetShm( val_name__, val__.size(), 0 ); }

    return MyStatus::SetVal_double( val_name__, val__ );

};


// ------------------------------------------------ //
///
/// @brief     This function defines a status-value array (strings).
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values.
/// @param[in] use_shm__  : Uses shared memory or not.
///            Note that an array size of the status-values cannot change
///            in the case of using shared memory.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::DefVal_string( const string &val_name__, const vector<string> &val__, const bool &use_shm__, const int &str_byte_size__ ){

    if( !ExistVal(val_name__) ) {
        lock_guard<recursive_mutex> lock(mtx_sts_);
        val_name_.push_back(val_name__);
    } else {
        cerr << "[MyStatus::DefVal_string] the status value \"" << val_name__ << "\" already exists." << endl;
        return false;
        //exit(1);
    }

    int str_byte_size = str_byte_size__;

    if(str_byte_size <= 0) {
        for(const auto &it : val__) {
            int size = static_cast<int>(it.size());
            if(size > str_byte_size) {
                str_byte_size = size;
            }
        }
    }

    if( use_shm__ ) { MyStatus::SetShm( val_name__, val__.size(), str_byte_size ); }

    return MyStatus::SetVal_string( val_name__, val__ );

};


// ------------------------------------------------ //
///
/// @brief     This function sets a component of the status-value with a subscript.
///            If the status-value uses the shared memory,
///            the subscript number must not exceed the size of the defined array.
///            In the case of such a exceeding, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__ : The status-value (boolean).
/// @param[in] no__  : The subscript no.
///            If the status-value is one component(a value), this number always needs to set zero(default).
///            Note that the number do not exceed the defined array size in the case of using a shared memory.
///            If there are no components before this specified one, the blank would be assorted by val__.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_bool( const string &val_name__, const bool &val__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<bool> val = val_bool_[val_name__];

    int val_size = static_cast<int>( val.size() );

    if( val_size <= no__ ) {

        for(int i=val_size; i<=no__; i++) { val.push_back( val__ ); }

    } else if( no__ < 0 ) {

        cerr << "[MyStatus::SetVal_bool] Cannot set the value : " << val_name__ << endl;
        return false;

    } else {

        val[no__] = val__;

    }

    return MyStatus::SetVal_bool( val_name__, val );

};


// ------------------------------------------------ //
///
/// @brief     This function sets a component of the status-value with a subscript.
///            If the status-value uses the shared memory,
///            the subscript number must not exceed the size of the defined array.
///            In the case of such a exceeding, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__ : The status-value (char-type).
/// @param[in] no__  : The subscript no.
///            If the status-value is one component(a value), this number always needs to set zero(default).
///            Note that the number do not exceed the defined array size in the case of using a shared memory.
///            If there are no components before this specified one, the blank would be assorted by val__.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_char( const string &val_name__, const char &val__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<char> val = val_char_[val_name__];

    int val_size = static_cast<int>( val.size() );

    if( val_size <= no__ ) {

        for(int i=val_size; i<=no__; i++) { val_char_[val_name__].push_back( val__ ); }

    } else if( no__ < 0 ) {

        cerr << "[MyStatus::SetVal_char] Cannot set the value : " << val_name__ << endl;
        return false;

    } else {

        val_char_[val_name__][no__] = val__;

    }

    return MyStatus::SetVal_char( val_name__, val_char_[val_name__] );

};


// ------------------------------------------------ //
///
/// @brief     This function sets a component of the status-value with a subscript.
///            If the status-value uses the shared memory,
///            the subscript number must not exceed the size of the defined array.
///            In the case of such a exceeding, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__ : The status-value (integer).
/// @param[in] no__  : The subscript no.
///            If the status-value is one component(a value), this number always needs to set zero(default).
///            Note that the number do not exceed the defined array size in the case of using a shared memory.
///            If there are no components before this specified one, the blank would be assorted by val__.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_int( const string &val_name__, const int &val__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<int> val = val_int_[val_name__];

    int val_size = static_cast<int>( val.size() );

    if( val_size <= no__ ) {

        for(int i=val_size; i<=no__; i++) { val.push_back( val__ ); }

    } else if( no__ < 0 ) {

        cerr << "[MyStatus::SetVal_int] Cannot set the value : " << val_name__ << endl;
        return false;

    } else {

        val[no__] = val__;

    }

    return MyStatus::SetVal_int( val_name__, val );

};


// ------------------------------------------------ //
///
/// @brief     This function sets a component of the status-value with a subscript.
///            If the status-value uses the shared memory,
///            the subscript number must not exceed the size of the defined array.
///            In the case of such a exceeding, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__ : The status-value (real number).
/// @param[in] no__  : The subscript no.
///            If the status-value is one component(a value), this number always needs to set zero(default).
///            Note that the number do not exceed the defined array size in the case of using a shared memory.
///            If there are no components before this specified one, the blank would be assorted by val__.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_double( const string &val_name__, const double &val__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<double> val = val_double_[val_name__];

    int val_size = static_cast<int>( val.size() );

    if( val_size <= no__ ) {

        for(int i=val_size; i<=no__; i++) { val.push_back( val__ ); }

    } else if( no__ < 0 ) {

        cerr << "[MyStatus::SetVal_double] Cannot set the value : " << val_name__ << endl;
        return false;

    } else {

        val[no__] = val__;

    }

    return MyStatus::SetVal_double( val_name__, val );

};


// ------------------------------------------------ //
///
/// @brief     This function sets a component of the status-value with a subscript.
///            If the status-value uses the shared memory,
///            the subscript number must not exceed the size of the defined array.
///            In the case of such a exceeding, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__ : The status-value (string).
/// @param[in] no__  : The subscript no.
///            If the status-value is one component(a value), this number always needs to set zero(default).
///            Note that the number do not exceed the defined array size in the case of using a shared memory.
///            If there are no components before this specified one, the blank would be assorted by val__.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_string( const string &val_name__, const string &val__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<string> val = val_string_[val_name__];

    int val_size = static_cast<int>( val.size() );

    if( val_size <= no__ ) {

        for(int i=val_size; i<=no__; i++) { val.push_back( val__ ); }

    } else if( no__ < 0 ) {

        cerr << "[MyStatus::SetVal_string] Cannot set the value : " << val_name__ << endl;
        return false;

    } else {

        val[no__] = val__;

    }

    return MyStatus::SetVal_string( val_name__, val );

};


// ------------------------------------------------ //
///
/// @brief     This function sets a status-value array.
///            If the status-value uses the shared memory,
///            The array size must be the same as the one of the defined array.
///            In the case of such a no-matching size, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values (boolean).
/// @return    success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_bool( const string &val_name__, const vector<bool> &val__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistVal(val_name__) ) {
        cerr << "[MyStatus::SetVal_bool] the status value \"" << val_name__ << "\" does not exist." << endl;
        return false;
        //exit(1);
    }

    vector<bool>().swap( val_bool_[val_name__] );

    val_bool_[val_name__] = val__;

    val_kind_[val_name__] = "bool";

    if( !val_sts_.empty() ) { Update( val_name__ ); } // 値チェックのフラグがあるならステータス更新

    if( shm_use_[val_name__] ) {

        if( shm_num_[val_name__] == static_cast<int>(val_bool_[val_name__].size()) ) {
            shm_.Write( val_name__, val_bool_[val_name__] );
        } else {
            cerr << "[MyStatus::SetVal_bool] No match the number of shared memories :" << val_name__ << endl;
            return false;
        }

    }

    return true;

};


// ------------------------------------------------ //
///
/// @brief     This function sets a status-value array.
///            If the status-value uses the shared memory,
///            The array size must be the same as the one of the defined array.
///            In the case of such a no-matching size, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values (char-type).
/// @return    success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_char( const string &val_name__, const vector<char> &val__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistVal(val_name__) ) {
        cerr << "[MyStatus::SetVal_char] the status value \"" << val_name__ << "\" does not exist." << endl;
        return false;
        //exit(1);
    }

    vector<char>().swap( val_char_[val_name__] );

    val_char_[val_name__] = val__;

    val_kind_[val_name__] = "char";

    if( !val_sts_.empty() ) { Update( val_name__ ); } // 値チェックのフラグがあるならステータス更新

    if( shm_use_[val_name__] ) {

        if( shm_num_[val_name__] == static_cast<int>(val_char_[val_name__].size()) ) {
            shm_.Write( val_name__, val_char_[val_name__] );
        } else {
            cerr << "[MyStatus::SetVal_char] No match the number of shared memories :" << val_name__ << endl;
            return false;
        }

    }

    return true;

};


// ------------------------------------------------ //
///
/// @brief     This function sets a status-value array.
///            If the status-value uses the shared memory,
///            The array size must be the same as the one of the defined array.
///            In the case of such a no-matching size, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values (integer).
/// @return    success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_int( const string &val_name__, const vector<int> &val__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistVal(val_name__) ) {
        cerr << "[MyStatus::SetVal_int] the status value \"" << val_name__ << "\" does not exist." << endl;
        return false;
        //exit(1);
    }

    vector<int>().swap( val_int_[val_name__] );

    val_int_[val_name__] = val__;

    val_kind_[val_name__] = "int";

    if( !val_sts_.empty() ) { Update( val_name__ ); } // 値チェックのフラグがあるならステータス更新

    if( shm_use_[val_name__] ) {

        if( shm_num_[val_name__] == static_cast<int>(val_int_[val_name__].size()) ) {
            shm_.Write( val_name__, val_int_[val_name__] );
        } else {
            cerr << "[MyStatus::SetVal_int] No match the number of shared memories :" << val_name__ << endl;
            return false;
        }

    }

    return true;

};


// ------------------------------------------------ //
///
/// @brief     This function sets a status-value array.
///            If the status-value uses the shared memory,
///            The array size must be the same as the one of the defined array.
///            In the case of such a no-matching size, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values (real number).
/// @return    success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_double( const string &val_name__, const vector<double> &val__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistVal(val_name__) ) {
        cerr << "[MyStatus::SetVal_double] the status value \"" << val_name__ << "\" does not exist." << endl;
        return false;
        //exit(1);
    }

    vector<double>().swap( val_double_[val_name__] );

    val_double_[val_name__] = val__;

    val_kind_[val_name__]   = "double";

    if( !val_sts_.empty() ) { Update( val_name__ ); } // 値チェックのフラグがあるならステータス更新

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_double_[val_name__].size()) ) {
            shm_.Write( val_name__, val_double_[val_name__] );
        } else {
            cerr << "[MyStatus::SetVal_double] No match the number of shared memories :" << val_name__ << endl;
            return false;
        }
    }

    return true;

};


// ------------------------------------------------ //
///
/// @brief     This function sets a status-value array.
///            If the status-value uses the shared memory,
///            The array size must be the same as the one of the defined array.
///            In the case of such a no-matching size, the status-value information would not be set to the shared memories.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val__      : The status-values (string).
/// @return    success or not.
///
// ------------------------------------------------ //


bool MyStatus::SetVal_string( const string &val_name__, const vector<string> &val__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistVal(val_name__) ) {
        cerr << "[MyStatus::SetVal_string] the status value \"" << val_name__ << "\" does not exist." << endl;
        return false;
        //exit(1);
    }

    vector<string>().swap( val_string_[val_name__] );

    val_string_[val_name__] = val__;

    val_kind_[val_name__]   = "string";

    if( !val_sts_.empty() ) { Update( val_name__ ); } // 値チェックのフラグがあるならステータス更新

    if( shm_use_[val_name__] ) {

        if( shm_num_[val_name__] == static_cast<int>(val_string_[val_name__].size()) ) {
            shm_.Write( val_name__, val_string_[val_name__], shm_string_size_[val_name__] );
        } else {
            cerr << "[MyStatus::SetVal_string] No match the number of shared memories :" << val_name__ << endl;
            return false;
        }

    }

    return true;

};


// ------------------------------------------------ //
///
/// @brief     This function gets a component of the status-value with a subscript.
/// @param[in] val_name__ : The status-value name.
/// @param[in] no__  : The subscript no.
///            The default number is zero.
/// @return    The status-value (boolean).
///
// ------------------------------------------------ //


bool MyStatus::GetVal_bool( const string &val_name__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_bool_[val_name__].size()) ) {
            shm_.Read( val_name__, val_bool_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_bool] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_bool_[val_name__][no__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a component of the status-value with a subscript.
/// @param[in] val_name__ : The status-value name.
/// @param[in] no__  : The subscript no.
///            The default number is zero.
/// @return    The status-value (char-type).
///
// ------------------------------------------------ //


char MyStatus::GetVal_char( const string &val_name__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_char_[val_name__].size()) ) {
            shm_.Read( val_name__, val_char_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_char] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_char_[val_name__][no__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a component of the status-value with a subscript.
/// @param[in] val_name__ : The status-value name.
/// @param[in] no__  : The subscript no.
///            The default number is zero.
/// @return    The status-value (integer).
///
// ------------------------------------------------ //


int MyStatus::GetVal_int( const string &val_name__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_int_[val_name__].size()) ) {
            shm_.Read( val_name__, val_int_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_int] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_int_[val_name__][no__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a component of the status-value with a subscript.
/// @param[in] val_name__ : The status-value name.
/// @param[in] no__  : The subscript no.
///            The default number is zero.
/// @return    The status-value (real number).
///
// ------------------------------------------------ //


double MyStatus::GetVal_double( const string &val_name__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_double_[val_name__].size()) ) {
            shm_.Read( val_name__, val_double_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_double] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_double_[val_name__][no__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a component of the status-value with a subscript.
/// @param[in] val_name__ : The status-value name.
/// @param[in] no__  : The subscript no.
///            The default number is zero.
/// @return    The status-value (string).
///
// ------------------------------------------------ //


string MyStatus::GetVal_string( const string &val_name__, const int &no__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_string_[val_name__].size()) ) {
            shm_.Read( val_name__, val_string_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_string] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_string_[val_name__][no__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a status-value array.
/// @param[in] val_name__ : The status-value name.
/// @return    The status-values (boolean).
///
// ------------------------------------------------ //


vector<bool> MyStatus::GetVal_bools( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_bool_[val_name__].size()) ) {
            shm_.Read( val_name__, val_bool_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_bool] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_bool_[val_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a status-value array.
/// @param[in] val_name__ : The status-value name.
/// @return    The status-values (char-type).
///
// ------------------------------------------------ //


vector<char> MyStatus::GetVal_chars( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_char_[val_name__].size()) ) {
            shm_.Read( val_name__, val_char_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_char] No match the number of shared memories :" << val_name__ << endl;
        }
    }

    return val_char_[val_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a status-value array.
/// @param[in] val_name__ : The status-value name.
/// @return    The status-values (integer).
///
// ------------------------------------------------ //


vector<int> MyStatus::GetVal_ints( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_int_[val_name__].size()) ) {
            shm_.Read( val_name__, val_int_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_int] No match the number of shared memories :" << val_name__ << endl;
        }
    };

    return val_int_[val_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a status-value array.
/// @param[in] val_name__ : The status-value name.
/// @return    The status-values (real number).
///
// ------------------------------------------------ //


vector<double> MyStatus::GetVal_doubles( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_double_[val_name__].size()) ) {
            shm_.Read( val_name__, val_double_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_double] No match the number of shared memories :" << val_name__ << endl;
        }
    };

    return val_double_[val_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets a status-value array.
/// @param[in] val_name__ : The status-value name.
/// @return    The status-values (string).
///
// ------------------------------------------------ //


vector<string> MyStatus::GetVal_strings( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( shm_use_[val_name__] ) {
        if( shm_num_[val_name__] == static_cast<int>(val_string_[val_name__].size()) ) {
            shm_.Read( val_name__, val_string_[val_name__] );
        } else {
            cerr << "[MyStatus::GetVal_string] No match the number of shared memories :" << val_name__ << endl;
        }
    };

    return val_string_[val_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function gets all status-values as strings.
/// @return    all status-values.
///
// ------------------------------------------------ //


vector<string> MyStatus::GetValS(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    vector<string> result;

    if( val_name_.empty() ) {
        result.push_back( "" );
        return result;
    };

    string result_tmp;

    for(const auto &it : val_name_) {

        str_.Sprintf( result_tmp, "%s [%s] =", it.c_str(), val_kind_[it].c_str());

        if( str_.Match(val_kind_[it], R"(.*bool)") ) {
            for(const auto &it2 : val_bool_[it]) {
                result_tmp += " ";
                string bool_str = "N";
                if(  it2 ) { bool_str = "T"; }
                if( !it2 ) { bool_str = "F"; }
                result_tmp += bool_str;
            }
        }

        if( str_.Match(val_kind_[it], R"(.*char)") ) {
            for(const auto &it2 : val_char_[it]) {
                string char_str;
                str_.Sprintf( char_str, "%c", it2 );
                result_tmp += " " + char_str;
            }
        }

        if( str_.Match(val_kind_[it], R"(.*int)") ) {
            for(const auto &it2 : val_int_[it]) {
                result_tmp += " " + to_string(it2);
            }
        }

        if( str_.Match(val_kind_[it], R"(.*double)") ) {
            for(const auto &it2 : val_double_[it]) {
                result_tmp += " " + to_string(it2);
            }
        }

        if( str_.Match(val_kind_[it], R"(.*string)") ) {
            for(const auto &it2 : val_string_[it]) {
                result_tmp += " " + it2;
            }
        }

        result.push_back( result_tmp );

    }

    return result;

};


// ------------------------------------------------ //
///
/// @brief     This function gets the kind of the status-value.
/// @param[in] val_name__ : The status-value name.
/// @return    The kind of the status-values.
///
// ------------------------------------------------ //


string MyStatus::GetVal_kind( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    return val_kind_[val_name__];

};


// ------------------------------------------------ //
///
/// @brief     This function associates the status with the status-value.
///            If the result evaluated by matching-patterns is true,
///            the associated status becomes true.
///            The result from two matching-patterns is logical AND, basically.
///            However, the below patterns are evaluated by logical OR.
///            @li "<  MIN", ">  MAX"  : A <  MIN or A >  MAX
///            @li "<= MIN", ">  MAX"  : A <= MIN or A >  MAX
///            @li "<  MIN", ">= MAX"  : A <  MIN or A >= MAX
///            @li "<= MIN", ">= MAX"  : A <= MIN or A >= MAX
///            Here, the 1st & 2nd pattern is correcpond to val_match_1__ & val_match_2__,
///            A is the status-value and MIN <= MAX.
/// @param[in] sts_name__ : The status name.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val_match_1__ : The matching-pattern to evaluate the status value. @n
///            The pattern examples are as below,
///            @li "== A", "!= A"
///            @li ">  A", ">= A", "<  A", "<= A"
///            @li "include A (inc A)", "exclude A (exc A)".
///            Here, A is a status-value.
/// @param[in] val_match_2__ : The matching-pattern to evaluate the status value.
///            Please see the above.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::Connect(
    const string &sts_name__,
    const string &val_name__,
    const string &val_match_1__,
    const string &val_match_2__
){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !ExistSts( sts_name__ ) ) {
        cerr << "[MyStatus::Connect] the status \"" << sts_name__ << "\" does not exist." << endl;
        exit(1);
    };

    if( !ExistVal( val_name__ ) ) {
        cerr << "[MyStatus::Connect] the status value \"" << val_name__ << "\" does not exist." << endl;
        exit(1);
    };

    val_sts_.insert( make_pair(val_name__, sts_name__) );

    val_match_.insert( make_pair(val_name__, sts_name__ + "\n" + val_match_1__ + "\n" + val_match_2__) ); // 改行コードを区切り文字にしている

    Update( val_name__ );

};


// ------------------------------------------------ //
///
/// @brief     This function updates all statuses with evaluating associated status-values.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::Update(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    for(const auto &it : val_name_) {

        auto it2 = val_sts_.find(it);

        if(it2 == val_sts_.end()) { continue; };

        MatchVal(it);

    };

};


// ------------------------------------------------ //
///
/// @brief     This function updates a status that is associated with the specified status-values.
/// @param[in] val_name__ : The status-value name.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::Update( const string &val_name__ ){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    auto it = val_sts_.find(val_name__);

    if(it == val_sts_.end()) { return; };

    /*
    int N_val = 0;

    if( val_kind_[val_name__] == "bool" ) {
        N_val = val_bool_[val_name__].size();
    }
    else if( val_kind_[val_name__] == "char" ) {
        N_val = val_char_[val_name__].size();
    }
    else if( val_kind_[val_name__] == "int" ) {
        N_val = val_int_[val_name__].size();
    }
    else if( val_kind_[val_name__] == "double" ) {
        N_val = val_double_[val_name__].size();
    }
    else if( val_kind_[val_name__] == "string" ) {
        N_val = val_string_[val_name__].size();
    }

    auto it1   = val_sts_.lower_bound(val_name__);
    auto last  = val_sts_.upper_bound(val_name__);

    while(it1 != last) {

        string sts_name = it1->second;

        sts_val_bool_[sts_name].assign(N_val, false);

        //cout << "::N_val " << sts_name << " " << N_val << endl;

        //vector<bool>().swap( sts_val_bool_[sts_name] );
        //sts_val_bool_[sts_name].reserve(N_val);

        //for(int i=0; i<N_val; i++) {
        //    sts_val_bool_[sts_name].push_back( false );
        //}

        ++it1;

    }
    */

    MatchVal( val_name__ );

};


// ------------------------------------------------ //
///
/// @brief     This function prints commands, messages, statuses and status-values.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::Print(){

    PrintCmd();

    PrintSts();

    PrintMsg();

    PrintVal();

};


// ------------------------------------------------ //
///
/// @brief     This function prints commands.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::PrintCmd(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( !cmd_.empty() ) {

        cout << "[Command]" << endl;

        for(const auto &it : cmd_) {
        //for(auto it = cmd_.begin(); it != cmd_.end(); ++it) {
            cout << "[" << it.first << "] " << it.second << endl;
        }

        cout << endl;

    };

    if( !macro_.empty() ) {

        cout << "[Macro]" << endl;

        for(const auto &it : macro_) {
        //for(auto it = macro_.begin(); it != macro_.end(); ++it) {
            for(const auto &it2 : macro_[it.first]) {
                cout << "[" << it.first << "] " << it2 << endl;
            }
        }

        cout << endl;

    };

};


// ------------------------------------------------ //
///
/// @brief     This function prints statuses.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::PrintSts(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( sts_name_.empty() ) { return; };

    cout << "[Status]" << endl;

    for(const auto &it : sts_name_) {
        string bool_str = "";
        if(  sts_bool_[it] ) { bool_str = "TRUE"; }
        if( !sts_bool_[it] ) { bool_str = "FALSE"; }
        printf("%-20s [%-7s] = %-5s \n",it.c_str(),sts_kind_[it].c_str(), bool_str.c_str() );
    }

    cout << endl;

};


// ------------------------------------------------ //
///
/// @brief     This function prints statuses, and prints detailed statuses corresponding to each status-value.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::PrintSts_detail(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( sts_name_.empty() ) { return; };

    cout << "[Status]" << endl;

    for(const auto &it : sts_name_) {

        string bool_str = "";
        if(  sts_bool_[it] ) { bool_str = "TRUE"; }
        if( !sts_bool_[it] ) { bool_str = "FALSE"; }

        string bool_str2 = "";
        for( const auto &it2 : sts_val_bool_[it] ) {
            if( it2 ) { bool_str2 += " T"; }
            else      { bool_str2 += " F"; }
        }

        printf("%-20s [%-6s] = %-5s :%s\n",it.c_str(),sts_kind_[it].c_str(), bool_str.c_str(), bool_str2.c_str() );

    }

    cout << endl;

};


// ------------------------------------------------ //
///
/// @brief     This function prints messages.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::PrintMsg(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( sts_name_.empty() ) { return; };

    cout << "[Messages]" << endl;

    for(const auto &it : sts_name_) {
        for(const auto &it2 : sts_msg_[it]) {
            printf("%-20s : [%10s] %-s \n", it.c_str(), it2.first.c_str(), it2.second.c_str());
        }
    }

    cout << endl;

};


// ------------------------------------------------ //
///
/// @brief     This function prints status-values.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::PrintVal(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    if( val_name_.empty() ) { return; };

    cout << "[Values]" << endl;

    for(const auto &it : val_name_) {

        printf("%-20s [%-6s] = ",it.c_str(),val_kind_[it].c_str());

        if( str_.Match(val_kind_[it], R"(.*bool)") ) {
            for(const auto &it2 : val_bool_[it]) {
                string bool_str = "N";
                if(  it2 ) { bool_str = "T"; }
                if( !it2 ) { bool_str = "F"; }
                cout << bool_str << "  ";
            }
        }

        if( str_.Match(val_kind_[it], R"(.*char)") ) {
            for(const auto &it2 : val_char_[it]) {
                cout << it2 << "  ";
            }
        }

        if( str_.Match(val_kind_[it], R"(.*int)") ) {
            for(const auto &it2 : val_int_[it]) {
                cout << it2 << "  ";
            }
        }

        if( str_.Match(val_kind_[it], R"(.*double)") ) {
            for(const auto &it2 : val_double_[it]) {
                printf("%f  ",it2);
                // cout << it2 << "  ";
            }
        }

        if( str_.Match(val_kind_[it], R"(.*string)") ) {
            for(const auto &it2 : val_string_[it]) {
                cout << it2 << "  ";
            }
        }

        // cout << endl;

        auto it3  = val_match_.lower_bound(it);
        auto last = val_match_.upper_bound(it);

        while(it3 != last) {
            string match_str = it3->second;
            str_.Replace_directly(match_str, R"(\n)", " : ");
            cout << " :: " << match_str;
            ++it3;
        }
        cout << endl;

    }

    cout << endl;

};


// ------------------------------------------------ //
///
/// @brief     This function prints the information of the shared memories.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::PrintShm(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    cout << "[Shared Memories]" << endl;

    shm_.Print();

    cout << endl;

};


// ------------------------------------------------ //
///
/// @brief     This function sets shared memory about the to the status-values.
/// @param[in] name__ : The status or status-value name.
/// @param[in] shm_num__ : The size of shared memory array. the default is 1.
/// @param[in] str_byte_size__ : The size of a shared memory component, which is a string type.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::SetShm( const string &name__, const int &shm_num__, const int &str_byte_size__ ){

    if(shm_num__ > 0) {
        shm_use_[name__] = true; // 使うか否か？
    }
    else {
        shm_use_[name__] = false;
    }

    shm_num_[name__] = shm_num__; // 値の配列数指定 (default:1)

    shm_string_size_[name__] = str_byte_size__; // stringの場合，その一変数あたりの文字数 (default:0)

};


// ------------------------------------------------ //
///
/// @brief     This function writes the shared memory information to a file.
/// @param[in] shm_file__ : The shared memory file.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::WriteShmFile(const string &shm_file__){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    shm_.WriteFile(shm_file__);

};


// ------------------------------------------------ //
///
/// @brief     This function clears the member variables.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::ClearStatusMember(){

    cmd_.clear();
    cmd_return_.clear();
    cmd_wait_.clear();

    macro_.clear();
    macro_return_.clear();
    macro_wait_.clear();

    sts_bool_.clear();
    sts_val_bool_.clear();
    sts_kind_.clear();
    sts_msg_.clear();

    msg_msg_.clear();

    val_bool_.clear();
    val_char_.clear();
    val_int_.clear();
    val_double_.clear();
    val_string_.clear();
    val_kind_.clear();
    val_sts_.clear();
    val_match_.clear();

    vector<string>().swap(sts_name_);
    vector<string>().swap(msg_name_);
    vector<string>().swap(val_name_);

    shm_use_.clear();
    shm_num_.clear();
    shm_string_size_.clear();

};


// ------------------------------------------------ //
///
/// @brief     This function removes the shared memories.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::CloseShm(){

    {

        lock_guard<recursive_mutex> lock(mtx_sts_);

        shm_use_.clear();
        shm_num_.clear();
        shm_string_size_.clear();

    }

    return shm_.Close();

};


// ------------------------------------------------ //
///
/// @brief     This function sets the parameter of whether to close the shared memories after the object ends.
/// @param[in] close_shm_at_end__ : The parameter of whether to close the shared memories after the object ends.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::Close_shm_at_end(const bool &close_shm_at_end__){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    close_shm_at_end_ = close_shm_at_end__;

};


// ------------------------------------------------ //
///
/// @brief     This function closes member variables and shared memories.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::Close(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    MyStatus::ClearStatusMember();

    return shm_.Close();

};


// ------------------------------------------------ //
///
/// @brief     This function clears member variables and shared memories.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::Clear(){

    lock_guard<recursive_mutex> lock(mtx_sts_);

    MyStatus::ClearStatusMember();

    return shm_.Clear();

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existance of the status.
/// @param[in] sts_name__ : The status name.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::ExistSts( const string &sts_name__ ){

    for(const auto &it : sts_name_) {
        if(it == sts_name__) { return true; }
    }

    /*
       auto it = find(sts_name_.begin(), sts_name_.end(), sts_name__);

       if( it != sts_name_.end() ) {
            // cout << "[MyStatus::ExistSts] The same status name has already existed." << endl;
            return true;
       };
     */

    return false;

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existance of the message.
/// @param[in] msg_name__ : The message name.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::ExistMsg( const string &msg_name__ ){

    for(const auto &it : msg_name_) {
        if(it == msg_name__) { return true; }
    }

    return false;

};


// ------------------------------------------------ //
///
/// @brief     This function checks the existance of the status-value.
/// @param[in] val_name__ : The status-value name.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyStatus::ExistVal( const string &val_name__ ){

    for(const auto &it : val_name_) {
        if(it == val_name__) { return true; }
    }

    return false;

};


// ------------------------------------------------ //
///
/// @brief     This function updates the status by checking the status-value for matching-patterns.
/// @param[in] val_name__ : The status-value name.
/// @return    None.
///
// ------------------------------------------------ //


void MyStatus::MatchVal( const string &val_name__ ){

    if( val_sts_.empty() ) { return; };

    auto it   = val_match_.lower_bound(val_name__);
    auto last = val_match_.upper_bound(val_name__);

    while (it != last) {

        vector<string> val_match = str_.Split( it->second, "\n" );

        if( val_match.size() == 2 ) { val_match.push_back(""); };

        string &val_sts     = val_match[0];
        string &val_match_1 = val_match[1];
        string &val_match_2 = val_match[2];

        vector<bool> MatchVal_calc_1 = MatchVal_calc( val_name__, val_match_1 );
        vector<bool> MatchVal_calc_2 = MatchVal_calc( val_name__, val_match_2 );

        if(MatchVal_calc_2.empty()) { MatchVal_calc_2 = MatchVal_calc_1; }

        int N_val = static_cast<int>( MatchVal_calc_1.size() );

        if( sts_kind_[val_sts] == "normal" ) {

            sts_bool_[val_sts] = true;
            sts_val_bool_[val_sts].assign(N_val, true);

            if( str_.Match(val_match_1, R"(^<.+)") && str_.Match(val_match_2, R"(^>.+)") ) { // OR
                for(int i=0; i<N_val; i++) {
                    //sts_val_bool_[val_sts][i] = true;
                    if( !MatchVal_calc_1[i] && !MatchVal_calc_2[i] ) {
                        sts_bool_[val_sts]        = false;
                        sts_val_bool_[val_sts][i] = false;
                    }
                }
            } else { // AND
                for(int i=0; i<N_val; i++) {
                    //sts_val_bool_[val_sts][i] = true;
                    if( !MatchVal_calc_1[i] || !MatchVal_calc_2[i] ) {
                        sts_bool_[val_sts]        = false;
                        sts_val_bool_[val_sts][i] = false;
                    }
                }
            }
        };

        if( sts_kind_[val_sts] == "warning" || sts_kind_[val_sts] == "error" ) {

            sts_bool_[val_sts] = false;
            sts_val_bool_[val_sts].assign(N_val, false);

            if( str_.Match(val_match_1, R"(^<.+)") && str_.Match(val_match_2, R"(^>.+)") ) { // OR
                for(int i=0; i<N_val; i++) {
                    //sts_val_bool_[val_sts][i] = false;
                    if( MatchVal_calc_1[i] || MatchVal_calc_2[i] ) {
                        sts_bool_[val_sts]        = true;
                        sts_val_bool_[val_sts][i] = true;
                    }
                }
            } else {
                for(int i=0; i<N_val; i++) { // AND
                    //sts_val_bool_[val_sts][i] = false;
                    if( MatchVal_calc_1[i] && MatchVal_calc_2[i] ) {
                        sts_bool_[val_sts]         = true;
                        sts_val_bool_[val_sts][i]  = true;
                    }
                }
            };

        };

        ++it;

    };

};


// ------------------------------------------------ //
///
/// @brief     This function checks whether the status-value matches the specified pattern condition.
/// @param[in] val_name__ : The status-value name.
/// @param[in] val_match__ : The matching-pattern.
/// @return    Matched or not.
///
// ------------------------------------------------ //


vector<bool> MyStatus::MatchVal_calc( const string &val_name__, const string &val_match__ ){

    if( val_match__ == "" ) { return {}; };

    vector<bool> result;

    //
    // == [bool], [char], [int], [double], [string]
    //

    if( str_.Match(val_match__, R"(^==\s+\S+)") ) {

        if( str_.Match(val_kind_[val_name__], R"(bool)") ) {
            bool val = false;
            if( str_.Replace(val_match__, R"(^==\s+)", "") == "true" ) { val = true; }
            for( const auto &it : val_bool_[val_name__]) {
                if( it == val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(char)") ) {
            string val = str_.Replace(val_match__, R"(^==\s+)", "");
            for( const auto &it : val_char_[val_name__]) {
                if( it == val[0] ) { result.push_back(true); }
                else               { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(int)") ) {
            int val = atoi( str_.Replace(val_match__, R"(^==\s+)", "").c_str() );
            for( const auto &it : val_int_[val_name__]) {
                if( it == val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(double)") ) {
            double val = str_.Str2d( str_.Replace(val_match__, R"(^==\s+)", "") );
            for( const auto &it : val_double_[val_name__]) {
                if( it == val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(string)") ) {
            string val = str_.Replace(val_match__, R"(^==\s+)", "");
            for( const auto &it : val_string_[val_name__]) {
                if( str_.Match( it, val ) ) { result.push_back(true); }
                else                        { result.push_back(false); }
            }
        }

        return result;

    };

    //
    // != [bool], [char], [int], [double], [string]
    //

    if( str_.Match(val_match__, R"(^!=\s+\S+)") ) {

        if( str_.Match(val_kind_[val_name__], R"(bool)") ) {
            bool val = false;
            if( str_.Replace(val_match__, R"(^!=\s+)", "") == "true" ) { val = true; }
            for( const auto &it : val_bool_[val_name__]) {
                if( it != val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(char)") ) {
            string val = str_.Replace(val_match__, R"(^!=\s+)", "");
            for( const auto &it : val_char_[val_name__]) {
                if( it != val[0] ) { result.push_back(true); }
                else               { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(int)") ) {
            int val = atoi( str_.Replace(val_match__, R"(^!=\s+)", "").c_str() );
            for( const auto &it : val_int_[val_name__]) {
                if( it != val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(double)") ) {
            double val = str_.Str2d( str_.Replace(val_match__, R"(^!=\s+)", "") );
            for( const auto &it : val_double_[val_name__]) {
                if( it != val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(string)") ) {
            string val = str_.Replace(val_match__, R"(^!=\s+)", "");
            for( const auto &it : val_string_[val_name__]) {
                if( !str_.Match( it, val ) ) { result.push_back(true); }
                else                         { result.push_back(false); }
                //if( it != val ) { result.push_back(true); }
                //else            { result.push_back(false); }
            }
        }

        return result;

    };

    //
    // >  [int], [double]
    //

    if( str_.Match(val_match__, R"(^>\s+\S+)") ) {

        if( str_.Match(val_kind_[val_name__], R"(int)") ) {
            int val = atoi( str_.Replace(val_match__, R"(^>\s+)", "").c_str() );
            for( const auto &it : val_int_[val_name__]) {
                if( it >  val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(double)") ) {
            double val = str_.Str2d( str_.Replace(val_match__, R"(^>\s+)", "") );
            for( const auto &it : val_double_[val_name__]) {
                if( it >  val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        return result;

    };

    //
    // >= [int], [double]
    //

    if( str_.Match(val_match__, R"(^>=\s+\S+)") ) {

        if( str_.Match(val_kind_[val_name__], R"(int)") ) {
            int val = atoi( str_.Replace(val_match__, R"(^>=\s+)", "").c_str() );
            for( const auto &it : val_int_[val_name__]) {
                if( it >= val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(double)") ) {
            double val = str_.Str2d( str_.Replace(val_match__, R"(^>=\s+)", "") );
            for( const auto &it : val_double_[val_name__]) {
                if( it >= val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        return result;

    };


    //
    // <  [int], [double]
    //

    if( str_.Match(val_match__, R"(^<\s+\S+)") ) {

        if( str_.Match(val_kind_[val_name__], R"(int)") ) {
            int val = atoi( str_.Replace(val_match__, R"(^<\s+)", "").c_str() );
            for( const auto &it : val_int_[val_name__]) {
                if( it <  val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(double)") ) {
            double val = str_.Str2d( str_.Replace(val_match__, R"(^<\s+)", "") );
            for( const auto &it : val_double_[val_name__]) {
                if( it <  val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        return result;

    };


    //
    // <= [int], [double]
    //

    if( str_.Match(val_match__, R"(^<=\s+\S+)") ) {

        if( str_.Match(val_kind_[val_name__], R"(int)") ) {
            int val = atoi( str_.Replace(val_match__, R"(^<=\s+)", "").c_str() );
            for( const auto &it : val_int_[val_name__]) {
                if( it <= val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        if( str_.Match(val_kind_[val_name__], R"(double)") ) {
            double val = str_.Str2d( str_.Replace(val_match__, R"(^<=\s+)", "") );
            for( const auto &it : val_double_[val_name__]) {
                if( it <= val ) { result.push_back(true); }
                else            { result.push_back(false); }
            }
        }

        return result;

    };

    //
    // include [string]
    //

    if( str_.Match(val_match__, R"(^in(c|cl|clu|clud|clude)\s+\S+)") ) {
        string val = ".*" + str_.Replace(val_match__, R"(^in(c|cl|clu|clud|clude)\s+)", "") + ".*";
        // string val = ".*" + str_.Replace(val_match__, R"(^inc\s+)", "") + ".*";
        for( const auto &it : val_string_[val_name__]) {
            if( str_.Match( it, val ) ) { result.push_back(true); }
            else                        { result.push_back(false); }
        }
        return result;
    };

    //
    // exclude [string]
    //

    if( str_.Match(val_match__, R"(^ex(c|cl|clu|clud|clude)\s+\S+)") ) {
        string val = ".*" + str_.Replace(val_match__, R"(^ex(c|cl|clu|clud|clude)\s+)", "") + ".*";
        // string val = ".*" + str_.Replace(val_match__, R"(^exc\s+)", "") + ".*";
        for( const auto &it : val_string_[val_name__]) {
            if( !str_.Match( it, val ) ) { result.push_back(true); }
            else { result.push_back(false); }
        }
        return result;
    };

    return {false};

};

// ------------------------------------------------ //


}  // MyAll


// ================================================ //
