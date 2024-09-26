// #include "stdio.h"
// #include <stdlib.h>

// typedef struct complex
// {
// 	float real;
// 	float imag;

// }complex;

// int main(){
// 	int i = 0;
// 	FILE *file, *file2;

// 	file  = fopen("/home/ivan/Desktop/Work_dir/test_file_all/sdr/resurs_file/imag_part.txt", "r");
//     file2 = fopen("/home/ivan/Desktop/Work_dir/test_file_all/sdr/resurs_file/real_part.txt", "r");
// 	complex *samples = (complex*) malloc(sizeof(complex)*5760);

//    if(file)
//     {
//         for (int i = 0; i < 5760; i++){
//             fscanf(file, "%f\n",&samples[i].imag );
//             fscanf(file2, "%f\n",&samples[i].real );
//             printf("%d ", (int)samples[i].imag);
//             printf("%d ", (int)samples[i].real);    
//         }

//         fclose(file);
//     } 

//     else{
//         printf("not file\n");
//     }
// }

//#include "ad9361.h"
#include "../stream/iiostream-common_copy.hpp" 
#include <stdio.h>

int main (int argc, char **argv)
{


	//printf("* Starting IO streaming (press CTRL+C to cancel)\n");
	
	//rx(argc, argv);
	stream(argc, argv);
	//shutdown();

	return 0;
}