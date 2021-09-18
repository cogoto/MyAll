// ================================================ //
///
/// @file   MyStatus.hpp
/// @brief  Class libraly of MyStatus.
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

#ifndef _MYALL_MYSTATUS_HPP_
#define _MYALL_MYSTATUS_HPP_

//

#include <map>

#include <mutex>

#include <string>

#include <vector>

//

#include "MyString.hpp"

#include "MyShmS.hpp"

//

namespace MyAll {

// ================================================ //
///
/// @class   MyStatus
/// @brief   My class for managing status.
/// @details This class object could manage normal/error statuses and status values.
///          This class can do the followings:
///          @li Performing the management of command statement.
///          @li Performing the management of binary command.
///          @li Performing the management of macro command statement.
///          @li Performing the status management.
///          @li Performing the status-value management.
/// @date    2020/01/01
///
// ================================================ //

class MyStatus {

 public:

    MyStatus();  ///< Constractor :

    ~MyStatus(); ///< Destractor :

    ///
    /// @name  Commands.
    /// @{

    //
    // Sets a command.
    //

    void SetCmd(
        const std::string &cmd_name__,
        const std::string &cmd__,             // command statement.
        const std::string &cmd_return__ = "", // command return string.
        const double &wait__ = -1             // wait time after the command.
    );                                        ///< Sets the command information. :

    //
    // Gets all commands.
    //

    std::map<std::string, std::string> GetCmd(); ///< Gets all command statements. :

    //

    std::map<std::string, std::string> GetCmd_return(); ///< Gets all command return strings. :

    //

    std::map<std::string, double> GetCmd_wait(); ///< Gets all wait time information. :

    //
    // Gets the command.
    //

    std::string GetCmd(
        const std::string &cmd_name__,
        const std::vector<std::string> &replace__ = {}

    ); ///< Gets the command statement. :

    //

    std::string GetCmd_return(
        const std::string &cmd_name__,
        const std::vector<std::string> &replace__ = {}

    ); ///< Gets the command return string. :

    //

    double GetCmd_wait(const std::string &cmd_name__); ///< Gets the wait time information. :

    /// @}
    /// @name  Binary commands.
    /// @{

    //
    // Sets a command.
    //

    void SetCmdC(
        const std::string &cmd_name__,
        const std::vector<char> &cmd__,             // command binaries.
        const std::vector<char> &cmd_return__ = {}, // command return binaries.
        const double &wait__ = -1                   // wait time after the command.
    );                                              ///< Sets the binary command information. :

    //
    // Gets all commands.
    //

    std::map<std::string, std::vector<char> > GetCmdC(); ///< Gets all binary command statements. :

    //

    std::map<std::string, std::vector<char> > GetCmdC_return(); ///< Gets all binary command return strings. :

    //

    std::map<std::string, double> GetCmdC_wait(); ///< Gets all wait time information. :

    //
    // Gets the command.
    //

    std::vector<char> GetCmdC(const std::string &cmd_name__); ///< Gets the binary command statement. :

    //

    std::vector<char> GetCmdC_return(const std::string &cmd_name__); ///< Gets the binary command return string. :

    //

    double GetCmdC_wait(const std::string &cmd_name__); ///< Gets the wait time information. :

    /// @}
    /// @name  Macro commands.
    /// @{

    //
    // Sets a macro command.
    //

    void SetMacro(
        const std::string &macro_name__,
        const std::string &macro__,           // macro command statement.
        const std::string &cmd_return__ = "", // macro command return string.
        const double &wait__ = -1             // wait time
    );                                        ///< Sets a macro command line one by one. :

    //
    // Gets the macro command.
    //

    std::vector<std::string> GetMacro(const std::string &macro_name__); ///< Gets the macro command statements. :

    //

    std::vector<std::string> GetMacro_return(const std::string &macro_name__); ///< Gets the macro command return strings. :

    //

    std::vector<double> GetMacro_wait(const std::string &macro_name__); ///< Gets the wait times. :

    /// @}
    /// @name  Statuses.
    /// @{

    void DefSts_norm(
        const std::string &sts_name__,
        const bool &sts_bool__ = true,
        const bool &use_shm__  = false,
        std::map<std::string, std::string> sts_msg__ = { { "NORM", "" } }
    ); ///< Defines the normal status. :

    //

    void DefSts_warn(
        const std::string &sts_name__,
        const bool &sts_bool__ = false,
        const bool &use_shm__  = false,
        std::map<std::string, std::string> sts_msg__ = { { "WARN", "[WARN]" } }
    ); ///< Defines the warning status. :

    //

    void DefSts_err(
        const std::string &sts_name__,
        const bool &sts_bool__ = false,
        const bool &use_shm__  = false,
        std::map<std::string, std::string> sts_msg__ = { { "ERR", "[ERROR]" } }
    ); ///< Defines the error status. :

