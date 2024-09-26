import numpy as np
import matplotlib.pyplot as plt






plt.figure(1)
#plt.title("rx buffer 1 sec(2.5MS) | tx once (5760S)")
#plt.title("rx buffer (10000S) | tx (10) (5760S) | ampl tx 2**14")

#plt.title("rx buffer 1 sec(2.5MS) | tx nothing")

rx = np.loadtxt("/home/ivan/Desktop/Work_dir/1440/OFDM_transceiver/src/resurrs/py_file.txt", dtype=complex)

plt.plot(abs(rx))
plt.show()