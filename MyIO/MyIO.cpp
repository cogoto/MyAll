// ================================================ //
///
/// @file   MyIO.cpp
/// @brief  Implementation of MyI and MyO.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyIO.hpp"

//

#include <cstdint>

#include <cstdio>

#include <cstdarg>

#include <cstring>

//

#include <complex>

#include <iostream>

//

namespace MyAll {

//

using namespace std;

// using uint = uint_fast32_t;
// using uint = unsigned int;


// ------------------------------------------------ //
///
/// @brief  First process.
/// @param[in] filename__ : a file you wanna read.
///
// ------------------------------------------------ //

MyI::MyI(const std::string &filename__) {

    MyI::SetFile(filename__);

    MyI::Init();

}


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyI::MyI() {

    MyI::Init();

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyI::~MyI() {

}


// ------------------------------------------------ //
///
/// @brief   This function initializes the member variables.
/// @return  None.
///
// ------------------------------------------------ //


void MyI::Init() {

    lock_guard<recursive_mutex> lock(mtx_);

    n_column_ = 0;
    // new_delimiter_ = " ";

    method_ = "fgets";
    use_comment_out_ = true;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the reading file.
/// @param[in] filename__ : a file you wanna read.
/// @return    None.
///
// ------------------------------------------------ //


void MyI::SetFile(const std::string &filename__) {

    lock_guard<recursive_mutex> lock(mtx_);

    filename_ = filename__;

}


// ------------------------------------------------ //
///
/// @brief     This function checks the existance of the file.
/// @param[in] filename__ : a file you wanna know the existance.
/// @return    exist or not.
///
// ------------------------------------------------ //


bool MyI::CheckExist(const std::string &filename__) {

    fstream f_file;
    f_file.open(filename__, ios_base::in);

    bool existance = f_file.is_open();

    f_file.close();

    return existance;

}


// ------------------------------------------------ //
///
/// @brief      This function reads data as a 2D string array from the file.
/// @param[out] data__ : 2D array data read from the file.
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @param[in]  use_blank__ : If you wanna adopt blank components, set this "true".
///             The default value is true.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadData(
    std::vector<std::vector<std::string> > &data__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__,
    const bool &use_blank__
) {

    vector<vector<string> >().swap(data__);

    lock_guard<recursive_mutex> lock(mtx_);

    //
    // read file
    //

    if (method_ == "fread") {
        if (!MyI::ReadFile_fread()) { return false; }
    }
    else if (method_ == "fstream") {
        if (!MyI::ReadFile_fread()) { return false; }
    }
    else { // fgets or other words. I recommend this method.
        if (!MyI::ReadFile_fgets()) { return false; }
    }

    //
    // remove comment out
    //

    if (use_comment_out_) {
        MyI::RemoveComment(comment_out__);
    }

    //
    // convert data in lines
    //

    data__.reserve(n_column_);

    if (use_blank__) {

        for (const auto &it : line_) {

            vector<string> column;

            str_.Split(column, it, delimiter__);

            if (column.size() <= 0) { continue; }

            data__.push_back(column);

        }

    }
    else {

        int i = 0;

        for (const auto &it1 : line_) {

            vector<string> column;

            str_.Split(column, it1, delimiter__);

            int N_col = static_cast<int>(column.size());

            data__.push_back(vector<string>());

            data__[i].reserve(N_col);

            for (const auto &it2 : column) {

                if (it2.size() != 0) {

                    data__[i].push_back(it2);

                }

            }

            i++;

        }

    }

    vector<string>().swap(line_);

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads file data with fget().
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadFile_fgets() {

    if (filename_.empty()) { return false; } // no setting input file.

    FILE *f_ptr = fopen(filename_.c_str(), "r");

    if (f_ptr == NULL) { return false; }

    char buffer[N_READ_];
    memset(buffer, '\0', sizeof( buffer ));

    vector<string>().swap(line_);

    vector<string> br { R"(\n)", R"(\r)" };

    while (fgets(buffer, N_READ_, f_ptr) != NULL) {

        // char* p = strchr(buffer, '\n');
        // if (p) { *p = '\0'; }

        line_.push_back(buffer);

        str_.Replace_directly(line_.back(), br, "");

    }

    fclose(f_ptr);

    n_column_ = line_.size();

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads file data with fread().
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadFile_fread() {

    if (filename_.empty()) { return false; } // no setting input file.

    FILE *f_ptr = fopen(filename_.c_str(), "r");

    if (f_ptr == NULL) { return false; }

    char buffer[N_READ_];
    memset(buffer, '\0', sizeof( buffer ));

    vector<string> str_buffer;

    int size      = N_READ_ - 1;
    int size_read = size;

    while (size >= size_read) {

        size = fread(buffer, sizeof(*buffer), size_read, f_ptr);

        if (size <= 0) { continue; }

        buffer[size] = '\0';

        str_buffer.push_back(buffer);

    }

    fclose(f_ptr);

    //
    // organize in order by lines.
    //

    vector<string>().swap(line_);
    vector<string> line_delimiter = { R"(\n)", R"(\r)" };

    string butt = "";

    for (const auto &it : str_buffer) {

        vector<string> line_tmp;

        str_.Split(line_tmp, butt + it, line_delimiter);

        if (line_tmp.size() == 1) {
            butt += it;
        }
        else if (line_tmp.size() > 1) {
            line_.insert(line_.end(), line_tmp.begin(), line_tmp.end() - 1);
            butt = line_tmp.back();
        }
        else {
            butt = "";
        }

    }

    if (butt != "") { line_.push_back(butt); }

    vector<string>().swap(str_buffer);

    n_column_ = line_.size();

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads file data with fstream().
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadFile_fstream() {

    if (filename_.empty()) { return false; } // no setting input file.

    vector<string>().swap(line_);

    fstream reading_file;

    ios::sync_with_stdio(false);

    reading_file.open(filename_, ios::in);

    if (!reading_file.is_open()) { return false; }

    string buffer;

    vector<string> br { R"(\n)", R"(\r)" };

    while (!reading_file.eof()) {

        buffer.clear();

        getline(reading_file, buffer);

        str_.Replace_directly(buffer, br, "");

        line_.push_back(buffer);

    }

    reading_file.close();

    n_column_ = line_.size();

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function removes comment-out lines.
/// @return     None.
///
// ------------------------------------------------ //


void MyI::RemoveComment(const std::string &comment_out__) {

    if (comment_out__.empty()) { return; }

    if (line_.size() <= 0) { return; }

    for (auto &&it : line_) {

        if (it.empty()) { continue; }

        it = str_.Split(it, comment_out__)[0];

    }

    int i = 0;

    for (const auto &it : line_) {

        if (it.empty()) { continue; }

        if (!str_.Match(it, R"(^\s+$)")) {
            line_[i] = it;
            i++;
        }

    }

    line_.erase(line_.begin() + i, line_.end());

}


// ------------------------------------------------ //
///
/// @brief      This function selects methods of the reading file (fgets/fread).
/// @return     None.
///
// ------------------------------------------------ //


void MyI::SelectReadMethod(const std::string &method__) {

    method_ = method__;

    if (method_ == "fread") {

        cout << "[MyI::SelectReadMethod] select fread() for reading file" << endl;
        cout << " > This method read the file of larger row lengths slower than fgets()." << endl;
        cout << " > However, No limit of a row length." << endl;

    }
    else if (method_ == "fstream") {

        cout << "[MyI::SelectReadMethod] select fstream() for reading file" << endl;
        cout << " > This method is slower in large files." << endl;
        cout << " > Take care!!" << endl;

    }
    else {

        cout << "[MyI::SelectReadMethod] select fgets() for reading file" << endl;
        cout << " > This method read the file of larger row lengths faster." << endl;
        cout << " > However, there is the limit of a row length < " << N_READ_ << "byte (N_READ_)" << endl;
        cout << " > Take care!!" << endl;

    }

}


// ------------------------------------------------ //
///
/// @brief      This function reads data as a 2D real-number array from the file.
/// @param[out] data__ : 2D array data read from the file.
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @param[in]  use_blank__ : If you wanna adopt blank components, set this "true".
///             The default value is true.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadDouble(
    std::vector<std::vector<double> > &data__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__,
    const bool &use_blank__
) {

    vector<vector<double> >().swap(data__);

    vector<vector<string> > str_data;

    if (!MyI::ReadData(str_data, delimiter__, comment_out__, use_blank__)) {
        // cerr << "[MyI::ReadDouble] cannot read the file." << endl;
        return false;
    }

    // bool result = true;

    {

        lock_guard<recursive_mutex> lock(mtx_);

        if (!str_.VecStr2d(str_data, data__)) {
            cerr << "[WARN][MyI::ReadDouble] cannot convert any data to the real-numbers." << endl;
            // result = false;
        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads data as a 2D complex-number array from the file.
/// @param[out] data__ : 2D array data read from the file.
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)", R"(\()", R"(\))"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @param[in]  use_blank__ : If you wanna adopt blank components, set this "true".
///             The default value is true.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadComplexDouble(
    std::vector<std::vector<std::complex<double> > > &data__,
    const std::string &complex_delimiter__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__,
    const bool &use_blank__
) {

    // vector<vector<complex<double> > >().swap(data__);

    vector<vector<string> > str_data;

    if (!MyI::ReadData(str_data, delimiter__, comment_out__, use_blank__)) {
        return false;
    }

    cout << "[ReadComplexDouble] exp. matrix size (" << str_data.size() << "," << str_data[0].size() << ")" << endl;
    cout << "[ReadComplexDouble] convert to complex double." << endl;

    bool result = true;

    {

        lock_guard<recursive_mutex> lock(mtx_);

        data__.resize(str_data.size());

        int i_row = 0;

        for (const auto &it1 : str_data) {

            data__[i_row].resize(it1.size());

            int i_col = 0;

            for (const auto &it2 : it1) {

                vector<string> str_data_splt = str_.Split(it2, complex_delimiter__);

                if (str_data_splt.size() == 2) {
                    data__[i_row][i_col] = complex<double>(str_.Str2d(str_data_splt[0], false), str_.Str2d(str_data_splt[1], false));
                }
                else {
                    cerr << "[MyI::ReadComplexDouble] Irregular format : " << it2 << " : [" << i_row << "," << i_col << "] -> set (0.0, 0.0i)" << endl;
                    data__[i_row][i_col] = complex<double>(0.0, 0.0);
                    result = false;
                }

                i_col++;

            }

            i_row++;

        }

    }

    return result;

}


// ------------------------------------------------ //
///
/// @brief      This function reads parameters as a map.
///             The map data have a format like "map[keyword] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg keyword ... column 1 (parameter names)
///             @arg value   ... column 2
/// @param[out] prm__ : The string map
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadParam(
    std::map<std::string, std::string> &prm__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__
) {

    map<string, string>().swap(prm__);

    vector<string>().swap(key_param_);

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) {
        return false;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); }  // if head data is blank, remove it.

        if (it.size() >= 2) {
            prm__[it[0]] = it[1];
            key_param_.push_back(it[0]);
        }
        else if (it.size() == 1) {
            prm__[it[0]] = "";
            key_param_.push_back(it[0]);
        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads parameters as a map.
///             The map data has a format like "map[keyword][no] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg keyword ... column 1 (parameter names)
///             @arg no      ... the number of the value.
///             @arg value   ... from column 2 to the end column.
///             The data would be like "map[column 1][0] = colmun 2", "map[column 1][1] = colmun 3"...
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadParam(
    std::map<std::string, std::map<int, std::string> > &prm__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__
) {

    map<string, map<int, string> >().swap(prm__);

    vector<string>().swap(key_param_);

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) {
        return false;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); }  // if head data is blank, remove it.

        int it_size = static_cast<int>( it.size());

        if (it_size >= 2) {

            for (int j = 1; j < it_size; j++) {
                prm__[it[0]][j - 1] = it[j];
            }

            key_param_.push_back(it[0]);

        }
        else if (it_size == 1) {

            prm__[it[0]][0] = "";

            key_param_.push_back(it[0]);

        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads parameters as a map.
///             The map data has a format like "map[keyword] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg keyword ... column 1 (parameter names)
///             @arg value   ... a combined string from column 2 to the end column with the new delimiter.
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @param[in]  new_delimiter__ : New delimiter strings to combine data.
///             The default value is " ".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadParam_all(
    std::map<std::string, std::string> &prm__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__,
    const std::string &new_delimiter__
) {

    map<string, string>().swap(prm__);

    vector<string>().swap(key_param_);

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) { return false; }

    lock_guard<recursive_mutex> lock(mtx_);

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); }  // if head data is blank, remove it.

        int it_size = static_cast<int>( it.size());

        if (it_size >= 2) {

            prm__[it[0]] = it[1];

            for (int j = 2; j < it_size; j++) {
                prm__[it[0]] += new_delimiter__ +  it[j];
            }

            key_param_.push_back(it[0]);

        }
        else if (it_size == 1) {

            prm__[it[0]] = "";

            key_param_.push_back(it[0]);

        }

    }


    /*
       for (uint i = 0; i < data.size(); i++) {

        if( data[i][0] == "" ) { data[i].erase( data[i].begin() ); }; // if head data is blank, remove it.

        if( data[i].size() == 1 ) {
            prm__[data[i][0]] = "";
            continue;
        };

        string str = "";
        for (uint j = 1; j < data[i].size(); j++) {
            if (j > 1) { str = str + new_delimiter_; }
            str = str + data[i][j];
        }

        prm__[data[i][0]] = str;

        key_param_.push_back(data[i][0]);

       }
     */

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads parameters as a map.
///             The map data has a format like "map[keyword] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg keyword ... column 1 (parameter names)
///             @arg value   ... data of column 2 converted to a real number.
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadParam(
    std::map<std::string, double> &prm__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__
) {

    map<string, double>().swap(prm__);

    vector<string>().swap(key_param_);

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) { return false; }

    lock_guard<recursive_mutex> lock(mtx_);

    // vector<vector<double> > data2;
    // str_.VecStr2d( data, data2 );

    int i = 0;

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); }  // if head data is blank, remove it.

        int it_size = static_cast<int>( it.size());

        if (it_size >= 2) {

            str_.Str2d(it[1], prm__[it[0]]);
            // prm__[it[0]] = str_.Str2d(it[1], false);

            key_param_.push_back(it[0]);

        }
        else if (it_size == 1) {

            prm__[it[0]] = 0;

            key_param_.push_back(it[0]);

        }

        i++;

    }

    return true;

}