    //

    void SetSts_norm(const std::string &sts_name__, const bool &sts_bool__); ///< Controls the normal status. :

    //

    void SetSts_warn(const std::string &sts_name__, const bool &sts_bool__); ///< Controls the warning status. :

    //

    void SetSts_err(const std::string &sts_name__, const bool &sts_bool__); ///< Controls the error status. :

    //

    std::vector<std::string> GetStsName(const std::string &kind__ = ""); ///< Gets status names. :

    //

    std::vector<std::string> GetStsName_norm(); ///< Gets all normal status names. :

    //

    std::vector<std::string> GetStsName_warn(); ///< Gets all warning status names. :

    //

    std::vector<std::string> GetStsName_err(); ///< Gets all error status names. :

    //
    // Get Status
    //

    std::vector<std::string> GetSts(const bool &sts_bool__, const std::string &kind__); ///< Gets the status names that have the specified state. :

    //

    std::vector<std::string> GetSts_norm(const bool &sts_bool__);  ///< Gets the normal status names that have the specified state. :

    //

    std::vector<std::string> GetSts_warn(const bool &sts_bool__);  ///< Gets the warning status names that have the specified state. :

    //

    std::vector<std::string> GetSts_err(const bool &sts_bool__); ///< Gets the error status names that have the specified state. :

    //

    bool GetSts(const std::string &sts_name__); ///< Gets the status. :

    //

    std::map<std::string, bool> GetSts(); ///< Gets all status. :

    //

    void GetSts(
        std::vector<std::string> &sts_name__,
        std::map<std::string, bool> &sts_bool__
    ); ///< Gets all status. :

    //

    std::vector<bool> GetSts_detail(const std::string &sts_name__); ///< Gets detail-status, which is associated with status-values. :

    //

    std::map<std::string, std::vector<bool> > GetSts_detail(); ///< Gets all detail-status, which are associated with status-values. :

    //

    void GetSts_detail(
        std::vector<std::string> &sts_name__,
        std::map<std::string, std::vector<bool> > &sts_bool__
    ); ///< Gets all status and detail-status, which are associated with status-values. :

    //

    std::vector<std::string> GetStsS(); ///< Gets all status with strings. :

    //
    // Check Status
    //

    bool CheckSts_norm(const bool &sts_bool__ = true); ///< Checks the existence of the specified state from normal statuses. :

    //

    bool CheckSts_warn(const bool &sts_bool__ = true); ///< Checks the existence of the specified state from warning statuses. :

    //

    bool CheckSts_err(const bool &sts_bool__ = true); ///< Checks the existence of the specified state from error statuses. :

    //

    bool CheckSts(const bool &sts_bool__, const std::string &kind__ = ""); ///< Checks the existence of the specified state from all statuses. :

    //

    bool CheckSts(const std::string &sts_name__); ///< Overload function to block use.

    //

    bool CheckSts(const char* sts_name__); ///< Overload function to block use.

    //

    //
    // Get Status Message
    //

    std::string GetStsMsg(
        const std::string &sts_name__,
        const std::string &msg_key__,
        const std::vector<std::string> &replace__ = {}
    ); /// Gets the status message. :

    /// @}
    /// @name  Messages.
    /// @{

    void DefMsg(
        const std::string &msg_name__,
        const std::string &msg__ = ""
    ); ///< Defines the message of the status. :

    //

    std::string GetMsg(
        const std::string &msg_name__,
        const std::vector<std::string> &replace__ = {}
    ); ///< Gets the message of the status. :

    /// @}
    /// @name  Status-values.
    /// @{

    //
    // Define status values
    //

    bool DefVal_bool(const std::string &val_name__, const bool &val__, const bool &use_shm__ = false); ///< Defines a status-value of the boolean. :

    //

    bool DefVal_char(const std::string &val_name__, const char &val__, const bool &use_shm__ = false); ///< Defines a status-value of the char-type. :

    //

    bool DefVal_int(const std::string &val_name__, const int &val__, const bool &use_shm__ = false); ///< Defines a status-value of the integer. :

    //

    bool DefVal_double(const std::string &val_name__, const double &val__, const bool &use_shm__ = false); ///< Defines a status-value of the real number. :

    //

    bool DefVal_string(
        const std::string &val_name__,
        const std::string &val__,
        const bool &use_shm__ = false,
        const int &string_byte_size__ = 0
    ); ///< Defines a status-value of string. :

    //

    bool DefVal_bool(const std::string &val_name__, const std::vector<bool> &val__, const bool &use_shm__ = false); ///< Defines status-values of the boolean. :

    //

