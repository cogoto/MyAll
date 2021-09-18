// ================================================ //
///
/// @file   MyMsg.hpp
/// @brief  Class libraly of MyMsg.
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

#ifndef _MYALL_MYMSG_HPP_
#define _MYALL_MYMSG_HPP_

//

#include <map>

#include <string>

#include <vector>

//

namespace MyAll {

// ================================================ //
///
/// @class   MyMsg
/// @brief   My class for dealing with messages.
/// @details Stock & organize massages in your functions.
///          This class object will stock messages during the process,
///          and finally, output organized message(s) when you need.
///          Note that this object shold have messages corresponding to one thing.
/// @date    2020/01/01
///
// ================================================ //

class MyMsg {

 public:

    MyMsg(const std::string &msg__ = ""); ///< Constractor :

    ~MyMsg();                             ///< Destractor :

    ///
    /// @name  Preparing.
    /// @{

    void Init(const std::string &msg__ = "");      ///< Initializes message :

    //

    void Head(const std::string &head_msg__ = ""); ///< Sets message header :

    /// @}
    /// @name  Setting message.
    /// @{

    void Set(const std::string &msg__); ///< Sets message :

    //

    void Set(const std::vector<std::string> &msg__); ///< Sets messages :

    //

    bool Set(const int &no__, const std::string &msg__); ///< Sets message :

    /// @}
    /// @name  Getting message.
    /// @{

    std::string Get(
        const std::string &organize__ = "ALL" // "ALL", "SUM" or "ADJUST"
    );                                        ///< Gets the organized message. :

    //

    std::vector<std::string> GetData(
        const std::string &organize__ = "ALL" // "ALL", "SUM" or "ADJUST"
    );                                        ///< Gets messages. :

    //

    void Print(
        const std::string &organize__ = "ALL" // "ALL", "SUM" or "ADJUST"
    );                                        ///< Prints messages. :

    /// @}

 private:

    ///
    /// @name  Organizing messages.
    /// @{

    void Organize_SUM(); ///< Organizes the same words. :

    //

    void Organize_ADJUST(); ///< Organizes the adjacent same words. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Message containers.
    /// @{

    std::vector<std::string> msg_; ///< Messages. :

    //

    std::vector<std::string> msg_organized_; ///< Messages organized by member functions.

    //

    std::string head_; ///< The header word of message. :

    /// @}

};


// ================================================ //
///
/// @class   MyMsgS
/// @brief   My Class for dealing with messages associated with the name labels.
/// @details This class object could control various messages with each name label. @n
///          As the main flow, this will stock messages during the process,
///          and finally, output organized message(s) when you need it.
/// @date    2020.01.01
///
// ================================================ //

class MyMsgS {

 public:

    MyMsgS();  ///< Constractor :

    ~MyMsgS(); ///< Destractor :

    ///
    /// @name  Preparing.
    /// @{

    void Init(); ///< Initializes all message :

    //

    void Init(const std::string &msg_name__, const std::string &msg__ = "");      ///< Initializes the message :

    //

    void Head(const std::string &msg_name__, const std::string &head_msg__ = ""); ///< Sets the message header :

    /// @}
    /// @name  Setting message.
    /// @{

    void Set(const std::string &msg_name__, const std::string &msg__); ///< Sets a message :

    //

    void Set(const std::string &msg_name__, const std::vector<std::string> &msg__); ///< Sets messages :

    //

    bool Set(const std::string &msg_name__, const int &no__, const std::string &msg__); ///< Sets a message :

    /// @}
    /// @name  Getting message.
    /// @{

    std::string Get(
        const std::string &msg_name__,
        const std::string &organize__ = "ALL" // "ALL", "SUM" or "SIDE_SUM"
    );                                        ///< Gets the message. :

    //

    std::vector<std::string> GetData(
        const std::string &msg_name__,
        const std::string &organize__ = "ALL" // "ALL", "SUM" or "SIDE_SUM"
    );                                        ///< Gets messages. :

    //

    void Print(
        const std::string &msg_name__,
        const std::string &organize__ = "ALL" // "ALL", "SUM" or "SIDE_SUM"
    );                                        ///< Prints messages. :

    /// @}

 private:

    ///
    /// @name  Organizing messages.
    /// @{

    void Organize_SUM(const std::string &msg_name__); ///< Organizes all messages. :

    //

    void Organize_ADJUST(const std::string &msg_name__); ///< Organizes all messages with joining adjacent same words. :

    /// @}
    /// @name  Checking.
    /// @{

    int CheckMsgName(const std::string &msg_name__); ///< Checks the message existence.

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Message containers.
    /// @{

    std::map<std::string, std::vector<std::string> > msg_; ///< Messages.

    //

    std::vector<std::string> msg_organized_; ///< Messages organized by member functions.

    //

    std::map<std::string, std::string> head_; ///< The header word of message. :

    /// @}

};

}  // MyAll

#endif // _MYALL_MYMSG_HPP_
