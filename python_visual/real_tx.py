import time                     # для измененя частоты смены кадров 
import adi                      # для работы с Adalm-Pluto
import matplotlib.pyplot as plt # для отрисовки графика
import numpy as np              # для выделения реальной и мнимой части

# Подключаемся к SDR
sdr = adi.Pluto("ip:192.168.2.1") 

# Устанавливаем значение несущей частоты в соответствии с каналом
sdr.rx_lo = 1900000000
sdr.sample_rate = int(2500000)
sdr.rx_buffer_size = int(16000)
sdr.gain_control_mode_chan0 = "manual"
sdr.tx_hardwaregain_chan0 = 0
sdr.tx_cyclic_buffer = False

tx_file = np.loadtxt("/home/ivan/Desktop/Work_dir/1440/OFDM_transceiver/src/resurrs/real_imag.txt", dtype=complex)

k = 0 
while k < 30:
    if k < 10:
        k+=1
        sdr.tx(tx_file)
        sdr.tx_destroy_buffer()
    else:
        sdr.rx_lo = 2300000000
        k+=1
        sdr.tx(tx_file)
        sdr.tx_destroy_buffer()