    bool DefVal_char(const std::string &val_name__, const std::vector<char> &val__, const bool &use_shm__ = false); ///< Defines status-values of the char-type. :

    //

    bool DefVal_int(const std::string &val_name__, const std::vector<int> &val__, const bool &use_shm__ = false); ///< Defines status-values of the integer. :

    //

    bool DefVal_double(const std::string &val_name__, const std::vector<double> &val__, const bool &use_shm__ = false); ///< Defines status-values of the real number. :

    //

    bool DefVal_string(
        const std::string &val_name__,
        const std::vector<std::string> &val__,
        const bool &use_shm__ = false,
        const int &string_byte_size__ = 0
    ); ///< Defines status-values of string. :

    //
    // Sets status-values
    //

    bool SetVal_bool(const std::string &val_name__, const bool &val__, const int &no__ = 0); ///< Sets a status-value of the boolean. :

    //

    bool SetVal_char(const std::string &val_name__, const char &val__, const int &no__ = 0); ///< Sets a status-value of the char-type. :

    //

    bool SetVal_int(const std::string &val_name__, const int &val__, const int &no__ = 0); ///< Sets a status-value of the integer. :

    //

    bool SetVal_double(const std::string &val_name__, const double &val__, const int &no__ = 0); ///< Sets a status-value of the real number. :

    //

    bool SetVal_string(const std::string &val_name__, const std::string &val__, const int &no__ = 0); ///< Sets a status-value of string. :

    //

    bool SetVal_bool(const std::string &val_name__, const std::vector<bool> &val__); ///< Sets status-values of the boolean. :

    //

    bool SetVal_char(const std::string &val_name__, const std::vector<char> &val__); ///< Sets status-values of the char-type. :

    //

    bool SetVal_int(const std::string &val_name__, const std::vector<int> &val__); ///< Sets status-values of the integer. :

    //

    bool SetVal_double(const std::string &val_name__, const std::vector<double> &val__); ///< Sets status-values of the real number. :

    //

    bool SetVal_string(const std::string &val_name__, const std::vector<std::string> &val__); ///< Sets status-values of string. :

    //
    // Gets status-values.
    //

    bool GetVal_bool(const std::string &val_name__, const int &no__ = 0); ///< Gets a status-value of the boolean. :

    //

    char GetVal_char(const std::string &val_name__, const int &no__ = 0); ///< Gets a status-value of the char-type. :

    //

    int GetVal_int(const std::string &val_name__, const int &no__ = 0); ///< Sets a status-value of the integer. :

    //

    double GetVal_double(const std::string &val_name__, const int &no__ = 0); ///< Sets a status-value of the real number. :

    //

    std::string GetVal_string(const std::string &val_name__, const int &no__ = 0); ///< Sets a status-value of string. :

    //

    std::vector<bool> GetVal_bools(const std::string &val_name__); ///< Gets status-values of the boolean. :

    //

    std::vector<char> GetVal_chars(const std::string &val_name__); ///< Gets status-values of the char-type. :

    //

    std::vector<int> GetVal_ints(const std::string &val_name__); ///< Sets status-values of the integer. :

    //

    std::vector<double> GetVal_doubles(const std::string &val_name__); ///< Sets status-values of the real number. :

    //

    std::vector<std::string> GetVal_strings(const std::string &val_name__); ///< Sets status-values of string. :

    //

    std::vector<std::string> GetValS(); ///< Gets all status-values as string. :

    //

    std::string GetVal_kind(const std::string &val_name__); ///< Gets the kind of the status-value. :

    //
    // Connects status and status-values
    //

    void Connect(
        const std::string &sts_name__,
        const std::string &val_name__,
        const std::string &match_val_1__,
        const std::string &match_val_2__ = ""
    ); ///< Associates the status with the status-value. :

    //
    // Updates status with the status-values
    //

    void Update(); ///< Updates all statuses based on associated status-values. :

    //

    void Update(const std::string &val_name__); ///< Updates the status based on the associated status-value. :

    //
    // Prints
    //

    void Print(); ///< Prints all information of this class. :

    //

    void PrintCmd(); ///< Prints all commands. :

    //

    void PrintSts(); ///< Prints all status. :

    //

    void PrintSts_detail(); ///< Prints all status corresponding to each status-value. :

    //

    void PrintMsg(); ///< Prints all status messages. :

    //

    void PrintVal(); ///< Prints all status-values. :

    //

    void PrintShm(); ///< Prints all information on shared memories. :

    //
    // Shared Memories
    //

    void WriteShmFile(const std::string &shm_file__); ///< Writes the information file of shared memories. :

    //

    bool CloseShm(); ///< Closes the semaphore and shared memories. :

    //

