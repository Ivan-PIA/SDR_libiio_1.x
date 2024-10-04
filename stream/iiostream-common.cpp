
// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Copyright (C) 2022 Analog Devices, Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
 */

#include "../stream/iiostream-common.hpp"
#include <iostream> 

#define step_ampl 0.5

typedef struct complex
{
	float real;
	float imag;

}complex;



// void write_to_file(char *path)
// {
// 	FILE *file;

// 	file  = fopen(path, "w");

// 	if(!file){
// 		std::cout << "error open file! " << std::endl;;
// 	}

// 	fprintf(file3, "%d\n", q);
// 	fprintf(file4, "%d\n", i);

// 	fclose(file);
// }


void tx(int argc, char **argv )
{
	

	struct iio_device *tx = context_tx(argc,argv);
	
    size_t tx_sample_sz;
	
    //rxmask = iio_create_channels_mask(iio_device_get_channels_count(rx));
    //rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
	printf("%s:%d\n", __func__, __LINE__);
	tx_sample_sz = iio_device_get_sample_size(tx, txmask);
	/////
	printf("%s:%d\n", __func__, __LINE__);
	const struct iio_device *dev;
	const struct iio_context *ctx;
	const struct iio_block *txblock;

	ssize_t ntx = 0;
	int err;
	printf("%s:%d\n", __func__, __LINE__);
	dev = iio_channel_get_device(tx0_i);

	ctx = iio_device_get_context(dev);
	printf("%s:%d\n", __func__, __LINE__);
	FILE *file, *file2;
	
	file  = fopen("../resurs_file/imag_part.txt", "r");
    file2 = fopen("../resurs_file/real_part.txt", "r");

	

	complex *samples = (complex*) malloc(sizeof(complex)*5760);

   if(file)
    {
        for (int i = 0; i < 5760; i++){
            fscanf(file, "%f\n",&samples[i].imag );
            fscanf(file2, "%f\n",&samples[i].real );
            //printf("%d ", (int)samples[i].imag);
           // printf("%d ", (int)samples[i].real);    
        }


    }
	else {
		std::cout << "error open file" << std::endl;
	} 
	
	int count = 0;
	while (count < 100) {

		
		int16_t *p_dat, *p_end;
		ptrdiff_t p_inc;

		/* WRITE: Get pointers to TX buf and write IQ to TX buf port 0 */
		// if(count > 50 && count < 55){
		if(count == 49 || count == 50 || count == 51){
			txblock = iio_stream_get_next_block(txstream);
			err = iio_err(txblock);

			if (err) {
				////ctx_perror(ctx, err, "Unable to send block");
				return;
			}
		}

		if(count == 50){
			p_inc = tx_sample_sz;
			p_end =  (int16_t*)iio_block_end(txblock);
			int i = 0;
			
			for (p_dat =  (int16_t*)iio_block_first(txblock, tx0_i); p_dat < p_end; p_dat += p_inc / sizeof(*p_dat)) {
				
				if(i > 5760){
					i = 0;
				}

				p_dat[0] = (int16_t)samples[i].real ; /* Real (I) */
				p_dat[1] = (int16_t)samples[i].imag ; /* Imag (Q) */
				i++;


			}
		}

		
		// txblock = iio_stream_get_next_block(txstream);
		// err = iio_err(txblock);

		// if (err) {
		// 	//ctx_perror(ctx, err, "Unable to send block");
		// 	return;
		// }
		ntx += BLOCK_SIZE / tx_sample_sz;
		count++;
	}

		
		//ctx_info(ctx, "\tRX %8.2f MSmp, TX %8.2f MSmp\n", nrx / 1e6, ntx / 1e6);
	

	
	fclose(file);
	fclose(file2);
	
	shutdown();
}

