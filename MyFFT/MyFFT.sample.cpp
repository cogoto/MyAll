//
//

#include <chrono>

#include <cmath>

#include <complex>

#include <fstream>

#include <iomanip>

#include <iostream>

#include <random>

#include <vector>

#include "MyFFT.hpp"

//

using namespace std;


//--------------------------------------------------//


vector<double> make_wave(
    int n,
    double dt,
    vector<double> amp_sin,
    vector<double> f_sin,
    vector<double> amp_cos,
    vector<double> f_cos,
    double offset,
    double noise_peak
) {

    double pi = 3.14159265358979;

    vector<double> I_t(n, 0);

    //

    random_device rnd;
    //mt19937 mt(rnd());
    mt19937 mt(2020);
    uniform_int_distribution<> range_rand(0, 999999);

    //

    for(int i=0;i<n;i++) {

        double t = dt * i;

        //

        for(uint_fast32_t i2=0;i2<amp_sin.size();i2++) {
            I_t[i] += amp_sin[i2] * sin(2 * pi * f_sin[i2] * t);
        }

        for(uint_fast32_t i2=0;i2<amp_cos.size();i2++) {
            I_t[i] += amp_cos[i2] * cos(2 * pi * f_cos[i2] * t);
        }

        I_t[i] += offset;

        I_t[i] += noise_peak * static_cast<double>(range_rand(mt)) / 1000000;

    }

    return I_t;

}


//--------------------------------------------------//


int main(int argc, char* argv[]) {

    //
    // parameters
    //

    int n = 8192; // the number of frequency data

    int n_thread = 4;

    double n_half_double = n / 2;

    int n_half_int = static_cast<int>(n_half_double + 0.5);

    //

    double dt = 0.1;

    double df = 1 / (dt * n);

    double noise_peak = 0.0;

    vector<double> a_sin = { 1, 4};
    vector<double> f_sin = { 1, 4};

    vector<double> a_cos = {2};
    vector<double> f_cos = {2};

    double offset = 10;

    //

    vector<double> I_t_1 = make_wave(n,   dt, a_sin, f_sin, a_cos, f_cos, offset, noise_peak);
    vector<double> I_t_2 = make_wave(n*8, dt, a_sin, f_sin, a_cos, f_cos, offset, noise_peak);

    //
    // FFT
    //

    cout << "The number of spectral points = " << n << endl;
    cout << "The number of threads         = " << n_thread << endl;

    MyAll::MyFFT fft_(n, 4);

    //fft_.SetWindow("hamming");

    //

    vector<double> t = fft_.T(dt);

    //

    chrono::system_clock::time_point start, end;

    //

    /*
    start = chrono::system_clock::now();

    vector<complex<double>> I_f_0;

    fft_._FFT_CT_multi(I_t_1, I_f_0);

    end = chrono::system_clock::now();
    cout  << "[par] dt = " << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;
    */

    //

    cout << "[seq]   time data size = " << I_t_1.size() << endl;
    cout << "[multi] time data size = " << I_t_2.size() << endl;

    //

    vector<complex<double>> I_f_1;

    start = chrono::system_clock::now();

    fft_.FFT_CT_seq(I_t_1, I_f_1);

    end = chrono::system_clock::now();
    cout << "[seq]   dt = " << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    vector<vector<complex<double>>> I_f_2;

    start = chrono::system_clock::now();

    fft_.FFT(I_t_2, I_f_2);

    end = chrono::system_clock::now();
    cout << "[multi] dt = " << chrono::duration_cast<chrono::nanoseconds>(end-start).count() << endl;

    //

    vector<double> f = fft_.F(df);

    //

    vector<double> I_f_1_real;
    vector<double> I_f_1_imag;
    vector<double> I_f_1_amp;
    vector<double> I_f_1_phas;

    fft_.C2RI(I_f_1, I_f_1_real, I_f_1_imag);
    fft_.C2AP(I_f_1, I_f_1_amp,  I_f_1_phas);

    //

    vector<complex<double>> I_f_1_rev;

    fft_.AP2C(I_f_1_amp, I_f_1_phas, I_f_1_rev);

    //

    for(auto &&it:I_f_1_rev) {
        it *= 1.0;
    }
    I_f_1_rev[0] = 0;

    vector<double> I_t_1_inv;

    //fft_.IFFT_CT_seq(I_f_1, I_t_1_inv);

    fft_.IFFT_CT_seq(I_f_1_rev, I_t_1_inv);

    //

    ofstream of_time;
    of_time.open("./MyFFT.sample.time.txt", ios::out);

    of_time << "# time [sec]" << endl;
    of_time << "# amplitude" << endl;
    of_time << "# amplitude (inversed)" << endl;

    for(int i=0; i<n; i++) {
        of_time << fixed << setw(6) << left << setprecision(3) << t[i] << " ";
        of_time << fixed << setw(9) << right << setprecision(6) << I_t_1[i] << " ";
        of_time << fixed << setw(9) << right << setprecision(6) << I_t_1_inv[i] << endl;
    }

    //

    ofstream of_freq;
    of_freq.open("./MyFFT.sample.freq.txt", ios::out);

    of_freq << "# freq [Hz]" << endl;
    of_freq << "# real" << endl;
    of_freq << "# imag" << endl;
    of_freq << "# amplitude" << endl;
    of_freq << "# phase [rad]" << endl;

    of_freq << "# ";

    for(int i=0; i<n; i++) {
        of_freq << fixed << setw(6) << left  << setprecision(3) << f[i] << " ";
        of_freq << fixed << setw(9) << right << setprecision(6) << I_f_1_real[i] << " ";
        of_freq << fixed << setw(9) << right << setprecision(6) << I_f_1_imag[i] << " ";
        if(i <= n_half_int) {
            of_freq << fixed << setw(9) << right << setprecision(6) << I_f_1_amp[i]  << " ";
            of_freq << fixed << setw(9) << right << setprecision(6) << I_f_1_phas[i] << " ";
        }
        of_freq << endl;
    }

    return 0;

}
