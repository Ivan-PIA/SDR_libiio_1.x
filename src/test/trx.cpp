// #include "../stream/iiostream-common.hpp" 


#include "../stream/iiostream-common.hpp"

#include <vector>
#include <complex> 
#include <iostream> 

// iio_info -a  : show ip

int main (int argc, char **argv)
{
	// FILE *file, *file2, *file3, *file4;
	// file  = fopen("../resurs_file/imag_part.txt", "r");
    // file2 = fopen("../resurs_file/real_part.txt", "r");
	// file3  = fopen("../resurs_file/rx_imag.txt", "w");
    // file4 = fopen("../resurs_file/rx_real.txt", "w");

	// // complex *samples = (complex*) malloc(sizeof(complex)*5760);

	// std::vector <comp> samples;

	// double real, imag;

   	// if(file)
    // {
    //     for (int i = 0; i < 5760; i++){
    //         fscanf(file, "%lf\n", &imag);
    //         fscanf(file2, "%lf\n", &real);

	// 		samples.push_back(comp(real,imag));
	// 		// std::cout << "(real,imag) = " << real << ", " << imag << std::endl;
    //         // printf("%d ", (int)samples[i].imag);
    //         // printf("%d ", (int)samples[i].real);    
    //     }

    // } 
	// else
	// {
	// 	printf("error_file\n");
	// }

	// // std::cout << "samples1: " << samples[11].real() << ", " << samples[11].imag() << std::endl;

	// std::vector<comp> rx_data;


    // std::thread write_thread(write_to_block, argv[1], std::ref(samples), 100);


    // std::thread read_thread([&rx_data, &argv] {
    //     rx_data = read_from_block(argv[1], 100);
    // });

  
    // write_thread.join();
    // read_thread.join();

	// // write_to_block(argv[1], samples, 100);
	// // rx_data = read_from_block(argv[1], 100);

	// std::cout << "size rx data: " << rx_data.size() << std::endl;

	// for (int i = 0; i < rx_data.size(); i ++){
	// 	fprintf(file3, "%d\n", (int16_t)rx_data[i].imag());
	// 	fprintf(file4, "%d\n", (int16_t)rx_data[i].real());
	// }


	stream(argc, argv);


	//shutdown();
	//printf("* Starting IO streaming (press CTRL+C to cancel)\n");
	
	// tx(argc, argv);
	
	//shutdown();

	return 0;
}