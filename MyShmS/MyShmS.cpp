// ================================================ //
///
/// @file   MyShmS.cpp
/// @brief  Implementation of MyShmS.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyShmS.hpp"

//

#include <cstring>

#include <sys/shm.h>

//

#include <iostream>

//

namespace MyAll {

//

using namespace std;

// using uint = unsigned int;
// using uint = uint_fast32_t;


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //

MyShmS::MyShmS() {

    if (!MyShmS::MakeSem()) {
        cerr << "[MyShmS] cannot make semaphore." << endl;
        return;
    }

    close_at_end_ = true;

    MyShmS::InitByteSize();

}


// ------------------------------------------------ //
///
/// @brief  First process. @n
///         The shared memory file would read in.
///
// ------------------------------------------------ //


MyShmS::MyShmS(const std::string shm_file__) {

    // Read share memory information from a file.

    MyShmS::ReadFile(shm_file__);

    close_at_end_ = true;

    MyShmS::InitByteSize();

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyShmS::~MyShmS() {

    if (close_at_end_) { Close(); }

}


// ------------------------------------------------ //
///
/// @brief     This function defines byte sizes of each shared memory type. @n
///            Because of the size of boolean-type depend on the compiler in c++,
///            the size of boolean-type is substituted one of char-type.
/// @return    None.
///
// ------------------------------------------------ //


void MyShmS::InitByteSize() {

    byte_size_["bool"]   = static_cast<int>(sizeof(char)); // substituted boolean-type to char-type.
    byte_size_["char"]   = static_cast<int>(sizeof(char));
    byte_size_["int"]    = static_cast<int>(sizeof(int));
    byte_size_["double"] = static_cast<int>(sizeof(double));

}


// ------------------------------------------------ //
///
/// @brief     This function gets the shared memory ID.
///            The obtained ID is stored in a member variable (shm_size_).
/// @param[in] name__ : the shared memory name.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyShmS::ShmGet(const std::string &name__) {

    if (type_[name__] != "string") {
        shm_size_[name__] = byte_size_[type_[name__]] * shm_num_[name__];
    }
    else {
        shm_size_[name__] = str_size_[name__] * shm_num_[name__];
    }

    if ((shm_id_[name__] = shmget(IPC_PRIVATE, shm_size_[name__], IPC_CREAT | 0600)) == -1) {
        perror("shmget");
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memory of beelean-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_bool__ : the shared memory data of beelean-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, bool &shm_bool__) {

    //
    // Because of the size of boolean-type depend on the compiler in c++,
    // the size of boolean-type is substituted one of char-type.
    //

    if (!MyShmS::Check_read(name__, "bool")) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    char shm_char = *shm_char_addr;

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

    shm_bool__ = (shm_char == '\x00' ? false : true);

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memory of char-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_char__ : the shared memory data of char-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, char &shm_char__) {

    if (!MyShmS::Check_read(name__, "char")) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    shm_char__ = *shm_char_addr;

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memory of string-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_string__ : the shared memory data of string-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, std::string &shm_string__) {

    if (!MyShmS::Check_read(name__, "string")) { return; }

    char* shm_string_addr;

    Sem_lock();

    if (( shm_string_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    shm_string__ = shm_string_addr; // char->string

    if (shmdt(shm_string_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

    // vector<string> pattern_before { R"(\n+$)" };
    // string pattern_after = "";
    // str_.Replace_directly( shm_string__, pattern_before, pattern_after );

    str_.Replace_directly(shm_string__, R"(\n+$)", "");

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memory of integer-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_int__ : the shared memory data of integer-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, int &shm_int__) {

    if (!MyShmS::Check_read(name__, "int")) { return; }

    int* shm_int_addr;

    Sem_lock();

    if (( shm_int_addr =  (int*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    shm_int__ = *shm_int_addr;

    if (shmdt(shm_int_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memory of real_number-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_double__ : the shared memory data of real_number-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, double &shm_double__) {

    if (!MyShmS::Check_read(name__, "double")) { return; }

    double* shm_double_addr;

    Sem_lock();

    if (( shm_double_addr =  (double*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    shm_double__ = *shm_double_addr;

    if (shmdt(shm_double_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memories of beelean-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_bool__ : the shared memory data of beelean-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, std::vector<bool> &shm_bool__) {

    vector<bool>().swap(shm_bool__);

    if (!MyShmS::Check_read(name__, "bool")) { return; }

    vector<char> shm_char;

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    for (int i = 0; i < shm_num_[name__]; i++) {

        shm_char.push_back(*(shm_char_addr + i));

    }

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

    for (const auto &it : shm_char) {
        shm_bool__.push_back((it == '\x00' ? false : true));
    }

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memories of char-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_char__ : the shared memory data of char-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, std::vector<char> &shm_char__) {

    vector<char>().swap(shm_char__);

    if (!MyShmS::Check_read(name__, "char")) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    for (int i = 0; i < shm_num_[name__]; i++) {

        shm_char__.push_back(*(shm_char_addr + i));

    }

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memories of string-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_string__ : the shared memory data of string-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, std::vector<std::string> &shm_string__) {

    vector<string>().swap(shm_string__);

    if (!MyShmS::Check_read(name__, "string")) { return; }

    char* shm_string_addr;

    Sem_lock();

    if (( shm_string_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    string shm_string_sum = shm_string_addr; // char->string

    if (shmdt(shm_string_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

    vector<string> pattern_before { R"(\n+$)" };
    string pattern_after = "";

    for (int i = 0; i < shm_num_[name__]; i++) {
        shm_string__.push_back(shm_string_sum.substr(i * str_size_[name__], str_size_[name__]));
        str_.Replace_directly(shm_string__[i], pattern_before, pattern_after);
    }

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memories of integer-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_int__ : the shared memory data of integer-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, std::vector<int> &shm_int__) {

    vector<int>().swap(shm_int__);

    if (!MyShmS::Check_read(name__, "int")) { return; }

    int* shm_int_addr;

    Sem_lock();

    if (( shm_int_addr =  (int*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    for (int i = 0; i < shm_num_[name__]; i++) {

        shm_int__.push_back(*(shm_int_addr + i));

    }

    if (shmdt(shm_int_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function reads the shared memories of real_number-type.
/// @param[in]  name__ : the shared memory name.
/// @param[out] shm_double__ : the shared memory data of real_number-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Read(const std::string &name__, std::vector<double> &shm_double__) {

    vector<double>().swap(shm_double__);

    if (!MyShmS::Check_read(name__, "double")) { return; }

    double* shm_double_addr;

    Sem_lock();

    if (( shm_double_addr =  (double*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    for (int i = 0; i < shm_num_[name__]; i++) {

        shm_double__.push_back(*(shm_double_addr + i));

    }

    if (shmdt(shm_double_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memory of beelean-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_bool__ : the writing shared memory data of boolean-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const bool &shm_bool__) {

    //
    // Because of the size of boolean-type depend on the compiler in c++,
    // the size of boolean-type is substituted one of char-type.
    //

    char shm_bool_char = (shm_bool__ ? '\x01' : '\x00'); // true = 0x01, false = 0x00

    if (!MyShmS::Check_write(name__, "bool", 1, 1)) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    *shm_char_addr = shm_bool_char;

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memory of char-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_char__ : the writing shared memory data of char-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const char &shm_char__) {

    if (!MyShmS::Check_write(name__, "char", 1, 1)) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    *shm_char_addr = shm_char__;

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memory of string-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_string__ : the writing shared memory data of string-type.
/// @param[in]  string_byte_size__ : the string size of the writing shared memory data.
///             The string size of the shared memory is fixed at the first writing time.
///             If the size is less than inputed string size at the first time,
///             the inputed string size would be adopted.
///             After the second time, you do not need to input this argument.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const std::string &shm_string__, const int &string_byte_size__) {

    if (!MyShmS::ExistName(name__)) {

        //
        // Only in the first process.
        //

        int shm_string_size = static_cast<int>(shm_string__.size());

        if (string_byte_size__ < shm_string_size) {
            str_size_[name__] = shm_string_size;
        }
        else {
            str_size_[name__] = string_byte_size__;
        }

        // if(str_size_[name__] <= 0) { str_size_[name__] = static_cast<int>(shm_string__.size()); };

    }

    if (!MyShmS::Check_write(name__, "string", 1, 1)) { return; }

    char* shm_string_addr;

    Sem_lock();

    if (( shm_string_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    strcpy(shm_string_addr, shm_string__.c_str());

    if (shmdt(shm_string_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memory of integer-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_int__ : the writing shared memory data of integer-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const int &shm_int__) {

    if (!MyShmS::Check_write(name__, "int", 1, 1)) { return; }

    int* shm_int_addr;

    Sem_lock();

    if (( shm_int_addr =  (int*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    *shm_int_addr = shm_int__;

    if (shmdt(shm_int_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memory of real_number-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_double__ : the writing shared memory data of real_number-type.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const double &shm_double__) {

    if (!MyShmS::Check_write(name__, "double", 1, 1)) { return; }

    double* shm_double_addr;

    Sem_lock();

    if (( shm_double_addr =  (double*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    *shm_double_addr = shm_double__;

    if (shmdt(shm_double_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memories of beelean-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_bool__ : the writing shared memory data of boolean-type.
/// @param[in]  shm_num__ : the number of writing shared memory array.
///             The number is fixed at the first writing time.
///             If the number is less than the one of inputed array at the first time,
///             the inputed array number would be adopted.
///             After the second time, you do not need to input this argument.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const std::vector<bool> &shm_bool__, const int &shm_num__) {

    vector<char> shm_bool_char;

    for (const auto &it : shm_bool__) {
        shm_bool_char.push_back((it ? '\x01' : '\x00'));
    }

    if (!MyShmS::Check_write(name__, "bool", shm_bool_char.size(), shm_num__)) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    int n = 0;

    for (const auto &it : shm_bool_char) {
        *(shm_char_addr + n) = it;
        n++;
    }

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memories of char-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_char__ : the writing shared memory data of char-type.
/// @param[in]  shm_num__ : the number of writing shared memory array.
///             The number is fixed at the first writing time.
///             If the number is less than the one of inputed array at the first time,
///             the inputed array number would be adopted.
///             After the second time, you do not need to input this argument.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const std::vector<char> &shm_char__, const int &shm_num__) {

    if (!MyShmS::Check_write(name__, "char", shm_char__.size(), shm_num__)) { return; }

    char* shm_char_addr;

    Sem_lock();

    if (( shm_char_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    int n = 0;

    for (const auto &it : shm_char__) {
        *(shm_char_addr + n) = it;
        n++;
    }

    if (shmdt(shm_char_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memories of string-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_string__ : the writing shared memory data of string-type.
/// @param[in]  string_byte_size__ : the string size of the writing shared memory data.
///             This string size is common for array components.
///             The size is fixed at the first writing time.
///             If the size is less than inputed string size at the first time,
///             the inputed string size would be adopted.
///             After the second time, you do not need to input this argument.
/// @param[in]  shm_num__ : the number of writing shared memory array.
///             The number is fixed at the first writing time.
///             If the number is less than the one of inputed array at the first time,
///             the inputed array number would be adopted.
///             After the second time, you do not need to input this argument.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const std::vector<std::string> &shm_string__, const int &string_byte_size__, const int &shm_num__) {

    if (!MyShmS::ExistName(name__)) {

        //
        // Only in the first process.
        //

        int shm_string_size = 0;
        for (const auto &it : shm_string__) {
            int it_2 = static_cast<int>(it.size());
            if (it_2 > shm_string_size) {
                shm_string_size = it_2;
            }
        }

        if (string_byte_size__ < shm_string_size) {
            str_size_[name__] = shm_string_size;
        }
        else {
            str_size_[name__] = string_byte_size__;
        }

        /*
           str_size_[name__] = string_byte_size__;
           if(str_size_[name__] <= 0) {
            cout << "[MyShmS::Write] Have to set string byte size." << endl;
            return;
           }
         */

    }

    if (!MyShmS::Check_write(name__, "string", shm_string__.size(), shm_num__)) { return; }

    string shm_string_sum = "";

    for (const auto &it : shm_string__) {

        string shm_string_tmp = it;
        shm_string_tmp.resize(str_size_[name__], '\x0a');

        shm_string_sum += shm_string_tmp;

    }

    /*
       for(int i=0;i<shm_num_[name__];i++){

        string shm_string_tmp = shm_string__[i];
        shm_string_tmp.resize(str_size_[name__],'\x0a');

        shm_string_sum += shm_string_tmp;

       }
     */

    // cout << "[MyShmS::Write] " << shm_num_[name__] << ":" << str_size_[name__] << endl;
    // cout << "[MyShmS::Write] shm_string_sum = " << shm_string_sum << ":::END" << endl;

    char* shm_string_addr;

    Sem_lock();

    if (( shm_string_addr =  (char*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    strcpy(shm_string_addr, shm_string_sum.c_str());

    if (shmdt(shm_string_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memories of integer-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_string__ : the writing shared memory array of integer-type.
/// @param[in]  shm_num__ : the number of writing shared memory array.
///             The number is fixed at the first writing time.
///             If the number is less than the one of inputed array at the first time,
///             the inputed array number would be adopted.
///             After the second time, you do not need to input this argument.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const std::vector<int> &shm_int__, const int &shm_num__) {

    if (!MyShmS::Check_write(name__, "int", shm_int__.size(), shm_num__)) { return; }

    int* shm_int_addr;

    Sem_lock();

    if (( shm_int_addr =  (int*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    int n = 0;

    for (const auto &it : shm_int__) {
        *(shm_int_addr + n) = it;
        n++;
    }

    if (shmdt(shm_int_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function writes the shared memories of real_number-type.
/// @param[in]  name__ : the shared memory name.
///             Do not use space, "(" and ")" for a shared memory file.
/// @param[in]  shm_string__ : the writing shared memory array of real_number-type.
/// @param[in]  shm_num__ : the number of writing shared memory array.
///             The number is fixed at the first writing time.
///             If the number is less than the one of inputed array at the first time,
///             the inputed array number would be adopted.
///             After the second time, you do not need to input this argument.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Write(const std::string &name__, const std::vector<double> &shm_double__, const int &shm_num__) {

    if (!MyShmS::Check_write(name__, "double", shm_double__.size(), shm_num__)) { return; }

    double* shm_double_addr;

    Sem_lock();

    if (( shm_double_addr =  (double*) shmat(shm_id_[name__], 0, 0)) == (void *) -1) { perror("shmat"); }

    int n = 0;

    for (const auto &it : shm_double__) {
        *(shm_double_addr + n) = it;
        n++;
    }

    if (shmdt(shm_double_addr) == -1) { perror("shmdt"); }

    Sem_unlock();

}


// ------------------------------------------------ //
///
/// @brief      This function create the semaphore.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::MakeSem() {

    if ((sem_id_ = semget(IPC_PRIVATE, 1, 0600)) == -1) {
        perror("semget");
        cerr << "[MyShmS::MakeSem] semget error." << endl;
        return false;
    }

    // cout << "[MyShmS::MakeSem] make semaphore : id = " << sem_id_ << endl;

    ctl_arg_.val = 1; // Initializes the semaphore value.

    if (semctl(sem_id_, 0, SETVAL, ctl_arg_) == -1) {
        perror("semctl");
        cerr << "[MyShmS::MakeSem] cannot set initial semaphore value." << endl;
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function locks shared memories using the semaphore.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Sem_lock() {

    return SemOp(LOCK);

}


// ------------------------------------------------ //
///
/// @brief      This function unlocks shared memories using the semaphore.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Sem_unlock() {

    return SemOp(UNLOCK);

}


// ------------------------------------------------ //
///
/// @brief      This function deals the semaphore operation.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::SemOp(const int &sem_op__) {

    struct sembuf sops[1];

    sops[0].sem_num = 0;        // semaphore no.
    sops[0].sem_op  = sem_op__; // operating, lock = -1, unlock = 1.
    sops[0].sem_flg = 0;        // flag

    // cout << "[MyShmS::SemOp] sem id = " << sem_id_ << " :: sembuf = ";
    // cout << sops[0].sem_num << " : " << sops[0].sem_op << " : " << sops[0].sem_flg << endl;

    if (semop(sem_id_, sops, 1) == -1) {
        perror("semop");
        return false;
        // exit(EXIT_FAILURE);
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads the information of shared memories fromt a file.
///             These shared memories are joined to the current ones.
///             The current semaphore is replaced to the read one.
/// @param[in]  shm_file__ : the shared memory file.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::ReadFile(const std::string &shm_file__) {

    bool result = true;

    vector<vector<string> > data;
    vector<string> delimiter = { R"(\s+)", R"(\()", R"(\))" };
    string comment_out       = "#";

    infile_.SetFile(shm_file__);
    infile_.ReadData(data, delimiter, comment_out);

    for (const auto &it : data) {

        if (it[0] == "[Shm]") {

            name_.push_back(it[1]); // name
            if (!str_.Str2i(it[2], shm_id_[it[1]])) { result = false; } // ID
            if (!str_.Str2i(it[3], shm_size_[it[1]]))   { result = false; } // total size of shared memories
            if (!str_.Str2i(it[4], shm_num_[it[1]]))    { result = false; } // the number of shared memories
            type_[it[1]] = it[5];     // type
            if (type_[it[1]] == "string") { str_size_[it[1]] = str_.Str2i(it[6]); }

            // cout << ":: " << name_.back() << " " << shm_id_[it[1]] << " " << type_[it[1]] << endl;

        }
        else if (it[0] == "[Semaphore_ID]") {

            int sem_id_new;
            if (str_.Str2i(it[1], sem_id_new)) {
                if (sem_id_new != sem_id_) {
                    if (!MyShmS::Close_sem()) {
                        cerr << "[MyShmS::ReadFile] Failed to close semaphore." << endl;
                    }
                    sem_id_ = sem_id_new;
                    // printf("[Semaphore_ID] %#x\n",sem_id_);
                }
            }
            else {
                result = false;
            }

        }

    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function write the information of shared memories fromt a file.
///             There is the same file already, the information are add to the file.
///             The space in the shared momory names is replaced to "_".
/// @param[in]  shm_file__ : the shared memory file.
/// @return     Success or not.
///
// ------------------------------------------------ //


void MyShmS::WriteFile(const std::string &shm_file__) {

    outfile_.Open(shm_file__);

    outfile_.Writef("#                           ID\n");
    outfile_.Writef("[Semaphore_ID]              %#x\n\n", sem_id_);

    outfile_.Writef("#####  NAME                 ID       BYTE N   TYPE ()\n");

    for (const auto &it : name_) {
        outfile_.Writef("[Shm]  %-20s",       str_.Replace(it, R"(\s+)", "_").c_str());
        outfile_.Writef(" %-#x %-4d %-3d %s", shm_id_[it], shm_size_[it], shm_num_[it], type_[it].c_str());
        if (type_[it] != "string") {
            outfile_.Writef(" (%d)\n", byte_size_[type_[it]]);
        }
        else {
            outfile_.Writef(" (%d)\n", str_size_[it]);
        }
    }

    outfile_.Close();

}


// ------------------------------------------------ //
///
/// @brief      This function sets whether the object closes
///             the semaphore and shared memories when the main program end.
/// @param[in]  close_at_end__ : close the semaphore & shared memories or not.
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Close_at_end(const bool &close_at_end__) {

    close_at_end_ = close_at_end__;

}


// ------------------------------------------------ //
///
/// @brief      This function closes the semaphore & shared memories.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Close() {

    return MyShmS::Close_sem();

}


// ------------------------------------------------ //
///
/// @brief      This function closes shared memories.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Close_shm() {

    bool result = true;

    // 共有メモリを削除
    for (const auto &it : name_) {
        if (shmctl(shm_id_[it], IPC_RMID, 0) == -1) {
            perror("shmctl");
            result = false;
        }
    }
    vector<string>().swap(name_);

    type_.clear();
    shm_id_.clear();
    shm_num_.clear();
    shm_size_.clear();

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function closes the semaphore and
///             shared memories are closed accordingly.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Close_sem() {

    //
    // If there are shared memories, close those beforehand.
    //

    if (!name_.empty()) {
        if (!MyShmS::Close_shm()) {
            cerr << "[MyShmS::Close_sem] Cannot remove shared memories before erasing the semaphore." << endl;
            return false;
        }
    }

    if (sem_id_ != 0) {

        if (semctl(sem_id_, 0, IPC_RMID, ctl_arg_) == -1) {
            perror("semctl");
            return false;
        }

        sem_id_ = 0;

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function initializes the semaphore & shared memories.
///             (After clearing those, the new semaphore would be created.)
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Init() {

    if (!MyShmS::Close()) {
        cerr << "[MyShmS::Init] Cannot close pre shared memories & semaphore." << endl;
        return false;
    }

    if (!MyShmS::MakeSem()) {
        cerr << "[MyShmS::Init] Cannot make new semaphore." << endl;
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function initializes shared memories.
///             The semaphore is used continuously.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Clear() {

    return MyShmS::Close_shm();

}


// ------------------------------------------------ //
///
/// @brief      This is a test function!!
///             The member variables of the semaphore &
///             shared memories are initialized.
///             The semaphore & shared memories itself are not initialized!
/// @return     Success or not.
///
// ------------------------------------------------ //


void MyShmS::Clear_shm_info() {

    vector<string>().swap(name_);

    type_.clear();
    shm_id_.clear();
    shm_num_.clear();
    shm_size_.clear();

    // sem_id_ = 0;

}


// ------------------------------------------------ //
///
/// @brief      This function gets the names of shared memories.
/// @return     The names of shared memories.
///
// ------------------------------------------------ //


std::vector<std::string> MyShmS::Name() {

    return name_;

}


// ------------------------------------------------ //
///
/// @brief      This function gets the type of the shared memory.
/// @param[in]  name__ : the shared memory name.
/// @return     The shared memory type.
///             @arg bool    : boolean type
///             @arg char    : character type
///             @arg string  : string type
///             @arg int     : integer type
///             @arg double  : real number type
///
// ------------------------------------------------ //


std::string MyShmS::Type(const std::string &name__) {

    if (!MyShmS::ExistName(name__)) {
        cout << "[MyShmS::Type] Not Exist : " << name__ << endl;
        return "";
    }

    return type_[name__];

}


// ------------------------------------------------ //
///
/// @brief      This function gets the number of the shared memory array.
/// @param[in]  name__ : the shared memory name.
/// @return     The number of the shared memory array.
///
// ------------------------------------------------ //


int MyShmS::N(const std::string &name__) {

    if (!MyShmS::ExistName(name__)) {
        return 0;
    }

    return shm_num_[name__];

}


// ------------------------------------------------ //
///
/// @brief      This function gets the string size of the shared memory.
/// @param[in]  name__ : the shared memory name.
/// @return     The string size of the shared memory.
///
// ------------------------------------------------ //


int MyShmS::StrSize(const std::string &name__) {

    if (!MyShmS::ExistName(name__)) {
        return 0;
    }

    return str_size_[name__];

}


// ------------------------------------------------ //
///
/// @brief      This function gets the byte size of the shared memory type.
/// @param[in]  name__ : the shared memory name.
/// @return     The byte size.
///
// ------------------------------------------------ //


int MyShmS::ByteSize(const std::string &name__) {

    return byte_size_[name__];

}


// ------------------------------------------------ //
///
/// @brief      This function confirms the existance of the shared memory.
/// @param[in]  name__ : the shared memory name.
/// @return     Exist or not.
///
// ------------------------------------------------ //


bool MyShmS::ExistName(const std::string &name__) {

    for (const auto &it : name_) {
        if (name__ == it) { return true; }
    }

    return false;

}


// ------------------------------------------------ //
///
/// @brief      This function checks before reading the shared memory.
/// @param[in]  name__ : the shared memory name.
/// @param[in]  type__ : the shared memory type.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Check_read(const std::string &name__, const std::string &type__) {

    if (!MyShmS::ExistName(name__)) {
        cout << "[MyShmS::Check_read] Not Exist : " << name__ << endl;
        return false;
    }

    if (type_[name__] != type__) {
        cout << "[MyShmS::Check_read] Argument type is not " + type__ + ". : current type = " << type_[name__] << endl;
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function checks before writing the shared memory.
/// @param[in]  name__ : the shared memory name.
/// @param[in]  type__ : the shared memory type.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyShmS::Check_write(const std::string &name__, const std::string &type__, const int &vec_size__, const int &shm_num__) {

    if (!MyShmS::ExistName(name__)) {

        name_.push_back(name__);

        if (shm_num__ < vec_size__) {
            // if(shm_num__ <= 0) {
            shm_num_[name__] = vec_size__;
        }
        else {
            shm_num_[name__] = shm_num__;
        }

        type_[name__] = type__;

        if (!MyShmS::ShmGet(name__)) {
            cout << "[MyShmS::Check_write] cannot Get share memory id : " << name__ << endl;
            return false;
        }

    }

    if (type_[name__] != type__) {
        cout << "[MyShmS::Check_write] Argument type is not " + type__ + ". : current type = " << type_[name__] << endl;
        return false;
    }

    if (shm_num_[name__] != vec_size__) {
        cout << "[MyShmS::Check_write] The array size do not coincide." << endl;
        return false;
    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function prints the information of shared memories.
/// @param[in]  name__ : the shared memory name.
///             If "", all information of shared memories are printed (default).
/// @return     None.
///
// ------------------------------------------------ //


std::string MyShmS::Info(const std::string &name__) {

    string info = "";

    //
    // Semaphore
    //

    info += str_.Sprintf_out("Semaphore_ID         [%-#x]\n", sem_id_);

    //
    // Shared memories
    //

    vector<char>   val_char;
    vector<string> val_string;
    vector<int>    val_int;
    vector<double> val_double;

    for (const auto &it : name_) {

        if (name__ != "") {
            if (name__ != it) {
                continue;
            }
        }

        if (Type(it) == "bool") {
            bool val_bool;
            vector<bool> vec_bool;
            if (N(it) == 1) {
                Read(it, val_bool);
                vec_bool.push_back(val_bool);
            }
            else {
                Read(it, vec_bool);
            }
            info += str_.Sprintf_out("%-20s [bool   ,%-#x] =", it.c_str(), shm_id_[it]);
            for (const auto &it2 : vec_bool) {
                if (it2) { info += " T"; }
                else     { info += " F"; }
            }
            info += "\n";
        }

        if (Type(it) == "char") {
            vector<char>().swap(val_char);
            if (N(it) == 1) {
                char val_char_0;
                Read(it, val_char_0);
                val_char.push_back(val_char_0);
            }
            else {
                Read(it, val_char);
            }
            info += str_.Sprintf_out("%-20s [char   ,%-#x] =", it.c_str(), shm_id_[it]);
            for (const auto &it2 : val_char) {
                info += str_.Sprintf_out(" %c", it2);
            }
            info += "\n";
        }

        if (Type(it) == "string") {
            vector<string>().swap(val_string);
            if (N(it) == 1) {
                string val_string_0;
                Read(it, val_string_0);
                val_string.push_back(val_string_0);
            }
            else {
                Read(it, val_string);
            }
            info += str_.Sprintf_out("%-20s [string ,%-#x] =", it.c_str(), shm_id_[it]);
            for (const auto &it2 : val_string) {
                info += " " + it2;
            }
            info += "\n";
        }

        if (Type(it) == "int") {
            vector<int>().swap(val_int);
            if (N(it) == 1) {
                int val_int_0;
                Read(it, val_int_0);
                val_int.push_back(val_int_0);
            }
            else {
                Read(it, val_int);
            }
            info += str_.Sprintf_out("%-20s [int    ,%-#x] =", it.c_str(), shm_id_[it]);
            for (const auto &it2 : val_int) {
                info += str_.Sprintf_out(" %d", it2);
            }
            info += "\n";
        }

        if (Type(it) == "double") {
            vector<double>().swap(val_double);
            if (N(it) == 1) {
                double val_double_0;
                Read(it, val_double_0);
                val_double.push_back(val_double_0);
            }
            else {
                Read(it, val_double);
            }
            info += str_.Sprintf_out("%-20s [double ,%-#x] =", it.c_str(), shm_id_[it]);
            for (const auto &it2 : val_double) {
                info += str_.Sprintf_out(" %f", it2);
            }
            info += "\n";
        }

    }

    return info;

}


// ------------------------------------------------ //
///
/// @brief      This function prints the information of shared memories.
/// @param[in]  name__ : the shared memory name.
///             If "", all information of shared memories are printed (default).
/// @return     None.
///
// ------------------------------------------------ //


void MyShmS::Print(const std::string &name__) {

    cout << MyShmS::Info(name__) << flush;

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