// ------------------------------------------------ //
///
/// @brief      This function reads parameters as a map.
///             The map data has a format like "map[keyword][no] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg keyword ... column 1 (parameter names)
///             @arg value   ... data of column 2-END converted to a real number.
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadParam(
    map<std::string, std::vector<double> > &prm__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__
) {

    map<std::string, std::vector<double> >().swap(prm__);

    vector<string>().swap(key_param_);

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) {
        return false;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); }  // if head data is blank, remove it.

        int it_size = static_cast<int>( it.size());

        vector<double> it2 = {};

        if (it_size >= 2) {

            for (int j = 1; j < it_size; j++) {
                it2.push_back( str_.Str2d(it[j], false) );
            }

            prm__[it[0]] = it2;

            key_param_.push_back(it[0]);

        }
        else if (it_size == 1) {

            prm__[it[0]] = vector<double> {};

            key_param_.push_back(it[0]);

        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads parameters with sections as a map.
///             The map data has a format like "map[section][keyword] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg section ... section names.
///             @arg keyword ... colmun 1 (parameter names)
///             @arg value   ... a combined string from column 2 to the end column with the new delimiter.
/// @param[in]  section_head__ : The header string of section lines
///             The default value is "[".
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {R"(\s+)"}.
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @param[in]  new_delimiter__ : New delimiter strings to combine data.
///             The default value is " ".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadSection(
    std::map<std::string, std::map<std::string, std::string> > &prm__,
    const std::string &section_head__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__,
    const std::string &new_delimiter__
) {

    map<string, map<string, string> >().swap(prm__);

    vector<string>().swap(section_);

    map<string, map<int, string> >().swap(key_section_);

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) { return false; }

    int key_no =  0;

    lock_guard<recursive_mutex> lock(mtx_);

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); }  // if head data is blank, remove it.

        if (MyI::CheckSectionLine(it[0], section_head__)) {
            key_no = 0;
            continue;
        }

        if (section_.size() == 0) { continue; }

        int it_size = static_cast<int>( it.size());

        if (it_size >= 2) {

            prm__[section_.back()][it[0]] = it[1];
            for (int j = 2; j < it_size; j++) {
                prm__[section_.back()][it[0]] += new_delimiter__ +  it[j];
            }

            key_section_[section_.back()][key_no] = it[0];
            key_no++;

        }
        else if (it_size == 1) {

            prm__[section_.back()][it[0]] = "";

            key_section_[section_.back()][key_no] = it[0];
            key_no++;

        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function reads parameters with sections as a map.
///             The map data has a format like "map[section][no] = value".
/// @param[out] prm__ : The parameters read from the file.
///             These components consist as below,
///             @arg section ... section names.
///             @arg no      ... the line number in the section.
///             @arg value   ... a string of the line
/// @param[in]  section_head__ : The header string of section lines
///             The default value is "[".
/// @param[in]  delimiter__ : Delimiter strings.
///             The default value is {} (uses the line data as is).
/// @param[in]  comment_out__ : Comment out string of the line header.
///             The default value is "#".
/// @param[in]  new_delimiter__ : New delimiter strings to combine data.
///             The default value is " ".
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::ReadSection(
    std::map<std::string, std::map<int, std::string> > &prm__,
    const std::string &section_head__,
    const std::vector<std::string> &delimiter__,
    const std::string &comment_out__,
    const std::string &new_delimiter__
) {

    map<string, map<int, string> >().swap(prm__);

    vector<string>().swap(section_);

    map<string, map<int, string> >().swap(key_section_); // key_section_ is blank when using this function.

    vector<vector<string> > data;

    if (!ReadData(data, delimiter__, comment_out__)) { return false; }

    int key_no = 0;

    lock_guard<recursive_mutex> lock(mtx_);

    for (auto &&it : data) {

        if (it[0] == "") { it.erase(it.begin()); } // if head data is blank, remove it.

        if (MyI::CheckSectionLine(it[0], section_head__)) {
            key_no = 0;
            continue;
        }

        if (section_.size() == 0) { continue; }

        int it_size = static_cast<int>( it.size());

        if (it_size > 0) {

            prm__[section_.back()][key_no] = it[0];
            for (int j = 1; j < it_size; j++) {
                prm__[section_.back()][key_no] += new_delimiter__ +  it[j];
            }

            key_no++;

        }

    }

    return true;

}


// ------------------------------------------------ //
///
/// @brief     This function checks the section lines.
///            For example, section lines are represented as below,
///            @li [section] @ section_head__ = "["
///            @li (section) @ section_head__ = "("
///            @li {section} @ section_head__ = "{"
///            @li <section> @ section_head__ = "<"
///            @li ##section @ section_head__ = "##"
/// @param[in] line__ : The line string.
/// @param[in] section_head__ : A header string to decide whether the line is the section's one or not.
///            See the above brief.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyI::CheckSectionLine(const std::string &line__, const std::string &section_head__) {

    string line = line__;

    str_.Replace_directly(line, R"(\n)",   "");
    str_.Replace_directly(line, R"(\r)",   "");

    str_.Replace_directly(line, R"(^\s+)", "");
    str_.Replace_directly(line, R"(\s+$)", "");

    if (line.find(section_head__) == 0) {

        if (section_head__ == "[") {
            str_.Replace_directly(line, "^\\[", "");
            str_.Replace_directly(line, "\\]$", "");
        }
        else if (section_head__ == "(") {
            str_.Replace_directly(line, "^\\(", "");
            str_.Replace_directly(line, "\\)$", "");
        }
        else if (section_head__ == "{") {
            str_.Replace_directly(line, "^\\{", "");
            str_.Replace_directly(line, "\\}$", "");
        }
        else if (section_head__ == "<") {
            str_.Replace_directly(line, "^\\<", "");
            str_.Replace_directly(line, "\\>$", "");
        }
        else {
            str_.Replace_directly(line, section_head__, "");
        }

        section_.push_back(line);

        // cout << "section = " << section_.back() << ", " << line << endl;

        return true;

    }

    return false;

}


