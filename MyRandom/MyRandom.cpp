// ================================================ //
///
/// @file   MyRandom.cpp
/// @brief  Implementation of MyRandom.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
// ================================================ //

#include "MyRandom.hpp"

#include <ctime>

#include <iostream>

#include <unordered_map>

#include <algorithm>

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


MyRandom::MyRandom() {

    MyRandom::Seed();

}


// ------------------------------------------------ //
///
/// @brief  First process.  @n
///         Initialize member variables.
///
// ------------------------------------------------ //


MyRandom::MyRandom(const std::string &seed_type__, const int &seed_val__) {

    MyRandom::Seed(seed_type__, seed_val__);

}

// ------------------------------------------------ //
///
/// @brief  End process.
///
// ------------------------------------------------ //


MyRandom::~MyRandom() {

}


// ------------------------------------------------ //
///
/// @brief     This function sets the seed of random numbers.
/// @param[in] seed_type__ : "seed" ... a number, "time" ... creates seed from time.
/// @param[in] seed_val__ : if seed_type__ = "seed", set the seed value.
/// @return    None.
///
// ------------------------------------------------ //


void MyRandom::Seed(const std::string &seed_type__, const int &seed_val__) {

    if (seed_type__ == "seed") {
        seed_type_ = seed_type__;
        seed_val_  = seed_val__;
    }
    else if (seed_type__ == "time") {
        seed_type_ = seed_type__;
        seed_val_  = static_cast<int>(time(nullptr));
    }
    else {
        random_device rnd;
        seed_type_ = "device";
        seed_val_  = rnd();
    }

    mt_engine_.seed(seed_val_);

}


// ------------------------------------------------ //
///
/// @brief     This function gets the random seed number.
/// @param[out] seed_type__ : seed type("seed" or "time")
/// @param[out] seed_val__ : the seed value.
/// @return    None.
///
// ------------------------------------------------ //


void MyRandom::GetSeed(std::string &seed_type__, int &seed_val__) {

    seed_type__ = seed_type_;
    seed_val__  = seed_val_;

}


// ------------------------------------------------ //
//
//
//
// ------------------------------------------------ //

/*
template<typename T> void combSort(vector<T>& v){
  size_t h = (v.size()*10)/13;
  bool is_sorted = false;
  while(!is_sorted){
    if(h==1)is_sorted = true;
    for(size_t i=0; i<v.size()-h; ++i){
      if(v[i]>v[i+h]){
        swap(v[i],v[i+h]);
        if(is_sorted)is_sorted = false;
      }
    }
    if(h>1) h = (h*10)/13;
    if(h==0) h = 1;
  }
}
*/

// ------------------------------------------------ //
//
//
//
// ------------------------------------------------ //

/*
// functional の greater<int> を通常は使う
struct stl_sort_functor_cmp {
    bool operator()(const int& a, const int&b) const { return b > a; }
};

void do_stl_sort_functor(vector<int>& v) {
    sort(v.begin(), v.end(), stl_sort_functor_cmp());
}

typedef void (*SortFunc)(vector<int>&);
void benchmark(const char *name, SortFunc func, const int size) {
    vector<int> v(size);
    for (vector<int>::iterator i = v.begin(); i != v.end(); ++i)
        *i = rand();
    double start_time = clock();
    func(v);
    double elapsed = clock() - start_time;
    printf("%s: %.2f\n", name, elapsed / CLOCKS_PER_SEC);
}
*/

// ------------------------------------------------ //
///
/// @brief   This function creates an array of uniform random numbers.
/// @param[in] rand_min__ : the minimum value of the array.
/// @param[in] rand_max__ : the maximum value of the array.
/// @param[in] size__ : array size.
/// @param[out] result__ : an array of uniform random numbers.
/// @param[in] overlap : exists overlapped numbers or not.
/// @return  Success or not.
///
// ------------------------------------------------ //


bool MyRandom::MakeVector_uni(
    const int &rand_min__,
    const int &rand_max__,
    const int &size__,
    vector<int> &result__,
    const bool &overlap
) {

    if(!overlap) {

        return MyRandom::MakeVector_uni_nooverlap(rand_min__, rand_max__, size__, result__);

    } else {

        MyRandom::MakeVector_uni_overlap(rand_min__, rand_max__, size__, result__);

        return true;

    }/// @brief   This function creates an array of uniform random numbers.
    /// @param[in] rand_min__ : the minimum value of the array.
    /// @param[in] rand_max__ : the maximum value of the array.
    /// @param[in] size__ : array size.
    /// @param[out] result__ : an array of uniform random numbers.
    /// @param[in] overlap : exists overlapped numbers or not.
    /// @return  Success or not.


}


// ------------------------------------------------ //
///
/// @brief   This function creates an array of uniform random values(non-overlapped).
/// @param[in] rand_min__ : the minimum value of the array.
/// @param[in] rand_max__ : the maximum value of the array.
/// @param[in] size__ : array size.
/// @param[out] result__ : an array of uniform random numbers.
/// @return  Success or not.
///
// ------------------------------------------------ //


