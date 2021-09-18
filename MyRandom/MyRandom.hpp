// ================================================ //
///
/// @file   MyRandom.hpp
/// @brief  Class libraly of MyRandom.
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

#ifndef _MYALL_MYRANDOM_HPP_
#define _MYALL_MYRANDOM_HPP_

//

#include <random>

#include <string>

#include <vector>

//

namespace MyAll {

// ================================================ //
///
/// @class   MyRandom
/// @brief   My class for dealing random numbers.
/// @details This class object could create the random number array, currently.
/// @date    2020/01/01 ver. 0.5
///
// ================================================ //

class MyRandom {

 public:

    MyRandom();  ///< Constractor :

    MyRandom(
        const std::string &seed_type__, // "device", "seed"
        const int &seed_val__
    ); ///<  :

    ~MyRandom(); ///< Destractor :

    ///
    /// @name  Sets the random seed.
    /// @{

    void Seed(
        const std::string &seed_type__ = "device", // "seed", "time", and "device"
        const int &seed_val__ = -1
    ); ///<  :

    //

    void GetSeed(std::string &seed_type__, int &seed_val__); ///<  :

    /// @}
    /// @name  Gets vectors.
    /// @{

    bool MakeVector_uni(
        const int &rand_min__,
        const int &rand_max__,
        const int &size__,
        std::vector<int> &result__,
        const bool &overlap = false
    ); ///<  :

    bool MakeVector_uni_nooverlap(
        const int &rand_min__,
        const int &rand_max__,
        const int &size__,
        std::vector<int> &result__
    ); ///<  :

    void MakeVector_uni_overlap(
        const int &rand_min__,
        const int &rand_max__,
        const int &size__,
        std::vector<int> &result__
    ); ///<  :

    /// @}

    void MakeSeqVector(const int &rand_min__, const int &rand_max__); ///<  :

    void FY_shuffle(std::vector<int> &data__, const int &size__ = -1); ///< Fisher-Yates shuffle :

    void CombSort(std::vector<int> &data__); ///<  :

    void SortUniqueMethod(const int &size__, const int &rand_min__, const int &rand_max__, std::vector<int> &result__);

 private:

    ///
    /// @name methods
    /// @{


    /// @}

    std::mt19937 mt_engine_;

    std::string seed_type_;

    int seed_val_;

    std::vector<int> seq_vector_;

};

}  // MyAll

// =======================================//


#endif /// _MYALL_MYRANDOM_HPP_