// ------------------------------------------------ //
///
/// @brief      This function Gets the all section names.
/// @param[out] section__ : The section names.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::GetSection(std::vector<std::string> &section__) {

    lock_guard<recursive_mutex> lock(mtx_);

    if (section_.size() > 0) {
        section__ = section_;
        return true;
    }

    return false;

}


// ------------------------------------------------ //
///
/// @brief      This function Gets all parameter names
///             that were read with ReadParam() or ReadParam_all().
/// @param[out] key__ : The parameter names.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::GetKey(std::vector<std::string> &key__) {

    lock_guard<recursive_mutex> lock(mtx_);

    if (key_param_.size() <= 0) { return false; }

    vector<string>().swap(key__);

    key__ = key_param_;

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function Gets all the parameter names
///             that were read with ReadSection().
/// @param[out] key__ : The parameter names with the sections.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::GetKey(std::map<std::string, std::map<int, std::string> > &key__) {

    lock_guard<recursive_mutex> lock(mtx_);

    if (key_section_.size() <= 0) { return false; }

    map<string, map<int, string> >().swap(key__);

    key__ = key_section_;

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function Gets all the section & parameter names
///             that were read with ReadSection().
/// @param[out] section__ : The section names.
/// @param[out] key__     : The parameter names with the sections.
/// @return     Success or not.
///
// ------------------------------------------------ //


