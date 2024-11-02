#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <iomanip>
#include <numeric>

using namespace std;



void freq_corr_pss(const vector<std::complex<double>>& pss, const vector<std::complex<double>>& matrix_name, int m, vector<std::complex<double>>& data_offset);

vector<complex<double>> cfo_CP(const vector<complex<double>>& ofdm, int N = 128, int N_G = 32);