# Installation


1. Install libraries: 

#### Linux:

```sh
sudo apt-get install build-essential git libxml2-dev bison flex libcdk5-dev cmake python3-pip libusb-1.0-0-dev libavahi-client-dev libavahi-common-dev libaio-dev
cd ~
git clone https://github.com/analogdevicesinc/libiio.git
cd libiio
mkdir build
cd build
cmake -DPYTHON_BINDINGS=ON ..
make -j$(nproc)
sudo make install
sudo ldconfig

cd ~
git clone https://github.com/analogdevicesinc/libad9361-iio.git
cd libad9361-iio
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install

```
2. Building project: 

```sh
git clone https://github.com/Ivan-PIA/SDR_libiio_1.x.git
cd SDR_libiio_1.x
mkdir build
cd build
cmake ..
make
```
3. Run:

```sh
./tx ip:192.168.2.1
./rx ip:192.168.2.1

```