bool MyI::GetSectionKey(
    std::vector<std::string> &section__,
    std::map<std::string, std::map<int, std::string> > &key__
) {

    if (!MyI::GetSection(section__)) { return false; }

    if (!MyI::GetKey(key__)) { return false; }

    return true;

}


// ------------------------------------------------ //
///
/// @brief      This function Gets the number of columns.
/// @return     The number of columns.
///
// ------------------------------------------------ //


int MyI::N() {

    lock_guard<recursive_mutex> lock(mtx_);

    return n_column_;

}


/*
   //--------------------------------------------------//
   //
   //    @brief : Check the file existance
   //
   //  @return : true = exist, false = no file
   //
   //--------------------------------------------------//

   bool MyI::CheckExistFile(){

    struct stat st;

    int exist_stat = stat( filename_.c_str(), &st );

   if (0 == exist_stat) {
        return true;
   }

    return false;

   };
 */


// ================================================ //


// ------------------------------------------------ //
///
/// @brief  First process.
/// @param[in] filename__ : a file you wanna output.
///
// ------------------------------------------------ //

MyO::MyO(const std::string &filename__) {

    lock_guard<recursive_mutex> lock(mtx_);

    open_ = false;

    MyO::SetMaxSize();

    MyO::Open(filename__);

}