void rx(int argc, char **argv){
	
	struct iio_device *rx = context_rx(argc,argv);
	size_t rx_sample_sz;
	rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
	const struct iio_device *dev;
	const struct iio_context *ctx;
	const struct iio_block *txblock, *rxblock;
	ssize_t nrx = 0;
	ssize_t ntx = 0;
	int err;
	
	dev = iio_channel_get_device(rx0_i);
	ctx = iio_device_get_context(dev);
	
	FILE *file3, *file4;
	


	file3  = fopen("../resurs_file/rx_imag.txt", "w");
    file4 = fopen("../resurs_file/rx_real.txt", "w");

	complex *samples = (complex*) malloc(sizeof(complex)*5760);


	
	int count = 0;
	while (count < 1000) {

		count++;
		int16_t *p_dat, *p_end;
		ptrdiff_t p_inc;

		rxblock = iio_stream_get_next_block(rxstream);
		err = iio_err(rxblock);
		if (err) {
			////ctx_perror(ctx, err, "Unable to receive block");
			return;
		}


		/* READ: Get pointers to RX buf and read IQ from RX buf port 0 */
		p_inc = rx_sample_sz;
		
		p_end = (int16_t*)iio_block_end(rxblock);
		for (p_dat = (int16_t*)iio_block_first(rxblock, rx0_i); p_dat < p_end;
		     p_dat += p_inc / sizeof(*p_dat)) {
			/* Example: swap I and Q */
			int16_t i = p_dat[0];
			int16_t q = p_dat[1];

			p_dat[0] = q;
			p_dat[1] = i;

			fprintf(file3, "%d\n", q);
			fprintf(file4, "%d\n", i);
		}

		printf("count : %d\n", count);
		nrx += BLOCK_SIZE / rx_sample_sz;
		
		//ctx_info(ctx, "\tRX %8.2f MSmp, TX %8.2f MSmp\n", nrx / 1e6, ntx / 1e6);
	}

	fclose(file3);
	fclose(file4);
	
	shutdown();

}

void stream(int argc, char **argv)
{
	

		////
	struct iio_device *rx = context_rx(argc,argv);
	struct iio_device *tx = context_tx(argc,argv);
	
    size_t rx_sample_sz,tx_sample_sz;
	
    //rxmask = iio_create_channels_mask(iio_device_get_channels_count(rx));
    //rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
	rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
	tx_sample_sz = iio_device_get_sample_size(tx, txmask);
	/////
	
	const struct iio_device *dev;
	const struct iio_context *ctx;
	const struct iio_block *txblock, *rxblock;
	ssize_t nrx = 0;
	ssize_t ntx = 0;
	int err;
	
	dev = iio_channel_get_device(rx0_i);
	ctx = iio_device_get_context(dev);
	
	FILE *file, *file2, *file3, *file4;
	
	file  = fopen("../resurs_file/imag_part.txt", "r");
    file2 = fopen("../resurs_file/real_part.txt", "r");

	file3  = fopen("../resurs_file/rx_imag.txt", "w");
    file4 = fopen("../resurs_file/rx_real.txt", "w");

	complex *samples = (complex*) malloc(sizeof(complex)*5760);

   if(file)
    {
        for (int i = 0; i < 5760; i++){
            fscanf(file, "%f\n",&samples[i].imag );
            fscanf(file2, "%f\n",&samples[i].real );
            //printf("%d ", (int)samples[i].imag);
           // printf("%d ", (int)samples[i].real);    
        }

        
    } 
	
	int count = 0;
	while (count < 100) {

		count++;
		int16_t *p_dat, *p_end;
		ptrdiff_t p_inc;

		rxblock = iio_stream_get_next_block(rxstream);
		err = iio_err(rxblock);
		if (err) {
			//ctx_perror(ctx, err, "Unable to receive block");
			return;
		}



    




		/* READ: Get pointers to RX buf and read IQ from RX buf port 0 */
		p_inc = rx_sample_sz;
		
		p_end =  (int16_t*)iio_block_end(rxblock);
		for (p_dat =  (int16_t*)iio_block_first(rxblock, rx0_i); p_dat < p_end;
		     p_dat += p_inc / sizeof(*p_dat)) {
			/* Example: swap I and Q */
			int16_t i = p_dat[0];
			int16_t q = p_dat[1];

			p_dat[0] = q;
			p_dat[1] = i;


		}

		/* WRITE: Get pointers to TX buf and write IQ to TX buf port 0 */
		// if(count > 50 && count < 55){
		if(count == 30 || count == 31 || count == 51 || count == 71 || count == 81){
			txblock = iio_stream_get_next_block(txstream);
			err = iio_err(txblock);

			if (err) {
				//ctx_perror(ctx, err, "Unable to send block");
				return;
			}
		}

		if(count == 30 || count == 50  || count == 70 || count == 80){

			// txblock = iio_stream_get_next_block(txstream);
			// err = iio_err(txblock);

			// if (err) {
			// 	//ctx_perror(ctx, err, "Unable to send block");
			// 	return;
			// }
			int ampl = 0;
			if(count == 30){
				ampl = 1;
			}
			else if(count == 50){
				ampl = 2;
			}
			else if(count == 70){
				ampl = 4;
			}
			else if(count == 80){
				ampl = 5;
			}
			ampl = 1;
			printf("dot start : %d\n", count * (5760*3));
			p_inc = tx_sample_sz;
			p_end =  (int16_t*)iio_block_end(txblock);
			int i = 0;
			
			for (p_dat =  (int16_t*)iio_block_first(txblock, tx0_i); p_dat < p_end; p_dat += p_inc / sizeof(*p_dat)) {
				
				if(i > 5760){
					i = 0;
				}

				p_dat[0] = (int16_t)samples[i].real * ampl; /* Real (I) */
				p_dat[1] = (int16_t)samples[i].imag * ampl; /* Imag (Q) */
				i++;


			}
		}
		nrx += BLOCK_SIZE / rx_sample_sz;
		ntx += BLOCK_SIZE / tx_sample_sz;
		//ctx_info(ctx, "\tRX %8.2f MSmp, TX %8.2f MSmp\n", nrx / 1e6, ntx / 1e6);
	}

	fclose(file);
	fclose(file2);
	fclose(file3);
	fclose(file4);
	
	shutdown();
}

