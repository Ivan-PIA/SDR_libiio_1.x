#include "freq_offset.hpp"





#define M_PI 3.14159265358979323846

template <typename T>

size_t argmax(vector<T> & v) {
    size_t max_index = 0;
    T max_value = v[0];
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i] > max_value) {
            max_index = i;
            max_value = v[i];
        }
    }
    return max_index;
}



void frequ_corr_pss(const vector<std::complex<double>>& pss, const vector<std::complex<double>>& matrix_name, double m, vector<std::complex<double>>& data_offset) {

    vector<std::complex<double>> corr_coef(pss.rbegin(), pss.rend());
    for (size_t i = 0; i < corr_coef.size(); i++) {
        corr_coef[i] = conj(corr_coef[i]);
    }

    vector<std::complex<double>> partA(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = 0; i < pss.size() / 2; i++) {
        for (size_t j = 0; j < matrix_name.size(); j++) {
            partA[i + j] += corr_coef[i] * matrix_name[j];
        }
    }

    vector<std::complex<double>> xDelayed(matrix_name.size());
    for (size_t i = 0; i < matrix_name.size() ; i++) {
        
        if(i < pss.size() / 2){
            xDelayed[i] = 0;
        }
        else{
            xDelayed[i] = matrix_name[i];
        }
    }

    
    vector<std::complex<double>> partB(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = pss.size() / 2; i < pss.size(); i++) {
        for (size_t j = 0; j < xDelayed.size(); j++) {
            partB[i + j] += corr_coef[i] * xDelayed[j];
        }
    }


    vector<double> correlation(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = 0; i < correlation.size(); i++) {
        correlation[i] = abs(partA[i] + partB[i]);
    }
    vector<std::complex<double>> phaseDiff(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = 0; i < phaseDiff.size(); i++) {
        phaseDiff[i] = partA[i] * conj(partB[i]);
    }


    size_t istart = argmax(correlation) ;
    std::complex<double> phaseDiff_max = phaseDiff[istart];


    double CFO = arg(phaseDiff_max) / (M_PI * 1 / m);
    vector<double> t(matrix_name.size());
    for (size_t i = 0; i < t.size(); i++) {
        t[i] = i ;
    }

    //cout << "CFO :" << conj(CFO) << endl;
    //CFO = -7018.845944622955;
    // Offset the data
    data_offset.resize(matrix_name.size());
    for (size_t i = 0; i < matrix_name.size(); i++) {
        
        data_offset[i] = matrix_name[i] * exp(-1i * double(2) * M_PI * conj(CFO) * double(t[i]/1920000));
    }
    
}






vector<complex<double>> cfo_CP(const vector<complex<double>>& ofdm, int N , int N_G) {
    
    // Ограничиваем длину сигнала до 5600
    // size_t length = min(ofdm.size(), size_t(5600));
    // vector<complex<double>> truncated_ofdm(ofdm.begin(), ofdm.begin());

    int symbol_length = N + N_G;
    int num_symbols = ofdm.size() / symbol_length;

    vector<double> cfo_estimates;

    for (int i = 0; i < num_symbols; ++i) {
        // Извлекаем символ из OFDM
        vector<complex<double>> symbol(ofdm.begin() + i * symbol_length, ofdm.begin() + (i + 1) * symbol_length);

        vector<complex<double>> cp_part(symbol.begin(), symbol.begin() + N_G);
        vector<complex<double>> data_part(symbol.begin() + N_G, symbol.begin() + N_G + N);

        // Вычисляем CFO для текущего символа
        complex<double> sum = 0;
        for (int j = 0; j < N; ++j) {
            sum += conj(cp_part[j]) * data_part[j];
        }
        double cfo_estimate = (1.0 / (2.0 * M_PI)) * arg(sum);
        cfo_estimates.push_back(cfo_estimate);
    }

    // Усредняем оценки CFO по всем символам 
    double average_cfo = accumulate(cfo_estimates.begin(), cfo_estimates.end(), 0.0) / cfo_estimates.size();

    //Нормируем CFO в зависимости от sample rate
    double cfo = (average_cfo * 1920000) / 128;

    // Временной вектор для коррекции сигнала
    int len = ofdm.size();
    vector<complex<double>> ofdm_corrected(len);
    
    for (int i = 0; i < len; ++i) {
        double t = static_cast<double>(i) / 1920000;
        ofdm_corrected[i] = ofdm[i] * exp(complex<double>(0, -2 * M_PI * cfo * t));
    }

    return ofdm_corrected;
}

