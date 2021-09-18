// ================================================ //
///
/// @file   MySysCmd.hpp
/// @brief  class libraly of MySysCmd.
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

#ifndef _MYALL_MYSYSCMD_HPP_
#define _MYALL_MYSYSCMD_HPP_

//

#include <vector>

#include <string>

#include <map>

//

namespace MyAll {

// ================================================ //
///
/// @class   MySysCmd
/// @brief   My Class for ordering system commands.
/// @details This class object could work as belows,
///          @li Execute system commands and receive its replies.
///          @li Honk beep sounds.
///          @li Print standard output with front & back colors, bold-character & underscore.
///          Some environments might need "gnu++11" to compile this class.
/// @date    2020/01/01
///
// ================================================ //

class MySysCmd {

 public:

    MySysCmd();  ///< Constractor :

    ~MySysCmd(); ///< Destractor :

    ///
    /// @name  System commands.
    /// @{

    bool System(const std::string &command__, const bool &print__=true); ///< Executing system commands. :

    bool System(const std::string &command__, std::vector<std::string> &return__); ///< Execute system commands & read the standard output. :

    /// @}
    /// @name  Beep sounds.
    /// @{

    void BeepSound(const int &count__); ///< Beep sounds :

    /// @}
    /// @name  Printing standard output with options.
    /// @{

    void Print(const std::string &str__, const std::string char_color__="black", const std::string back_color__="", const bool bold__=false, const bool underscore__=false); ///< standard output with options :

    /// @}

 private:

    ///
    /// @name  An aid of beep sounds.
    /// @{

    void BeepSound_org(const int &count__=1); ///< A beep sound thread. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Color parameters.
    /// @{

    bool term_color_define_;

    std::map<std::string, std::string> char_color_list_;

    std::map<std::string, std::string> back_color_list_;

    /// @}

};

}  // MyAll

#endif /// _MYALL_MYSYSCMD_HPP_