// ------------------------------------------------ //
///
/// @brief  First process.
///
// ------------------------------------------------ //


MyO::MyO() {

    lock_guard<recursive_mutex> lock(mtx_);

    open_     = false;

    filename_ = "";

    MyO::SetMaxSize();

}


// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyO::~MyO() {

    MyO::Close();

}


// ------------------------------------------------ //
///
/// @brief     This function opens the new writing file.
/// @param[in] filename__ : A file you wanna output the data.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Open(const std::string &filename__) {

    if (!MyO::Close()) {
        cerr << "[ERROR][MyO::Open] Cannot close before the file opening." << endl;
        return open_;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    filename_ = filename__;

    if (filename_.size() <= 0) {
        cerr << "[ERROR][MyO::Open] Not found output file name." << endl;
        return open_;
    }

    writing_file_.open(filename_, ios::out);

    if (writing_file_.is_open()) { open_ = true; }

    return open_;

}


// ------------------------------------------------ //
///
/// @brief     This function re-opens the existing file and add the data.
///            A new file could be also specified.
/// @param[in] filename__ : A file you wanna output the data.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::ReOpen(const std::string &filename__) {

    if (!MyO::Close()) {
        cerr << "[ERROR][MyO::Open] Cannot close before the file opening." << endl;
        return open_;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    filename_ = filename__;

    if (filename_.size() <= 0) {
        cerr << "[ERROR][MyO::ReOpen] Not found output file name." << endl;
        return open_;
    }

    writing_file_.open(filename_, ios::out | ios::app);

    if (writing_file_.is_open()) { open_ = true; }

    return open_;

}


