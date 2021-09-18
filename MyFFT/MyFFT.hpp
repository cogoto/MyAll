//========================================//
///
/// @file   MyFFT.hpp
/// @brief  class libraly of MyFFT.
/// @author cogoto
///
/// [MyAll]
/// Copyright (c) 2021 cogoto
/// Released under the MIT license
/// https://opensource.org/licenses/mit-license.php
///
//========================================//

#ifndef _MYALL_MYFFT_HPP_
#define _MYALL_MYFFT_HPP_

//

#include <cstdint>

#include <complex>

#include <condition_variable>

#include <vector>

#include <mutex>

//#include "MyChronos.hpp"

//#include "Eigen/Core"

//

namespace MyAll {

//========================================//
///
/// @class   MyFFT
/// @brief   Fast Fourier Transform with multi-threading.
/// @details Calcurates FFT using multi-threads with multi-core CPUs.
/// @date    Dec. 2020
///
//========================================//

class MyFFT {

public:

    MyFFT(const int &n_data__, const int &n_thread__ = 0); ///< Constractor :

    ~MyFFT(); ///< Destractor :

    ///
    /// @name  Execute FFT.
    /// @{

    bool FFT(); ///< :

    bool IFFT(); ///< :

    /// @}
    /// @name  Preparing regressions.
    /// @{

    void Init(const int &n_data__, const int &n_thread__ = 0); ///< Init parameters :

    bool SetN(const int &n_data__); ///< Sets the number of data :

    void Print(const bool &use__=true); ///< Print status? :

    //bool SetData_t(const std::vector<double> &t__); ///< Sets data :

    //bool SetData_f(const std::vector<std::complex<double>> &complex__); ///< Set data :

    //bool SetData_f(const std::vector<double> &f__, const std::vector<double> &phas__ = {}); ///< Set data :

    /// @}
    /// @name  gets freq- or time-axis values.
    /// @{

    std::vector<double> F(const double &df__, const double &offset__ = 0, const bool &wrap_back__ = true); ///< :

    std::vector<double> T(const double &dt__, const double &offset__ = 0); ///< :

    /// @}
    /// @name  Gets each FFT result of large amount of data.
    /// @{

    bool FFT(const std::vector<std::complex<double>> &t__, std::vector<std::vector<std::complex<double>>> &f__, bool zero_padding__ = false); ///< :

    bool FFT(const std::vector<double> &t__, std::vector<std::vector<std::complex<double>>> &f__, bool zero_padding__ = false); ///< :

    //std::vector<std::vector<std::complex<double>>> FFT(const std::vector<double> &t__, bool zero_padding__ = false); ///< :

    /// @}
    /// @name  Gets integlated FFT result of large amount of data.
    /// @{

    bool FFT(const std::vector<std::complex<double>> &t__, std::vector<std::complex<double>> &f__, bool zero_padding__ = false); ///< :

    bool FFT(const std::vector<double> &t__, std::vector<std::complex<double>> &f__, bool zero_padding__ = false); ///< :

    std::vector<std::complex<double>> FFT(const std::vector<double> &t__, bool zero_padding__ = false); ///< :

    /// @}
    /// @name  Execute FFT with multi-thread calculation in a process.
    /// @{

    bool _FFT_CT_multi(const std::vector<double> &t__, std::vector<std::complex<double>> &f__); ///< Cooley & Tukey FFT method:

    /// @}
    /// @name  Execute FFT sequentialy with Cooley & Tukey method.
    /// @{

    bool FFT_CT_seq(const std::vector<std::complex<double>> &t__, std::vector<std::complex<double>> &f__); ///< :

    bool FFT_CT_seq(const std::vector<double> &t__, std::vector<std::complex<double>> &f__); ///< :

    std::vector<std::complex<double>> FFT_CT_seq(const std::vector<double> &t__); ///< :

    //

    bool IFFT_CT_seq(const std::vector<std::complex<double>> &f__, std::vector<std::complex<double>> &t__); ///< :

    bool IFFT_CT_seq(const std::vector<std::complex<double>> &f__, std::vector<double> &t__); ///< :

    std::vector<std::complex<double>> IFFT_CT_seq(const std::vector<std::complex<double>> &f__); ///< :

    /// @}
    /// @name  Makes window functions.
    /// @{

    void SetWindow(const std::string &window_func_name__, const bool &use_acf__ = true); ///< :

    void GetWindow(std::vector<double> &wf__); ///< :

    void ApplyWindow(std::vector<std::complex<double>> &t__); ///< :

    //void ResetWindow(std::vector<std::complex<double>> &t__); ///< :

