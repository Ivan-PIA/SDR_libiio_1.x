import time                     # для измененя частоты смены кадров 
import adi                      # для работы с Adalm-Pluto
import matplotlib.pyplot as plt # для отрисовки графика
import numpy as np              # для выделения реальной и мнимой части

# Подключаемся к SDR
sdr = adi.Pluto("ip:192.168.2.1") 

# Устанавливаем значение несущей частоты в соответствии с каналом
sdr.rx_lo = 2300000000
sdr.sample_rate = int(2500000)
sdr.rx_buffer_size = int(16000)
sdr.gain_control_mode_chan0 = "manual"
sdr.rx_hardwaregain_chan0 = 20

PATH = "/home/ivan/Desktop/Work_dir/1440/OFDM_transceiver/src/resurrs/py_file.txt"

rx_file = np.zeros(0)
for i in range(500):
    rx = sdr.rx()
    rx_file = np.concatenate([rx_file,rx])

np.savetxt(PATH,rx_file)