// ------------------------------------------------ //
///
/// @brief     This function opens the file which was used previously and add the data.
/// @return    Success or not.
///
// ------------------------------------------------ //

bool MyO::ReOpen() {

    if (!MyO::Close()) {
        cerr << "[ERROR][MyO::Open] Cannot close before the file opening." << endl;
        return open_;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    if (filename_.size() <= 0) {
        cout << "[MyO::ReOpen] Not found output file name." << endl;
        return open_;
    }

    writing_file_.open(filename_, ios::out | ios::app);

    if (writing_file_.is_open()) { open_ = true; }

    return open_;

}


// ------------------------------------------------ //
///
/// @brief     This function closes the writing file.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Close() {

    lock_guard<recursive_mutex> lock(mtx_);

    if (writing_file_.is_open()) {
        writing_file_.close();
    }

    if (writing_file_.is_open()) {
        open_ = true;
    }
    else {
        open_ = false;
    }

    return !open_;

}


// ------------------------------------------------ //
///
/// @brief     This function sets the blank file.
/// @param[in] filename__ : A blank file.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Blank(const std::string &filename__) {

    if (!MyO::Close()) {
        cerr << "[ERROR][MyO::Open] Cannot close before the file opening." << endl;
        return open_;
    }

    lock_guard<recursive_mutex> lock(mtx_);

    filename_ = filename__;

    if (filename_.size() <= 0) {
        cerr << "[ERROR][MyO::Blank] Not found output file name." << endl;
        return open_;

    }

    writing_file_.open(filename_, ios::out | ios::trunc);

    if (writing_file_.is_open()) { open_ = true; }

    MyO::Write("");

    return MyO::Close();

}



