//==================================================//
///
/// @file   MyFFT.cpp
/// @brief  Implementation of MyFFT.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
//==================================================//

#include "MyFFT.hpp"

#include <algorithm>

#include <chrono>

#include <cstdint>

#include <ctime>

//#include <execution>

#include <iostream>

#include <iomanip>

//#include <Eigen/Dense>

#include <complex>

#include <thread>

#include <time.h>

//

namespace MyAll {

//

using namespace std;

//using namespace Eigen;

//using fint = int_fast32_t;

//using ufint = uint_fast32_t;


//--------------------------------------------------//
///
/// @brief  First process. Initialize member variables.
///
//--------------------------------------------------//


MyFFT::MyFFT(const int &n_data__, const int &n_thread__) {

    print_ = false;

    MyFFT::Init(n_data__, n_thread__);

};


//--------------------------------------------------//
///
/// @brief   End process.
///
//--------------------------------------------------//


MyFFT::~MyFFT() {

};


//--------------------------------------------------//
///
/// @brief   Initialize calculation parameters.
/// @return  None.
///
//--------------------------------------------------//


void MyFFT::Init(const int &n_data__, const int &n_thread__) {

    if(!SetN(n_data__)) { exit(0); }

    //

    double theta = 2 * PI_ / n_;

    complex<double> Wo = complex<double> (cos(theta), -sin(theta));

    W_0_.reserve(n_half_);
    W_0_.push_back(1.0L);
    for(uint_fast32_t i=1;i<n_half_;i++) { W_0_.push_back(W_0_[i-1] * Wo); }

    //

    n_thread_ = n_thread__;

    int n_cpu = static_cast<int>(thread::hardware_concurrency());

    if(n_thread__ <= 0) {
        n_thread_ = n_cpu - 1;
        if(n_thread_ <= 0) { n_thread_ = 1; }
    }
    else if(n_thread_ > n_cpu) {
        n_thread_ = n_cpu;
    }

    if(print_) { cout << "[MyFFT] n_thread = " << n_thread_ << endl; }

    //cout << "[MyFFT] n_thread = " << n_thread_ << endl;

    //

    thread_exec_.reserve(n_thread_);
    //for(int i=0;i<n_thread_;i++) { thread_exec_.push_back(0); }
    for(int i=0;i<n_thread_;i++) { thread_exec_.push_back(-1); }

    //

    n_stream_ = static_cast<uint_fast32_t>(static_cast<double>(n_half_) / static_cast<double>(n_thread_));
    n_stream_ += 1;

    //cout << ":: " << n_ << " : " << n_thread_ << " : " << n_half_ << " : " << n_step_ << " : " << endl;

    //

    vector<vector<uint_fast32_t>>().swap(iW_);
    iW_.resize(n_step_);
    for(auto &&it:iW_) {
        //it.resize(n_);
        it.reserve(n_);
        for(uint_fast32_t i=0;i<n_;i++) { it.push_back(0); }
    }

    //iW_.resize(n_);

    //W_.reserve(n_); // cannot resize the object! (It will occur core-dump)
    //for(uint_fast32_t i=0;i<n_;i++) { W_.push_back(1.0L); }

    //

    vector<vector<uint_fast32_t>>().swap(m_);
    m_.resize(n_step_);
    for(auto &&it:m_) {
        //it.resize(n_half_);
        it.reserve(n_half_);
        for(uint_fast32_t i=0;i<n_half_;i++) { it.push_back(0); }
    }

    //m_.reserve(n_half_);
    //for(uint_fast32_t i=0;i<n_half_;i++) { m_.push_back(0); }

    n_block_half_ = n_;

    uint_fast32_t i0, sss;

    for(uint_fast32_t s=0;s<n_step_;s++) {

        n_block_ = n_block_half_;
        n_block_half_ >>= 1;

        i0 = 0;

        for(uint_fast32_t i1=0; i1<n_block_half_; i1++) {

            sss = (i1 << s);

            for(uint_fast32_t i2=i1; i2<n_; i2+=n_block_) {
                m_[s][i0] = i2;
                iW_[s][i2] = sss;
                i0++;
            }

        }

    }

    //

    i_bit_rev_.resize(n_);

    uint_fast32_t i2;

    for(uint_fast32_t i1=0;i1<n_;i1++) {

        i2 = i1;

        i2 = (((i2 & 0xaaaaaaaa) >> 1) | ((i2 & 0x55555555) << 1));
        i2 = (((i2 & 0xcccccccc) >> 2) | ((i2 & 0x33333333) << 2));
        i2 = (((i2 & 0xf0f0f0f0) >> 4) | ((i2 & 0x0f0f0f0f) << 4));
        i2 = (((i2 & 0xff00ff00) >> 8) | ((i2 & 0x00ff00ff) << 8));
        i2 = ((i2 >> 16) | (i2 << 16)) >> (32 - n_step_);

        if(i2 > i1) {
            i_bit_rev_[i1] = i2;
        }
        else {
            i_bit_rev_[i1] = i1;
            //i_bit_rev_[i1] = -1;
        }

    }

    //

    window_func_name_ = "";

    //SetWindow(""); // rectangular window function

    //

    /*
    uint_fast32_t thread_no = 0;

    vector<thread> ths( n_thread_ );

    for (auto &&th : ths) {

        th = thread(
            &MyFFT::_calc_butterfly,
            this,
            thread_no
        );

        thread_no++;

    }

    for (auto &&th : ths) {
        try {
            if(th.joinable()) {
                th.join();
            }
        } catch(const std::system_error& e) {
            cout << "Caught system_error with code " << e.code() << " meaning " << e.what() << endl;
        }
    }
    */


    //

    return;

};



//--------------------------------------------------//
///
/// @brief   Sets the number of data.
/// @return  bool.
///
//--------------------------------------------------//


bool MyFFT::SetN(const int &n_data__) {

    int n_check_pre = 0;
    int n_check_now = 1;

    for(int i=0;i<32;i++) {

        n_check_pre = n_check_now;

        n_check_now = (n_check_now << 1);

        if(n_check_now == n_data__) { break; }

        if(n_check_pre < n_data__ && n_data__ < n_check_now) {
            cerr << "[ERROR][MyFFT] The number of data is not a multiply of 2." << endl;
            return false;
        }

    }

    n_ = n_data__;

    n_half_ = (n_ >> 1);

    n_step_ = static_cast<uint_fast32_t>(log2(n_));

    //coeff_amp_ = 2.0 / n_;

    //coeff_amp_ = complex<double> (1.0 / n_, 0.0);

    return true;

}


//--------------------------------------------------//
///
/// @brief     Control standard outputs of current status.
/// @param[in] use__ : Print stdout? (yes=true, no=false)
/// @return    None.
///
//--------------------------------------------------//


void MyFFT::Print(const bool &use__){

    print_ = use__;

};


//--------------------------------------------------//
///
/// @brief   Gets frequency values.
/// @return  None.
///
//--------------------------------------------------//


std::vector<double> MyFFT::F(const double &df__, const double &offset__, const bool &wrap_back__) {

    df_ = df__;

    dt_ = 1 / (n_ * df_);

    vector<double> f;

    if(wrap_back__) { f.reserve(n_); }
    else            { f.reserve(n_half_+1); }

    for(uint_fast32_t i=0;i<=n_half_;i++) {
        f.push_back(df_ * i + offset__);
    }

    //f[n_half_] *= -1;

    if(wrap_back__) {

        for(int i=(n_half_-1);i>0;i--) {
            f.push_back(-1 * f[i]);
        }

    }

    return f;

}


//--------------------------------------------------//
///
/// @brief   Gets time values.
/// @return  None.
///
//--------------------------------------------------//


std::vector<double> MyFFT::T(const double &dt__, const double &offset__) {

    dt_ = dt__;

    df_ = 1 / (n_ * dt_);

    vector<double> t;

    t.reserve(n_);

    for(uint_fast32_t i=0;i<n_;i++) {
        t.push_back(dt_ * i + offset__);
    }

    return t;

}


//--------------------------------------------------//
///
/// @brief   Executing FFT
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::_FFT_CT_multi(const vector<double> &t__, vector<complex<double>> &f__) {

    if(n_ != static_cast<uint_fast32_t>(t__.size())) { return false; }

    chrono::system_clock::time_point start, end;
    //chrono::system_clock::time_point start1, end1;

    //struct timespec ts;
    //ts.tv_sec  = 0;
    //ts.tv_nsec = 0;

    start = chrono::system_clock::now();

    //f_ = &f__;

    vector<complex<double>> ().swap(f_);
    f_.reserve(n_);
    for(const auto &it:t__) {
        f_.push_back(complex<double> (it, 0.0));
    }

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [init val] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    //start = chrono::system_clock::now();
    cout << ":" << endl;
    vector<thread> ths( n_thread_ );

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [prep ths] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    //for (int i=0;i<n_thread_;i++) { thread_exec_[i] = 0; }

    //start = chrono::system_clock::now();

    uint_fast32_t thread_no = 0;

    thread_init_cnt_ = 0;

    for (auto &&th : ths) {

        uint_fast32_t i_begin = n_stream_ * thread_no;
        uint_fast32_t i_end   = i_begin + n_stream_;

        if(i_end > n_half_) { i_end = n_half_; }

        th = thread(
            &MyFFT::_calc_butterfly,
            this,
            thread_no,
            i_begin,
            i_end
        );

        //nanosleep(&ts, NULL);

        thread_no++;

    }

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [make ths] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    //start = chrono::system_clock::now();

    //while(thread_init_cnt_ < n_thread_) {
    //    continue;
    //}

    while(1) {
        {
            lock_guard<mutex> lock(mtx_thread_);
            //lock_guard<recursive_mutex> lock(mtx_thread_);
            if(thread_init_cnt_ == n_thread_) { break; }
        }
    }

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [wait ini] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    start = chrono::system_clock::now();

    n_block_half_ = n_;

    thread_cnt_ = 0;

    for(s_=0;s_<n_step_;s_++) {
    //for(uint_fast32_t s=1;s<=n_step_;s++) {

        n_block_ = n_block_half_;
        n_block_half_ >>= 1;

        //s_ = s;

        //

        //start1 = chrono::system_clock::now();

        /*{
            lock_guard<mutex> lock(mtx_thread_);
            //lock_guard<recursive_mutex> lock(mtx_thread_);
            n_calc_ = 0;
            for (int i=0;i<n_thread_;i++) {
                thread_exec_[i] = 1;
            }
        }*/


        {
            unique_lock<std::mutex> wait_lock(mtx_wait_);
            thread_ready_ = true;
        }

        cond_.notify_all();

        //end1 = chrono::system_clock::now();
        //cout << "[FFT_CT] [loop] [exec ths] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end1-start1).count() << endl;

        //

        /*
        start1 = chrono::system_clock::now();

        //int i_test = 0;

        for(;;) {
            {
                lock_guard<mutex> lock(mtx_thread_);
                if(thread_cnt_ >= 0) {
                    cout << ":: " << thread_cnt_ << endl;
                    break;
                }
            }
        }

        end1 = chrono::system_clock::now();
        cout << "[FFT_CT] [loop] [test] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end1-start1).count() << endl;
        */

        //

        //{
            //lock_guard<mutex> lock(mtx_thread_);
            //start1 = chrono::system_clock::now();
            //cout << "[FFT_CT] [loop] wait st = " << time_.HMS_nsec() << endl;
        //}

        int i = 0;
        //while(1) {
        for(;;) {
            {
                lock_guard<mutex> lock(mtx_thread_);
                //lock_guard<recursive_mutex> lock(mtx_thread_);
                //cout << thread_cnt_ << ":" << n_thread_ << endl;
                if(thread_cnt_ == n_thread_) { break; }
            }
            //if(thread_cnt_ >= n_thread_) {
                //cout << "wait count = " << i << endl;
            //    break;
            //}
            i++;
            //if(i==30) {cout << thread_cnt_ << endl;}
            //cout << thread_cnt_ << " " ;
            //cout << i << " " ;
            //nanosleep(&ts, NULL);
            //continue;
        }

        thread_cnt_ = 0;

        thread_ready_ = false;

        /*{
            lock_guard<mutex> lock(mtx_thread_);
            //cout << "[FFT_CT] [loop] wait et = " << time_.HMS_nsec() << endl;
            cout << "wait loop n = " << i << endl ;
            //end1 = chrono::system_clock::now();
            //cout << "[FFT_CT] [loop] [wait ths] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end1-start1).count() << endl;
        }*/

    }

    //

    {
        lock_guard<mutex> lock(mtx_thread_);
        //lock_guard<recursive_mutex> lock(mtx_thread_);
        for (int i=0;i<n_thread_;i++) {
            thread_exec_[i] = -1;
        }
    }

    //

    for (auto &&th : ths) {
        try {
            if(th.joinable()) {
                th.join();
            }
        } catch(const std::system_error& e) {
            cerr << "[FFT_CT] Caught system_error with code " << e.code() << " meaning " << e.what() << endl;
            return false;
        }
    }

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [exec fft] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //
    // Reverse bits
    //

    //start = chrono::system_clock::now();

    complex<double> tmp_swap;

    int i1 = 0;

    for(const auto &i2 : i_bit_rev_) {
        tmp_swap = f_[i1];
        f_[i1] = f_[i2];
        f_[i2] = tmp_swap;
        i1++;
    }

    /*
    uint_fast32_t i2;

    for(uint_fast32_t i1=0;i1<n_;i1++) {
        if(i_bit_rev_[i1] != -1) {
            i2 = i_bit_rev_[i1];
            tmp_swap = f_[i1];
            f_[i1] = f_[i2];
            f_[i2] = tmp_swap;
        }
    }
    */

    /*
    uint_fast32_t i2;

    for(uint_fast32_t i1=0;i1<n_;i1++) {

        i2 = i1;

        i2 = (((i2 & 0xaaaaaaaa) >> 1) | ((i2 & 0x55555555) << 1));
        i2 = (((i2 & 0xcccccccc) >> 2) | ((i2 & 0x33333333) << 2));
        i2 = (((i2 & 0xf0f0f0f0) >> 4) | ((i2 & 0x0f0f0f0f) << 4));
        i2 = (((i2 & 0xff00ff00) >> 8) | ((i2 & 0x00ff00ff) << 8));
        i2 = ((i2 >> 16) | (i2 << 16)) >> (32 - n_step_);

        if(i2 > i1) {
            tmp_swap = f_[i1];
            f_[i1] = f_[i2];
            f_[i2] = tmp_swap;
        }

    }
    */

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [bit rev.] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    //start = chrono::system_clock::now();

    f__ = f_;

    //end = chrono::system_clock::now();
    //cout << "[FFT_CT] [copy val] = " << fixed << setw(9) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    return true;


};


//--------------------------------------------------//


void MyFFT::_calc_butterfly(
    uint_fast32_t thread_no__,
    uint_fast32_t i_begin__,
    uint_fast32_t i_end__
) {

    //chrono::system_clock::time_point start, end;

    uint_fast32_t m1, m2, n_step;

    complex<double> tmp;

    //uint_fast32_t n_block_half = n_block_half_;

    {
        lock_guard<mutex> lock(mtx_thread_);
        //lock_guard<recursive_mutex> lock(mtx_thread_);
        //unique_lock<mutex> lock(mtx_thread_);
        thread_exec_[thread_no__] = 0;
        thread_init_cnt_++;
        //cout << "[_calc_butterfly] init end : " << thread_no__ << endl;
        n_step = n_step_;
    }

    //sleep(1);
    //this_thread::sleep_for(chrono::nanoseconds(1));

    //

    //int thread_exec = 0;

    //uint_fast32_t n_calc;

    uint_fast32_t n_block_half;



    for(uint_fast32_t s=0;s<n_step;s++) {

    //while(thread_exec_[thread_no__] >= 0) {
        //while(1) {

        //this_thread::sleep_for(chrono::nanoseconds(1));

        //cout << "th " << thread_no__ << " sts = " << thread_exec_[thread_no__] << endl;

        /*
        {
            lock_guard<mutex> lock(mtx_thread_);
            //lock_guard<recursive_mutex> lock(mtx_thread_);
            thread_exec = thread_exec_[thread_no__];
        }
        */

        {
            unique_lock<std::mutex> wait_lock(mtx_wait_);

            // データの準備ができるまで待機してから処理する
            cond_.wait(wait_lock, [this] { return thread_ready_; });

        //if(thread_exec_[thread_no__] == 1) {

            //start = chrono::system_clock::now();

            {
                lock_guard<mutex> lock(mtx_thread_);
                thread_exec_[thread_no__] = 0;
                n_block_half = n_block_half_;
                //cout << "[CALC] " << thread_no__ << " : st = " << time_.HMS_nsec() << endl;
            }

            //{
            //    lock_guard<mutex> lock(mtx_thread_);
            //    //lock_guard<recursive_mutex> lock(mtx_thread_);
            //    thread_exec_[thread_no__] = 0;
            //}

            for(uint_fast32_t i=i_begin__;i<i_end__;i++) {
                m1 = m_[s_][i];
                m2 = m1 + n_block_half;
                tmp = f_[m1] - f_[m2];
                f_[m1] += f_[m2];
                f_[m2] = tmp * W_0_[iW_[s_][m1]];
            }

            /*
            while(1) {

                {
                    lock_guard<mutex> lock(mtx_thread_);
                    n_calc = n_calc_;
                    n_calc_++;
                    if(n_calc_ >= n_half_) { break; }
                }

                m1 = m_[s_][n_calc];
                m2 = m1 + n_block_half;
                tmp = f_[m1] - f_[m2];
                f_[m1] += f_[m2];
                f_[m2] = tmp * W_0_[iW_[s_][m1]];

            }
            */

            //end = chrono::system_clock::now();

            {
                lock_guard<mutex> lock(mtx_thread_);
                //lock_guard<recursive_mutex> lock(mtx_thread_);
                //unique_lock<mutex> lock(mtx_thread_);
                thread_cnt_++;
                //cout << "[CALC] " << thread_no__ << " : et = " << time_.HMS_nsec() << endl;
                //cout << "[CALC] " << thread_no__ << " : dt =  " << fixed << setw(15) << right << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;
            }

            //thread_ready_ = false;

        }

        //if(thread_exec < 0) { break; }

        //this_thread::sleep_for(chrono::nanoseconds(100));

    }

    return;

};

/*
void MyFFT::_calc_butterfly(
    uint_fast32_t i_begin__,
    uint_fast32_t i_end__,
    uint_fast32_t &Nb_half__,
    vector<complex<double>> &f__,
    vector<complex<double>> &W__,
    vector<uint_fast32_t> &w__,
    vector<uint_fast32_t> &m__
) {

    uint_fast32_t i1;
    uint_fast32_t i2;
    complex<double> tmp;

    //cout << "[begin, end] = [" << i_begin__ << ", " << i_end__ << "]" << endl;

    if(calc_butterfly_) {

        for(uint_fast32_t i=i_begin__;i<i_end__;i++) {
            i1 = m__[i];
            i2 = i1 + Nb_half__;
            tmp = f__[i1] - f__[i2];
            f__[i1] += f__[i2];
            f__[i2] = tmp * W__[w__[i1]];
        }

    }


    return;

};
*/


//--------------------------------------------------//
///
/// @brief   Implementation of multi-thread FFT with a number of data.
/// @return  None.
///
//--------------------------------------------------//


void MyFFT::_calc_fft(
    const uint_fast32_t &thread_no__,
    vector<vector<complex<double>>> &x__,
    const int &n_set__,
    int &i_set__
) {

    int i_set;
    uint_fast32_t n;
    vector<int> i_bit_rev;

    {
        unique_lock<std::mutex> wait_lock(mtx_wait_);
        i_bit_rev = i_bit_rev_;
        n = n_;
    }

    while(1) {

        {
            unique_lock<std::mutex> wait_lock(mtx_wait_);

            i_set__++;
            i_set = i_set__;

            if(i_set__ >= n_set__) { break; }

            //cout << "[_calc_fft] th.no = " << thread_no__ << ", i_set = " << i_set__;
            //cout << ", size = " << x__.size() << ", " <<  x__[i_set].size() << endl;

        }

        _FFT_CT(x__[i_set], n, i_bit_rev);

        //cout << "[_calc_fft] fin " << endl;

    }

    return;

}


//--------------------------------------------------//
///
/// @brief   Executing FFT with a number of data.
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::FFT(
    const vector<complex<double>> &t__,
    vector<vector<complex<double>>> &f__,
    bool zero_padding__
) {

    int n_set = static_cast<int>(t__.size()/n_);

    if(zero_padding__) { n_set++; }

    vector<vector<complex<double>>> ().swap(f__);
    vector<complex<double>> f_part;

    f__.reserve(n_set);
    f_part.resize(n_);

    uint_fast32_t i1 = 0;

    for(const auto &it : t__) {

        f_part[i1] = it;

        i1++;

        if(i1 >= n_) {
            f__.push_back(f_part);
            i1 = 0;
        }

    }

    //cout << "size = " << f__.size() << endl;
    //cout << "part size =";
    //for(const auto &it:f__) { cout << " " << it.size() << ","; }
    //cout << endl;

    if(zero_padding__) {
        for(uint_fast32_t i2=i1;i2<n_;i2++) {
            f_part[i2] = complex<double> (0.0, 0.0);
        }
        f__.push_back(f_part);
    }

    //

    if(window_func_name_ != "") {
        for(auto &&it : f__) {
            ApplyWindow(it);
        }
    }

    //

    vector<thread> ths( n_thread_ );

    uint_fast32_t thread_no = 0;

    int i_set = -1;

    for (auto &&th : ths) {

        th = thread(
            &MyFFT::_calc_fft,
            this,
            thread_no,
            std::ref(f__),
            std::ref(n_set),
            std::ref(i_set)
        );

        thread_no++;

    }

    for (auto &&th : ths) {
        try {
            if(th.joinable()) {
                th.join();
            }
        } catch(const std::system_error& e) {
            cout << "Caught system_error with code " << e.code() << " meaning " << e.what() << endl;
        }
    }

    return true;

}


//--------------------------------------------------//


bool MyFFT::FFT(
    const vector<double> &t__,
    vector<vector<complex<double>>> &f__,
    bool zero_padding__
) {

    vector<complex<double>> t;
    t.reserve(t__.size());
    for(const auto &it:t__) {
        t.push_back(complex<double> (it, 0.0));
    }

    return FFT(t, f__);

}


//--------------------------------------------------//

/*
vector<vector<complex<double>>> MyFFT::FFT(
    const vector<double> &t__,
    bool zero_padding__
) {

    vector<complex<double>> t;
    t.reserve(t__.size());
    for(const auto &it:t__) {
        t.push_back(complex<double> (it, 0.0));
    }

    vector<vector<complex<double>>> f;

    FFT(t, f);

    return f;

}
*/


//--------------------------------------------------//


bool MyFFT::FFT(
    const vector<complex<double>> &t__,
    vector<complex<double>> &f__,
    bool zero_padding__
) {

    vector<vector<complex<double>>> f;

    FFT(t__, f);

    vector<complex<double>> ().swap(f__);
    f__.resize(n_);
    complex<double> zero(0.0, 0.0);

    for(auto &&it : f__) { it = zero; }

    for(auto &&it1 : f) {

        int i = 0;

        for(auto &&it2 : it1) {

            f__[i] += it2;
            i++;

        }

    }

    return true;

}


//--------------------------------------------------//


bool MyFFT::FFT(
    const vector<double> &t__,
    vector<complex<double>> &f__,
    bool zero_padding__
) {

    vector<complex<double>> t;
    t.reserve(t__.size());
    for(const auto &it:t__) {
        t.push_back(complex<double> (it, 0.0));
    }

    return FFT(t, f__);

}


//--------------------------------------------------//


vector<complex<double>> MyFFT::FFT(
    const vector<double> &t__,
    bool zero_padding__
) {

    vector<complex<double>> t;
    t.reserve(t__.size());
    for(const auto &it:t__) {
        t.push_back(complex<double> (it, 0.0));
    }

    vector<complex<double>> f;

    FFT(t, f);

    return f;

}


//--------------------------------------------------//
///
/// @brief   Executing FFT
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::_FFT_CT(
    vector<complex<double>> &x__,
    uint_fast32_t &n__,
    vector<int> &i_bit_rev__
) {

    double thetaT = PI_ / n__;;
    uint_fast32_t k = n__;

    uint_fast32_t n;
    complex<double> T;
    complex<double> tmp;
    complex<double> phiT = complex<double>(cos(thetaT), -sin(thetaT));

    //

    while(k > 1) {

        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = 1.0L;

        for (uint_fast32_t l = 0; l < k; l++) {

            for (uint_fast32_t a = l; a < n__; a += n) {

                uint_fast32_t b = a + k;
                tmp = x__[a] - x__[b];
                x__[a] += x__[b];
                x__[b] = tmp * T;

            }

            T *= phiT;

        }

    }

    //
    // Reverse bits
    //

    complex<double> tmp_swap;

    int i1 = 0;

    for(const auto &i2 : i_bit_rev__) {
        tmp_swap = x__[i1];
        x__[i1] = x__[i2];
        x__[i2] = tmp_swap;
        i1++;
    }

    return true;

}


//--------------------------------------------------//
///
/// @brief   Executing FFT with the specified number of data, sequentialy.
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::FFT_CT_seq(const vector<complex<double>> &t__, vector<complex<double>> &f__) {

    if(static_cast<uint_fast32_t>(t__.size()) != n_) {
        cerr << "[ERROR][MyFFT] The number of input data is not a multiply of 2." << endl;
        return false;
    }

    //

    f__ = t__;

    if(window_func_name_ != "") { ApplyWindow(f__); }

    if(_FFT_CT_seq(f__)) {

        for(auto &&it : f__) {
            it /= n_; // Correcting the amplitude.
        }

        return true;

    } else {

        return false;

    }

}


//--------------------------------------------------//


bool MyFFT::FFT_CT_seq(const vector<double> &t__, vector<complex<double>> &f__) {

    if(static_cast<uint_fast32_t>(t__.size()) != n_) {
        cerr << "[ERROR][MyFFT] The number of input data is not a multiply of 2." << endl;
        return false;
    }

    //

    vector<complex<double>> ().swap(f__);
    f__.reserve(n_);
    for(const auto &it:t__) {
        f__.push_back(complex<double> (it, 0.0));
    }

    if(window_func_name_ != "") { ApplyWindow(f__); }

    if(_FFT_CT_seq(f__)) {

        for(auto &&it : f__) {
            it /= n_; // Correcting the amplitude.
        }

        return true;

    } else {

        return false;

    }

}


//--------------------------------------------------//


vector<complex<double>> MyFFT::FFT_CT_seq(const vector<double> &t__) {

    vector<complex<double>> f;

    if(static_cast<uint_fast32_t>(t__.size()) != n_) {
        cerr << "[ERROR][MyFFT] The number of input data is not a multiply of 2." << endl;
        return f;
    }

    //

    if(FFT_CT_seq(t__, f)) {

        for(auto &&it : f) {
            it /= n_; // Correcting the amplitude.
        }

    } else {

        vector<complex<double>> ().swap(f);

    }

    return f;

}


//--------------------------------------------------//
///
/// @brief   Executing inverted FFT
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::IFFT_CT_seq(const vector<complex<double>> &f__, vector<complex<double>> &t__) {

    if(!Preparing_IFFT(f__, t__)) { return false; }

    //

    if(_FFT_CT_seq(t__)) {

        for(auto &&it : t__) { it = conj(it); }

        return true;

    } else {

        return false;

    }

}


//--------------------------------------------------//


bool MyFFT::IFFT_CT_seq(const vector<complex<double>> &f__, vector<double> &t__) {

    vector<complex<double>> t;

    if(!Preparing_IFFT(f__, t)) { return false; }

    //

    if(_FFT_CT_seq(t)) {

        for(auto &&it : t) { it = conj(it); }

    } else {

        return false;

    }

    //

    vector<double> ().swap(t__);
    t__.reserve(n_);

    for(const auto &it : t) {
        t__.push_back(it.real());
    }

    return true;

}


//--------------------------------------------------//


vector<complex<double>> MyFFT::IFFT_CT_seq(const vector<complex<double>> &f__) {

    vector<complex<double>> t;

    if(!Preparing_IFFT(f__, t)) { return t; }

    //

    if(_FFT_CT_seq(t)) {

        for(auto &&it : t) { it = conj(it); }

    } else {

        vector<complex<double>> ().swap(t);

    }

    return t;

}


//--------------------------------------------------//


bool MyFFT::Preparing_IFFT(const vector<complex<double>> &f__, vector<complex<double>> &t__) {

    if(static_cast<uint_fast32_t>(f__.size()) == n_) {

        t__ = f__;

    }
    else if(static_cast<uint_fast32_t>(f__.size()) == (n_half_ + 1)) {

        vector<complex<double>> ().swap(t__);
        t__.reserve(n_);

        for(const auto &it : f__) {
            t__.push_back(it);
        }

        for(int i=n_half_; i>0; i--) {
            t__.push_back(t__[i]);
        }

    }
    else {

        cerr << "[ERROR][MyFFT] The number of input data is irregular. : " << f__.size() << endl;
        return false;

    }

    //

    for(auto &&it : t__) { it = conj(it); }

    return true;

}


//--------------------------------------------------//
///
/// @brief   Executing FFT
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::_FFT_CT_seq(vector<complex<double>> &x__) {

    uint_fast32_t n;

    uint_fast32_t N;
    double thetaT;

    {
        unique_lock<std::mutex> wait_lock(mtx_wait_);

        N = n_;
        thetaT = PI_ / N;
    }

    uint_fast32_t k = N;

    complex<double> T;
    complex<double> tmp;
    complex<double> phiT = complex<double>(cos(thetaT), -sin(thetaT));

    //

    while(k > 1) {

        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = 1.0L;

        for (uint_fast32_t l = 0; l < k; l++) {

            for (uint_fast32_t a = l; a < N; a += n) {

                uint_fast32_t b = a + k;

                tmp = x__[a] - x__[b];
                x__[a] += x__[b];
                x__[b] = tmp * T;

            }

            T *= phiT;

        }

    }

    //
    // Reverse bits
    //

    complex<double> tmp_swap;

    int i1 = 0;

    {
        unique_lock<std::mutex> wait_lock(mtx_wait_);

        for(const auto &i2 : i_bit_rev_) {
            tmp_swap = x__[i1];
            x__[i1] = x__[i2];
            x__[i2] = tmp_swap;
            i1++;
        }
    }

    return true;

}


//--------------------------------------------------//
///
/// @brief     GSts the window function.
/// @param[in] window_func_name__ : name of window function
///            @arg "hanning"  = Hanning(Hann) window function
///            @arg "hamming"  = Hamming window function
///            @arg "blackman" = Blackman window function
///            @arg "flat-top" = Flat top window function
///            @arg "" or others = rectangular window function
/// @return    None.
///
//--------------------------------------------------//


void MyFFT::SetWindow(const string &window_func_name__, const bool &use_acf__) {

    window_func_name_ = window_func_name__;

    use_acf_ = use_acf__;

    //

    vector<double> ().swap(wf_);
    wf_.resize(n_);

    double coeff = 2 * PI_ / n_;

    double sum = 0;

    int i = 0;

    if(window_func_name_ == "hanning") {

        for(auto &&it : wf_) {
            it = 0.5 - 0.5 * cos(coeff * i);
            sum += it;
            i++;
        }
        acf_ = n_ / sum;

    }
    else if(window_func_name_ == "hamming") {

        for(auto &&it : wf_) {
            it = 0.54 - 0.46 * cos(coeff * i);
            sum += it;
            i++;
        }
        acf_ = n_ / sum;

    }
    else if(window_func_name_ == "blackman") {

        for(auto &&it : wf_) {
            it = 0.42 - 0.5 * cos(coeff * i) + 0.08 * cos(2 * coeff * i);
            sum += it;
            i++;
        }
        acf_ = n_ / sum;

    }
    else if(window_func_name_ == "flat-top") {

        for(auto &&it : wf_) {
            it   = 1 - 1.93 * cos(coeff * i);
            it  +=  1.29  * cos(2 * coeff * i);
            it  += -0.388 * cos(3 * coeff * i);
            it  +=  0.032 * cos(4 * coeff * i);
            sum += it;
            i++;
        }
        acf_ = n_ / sum;

    }
    else {

        for(auto &&it : wf_) { it = 1.0; i++; }
        acf_ = 1.0;

    }

    return;


}


//--------------------------------------------------//
///
/// @brief      Gets the window function.
/// @param[out] wf__ : the window function array
/// @return     None.
///
//--------------------------------------------------//


void MyFFT::GetWindow(std::vector<double> &wf__) {

    wf__ = wf_;

    return;

}


//--------------------------------------------------//
///
/// @brief      Applys the window function to time data.
/// @param[out] w__ : the window function array
/// @return     None.
///
//--------------------------------------------------//


void MyFFT::ApplyWindow(vector<complex<double>> &t__) {

    uint_fast32_t n = t__.size();

    if(n >= n_) {

        if(use_acf_) {
            for(uint_fast32_t i=0;i<n_;i++) { t__[i] = acf_ * wf_[i] * t__[i]; }
        } else {
            for(uint_fast32_t i=0;i<n_;i++) { t__[i] = wf_[i] * t__[i]; }
        }

        if(t__.size() > n_) {
            t__.resize(n_);
        }

    }

    return;

}


/*
//--------------------------------------------------//
///
/// @brief   Flat top window function
/// @return  None.
///
//--------------------------------------------------//


void MyFFT::GetWF_FlatTop(std::vector<double> &w__) {

    vector<double> ().swap(w__);
    w__.resize(n_);

    double coeff = 2 * PI_ / n_;

    int i = 0;

    for(auto &&it : w__) {
        it  = 1 - 1.93 * cos(coeff * i)
        it +=  1.29  * cos(2 * coeff * i)
        it += -0.388 * cos(3 * coeff * i)
        it += +0.032 * cos(4 * coeff * i)
        i++;
    }

    return;

}
*/

//--------------------------------------------------//
///
/// @brief   Converting data.
/// @return  None.
///
//--------------------------------------------------//


bool MyFFT::C2RI(const vector<complex<double>> &c__, vector<double> &c_real__, vector<double> &c_imag__) {

    int n = c__.size();

    if(n == 0 || (n%2) != 0) {
        return false;
    }

    vector<double> ().swap(c_real__);
    vector<double> ().swap(c_imag__);

    c_real__.reserve(n);
    c_imag__.reserve(n);

    for(const auto &it : c__) {
        c_real__.push_back(it.real());
        c_imag__.push_back(it.imag());
    }

    return true;

}


//--------------------------------------------------//


bool MyFFT::RI2C(const vector<double> &c_real__, const vector<double> &c_imag__, vector<complex<double>> &c__) {

    int n_real = c_real__.size();
    int n_imag = c_imag__.size();

    if(n_real == 0 || n_real != n_imag) {
        return false;
    }

    vector<complex<double>> ().swap(c__);

    c__.reserve(n_real);

    for(int i=0;i<n_real;i++) {
        c__.push_back(complex<double> (c_real__[i], c_imag__[i]));
    }

    return true;

}


//--------------------------------------------------//


bool MyFFT::C2AP(const vector<complex<double>> &c__, vector<double> &c_amp__, vector<double> &c_phas__) {

    int n = c__.size();
    int n_half = n / 2;

    if(n == 0 || (n%2) != 0) {
        return false;
    }

    vector<double> ().swap(c_amp__);
    vector<double> ().swap(c_phas__);

    c_amp__.reserve(n_half + 1);
    c_phas__.reserve(n_half + 1);

    for(int i=0;i<=n_half;i++) {
        c_amp__.push_back(abs(c__[i]));
        c_phas__.push_back(arg(c__[i]));
    }

    for(int i=0;i<n_half;i++) {
        c_amp__[n_half-i] += abs(c__[n_half+i]);
    }

    return true;

}


//--------------------------------------------------//


bool MyFFT::AP2C(const vector<double> &c_amp__, const vector<double> &c_phas__, vector<complex<double>> &c__) {

    int n_amp  = c_amp__.size();
    int n_phas = c_phas__.size();

    if(n_amp == 0 || n_amp != n_phas) {
        return false;
    }

    int n = (n_amp -1) * 2;

    vector<complex<double>> ().swap(c__);

    c__.reserve(n);

    c__.push_back(complex<double> (c_amp__[0]*cos(c_phas__[0]), c_amp__[0]*sin(c_phas__[0])));

    for(int i=1;i<n_amp;i++) {
        c__.push_back(complex<double> (0.5 * c_amp__[i]*cos(c_phas__[i]), 0.5 * c_amp__[i]*sin(c_phas__[i])));
    }

    //cout << "00" << endl;

    for(int i=n_amp-2;i>0;i--) {
        c__.push_back(conj(c__[i]));
    }

    //cout << "01 : " << c__.size() << endl;

    return true;

}


//--------------------------------------------------//


};
