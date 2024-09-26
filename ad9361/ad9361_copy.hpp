// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * libiio - AD9361 IIO streaming example
 *
 * Copyright (C) 2014 IABG mbH
 * Author: Michael Feilen <feilen_at_iabg.de>
 **/

#ifndef AD9361_COPY_H
#define AD9361_COPY_H

#include <iio/iio.h>
// #include <iio/iio-debug.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

/* helper macros */
#define MHZ(x) ((long long)(x*1000000.0 + .5))
#define GHZ(x) ((long long)(x*1000000000.0 + .5))

#define IIO_ENSURE(expr) { \
	if (!(expr)) { \
		(void) fprintf(stderr, "assertion failed (%s:%d)\n", __FILE__, __LINE__); \
		(void) abort(); \
	} \
}

#define BLOCK_SIZE (5760*2)

/* RX is input, TX is output */
enum iodev { RX, TX };

/* common RX and TX streaming params */
struct stream_cfg {
	long long bw_hz; // Analog banwidth in Hz
	long long fs_hz; // Baseband sample rate in Hz
	long long lo_hz; // Local oscillator frequency in Hz
	const char* rfport; // Port name
};

/* static scratch mem for strings */
static char tmpstr[64];

/* IIO structs required for streaming */
extern struct iio_context *ctx;
extern struct iio_channel *rx0_i;
extern struct iio_channel *rx0_q;
extern struct iio_channel *tx0_i;
extern struct iio_channel *tx0_q;
extern struct iio_buffer  *rxbuf;
extern struct iio_buffer  *txbuf;
extern struct iio_stream  *rxstream ;
extern struct iio_stream  *txstream;

extern struct iio_channels_mask *rxmask; // общевидимой
extern struct iio_channels_mask *txmask;


void stop_stream(void);
/* cleanup and exit */
void shutdown(void);

static void handle_sig(int sig);
/* check return value of attr_write function */
static void errchk(int v, const char* what);
/* write attribute: long long int */
static void wr_ch_lli(struct iio_channel *chn, const char* what, long long val);

/* write attribute: string */
static void wr_ch_str(struct iio_channel *chn, const char* what, const char* str);

/* helper function generating channel names */
static char* get_ch_name(const char* type, int id);

/* returns ad9361 phy device */
static struct iio_device* get_ad9361_phy(void);

/* finds AD9361 streaming IIO devices */
static bool get_ad9361_stream_dev(enum iodev d, struct iio_device **dev);

/* finds AD9361 streaming IIO channels */
static bool get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn);

/* finds AD9361 phy IIO configuration channel with id chid */
static bool get_phy_chan(enum iodev d, int chid, struct iio_channel **chn);

/* finds AD9361 local oscillator IIO configuration channels */
static bool get_lo_chan(enum iodev d, struct iio_channel **chn);

/* applies streaming configuration through IIO */
static bool cfg_ad9361_streaming_ch(struct stream_cfg *cfg, enum iodev type, int chid);

struct iio_device* context_tx(int argc, char **argv);

struct iio_device* context_rx(int argc, char **argv);


#endif