void stream_amplitude(int argc, char **argv)
{
	

		////
	struct iio_device *rx = context_rx(argc,argv);
	struct iio_device *tx = context_tx(argc,argv);
	
    size_t rx_sample_sz,tx_sample_sz;
	
    //rxmask = iio_create_channels_mask(iio_device_get_channels_count(rx));
    //rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
	rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
	tx_sample_sz = iio_device_get_sample_size(tx, txmask);
	/////
	
	const struct iio_device *dev;
	const struct iio_context *ctx;
	const struct iio_block *txblock, *rxblock;
	ssize_t nrx = 0;
	ssize_t ntx = 0;
	int err;
	
	dev = iio_channel_get_device(rx0_i);
	ctx = iio_device_get_context(dev);
	
	FILE *file, *file2, *file3, *file4;
	
	file  = fopen("../resurs_file/imag_part.txt", "r");
    file2 = fopen("../resurs_file/real_part.txt", "r");

	file3  = fopen("../resurs_file/rx_imag.txt", "w");
    file4 = fopen("../resurs_file/rx_real.txt", "w");

	complex *samples = (complex*) malloc(sizeof(complex)*5760);

   if(file)
    {
        for (int i = 0; i < 5760; i++){
            fscanf(file, "%f\n",&samples[i].imag );
            fscanf(file2, "%f\n",&samples[i].real );
            //printf("%d ", (int)samples[i].imag);
           // printf("%d ", (int)samples[i].real);    
        }

        
    } 

	float ampl = 1;
	int count = 0;
	while (count < 100) {

		
		int16_t *p_dat, *p_end;
		ptrdiff_t p_inc;

		rxblock = iio_stream_get_next_block(rxstream);
		err = iio_err(rxblock);
		if (err) {
			//ctx_perror(ctx, err, "Unable to receive block");
			return;
		}



		
		if(count > 50 && count < 70 ){
			ampl += step_ampl;
		}



		/* READ: Get pointers to RX buf and read IQ from RX buf port 0 */
		p_inc = rx_sample_sz;
		
		p_end =  (int16_t*)iio_block_end(rxblock);
		for (p_dat =  (int16_t*)iio_block_first(rxblock, rx0_i); p_dat < p_end;
		     p_dat += p_inc / sizeof(*p_dat)) {
			/* Example: swap I and Q */
			int16_t i = p_dat[0];
			int16_t q = p_dat[1];

			p_dat[0] = q;
			p_dat[1] = i;

			fprintf(file3, "%d\n", q);
			fprintf(file4, "%d\n", i);
		}

		/* WRITE: Get pointers to TX buf and write IQ to TX buf port 0 */
		// if(count > 50 && count < 55){

		txblock = iio_stream_get_next_block(txstream);
		err = iio_err(txblock);

		if (err) {
			//ctx_perror(ctx, err, "Unable to send block");
			return;
		}


			// txblock = iio_stream_get_next_block(txstream);
			// err = iio_err(txblock);

			// if (err) {
			// 	//ctx_perror(ctx, err, "Unable to send block");
			// 	return;
			// }

		//printf("dot start : %d\n", count * (5760*3));
		std :: cout << "ampl: " << ampl << std::endl;
		p_inc = tx_sample_sz;
		p_end =  (int16_t*)iio_block_end(txblock);
		int i = 0;
		
		for (p_dat =  (int16_t*)iio_block_first(txblock, tx0_i); p_dat < p_end; p_dat += p_inc / sizeof(*p_dat)) {
			
			if(i > 5760){
				i = 0;
			}

			p_dat[0] = (int16_t)samples[i].real * ampl; /* Real (I) */
			p_dat[1] = (int16_t)samples[i].imag * ampl; /* Imag (Q) */
			i++;



		}
		count++;
	}
	nrx += BLOCK_SIZE / rx_sample_sz;
	ntx += BLOCK_SIZE / tx_sample_sz;
		//ctx_info(ctx, "\tRX %8.2f MSmp, TX %8.2f MSmp\n", nrx / 1e6, ntx / 1e6);
	

	fclose(file);
	fclose(file2);
	fclose(file3);
	fclose(file4);
	
	shutdown();
}