// ------------------------------------------------ //
///
/// @brief     This function checks the existance of the file.
/// @param[in] filename__ : A file you wanna know the existance.
/// @return    exist or not.
///
// ------------------------------------------------ //


bool MyO::CheckExist(const std::string &filename__) {

    fstream f_file;
    f_file.open(filename__, ios_base::in);

    bool existance = f_file.is_open();

    f_file.close();

    return existance;

}


// ------------------------------------------------ //
///
/// @brief     This function writes string data to the file without line feeds.
///            If you need a line feed in the tail, please set it manually.
/// @param[in] data__ : Writing data string.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Write(const std::string &data__) {

    lock_guard<recursive_mutex> lock(mtx_);

    writing_file_ << data__ << flush;

    return !writing_file_.fail();

}


// ------------------------------------------------ //
///
/// @brief     This function writes string data array to the file.
///            Note that a line feed is added in the tail.
/// @param[in] data__ : Data strings.
/// @param[in] delimiter__ : A delimiter inserted between strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Write(const std::vector<std::string> &data__, const std::string &delimiter__) {

    lock_guard<recursive_mutex> lock(mtx_);

    string str = "";

    // int data_size = static_cast<int>( data__.size() );

    // for (int i = 0; i < data_size; i++) {
    //    if (i > 0) { str = str + delimiter__; }
    //    str = str + data__[i];
    // }

    for (const auto &it : data__) {
        str += it + delimiter__;
    }

    str = str.substr(0, str.size() - delimiter__.size());

    writing_file_ << str << endl;

    return !writing_file_.fail();

}


// ------------------------------------------------ //
///
/// @brief     This function writes string data array to the file.
///            Note that a line feed is added in the tail of rows.
/// @param[in] data__ : Data strings.
/// @param[in] delimiter__ : A delimiter inserted between strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Write(
    const std::vector<std::vector<std::string> > &data__,
    const std::string &delimiter__
) {

    lock_guard<recursive_mutex> lock(mtx_);

    /*
       int data_size_1 = static_cast<int>( data__.size() );

       for (int i1 = 0; i1 < data_size_1; i1++) {

        string str = "";

        int data_size_2 = static_cast<int>( data__[i1].size() );

        for (int i2 = 0; i2 < data_size_2; i2++) {
            if (i2 > 0) { str = str + delimiter__; }
            str = str + data__[i1][i2];
        }

        writing_file_ << str << endl;

       }
     */

    string str = "";

    for (const auto &it1 : data__) {

        for (const auto &it2 : it1) {

            str += it2 + delimiter__;

        }

        str = str.substr(0, str.size() - delimiter__.size()) + '\n';

    }

    writing_file_ << str << endl;

    return !writing_file_.fail();

}


// ------------------------------------------------ //
///
/// @brief     This function writes formated string data to the file.
/// @param[in] format__ : Format expressions.
/// @param[in] ... : A series of data.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Writef(const char* format__, ...) {

    lock_guard<recursive_mutex> lock(mtx_);

    // const uint N_MAX = 8192;

    va_list arg;
    char    str_c[n_max_];

    va_start(arg, format__);
    vsprintf(str_c, format__, arg);
    va_end(arg);

    string str = str_c;

    if (static_cast<int>(str.size()) > n_max_) {
        cout << "[MyO::Writef] ERROR : FLOWING SIZE (>" << n_max_ << ")" << endl;
    }

    writing_file_ << str << flush;

    return !writing_file_.fail();

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) string data to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            If you need a line feed in the tail, please set it manually.
/// @param[in] filename__ : The writing file name.
/// @param[in] data__ : Data strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Out(const std::string &filename__, const std::string &data__) {

    bool result = true;

    lock_guard<recursive_mutex> lock(mtx_);

    if (filename__.size() > 0) {
        if (!MyO::ReOpen(filename__)) { result = false; }
    }
    else {
        if (!MyO::ReOpen()) { result = false; }
    }

    if (!MyO::Write(data__)) { result = false; }

    if (!MyO::Close()) { result = false; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) string data to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            In this case, the previous file is selected.
