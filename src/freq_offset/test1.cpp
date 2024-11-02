#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numeric>
#include </home/ivan/SDR_libiio_1.x/src/freq_offset/freq_offset.hpp>

using namespace std;


int main() {
    // Пример использования функции
    vector<complex<double>> ofdm_signal(5750); // Заполните этот вектор вашими данными
    // Инициализация ofdm_signal (например, случайные значения)
    for (size_t i = 0; i < ofdm_signal.size(); ++i) {
        ofdm_signal[i] = complex<double>(rand() % 100, rand() % 100); // Пример инициализации
    }

    vector<complex<double>> corrected_signal = cfo_CP(ofdm_signal, 128, 32);

    // Вывод или дальнейшая обработка corrected_signal
    for (const auto& val : corrected_signal) {
        cout << val << endl;
    }

    return 0;
}