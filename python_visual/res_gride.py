import matplotlib.pyplot as plt
import numpy as np
from context import *



def resource_grid(data, count_frames, len_frame, Nfft, cp, title = ""): 

    """
        Параметры:

        `data`        - данные для ресурсной сетки 
        `count_frame` - количество OFDM фреймов бля ресурсной сетки
        `len_frame`   - количество ofdm-символов в ofdm фрейме
        `Nftt`        - количество поднесущих
        `cp`          - защитный префикс

    """
    data1 = data[:(Nfft+cp)*len_frame*count_frames]
    print(len(data1))
    half_nfft = Nfft//2

    # преобразуем в матрицу 
    data_freq = data1.reshape(len_frame*count_frames, (Nfft+cp))

    # обрезаем циклический префикс
    data1 = data_freq[:, cp:]
    # производим обратное преобразование фурье и транспонируем матрицу для удобного вывода на карте
    data2 = np.fft.fft(data1).T

    # переставляем строки местами из-за не шифтнутых частот
    temp = np.copy(data2[0:half_nfft, :])
    data2[0:half_nfft, :] = data2[half_nfft:Nfft, :]
    data2[half_nfft:Nfft, :] = temp

    #plt.figure()
    plt.title(title)
    plt.imshow(abs(data2), cmap='jet',interpolation='nearest', aspect='auto') #abs(10*np.log10(data2))
    plt.xlabel('OFDM symbol')
    plt.ylabel('Subcarriers')
    plt.colorbar()#label='dB'
    #plt.show()


real = np.loadtxt("/home/ivan/Desktop/Work_dir/test_file_all/sdr/resurs_file/rx_real.txt")


imag = np.loadtxt("/home/ivan/Desktop/Work_dir/test_file_all/sdr/resurs_file/rx_imag.txt")

# real = real[:int(5.760e6)]
# imag = imag[:int(5.760e6)]

rx = np.vectorize(complex)(real, imag)
# rx  = rx[900000:]
# rx = rx[:2000]
print(len(rx))
N_fft = 128
GB_len = 55
CP = 32
N_pilot = 6


plt.figure(figsize=(10,10))
plt.subplot(2,1,1)
resource_grid(rx, 5700, 6, N_fft, CP, title="tx 1 buffer")

# data = corr_pss_time(rx,N_fft)

# data_cor = calculate_correlation(N_fft, data, 15000)#

# plt.subplot(2,1,2)
# resource_grid(data_cor,5,6,N_fft, CP)

plt.show()