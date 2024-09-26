import numpy as np
import matplotlib.pyplot as plt
import time

real = np.loadtxt("/home/ivan/Desktop/Work_dir/test_file_all/sdr/resurs_file/rx_real.txt")


imag = np.loadtxt("/home/ivan/Desktop/Work_dir/test_file_all/sdr/resurs_file/rx_imag.txt")

real = real[:int(5.760e6)]
imag = imag[:int(5.760e6)]

rx = np.vectorize(complex)(real, imag)

print(len(rx))
print(rx[:10])

plt.figure(1)
#plt.title("rx buffer 1 sec(2.5MS) | tx once (5760S)")
#plt.title("rx buffer 1 sec(2.5MS) | tx cycle (5760S) | ampl tx 2**14")

plt.title("rx buffer (5760*100) | tx 1 (5760*3)")
plt.plot(abs(rx))
plt.show()


