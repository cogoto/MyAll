// ================================================ //
///
/// @file   MyIO.hpp
/// @brief  Class libraly of MyI and MyO.
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

#ifndef _MYALL_MYIO_HPP_
#define _MYALL_MYIO_HPP_

//

#include <complex>

#include <fstream>

#include <map>

#include <mutex>

#include <string>

#include <vector>

//

#include "MyString.hpp"

//

namespace MyAll {

// ================================================ //
///
/// @class   MyI
/// @brief   My class for reading files.
/// @details This class object reads the data from files.
/// @date    2017/05/23 ver. 1.1
///          2019/12/24 ver. 1.2
///
// ================================================ //

class MyI {

 public:

    MyI(const std::string &filename__); ///< Constractor :

    MyI();                              ///< Constractor :

    ~MyI();                             ///< Destractor :

    ///
    /// @name  Inputs the reading file.
    /// @{

    void SetFile(const std::string &filename__); ///< Sets the reading file. :

    /// @}
    /// @name  Checking the file existance.
    /// @{

    bool CheckExist(const std::string &filename__); ///< Checks the file existance. :

    /// @}
    /// @name  Reading data.
    /// @{

    bool ReadData(
        std::vector<std::vector<std::string> > &data__,
        const std::vector<std::string> &delimiter__ = {},
        const std::string &comment_out__ = "#",
        const bool &use_blank__ = true
    );  ///< Reads data as 2D string array. :

    //

    bool ReadDouble(
        std::vector<std::vector<double> > &data__,
        const std::vector<std::string> &delimiter__ = {},
        const std::string &comment_out__ = "#",
        const bool &use_blank__ = true
    ); ///< Reads data as 2D real-number array. :

    //

    bool ReadComplexDouble(
        std::vector<std::vector<std::complex<double> > > &data__,
        const std::string &complex_delimiter__      = ",",
        const std::vector<std::string> &delimiter__ = { R"(\s+)", R"(\()", R"(\))" },
        const std::string &comment_out__ = "#",
        const bool &use_blank__ = false
    ); ///< Reads data as 2D complex number array.

    //

    /// @}
    /// @name  Reading data as the parameter map.
    /// @{

    bool ReadParam(
        std::map<std::string, std::string> &prm__,
        const std::vector<std::string> &delimiter__ = { R"(\s+)" },
        const std::string &comment_out__ = "#"
    ); ///< Reads string data as a parameter map. (key = column 1, data= column 2) :

    //

    bool ReadParam(
        std::map<std::string, std::map<int, std::string> > &prm__,
        const std::vector<std::string> &delimiter__ = { R"(\s+)" },
        const std::string &comment_out__ = "#"
    ); ///< Reads string data as a parameter map. (key = column 1, data= column 2 ~ end) :

    //

    bool ReadParam_all(
        std::map<std::string, std::string> &prm__,
        const std::vector<std::string> &delimiter__ = { R"(\s+)" },
        const std::string &comment_out__   = "#",
        const std::string &new_delimiter__ = " "
    ); ///< Reads string data as a parameter map. (key = column 1, data= combined column 2 to end) :

    //

    bool ReadParam(
        std::map<std::string, double> &prm__,
        const std::vector<std::string> &delimiter__ = { R"(\s+)" },
        const std::string &comment_out__ = "#"
    ); ///< Reads real number data as a parameter map. (key = column 1, data= column 2 as real numbers) :

    //

    bool ReadParam(
        std::map<std::string, std::vector<double> > &prm__,
        const std::vector<std::string> &delimiter__ = { R"(\s+)" },
        const std::string &comment_out__ = "#"
    ); ///< Reads real number data as a parameter map. (key = column 1, data= column 2 to end) :

    /// @}
    /// @name  Reading data as the parameter map with sections.
    /// @{

    bool ReadSection(
        std::map<std::string, std::map<std::string, std::string> > &prm__,
        const std::string &section_head__  = "[",
        const std::vector<std::string> &delimiter__ = { R"(\s+)" },
        const std::string &comment_out__   = "#",
        const std::string &new_delimiter__ = " "
    ); ///< Reads string data as a parameter map with sections. (key = section name & column 1, data= combined column 2 to end) :

    //

    bool ReadSection(
        std::map<std::string, std::map<int, std::string> > &prm__,
        const std::string &section_head__  = "[",
        const std::vector<std::string> &delimiter__ = {},
        const std::string &comment_out__   = "#",
        const std::string &new_delimiter__ = " "
    ); ///< Reads string data as a parameter map with sections. (key = section name & line no., data= line) :

    /// @}
    /// @name  Gets keywords.
    /// @{

    bool GetSection(std::vector<std::string> &section__); ///< Gets the section names. :

    //

    bool GetKey(std::vector<std::string> &key__); ///< Gets the keywords resulted from ReadParam() & ReadParam_all(). :

    //

    bool GetKey(
        std::map<std::string,
                 std::map<int, std::string> > &key__
    ); ///< Gets the keywords resulted from ReadSection(). :

    //

    bool GetSectionKey(
        std::vector<std::string> &section__,
        std::map<std::string, std::map<int, std::string> > &key__
    ); ///< Gets the section names & keywords resulted from ReadSection(). :

    /// @}
    /// @name  Information.
    /// @{

    int N(); ///< The number of columns. :

    /// @}
    /// @name  Control.
    /// @{