    void Close_shm_at_end(const bool &close_shm_at_end__ = true); ///< Closes the semaphore and shared memories after the object end. :

    //
    // Closes & Clears
    //

    bool Close(); ///< Closes all status information, shared memories, and the semaphore. :

    bool Clear(); ///< Closes all status information and shared memories. :

 private:

    void DefSts(
        const std::string &sts_name__,
        const bool &sts_bool__,
        const bool &use_shm__,
        std::map<std::string, std::string> &sts_msg__,
        const std::string &kind__
    ); /// Defines a status along with the kind of values.

    //

    void SetSts(const std::string &sts_name__, const bool &sts_bool__); ///< Sets the status. :

    //

    bool ExistSts(const std::string &sts_name__); ///< Checks the existance of the status. :

    //

    bool ExistMsg(const std::string &msg_name__); ///< Checks the existance of the status message. :

    //

    bool ExistVal(const std::string &val_name__); ///< Checks the existance of the status value. :

    //

    void MatchVal(const std::string &val_name__); ///< Checks the status value against conditions. :

    //

    std::vector<bool> MatchVal_calc(const std::string &val_name__, const std::string &val_match__); ///< Implement of checking the status value against conditions. :


    //
    // Shared Memories
    //

    void SetShm(const std::string &name__, const int &shm_num__ = 1, const int &string_byte_size__ = 0); ///< Makes setting related to shared memory. :


    //
    // Clearing status members
    //

    void ClearStatusMember(); ///< Clears all member variables rerated to status. :

    //
    // Commands for Getting Status
    //

    std::map<std::string, std::string> cmd_;        ///< commands.

    std::map<std::string, std::string> cmd_return_; ///< return strings.

    std::map<std::string, double> cmd_wait_;        ///< wait times after commands.

    //

    std::map<std::string, std::vector<char> > cmd_c_;        ///< binary commands.

    std::map<std::string, std::vector<char> > cmd_c_return_; ///< return strings of binary commands.

    std::map<std::string, double> cmd_c_wait_;               ///< wait times after binary commands.

    //

    std::map<std::string, std::vector<std::string> > macro_;        ///< macro commands.

    std::map<std::string, std::vector<std::string> > macro_return_; ///< return strings of macro commands.

    std::map<std::string, std::vector<double> > macro_wait_;        ///< wait times after macro commands.

    //
    // Statuses
    //

    std::vector<std::string> sts_name_;                                  ///< status names．(includes normal, warning and error status)

    std::map<std::string, bool> sts_bool_;                               ///< statuses.

    std::map<std::string, std::vector<bool> > sts_val_bool_;             ///< statuses corresponding to each associated status-value.

    std::map<std::string, std::string> sts_kind_;                        ///< the kind of status. ("normal", "warning" and "error")

    std::map<std::string, std::map<std::string, std::string> > sts_msg_; ///< status messages.

    // std::map<std::string, std::string> sts_msg_T_;

    // std::map<std::string, std::string> sts_msg_F_;

    //
    // Messages
    //

    std::vector<std::string> msg_name_;          ///< message names

    std::map<std::string, std::string> msg_msg_; ///< messages

    //
    // Status Values
    //

    std::vector<std::string> val_name_;                           ///< status-value names.

    std::map<std::string, std::vector<bool> > val_bool_;          ///< status-values of the boolean.

    std::map<std::string, std::vector<char> > val_char_;          ///< status-values of the char-type.

    std::map<std::string, std::vector<int> > val_int_;            ///< status-values of the integer.

    std::map<std::string, std::vector<double> > val_double_;      ///< status-values of the real number.

    std::map<std::string, std::vector<std::string> > val_string_; ///< status-values of string.

    std::map<std::string, std::string> val_kind_;                 ///< the kinds of status-values.

    std::multimap<std::string, std::string> val_sts_;             ///< status names(=values) associated with status-values(=keywords).

    std::multimap<std::string, std::string> val_match_;           ///< condition strings of matching.

    //
    // Shared Memories
    //

    std::map<std::string, bool> shm_use_;        ///< Uses shared memory or not.

    std::map<std::string, int> shm_num_;         ///< the number of each shared memory array.

    std::map<std::string, int> shm_string_size_; ///< string size of each shared memory.

    bool close_shm_at_end_;                      ///< closes the semaphore & shared memories after the object end.

    //
    // mutex
    //

    std::recursive_mutex mtx_cmd_; ///< mutex for executing commands.

    std::recursive_mutex mtx_sts_; ///< mutex for status.

    //
    // Objects
    //

    MyAll::MyString str_; ///< For dealing strings.

    MyAll::MyShmS shm_;   ///< For dealing shared memories.


};

} // MyAll

#endif //_MYALL_MYSTATUS_HPP_
