// #include "../stream/iiostream-common.hpp" 

#include "../../ad9361/ad9361.hpp"

#include "../correlation/time_corr.hpp"
#include <vector>
#include <complex> 
#include <iostream> 

// iio_info -a  : show ip

int main (int argc, char **argv)
{
	char *ip = argv[1];

	FILE *file3, *file4;

	file3  = fopen("/home/ivan/SDR_libiio_1.x/resurs_file/t_rx_imag.txt", "w");
    file4 = fopen("/home/ivan/SDR_libiio_1.x/resurs_file/t_rx_real.txt", "w");


	if (!file3){
		std::cout << "error ept!" << std::endl;
	}

	std::vector<comp> rx_data;

    // rx_data = read_from_block(ip, 1000);

	rx_data = read_from_block_real(ip);

	std::cout << "size rx data: " << rx_data.size() << std::endl;

	for (int i = 0; i < rx_data.size()-100; i ++){
		fprintf(file3, "%d\n", (int16_t)rx_data[i].imag());
		fprintf(file4, "%d\n", (int16_t)rx_data[i].real());
	}

	int conv = convolve(rx_data);
	shutdown();

	std::cout << "Receve done!" << std::endl;

	return 0;
}