bool MyRandom::MakeVector_uni_nooverlap(
    const int &rand_min__,
    const int &rand_max__,
    const int &size__,
    vector<int> &result__
) {

    if((rand_max__-rand_min__+1) < size__) { return false; }

    //MyRandom::SortUniqueMethod(size__, rand_min__, rand_max__, result__);

    MyRandom::MakeSeqVector(rand_min__, rand_max__);

    result__ = seq_vector_;

    MyRandom::FY_shuffle(result__, size__);

    //cout << "[result__] ";
    //for(const auto &it:result__) { cout << it << " "; }
    //cout << endl;

    //sort(result__.begin(), result__.end()); // fast

    //MyRandom::CombSort(result__); // slow

    return true;

}


// ------------------------------------------------ //
///
/// @brief   This function creates an array of uniform random values(allowing overlapped ones).
/// @param[in] rand_min__ : the minimum value of the array.
/// @param[in] rand_max__ : the maximum value of the array.
/// @param[in] size__ : array size.
/// @param[out] result__ : an array of uniform random numbers.
/// @return  Success or not.
///
// ------------------------------------------------ //


void MyRandom::MakeVector_uni_overlap(
    const int &rand_min__,
    const int &rand_max__,
    const int &size__,
    vector<int> &result__
) {

    vector<int> ().swap(result__);
    result__.resize(size__);

    uniform_int_distribution<int> uni_dist(rand_min__, rand_max__);

    for(auto &&it:result__) {
        it = uni_dist(mt_engine_);
    }

    return;

}


// ------------------------------------------------ //
///
/// @brief   This function creates a sequencial array(seq_vector_) which is the member value.
/// @param[in] rand_min__ : the minimum value of the array.
/// @param[in] rand_max__ : the maximum value of the array.
/// @return  None.
///
// ------------------------------------------------ //


void MyRandom::MakeSeqVector(const int &rand_min__, const int &rand_max__){

    int rand_min = rand_min__;
    int rand_max = rand_max__;

    if(rand_min__ > rand_max__) {
        swap(rand_min, rand_max);
    }

    int rand_range = rand_max - rand_min + 1;

    if(static_cast<int>(seq_vector_.size()) == rand_range) {
        if(seq_vector_[0] == rand_min) {
            if(seq_vector_.back() == rand_max) {
                return;
            }
        }
    }

    vector<int> ().swap(seq_vector_);

    seq_vector_.resize(rand_range);

    int i = rand_min;
    for(auto &&it:seq_vector_) {
        it = i;
        i++;
    }

    return;

}


// ------------------------------------------------ //
///
/// @brief   This function executes Fisher-Yates shuffle.
/// @param[out] data__ : data to sort.
/// @param[in] size__ : maximum size of the data.
/// @return  None.
///
// ------------------------------------------------ //


void MyRandom::FY_shuffle(vector<int> &data__, const int &size__){

    int data_size = static_cast<int>(data__.size());

    int size_0 = data_size;
    if(size__ > 0 || size__ < data_size) {
        size_0 = size__;
    }

    int data_size_uni = data_size - 1;

    for(int i=0; i<size_0; i++) {

        int j = uniform_int_distribution<int>(i, data_size_uni)(mt_engine_);

        //int tmp = data__[i];
        //data__[i] = data__[j];
        //data__[j] = tmp;
        swap(data__[i], data__[j]);

    }

    data__.resize(size_0);

    return;

}


// ------------------------------------------------ //
///
/// @brief   This function executes comb-sort.
/// @param[out] data__ : data to sort.
/// @return  None.
///
// ------------------------------------------------ //


void MyRandom::CombSort(vector<int> &data__) {

    int h = static_cast<int>(data__.size() / 1.3);
    int data_size = static_cast<int>(data__.size());

    bool is_sorted = false;

    while(!is_sorted){

        if(h==1) { is_sorted = true; }

        int m = data_size - h;

        for(int i=0; i<m; i++){
            if(data__[i] > data__[i+h]){
                swap(data__[i], data__[i+h]);
                is_sorted = false;
            }
        }

        if(h >  1) { h = static_cast<int>(h / 1.3); }
        if(h == 0) { h = 1; }

    }

    return;

}


// ------------------------------------------------ //
///
/// @brief   This function creates a uniform random array, then sorting it, and removing the overlapped value.
/// @param[in] size__ : array size.
/// @param[in] rand_min__ : the minimum value of the array.
/// @param[in] rand_max__ : the maximum value of the array.
/// @param[out] result__ : output array.
/// @return  None.
///
// ------------------------------------------------ //


void MyRandom::SortUniqueMethod(const int &size__, const int &rand_min__, const int &rand_max__, vector<int> &result__){

    vector<int> ().swap(result__);
    result__.reserve(size__);

    //auto engine = create_rand_engine();
    uniform_int_distribution<int> uni_dist(rand_min__, rand_max__);

    const int make_size = static_cast<int>(size__*1.2);

    //std::uniform_int_distribution<int>(rand_min__, rand_min__)(mt_engine_)

    while(static_cast<int>(result__.size()) < size__){

        while(static_cast<int>(result__.size()) < make_size) {
            result__.push_back(uni_dist(mt_engine_));
        }

        std::sort(result__.begin(), result__.end());
        auto unique_end = std::unique(result__.begin(), result__.end());

        if(size__ < std::distance(result__.begin(), unique_end)){
            unique_end = std::next(result__.begin(), size__);
        }
        result__.erase(unique_end, result__.end());

    }


    return;

}


// ------------------------------------------------ //


}  // MyAll


// ================================================ //