///            If you need a line feed in the tail, please set it manually.
/// @param[in] data__ : Data strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Out(const std::string &data__) {

    return MyO::Out("", data__);

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) string array to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            Note that a line feed would be added in the tail.
/// @param[in] filename__  : The writing file name.
/// @param[in] data__      : Data strings.
/// @param[in] delimiter__ : A delimiter inserted between strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Out(
    const std::string &filename__,
    const std::vector<std::string> &data__,
    const std::string &delimiter__
) {

    bool result = true;

    lock_guard<recursive_mutex> lock(mtx_);

    if (filename__.size() > 0) {
        if (!MyO::ReOpen(filename__)) { result = false; }
    }
    else {
        if (!MyO::ReOpen()) { result = false; }
    }

    if (!MyO::Write(data__, delimiter__)) { result = false; }

    if (!MyO::Close()) { result = false; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) string array to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            In this case, the previous file is selected.
///            Note that a line feed would be added in the tail.
/// @param[in] data__      : Data strings.
/// @param[in] delimiter__ : A delimiter inserted between strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Out(const std::vector<std::string> &data__, const std::string &delimiter__) {

    return MyO::Out("", data__, delimiter__);

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) string array to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            Note that a line feed would be added in the tail of rows.
/// @param[in] filename__  : The writing file name.
/// @param[in] data__      : Data strings.
/// @param[in] delimiter__ : A delimiter inserted between strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Out(
    const std::string &filename__,
    const std::vector<std::vector<std::string> > &data__,
    const std::string &delimiter__
) {

    bool result = true;

    lock_guard<recursive_mutex> lock(mtx_);

    if (filename__.size() > 0) {
        if (!MyO::ReOpen(filename__)) { result = false; }
    }
    else {
        if (!MyO::ReOpen()) { result = false; }
    }

    if (!MyO::Write(data__, delimiter__)) { result = false; }

    if (!MyO::Close()) { result = false; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) string array to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            In this case, the previous file is selected.
///            Note that a line feed would be added in the tail of rows.
/// @param[in] data__      : Data strings.
/// @param[in] delimiter__ : A delimiter inserted between strings.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Out(
    const std::vector<std::vector<std::string> > &data__,
    const std::string &delimiter__
) {

    return MyO::Out("", data__, delimiter__);

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) formated string to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            If you need a line feed in the tail, please set it manually.
/// @param[in] filename__  : The writing file name.
/// @param[in] format__    : Format expressions.
/// @param[in] ...         : A series of data.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Outf(const std::string &filename__, const char* format__, ...) {

    bool result = true;

    lock_guard<recursive_mutex> lock(mtx_);

    if (filename__.size() > 0) {
        if (!MyO::ReOpen(filename__)) { result = false; }
    }
    else {
        if (!MyO::ReOpen()) { result = false; }
    }

    va_list arg;
    char    str_c[n_max_];

    va_start(arg, format__);
    vsprintf(str_c, format__, arg);
    va_end(arg);

    string str = str_c;

    if (static_cast<int>(str.size()) > n_max_) {
        cout << "[MyO::Outf] ERROR : FLOWING SIZE (>" << n_max_ << ")" << endl;
    }

    writing_file_ << str << flush;

    if (writing_file_.fail()) { result = false; }

    if (!MyO::Close()) { result = false; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function writes (adds) formated string to the file instantly.
///            Internally, this operates the file opening, writing, and closing.
///            In this case, the previous file is selected.
///            If you need a line feed in the tail, please set it manually.
/// @param[in] format__    : Format expressions.
/// @param[in] ...         : A series of data.
/// @return    Success or not.
///
// ------------------------------------------------ //


bool MyO::Outf(const char* format__, ...) {

    bool result = true;

    lock_guard<recursive_mutex> lock(mtx_);

    if (!MyO::ReOpen()) { result = false; }

    va_list arg;
    char    str_c[n_max_];

    va_start(arg, format__);
    vsprintf(str_c, format__, arg);
    va_end(arg);

    string str = str_c;

    if (static_cast<int>(str.size()) > n_max_) {
        cout << "[MyO::Outf] ERROR : FLOWING SIZE (>" << n_max_ << ")" << endl;
    }

    writing_file_ << str << flush;

    if (writing_file_.fail()) { result = false; }

    if (!MyO::Close()) { result = false; }

    return result;

}


// ------------------------------------------------ //
///
/// @brief     This function sets a maximum output size of characters at once.
/// @param[in] n_max__    : The maximum size of characters.
///            The default value is 8192 Byte.
/// @return    None.
///
// ------------------------------------------------ //


void MyO::SetMaxSize(const int &n_max__) {

    lock_guard<recursive_mutex> lock(mtx_);

    if (n_max__ >= 0) {
        n_max_ = n_max__;
    }
    else {
        cout << "[MyO::SetMaxSize] Irregular size -> set to 8192" << endl;
        n_max_ = 8192;
    }

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
