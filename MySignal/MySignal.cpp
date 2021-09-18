// ================================================ //
///
/// @file   MySignal.cpp
/// @brief  Implementation of MySignal.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MySignal.hpp"

#include <signal.h>

#include <iostream>

//

namespace MyAll {

///

using namespace std;


// ------------------------------------------------ //
///
/// @brief  First process.  @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MySignal::MySignal() {

    MySignal::_Init();

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MySignal::~MySignal() {

}


// ------------------------------------------------ //
//
// Initializating a signal flag (a static member variable).
//
// ------------------------------------------------ //


bool MySignal::myall_mysignal_signal_ = false;


// ------------------------------------------------ //
//
// Initializating a print flag (a static member variable).
//
// ------------------------------------------------ //


bool MySignal::myall_mysignal_print_ = false;


// ------------------------------------------------ //
//
// Initializating a initialization flag (a static member variable).
//
// ------------------------------------------------ //


bool MySignal::myall_mysignal_init_ = false;


// ------------------------------------------------ //
///
/// @brief   This function sets a signal handler.
/// @return  None.
///
// ------------------------------------------------ //


void MySignal::_Init() {

    //
    // Setting Signal handler.
    //

    if (!myall_mysignal_init_) {

        if (signal(SIGINT, MySignal::_SignalHandler) == SIG_ERR) {
            cerr << "[MySignal::SignalHandler] Cannot catch SIGINT" << endl;
        }

        if (signal(SIGUSR1, MySignal::_SignalHandler) == SIG_ERR) {
            cerr << "[MySignal::SignalHandler] Cannot catch SIGUSR1" << endl;
        }

        if (signal(SIGTERM, MySignal::_SignalHandler) == SIG_ERR) {
            cerr << "[MySignal::SignalHandler] Cannot catch SIGTERM" << endl;
        }

        // if ( signal(SIGKILL, MySignal::_SignalHandler) == SIG_ERR ) {
        //    cerr << "[MySignal::SignalHandler] Cannot catch SIGKILL" << endl;
        // }

        // if ( signal(SIGSTOP, MySignal::_SignalHandler) == SIG_ERR ) {
        //    cerr << "[MySignal::SignalHandler] Cannot catch SIGSTOP" << endl;
        // }

        /*
           if ( signal(SIGINT, SignalHandler_MySignal) == SIG_ERR ) {
            cerr << "[MySignal::SignalHandler] Cannot catch SIGINT" << endl;
           }

           if ( signal(SIGUSR1, SignalHandler_MySignal) == SIG_ERR ) {
            cerr << "[MySignal::SignalHandler] Cannot catch SIGUSR1" << endl;
           }

           if ( signal(SIGTERM, SignalHandler_MySignal) == SIG_ERR ) {
            cerr << "[MySignal::SignalHandler] Cannot catch SIGTERM" << endl;
           }

           //if ( signal(SIGKILL, SignalHandler_MySignal) == SIG_ERR ) {
           //    cerr << "[MySignal::SignalHandler] Cannot catch SIGKILL" << endl;
           //}

           //if ( signal(SIGSTOP, SignalHandler_MySignal) == SIG_ERR ) {
           //    cerr << "[MySignal::SignalHandler] Cannot catch SIGSTOP" << endl;
           //}
         */

        myall_mysignal_init_ = true;

    }

}


// ------------------------------------------------ //
///
/// @brief   This function checks the existence of signals.
/// @return  The signal flag.
///
// ------------------------------------------------ //


bool MySignal::SignalCheck() {

    return MySignal::myall_mysignal_signal_;

}


// ------------------------------------------------ //
///
/// @brief   This function sets whether to inform with printing
///          when the signal is received.
/// @return  None.
///
// ------------------------------------------------ //


void MySignal::Print(const bool &print__) {

    MySignal::myall_mysignal_print_ = print__;

}


/*
   void MySignal::_Set(){

    MySignal::myall_mysignal_signal_ = true;

   };


   bool MySignal::_PrintCheck(){

    return MySignal::myall_mysignal_print_;

   };
 */


// ------------------------------------------------ //
///
/// @brief   The callback function of signal handler.
/// @return  None.
///
// ------------------------------------------------ //


void MySignal::_SignalHandler(int signo__) {

    MySignal::myall_mysignal_signal_ = true;

    if (signo__ == SIGUSR1) {
        if (MySignal::myall_mysignal_print_) { cout << "[MySignal] Received SIGUSR1" << endl; }
    }
    else if (signo__ == SIGTERM) {
        if (MySignal::myall_mysignal_print_) { cout << "[MySignal] Received SIGTERM" << endl; }
    }

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //


/*
   void SignalHandler_MySignal(int signo__){

    MyAll::MySignal signal_val;

    signal_val._Set();

    if (signo__ == SIGUSR1) {
        if( signal_val._PrintCheck() ) { std::cout << "[MySignal] Received SIGUSR1" << std::endl; };
    }
    else if (signo__ == SIGTERM) {
        if( signal_val._PrintCheck() ) { std::cout << "[MySignal] Received SIGTERM" << std::endl; };
    }

   };
 */


// ================================================ //
