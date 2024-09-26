import numpy as np
import matplotlib.pyplot as plt


rx = np.loadtxt("/home/ivan/Desktop/Work_dir/1440/SDR_OFDM_TX_RX/file_debag/log/log.txt")

plt.figure(1)
#plt.title("rx buffer 1 sec(2.5MS) | tx once (5760S)")

plt.plot(rx)
plt.show()