void receive_data(struct iio_device* rx, size_t rx_sample_sz, FILE* file3, FILE* file4) {
    int count = 0;
    int err;
    const struct iio_block *rxblock;
    
    while (count < 100) {
        int16_t *p_dat, *p_end;
        ptrdiff_t p_inc;
        
        rxblock = iio_stream_get_next_block(rxstream);
        err = iio_err(rxblock);
        if (err) {
            std::cerr << "Unable to receive block" << std::endl;
            return;
        }
        
        p_inc = rx_sample_sz;
        p_end =  (int16_t*)iio_block_end(rxblock);
        for (p_dat =  (int16_t*)iio_block_first(rxblock, rx0_i); p_dat < p_end; p_dat += p_inc / sizeof(*p_dat)) {
            int16_t i = p_dat[0];
            int16_t q = p_dat[1];
            p_dat[0] = q;
            p_dat[1] = i;
            fprintf(file3, "%d\n", q);
            fprintf(file4, "%d\n", i);
        }
        count++;
    }
}

void send_data(struct iio_device* tx, size_t tx_sample_sz, std::vector<complex>& samples, float ampl) {
    int count = 0;
    int err;
    const struct iio_block *txblock;
    
    while (count < 100) {
        int16_t *p_dat, *p_end;
        ptrdiff_t p_inc;
        
        if(count > 50 && count < 70 ){
            ampl += step_ampl;
        }
        
        txblock = iio_stream_get_next_block(txstream);
        err = iio_err(txblock);
        if (err) {
            std::cerr << "Unable to send block" << std::endl;
            return;
        }
        
        p_inc = tx_sample_sz;
        p_end =  (int16_t*)iio_block_end(txblock);
        int i = 0;
        
        for (p_dat =  (int16_t*)iio_block_first(txblock, tx0_i); p_dat < p_end; p_dat += p_inc / sizeof(*p_dat)) {
            if(i > 5760){
                i = 0;
            }
            p_dat[0] = (int16_t)samples[i].real * ampl;
            p_dat[1] = (int16_t)samples[i].imag * ampl;
            i++;
        }
        count++;
    }
}

void stream_amplitude_thread(int argc, char **argv) 
{
    struct iio_device *rx = context_rx(argc, argv);
    struct iio_device *tx = context_tx(argc, argv);
    
    size_t rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
    size_t tx_sample_sz = iio_device_get_sample_size(tx, txmask);
    
    FILE *file = fopen("../resurs_file/imag_part.txt", "r");
    FILE *file2 = fopen("../resurs_file/real_part.txt", "r");
    FILE *file3 = fopen("../resurs_file/rx_imag.txt", "w");
    FILE *file4 = fopen("../resurs_file/rx_real.txt", "w");
    
    std::vector<complex> samples(5760);
    
    if (file && file2) {
        for (int i = 0; i < 5760; i++) {
            fscanf(file, "%f\n", &samples[i].imag);
            fscanf(file2, "%f\n", &samples[i].real);
        }
    }
    
    float ampl = 1.0;
    
    // Запуск потоков для отправки и приема данных
    std::thread rx_thread(receive_data, rx, rx_sample_sz, file3, file4);
    std::thread tx_thread(send_data, tx, tx_sample_sz, std::ref(samples), ampl);
    
    // Ожидание завершения потоков
    rx_thread.join();
    tx_thread.join();
    
    fclose(file);
    fclose(file2);
    fclose(file3);
    fclose(file4);
    
    shutdown();
}
