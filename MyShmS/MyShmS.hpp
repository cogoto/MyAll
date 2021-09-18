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

#ifndef _MYALL_MYSHMS_HPP_
#define _MYALL_MYSHMS_HPP_

//

#include <sys/sem.h>

//

#include <vector>

#include <map>

#include <string>

//

#include "MyString.hpp"

#include "MyIO.hpp"

//

namespace MyAll {

// ================================================ //
///
/// @class   MyShmS
/// @brief   My Class for using shared memories.
/// @details This class could deal with shared memories with a semaphore safely.
/// @date    2020/01/01
///
// ================================================ //

class MyShmS {

 public:

    MyShmS();                             ///< Constractor (using new shared memories) :

    MyShmS(const std::string shm_file__); ///< Constractor (dealing shared memories that already exists) :

    ~MyShmS();                            ///< Destractor :

    ///
    /// @name  Reading shared memories.
    /// @{

    void Read(const std::string &name__, bool &shm_bool__); ///< Reads the shared memory (boolean) :

    //

    void Read(const std::string &name__, char &shm_char__); ///< Reads the shared memory (character) :

    //

    void Read(const std::string &name__, std::string &shm_string__); ///< Reads the shared memory (string) :

    //

    void Read(const std::string &name__, int &shm_int__); ///< Reads the shared memory (integer) :

    //

    void Read(const std::string &name__, double &shm_double__); ///< Reads the shared memory (real number) :

    //

    void Read(const std::string &name__, std::vector<bool> &shm_bool__); ///< Reads the shared memories (boolean) :

    //

    void Read(const std::string &name__, std::vector<char> &shm_char__); ///< Reads the shared memories (character) :

    //

    void Read(const std::string &name__, std::vector<std::string> &shm_string__); ///< Reads the shared memories (string) :

    //

    void Read(const std::string &name__, std::vector<int> &shm_int__); ///< Reads the shared memories (integer) :

    //

    void Read(const std::string &name__, std::vector<double> &shm_double__); ///< Reads the shared memories (real number) :

    /// @}
    /// @name  Writing shared memories.
    /// @{

    void Write(
        const std::string &name__,
        const bool &shm_bool__
    ); ///< Writes the shared memory (boolean) :

    //

    void Write(
        const std::string &name__,
        const char &shm_char__
    ); ///< Writes the shared memory (character) :

    //

    void Write(
        const std::string &name__,
        const std::string &shm_string__,
        const int &string_byte_size__ = 0
    ); ///< Writes the shared memory (string) :

    //

    void Write(
        const std::string &name__,
        const int &shm_int__
    ); ///< Writes the shared memory (integer) :

    //

    void Write(
        const std::string &name__,
        const double &shm_double__
    ); ///< Writes the shared memory (real number) :

    //

    void Write(
        const std::string &name__,
        const std::vector<bool> &shm_bool__,
        const int &shm_num__ = 0
    ); ///< Writes the shared memories (boolean) :

    //

    void Write(
        const std::string &name__,
        const std::vector<char> &shm_char__,
        const int &shm_num__ = 0
    ); ///< Writes the shared memories (character) :

    //

    void Write(
        const std::string &name__,
        const std::vector<std::string> &shm_string__,
        const int &string_byte_size__ = 0,
        const int &shm_num__ = 0
    ); ///< Writes the shared memories (string) :

    //

    void Write(
        const std::string &name__,
        const std::vector<int> &shm_int__,
        const int &shm_num__ = 0
    ); ///< Writes the shared memories (integer) :

    //

    void Write(
        const std::string &name__,
        const std::vector<double> &shm_double__,
        const int &shm_num__ = 0
    ); ///< Writes the shared memories (real number) :

    /// @}
    /// @name  Dealing with the file of shared memoriy information.
    /// @{

    bool ReadFile(const std::string &shm_file__); ///< Reads the (additional) shared memory file. :

    //

    void WriteFile(const std::string &shm_file__); ///< Writes the shared memory information to the file. :

    /// @}
    /// @name  Closing the shared memories.
    /// @{

    void Close_at_end(const bool &close_at_end__ = true); ///< Sets Whether the object closes the shared memories & semaphore after that end or not. :

