/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Copyright (C) 2022 Analog Devices, Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
 */

#ifndef __IIOSTREAM_COMMON_H__
#define __IIOSTREAM_COMMON_H__

#include <stddef.h>
#include <stdio.h>

#include "../ad9361/ad9361.hpp"


#include <vector> 
#include <thread> 

struct iio_channel;
struct iio_stream;
struct complex;

// void stop_stream(void);

void tx(int argc, char **argv);

void rx(int argc, char **argv);

void stream(int argc, char **argv);

void stream_amplitude(int argc, char **argv);

void receive_data(struct iio_device* rx, size_t rx_sample_sz, FILE* file3, FILE* file4);

void send_data(struct iio_device* tx, size_t tx_sample_sz, std::vector<complex>& samples, float ampl);

void stream_amplitude_thread(int argc, char **argv);


#endif /* __IIOSTREAM_COMMON_H__ */