    void SelectReadMethod(const std::string &method__ = "fgets"); ///< Sets the reading method [fgets/fread/fstream]. :

    //

    void UseCommentOut(const bool &use_comment_out__ = true); ///< Sets whether to use comment out. :

    /// @}

 private:

    ///
    /// @name  Initialization.
    /// @{

    void Init(); ///< Initializes members. :

    /// @}
    /// @name  Reading with each method.
    /// @{

    bool ReadFile_fgets(); ///< For fast reading of larger rows (* however, only use when the row length is under N_READ_). :

    //

    bool ReadFile_fread(); ///< For fast reading of smaller rows. :

    //

    bool ReadFile_fstream(); ///< Not recommend! (poor performance in large files). :

    /// @}
    /// @name  Tools.
    /// @{

    void RemoveComment(const std::string &comment_out__ = ""); /// Removes comment outs from data. :

    //

    bool CheckSectionLine(
        const std::string &line__,
        const std::string &section_head__
    ); /// Checks whether the line is a section line. :

    /// @}

    // -------------------------------------------- //

    ///
    /// @name  Parameters.
    /// @{

    const int N_READ_ = 1048576; ///< The readable string size at one time (1 MB).

    int n_column_;               ///< The number of columns of the input file.

    //

    std::string filename_;     ///< The reading file name.

    std::string method_;       ///< The reading method.

    bool use_comment_out_;     ///< Uses comment out or not.

    std::recursive_mutex mtx_; ///< mutex for reading.

    // std::string new_delimiter_; ///< The delimiter string used when the column data is combined. default is a space.

    //

    std::vector<std::string> section_;                               ///< Section names.

    std::vector<std::string> key_param_;                             ///< Keywords.

    std::map<std::string, std::map<int, std::string> > key_section_; ///< Keywords with sections.

    std::vector<std::string> line_;                                  ///< Line data.

    /// @}
    /// @name  Tool objects.
    /// @{

    MyAll::MyString str_; /// For string operation.

    /// @}

};


// ================================================ //
///
/// @class   MyO
/// @brief   My class for writing files.
/// @details This class object writes data to the file.
/// @date    2017/05/23 ver. 1.1
///          2019/12/24 ver. 1.2
///
// ================================================ //

class MyO {

 public:

    MyO(const std::string &filename__); ///< Constractor :

    MyO();                              ///< Constractor :

    ~MyO();                             ///< Destractor :

    ///
    /// @name  File open & close.
    /// @{

    bool Open(const std::string &filename__);   ///< File open

    bool ReOpen(const std::string &filename__); ///< File open or re-open

    bool ReOpen();                              ///< File re-open

    bool Close();                               ///< File close

    bool Blank(const std::string &filename__);  ///< Set a blank file.

    /// @}
    /// @name  Checking the file existance.
    /// @{

    bool CheckExist(const std::string &filename__); ///< Checks the file existance. :

    /// @}
    /// @name  Writing to the file.
    /// @{

    bool Write(const std::string &data__); ///< Writes a string (without LF). :

    bool Write(
        const std::vector<std::string> &data__,
        const std::string &delimiter__ = " "
    ); ///< Writes string array components as a line (with LF). :

    bool Write(
        const std::vector<std::vector<std::string> > &data__,
        const std::string &delimiter__ = " "
    ); ///< Writes a 2D string array. :

    //

    bool Writef(const char* format__, ...); ///< Writes a formated string (without LF) :

    /// @}
    /// @name  Flushing data to the file.
    /// @{

    bool Out(const std::string &filename__, const std::string &data__); ///< Flushs a string to the file :

    bool Out(const std::string &data__);                                ///< Flushs a string to the registerd file :

    //

    bool Out(
        const std::string &filename__,
        const std::vector<std::string> &data__,
        const std::string &delimiter__ = " "
    ); ///< Flushs a string array as a line to the file :

    bool Out(
        const std::vector<std::string> &data__,
        const std::string &delimiter__ = " "
    );  ///< Flushs a string array as a line to the registerd file :

    //

    bool Out(
        const std::string &filename__,
        const std::vector<std::vector<std::string> > &data__,
        const std::string &delimiter__ = " "
    ); ///< Flushs a 2D string array to the file. :

    bool Out(
        const std::vector<std::vector<std::string> > &data__,
        const std::string &delimiter__ = " "
    ); ///< Flushs a 2D string array to the registerd file. :

    //

    bool Outf(const std::string &filename__, const char* format__, ...); ///< Flushs a formated string to the file :

    bool Outf(const char* format__, ...);                                ///< Flushs a formated string to the registerd file :

    /// @}
    /// @name  The maximum writing size of data at one time.
    /// @{

    void SetMaxSize(const int &n_max__ = 8192); ///< Sets the maximum writing size of data at one time. :

    /// @}

 private:

    ///
    /// @name  Parameters
    /// @{

    std::string filename_;      ///< The writing file name.

    std::fstream writing_file_; ///< The file stream for writing.

    std::recursive_mutex mtx_;  ///< The mutex for waiting to write.

    bool open_;                 ///< Opened a file or not.

    int n_max_;                 ///< The number of characters for formatted outputs.

    // const int N_MAX = 8192; // The number of char for formatted outputs

    /// @}

};

} // MyAll

#endif ///_MYALL_MYIO_HPP_
