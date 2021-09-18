// ================================================ //
///
/// @file   MySignal.hpp
/// @brief  Class libraly of MySignal.
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

#ifndef _MYALL_MYSIGNAL_HPP_
#define _MYALL_MYSIGNAL_HPP_

//

namespace MyAll {

// ================================================ //
///
/// @class   MySignal
/// @brief   My class for dealing system signals.
/// @details This class object could detect
///          @li SIGINT signals
///          @li SIGUSR1 signals
///          @li SIGTERM signals.
///          Usually, SignalCheck() function of this object will set
///          in the progtram loop.
///          The signal flag is a static variable (=common in this cless)
///          Thus, if one of the thread at multi-process or multi-thread detecte the signal,
///          other processes are also detect.
///          So, be careful not to perform post-processing multiple.
/// @date    2020/01/01 ver. 1.0
///
// ================================================ //

class MySignal {

 public:

    MySignal();  ///< Constractor :

    ~MySignal(); ///< Destractor :

    ///
    /// @name  Printing.
    /// @{

    void Print(const bool &print__ = true); ///< Prints when detect signals or not. :

    /// @}
    /// @name  Checking.
    /// @{

    bool SignalCheck(); ///< checks the signal existence. :

    /// @}

 private:

    ///
    /// @name  Setting signal handler.
    /// @{

    void        _Init(); ///< Sets signal handler functions :

    static void _SignalHandler(int signo__);

    // void _Set(); ///< Sets that the signal flag is true. Use only from SignalHandler_MySignal(). :

    // bool _PrintCheck(); ///< Prints or not. Use only from SignalHandler_MySignal(). :

    // -------------------------------------------- //

    /// @}
    /// @name  Control parameters (static member variables)
    /// @{

    static bool myall_mysignal_signal_; // Signal flag. :

    static bool myall_mysignal_print_;  // Prints signal incidence or not. :

    static bool myall_mysignal_init_;   // Init flag. :

    /// @}

};

}  // MyAll

// =======================================//


// void SignalHandler_MySignal(int signo__);


// =======================================//


#endif /// _MYALL_MYSIGNAL_HPP_
