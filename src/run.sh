
cd /home/ivan/SDR_libiio_1.x/build

cmake ..
make

./rx ip:192.168.2.4

/bin/python3 /home/ivan/SDR_libiio_1.x/python_visual/time_areol_rx_1sec.py