    //void GetWF_Hanning(std::vector<double> &w__); ///< :

    //void GetWF_Hamming(std::vector<double> &w__); ///< :

    //void GetWF_Blackman(std::vector<double> &w__); ///< :

    //void GetWF_FlatTop(std::vector<double> &w__); ///< :

    /// @}
    /// @name  Converts data.
    /// @{

    bool C2RI(const std::vector<std::complex<double>> &c__, std::vector<double> &c_real__, std::vector<double> &c_imag__);

    bool RI2C(const std::vector<double> &c_real__, const std::vector<double> &c_imag__, std::vector<std::complex<double>> &c__);

    bool C2AP(const std::vector<std::complex<double>> &c__, std::vector<double> &c_amp__, std::vector<double> &c_phas__);

    bool AP2C(const std::vector<double> &c_amp__, const std::vector<double> &c_phas__, std::vector<std::complex<double>> &c__);

    /// @}
    /// @name  Gets results.
    /// @{

    //Eigen::VectorXd Get() const; ///< Gets results :

    //std::vector<double> GetVector() const; ///< Gets results :

    /// @}

private:

    ///
    /// @name  Core functions of FFT.
    /// @{

    void _calc_butterfly(
        uint_fast32_t thread_no__,
        uint_fast32_t i_begin__,
        uint_fast32_t i_end__
    ); ///< :

    void _calc_fft(
        const uint_fast32_t &thread_no__,
        std::vector<std::vector<std::complex<double>>> &x__,
        const int &n_set__,
        int &i_set__
    ); ///< :

    bool _FFT_CT(
        std::vector<std::complex<double>> &x__,
        uint_fast32_t &n__,
        std::vector<int> &i_bit_rev__
    ); ///< :

    bool _FFT_CT_seq(std::vector<std::complex<double>> &x__); ///< :

    /*
    void _calc_butterfly(
        uint_fast32_t i_begin__,
        uint_fast32_t i_end__,
        uint_fast32_t &Nb_half__,
        std::vector<std::complex<double>> &f__,
        std::vector<std::complex<double>> &W__,
        std::vector<uint_fast32_t> &w__,
        std::vector<uint_fast32_t> &m__
    );
    */

    bool Preparing_IFFT(const std::vector<std::complex<double>> &f__, std::vector<std::complex<double>> &t__);

    /// @}
    /// @name Convert matrix data.
    /// @{

    //std::vector<double> _vecXd2vector(Eigen::VectorXd x__) const; ///< Convert data :

    //std::vector<std::vector<double>> _matXd2vector(Eigen::MatrixXd x__) const; ///< Convert data :

    //Eigen::VectorXd _vector2vecXd(std::vector<double> x__) const; ///< Convert data :

    //Eigen::MatrixXd _vector2matXd(std::vector<std::vector<double>> x__) const; ///< Convert data :

    /// @}

    //========//

    ///
    /// @name  Control parameters.
    /// @{

    int n_thread_;

    std::mutex mtx_thread_;    // for command wait of server

    //std::recursive_mutex mtx_thread_;    // for command wait of server

    static constexpr double PI_ = 3.1415926535897932384626433832795;

    //static constexpr double TWO_PI = 6.2831853071795864769252867665580;

    uint_fast32_t n_;

    uint_fast32_t n_half_;

    uint_fast32_t n_step_;

    uint_fast32_t n_stream_; ///< the number of calcuration size in a thread.

    //

    uint_fast32_t n_block_; ///< current block size.

    uint_fast32_t n_block_half_; ///< current half block size.

    uint_fast32_t n_calc_; ///<

    uint_fast32_t s_; ///< current step no.

    std::vector<std::complex<double>> W_0_;

    std::vector<std::vector<uint_fast32_t>> iW_; ///< [n_step_][n_]

    std::vector<std::vector<uint_fast32_t>> m_; ///< [n_step_][n_half_]

    std::vector<int> i_bit_rev_;

    //std::complex<double> coeff_amp_;

    std::string window_func_name_;

    std::vector<double> wf_;

    //

    std::vector<int> thread_exec_;

    int thread_init_cnt_;

    int thread_cnt_;

    //

    std::vector<double> t_;

    std::vector<std::complex<double>> f_;

    //

    double dt_;

    double df_;

    double acf_; ///< Amplitude Correction Factor

    bool use_acf_;

    //

    std::mutex mtx_wait_;    // for command wait of server

    std::condition_variable cond_;

    bool thread_ready_;

    //

    bool print_;

    //

    //MyChronos time_;

    /// @}

};

}; // MyAll

#endif /// _MYALL_MYFFT_HPP_