    //

    bool Close(); ///< Closes the shared memories & semaphore. :

    /// @}
    /// @name  Initialization.
    /// @{

    bool Init(); ///< Initializes the shared memories & semaphore. :

    /// @}
    /// @name  Clearing.
    /// @{

    bool Clear(); ///< Clears all the shared memories.

    //

    void Clear_shm_info(); ///< * Do not use it! * Clears the member variables only. :

    /// @}
    /// @name  Tools.
    /// @{

    std::vector<std::string> Name(); ///< Gets names of shared memories. :

    //

    std::string Type(const std::string &name__); ///< Gets the kind of the shared memories. :

    //

    int N(const std::string &name__); ///< Gets the number of the shared memories. :

    //

    int StrSize(const std::string &name__); ///< Gets the number of characters in the string shared memories. :

    //

    int ByteSize(const std::string &name__); ///< Gets the byte size of the shared memory type. :

    //

    std::string Info(const std::string &name__ = ""); ///< Gets the shared momory information.

    //

    void Print(const std::string &name__ = ""); ///< Prints the shared momory information.

    /// @}

 private:

    ///
    /// @name  Setting byte sizes.
    /// @{

    void InitByteSize(); ///< Defines byte sizes of each shared memory type. :

    /// @}
    /// @name  The semaphore.
    /// @{

    bool MakeSem(); ///< Makes a semaphore. :

    //

    bool Sem_lock(); ///< lock with the semaphore. :

    //

    bool Sem_unlock(); ///< unlock with the semaphore. :

    //

    bool SemOp(const int &sem_op__); ///< Operates the semaphore. :

    /// @}
    /// @name  Getting ID of the shared memory.
    /// @{

    bool ShmGet(const std::string &name__); ///< Gets the shared momory ID. :

    /// @}
    /// @name  Closing shared memories & a semaphore.
    /// @{

    bool Close_shm(); ///< Closes the shared momories. :

    //

    bool Close_sem(); ///< Closes the semaphore. :

    /// @}
    /// @name  Checking.
    /// @{

    bool ExistName(const std::string &name__); ///< Checks a existance of the shared memory.

    //

    bool Check_read(
        const std::string &name__,
        const std::string &type__
    ); ///< Checks before reading the shared memory. :

    //

    bool Check_write(
        const std::string &name__,
        const std::string &type__,
        const int &vec_size__,
        const int &shm_num__
    ); ///< Checks before writing the shared memory. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Shared memory parameters.
    /// @{

    std::vector<std::string> name_;           ///< Names of shared memories.

    std::map<std::string, std::string> type_; ///< Types of shared memories.

    std::map<std::string, int> shm_id_;       ///< Shared memory IDs.

    std::map<std::string, int> shm_num_;      ///< Numbers of each shared memory array.

    std::map<std::string, int> shm_size_;     ///< Sizes of each shared memory (=shm_num_ * byte_size_) [Byte]

    std::map<std::string, int> byte_size_;    ///< Byte sizes of each type (other than string-type).

    std::map<std::string, int> str_size_;     ///< Sizes of each string-type shared memory.

    bool close_at_end_; ///< Closes shared memories & the semaphore after the object life is ended or not.

    /// @}
    /// @name  Semaphore parameters.
    /// @{

    int sem_id_;                ///< The semaphore ID.

    union semun {
        int val;                // semaphore value.
        struct semid_ds *buf;   // buffer for IPC_STAT & IPC_SET
        unsigned short * array; // array for GETALL & SETALL
    } ctl_arg_;                 ///< The structure for semaphores.

    const int LOCK   = -1;      ///< A constant for locking with a semaphore.

    const int UNLOCK =  1;      ///< A constant for unlocking with a semaphore.

    // bool use_sem_; // Uses semaphores or not.

    /// @}
    /// @name  Tools.
    /// @{

    MyString str_; ///< For dealing strings.

    MyI infile_;   ///< For reading files.

    MyO outfile_;  ///< For writing files.

    /// @}

};

} // MyAll

#endif /// _MYALL_MYSHMS_HPP_
