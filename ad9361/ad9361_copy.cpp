// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * libiio - AD9361 IIO streaming example
 *
 * Copyright (C) 2014 IABG mbH
 * Author: Michael Feilen <feilen_at_iabg.de>
 **/


#include "../ad9361/ad9361_copy.hpp"

struct iio_context *ctx   = NULL;
struct iio_channel *rx0_i = NULL;
struct iio_channel *rx0_q = NULL;
struct iio_channel *tx0_i = NULL;
struct iio_channel *tx0_q = NULL;
struct iio_buffer  *rxbuf = NULL;
struct iio_buffer  *txbuf = NULL;
struct iio_stream  *rxstream = NULL;
struct iio_stream  *txstream = NULL;

struct iio_channels_mask *rxmask = NULL;
struct iio_channels_mask *txmask = NULL;

static bool stop = false;

void stop_stream(void)
{
	stop = true;
}

/* cleanup and exit */
void shutdown(void)
{
	printf("* Destroying streams\n");
	if (rxstream) {iio_stream_destroy(rxstream); }
	if (txstream) { iio_stream_destroy(txstream); }

	printf("* Destroying buffers\n");
	if (rxbuf) { iio_buffer_destroy(rxbuf); }
	if (txbuf) { iio_buffer_destroy(txbuf); }

	printf("* Destroying channel masks\n");
	if (rxmask) { iio_channels_mask_destroy(rxmask); }
	if (txmask) { iio_channels_mask_destroy(txmask); }

	printf("* Destroying context\n");
	if (ctx) { iio_context_destroy(ctx); }
	exit(0);
}


static void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop_stream();
}

/* check return value of attr_write function */
static void errchk(int v, const char* what) {
	 if (v < 0) { fprintf(stderr, "Error %d writing to channel \"%s\"\nvalue may not be supported.\n", v, what); shutdown(); }
}

/* write attribute: long long int */
static void wr_ch_lli(struct iio_channel *chn, const char* what, long long val)
{
	const struct iio_attr *attr = iio_channel_find_attr(chn, what);

	errchk(attr ? iio_attr_write_longlong(attr, val) : -ENOENT, what);
}

/* write attribute: string */
static void wr_ch_str(struct iio_channel *chn, const char* what, const char* str)
{
	const struct iio_attr *attr = iio_channel_find_attr(chn, what);

	errchk(attr ? iio_attr_write_string(attr, str) : -ENOENT, what);
}

/* helper function generating channel names */
static char* get_ch_name(const char* type, int id)
{
	snprintf(tmpstr, sizeof(tmpstr), "%s%d", type, id);
	return tmpstr;
}

/* returns ad9361 phy device */
struct iio_device* get_ad9361_phy(void)
{
	struct iio_device *dev =  iio_context_find_device(ctx, "ad9361-phy");
	IIO_ENSURE(dev && "No ad9361-phy found");
	return dev;
}

/* finds AD9361 streaming IIO devices */
bool get_ad9361_stream_dev(enum iodev d, struct iio_device **dev)
{
	switch (d) {
	case TX: *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc"); return *dev != NULL;
	case RX: *dev = iio_context_find_device(ctx, "cf-ad9361-lpc");  return *dev != NULL;
	default: IIO_ENSURE(0); return false;
	}
}

/* finds AD9361 streaming IIO channels */
bool get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn)
{
	*chn = iio_device_find_channel(dev, get_ch_name("voltage", chid), d == TX);
	if (!*chn)
		*chn = iio_device_find_channel(dev, get_ch_name("altvoltage", chid), d == TX);
	return *chn != NULL;
}

/* finds AD9361 phy IIO configuration channel with id chid */
bool get_phy_chan(enum iodev d, int chid, struct iio_channel **chn)
{
	switch (d) {
	case RX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("voltage", chid), false); return *chn != NULL;
	case TX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("voltage", chid), true);  return *chn != NULL;
	default: IIO_ENSURE(0); return false;
	}
}

/* finds AD9361 local oscillator IIO configuration channels */
bool get_lo_chan(enum iodev d, struct iio_channel **chn)
{
	switch (d) {
	 // LO chan is always output, i.e. true
	case RX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("altvoltage", 0), true); return *chn != NULL;
	case TX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("altvoltage", 1), true); return *chn != NULL;
	default: IIO_ENSURE(0); return false;
	}
}

/* applies streaming configuration through IIO */
bool cfg_ad9361_streaming_ch(struct stream_cfg *cfg, enum iodev type, int chid)
{
	const struct iio_attr *attr,*attr_gain;
	struct iio_channel *chn = NULL;

	// Configure phy and lo channels
	printf("* Acquiring AD9361 phy channel %d\n", chid);
	if (!get_phy_chan(type, chid, &chn)) {	return false; }

	attr = iio_channel_find_attr(chn, "rf_port_select");
	attr_gain = iio_channel_find_attr(chn, "gain_control_mode");

	if (attr)
		errchk(iio_attr_write_string(attr, cfg->rfport), cfg->rfport);

	if (attr_gain)
		errchk(iio_attr_write_string(attr_gain, "manual"), "manual");

	wr_ch_lli(chn, "rf_bandwidth",       cfg->bw_hz);
	wr_ch_lli(chn, "sampling_frequency", cfg->fs_hz);
	
	if(type == RX){
		wr_ch_lli(chn, "hardwaregain",20.0);
	}
	else {
		wr_ch_lli(chn, "hardwaregain", 0.0);
	}

	// Configure LO channel
	printf("* Acquiring AD9361 %s lo channel\n", type == TX ? "TX" : "RX");
	if (!get_lo_chan(type, &chn)) { return false; }

	wr_ch_lli(chn, "frequency", cfg->lo_hz);

	//int ret = iio_channel_attr_write(chn,"gain_control_mode", "manual");

	return true;

	//attr_gain =  iio_channel_find_attr(chn, "gain_control_mode");

	// iio_channel_attr_write(chn, "gain_control_mode", "manual");

	// //attr_gain = iio_channel_find_attr(chn, "gain_control_mode");
	// if (type == RX){   
		
    // 	wr_ch_lli(chn, "hardwaregain", 20); // RX gain
	// }

	// else {
	// 	//wr_ch_lli(chn, "powerdown", 1);
	// 	wr_ch_lli(chn, "hardwaregain", 0);
	// }


}


struct iio_device* context_tx(int argc, char **argv){
	
	struct iio_device *tx;
	size_t ntx = 0;
	size_t tx_sample_sz;

	struct stream_cfg txcfg;

	int err;

	// TX stream config
	txcfg.bw_hz = MHZ(1.5); // 1.5 MHz rf bandwidth
	txcfg.fs_hz = MHZ(2.5);   // 2.5 MS/s tx sample rate
	txcfg.lo_hz = GHZ(1.8); // 2.5 GHz rf frequency
	txcfg.rfport = "A"; // port A (select for rf freq.)

	printf("* Acquiring IIO context\n");
	if (argc == 1) {
		IIO_ENSURE((ctx = iio_create_context(NULL, NULL)) && "No context");
	}
	else if (argc == 2) {
		IIO_ENSURE((ctx = iio_create_context(NULL, argv[1])) && "No context");
	}
	IIO_ENSURE(iio_context_get_devices_count(ctx) > 0 && "No devices");

	printf("* Acquiring AD9361 streaming devices\n");
	IIO_ENSURE(get_ad9361_stream_dev(TX, &tx) && "No tx dev found");

	printf("* Configuring AD9361 for streaming\n");

	IIO_ENSURE(cfg_ad9361_streaming_ch(&txcfg, TX, 0) && "TX port 0 not found");

	printf("* Initializing AD9361 IIO streaming channels\n");

	IIO_ENSURE(get_ad9361_stream_ch(TX, tx, 0, &tx0_i) && "TX chan i not found");
	IIO_ENSURE(get_ad9361_stream_ch(TX, tx, 1, &tx0_q) && "TX chan q not found");


	txmask = iio_create_channels_mask(iio_device_get_channels_count(tx));
	if (!txmask) {
		fprintf(stderr, "Unable to alloc channels mask\n");
		shutdown();
	}

	printf("* Enabling IIO streaming channels\n");
	iio_channel_enable(tx0_i, txmask);
	iio_channel_enable(tx0_q, txmask);

	printf("* Creating non-cyclic IIO buffers with 1 MiS\n");

	txbuf = iio_device_create_buffer(tx, 0, txmask);
	
	err = iio_err(txbuf);
	if (err) {
		txbuf = NULL;
		// dev_perror(tx, err, "Could not create TX buffer");
		shutdown();
	}
	printf("deb11\n");

	txstream = iio_buffer_create_stream(txbuf, 4, BLOCK_SIZE);
	err = iio_err(txstream);
	if (err) {
		txstream = NULL;
		// dev_perror(tx, iio_err(txstream), "Could not create TX stream");
		shutdown();
	}
	printf("deb12\n");
	return tx;
}


struct iio_device* context_rx(int argc, char **argv){
	// Streaming devices

	struct iio_device *rx;

	// RX and TX sample counters
	size_t nrx = 0;


	// RX and TX sample size
	

	// Stream configurations
	struct stream_cfg rxcfg;

	int err;

	// Listen to ctrl+c and IIO_ENSURE
	//signal(SIGINT, handle_sig);

	// RX stream config
	rxcfg.bw_hz = MHZ(2);  // 2 MHz rf bandwidth
	rxcfg.fs_hz = MHZ(2.5);// 2.5 MS/s rx sample rate
	rxcfg.lo_hz = GHZ(1.8);// 2.5 GHz rf frequency
	rxcfg.rfport = "A_BALANCED"; // port A (select for rf freq.)



	printf("* Acquiring IIO context\n");
	if (argc == 1) {
		IIO_ENSURE((ctx = iio_create_context(NULL, NULL)) && "No context");
	}
	else if (argc == 2) {
		IIO_ENSURE((ctx = iio_create_context(NULL, argv[1])) && "No context");
	}
	IIO_ENSURE(iio_context_get_devices_count(ctx) > 0 && "No devices");

	printf("* Acquiring AD9361 streaming devices\n");

	IIO_ENSURE(get_ad9361_stream_dev(RX, &rx) && "No rx dev found");

	printf("* Configuring AD9361 for streaming\n");
	IIO_ENSURE(cfg_ad9361_streaming_ch(&rxcfg, RX, 0) && "RX port 0 not found");


	printf("* Initializing AD9361 IIO streaming channels\n");
	IIO_ENSURE(get_ad9361_stream_ch(RX, rx, 0, &rx0_i) && "RX chan i not found");
	IIO_ENSURE(get_ad9361_stream_ch(RX, rx, 1, &rx0_q) && "RX chan q not found");


	rxmask = iio_create_channels_mask(iio_device_get_channels_count(rx));
	if (!rxmask) {
		fprintf(stderr, "Unable to alloc channels mask\n");
		shutdown();
	}


	printf("* Enabling IIO streaming channels\n");
	iio_channel_enable(rx0_i, rxmask);
	iio_channel_enable(rx0_q, rxmask);


	printf("* Creating non-cyclic IIO buffers with 1 MiS\n");
	rxbuf = iio_device_create_buffer(rx, 0, rxmask);
	err = iio_err(rxbuf);
	if (err) {
		rxbuf = NULL;
		// dev_perror(rx, err, "Could not create RX buffer");
		shutdown();
	}

	rxstream = iio_buffer_create_stream(rxbuf, 4, BLOCK_SIZE);

	return rx;

}



/* simple configuration and streaming */
/* usage:
 * Default context, assuming local IIO devices, i.e., this script is run on ADALM-Pluto for example
 $./a.out
 * URI context, find out the uri by typing `iio_info -s` at the command line of the host PC
 $./a.out usb:x.x.x
 */
// int main (int argc, char **argv)
// {
// 	struct iio_device *rx = context_rx(argc,argv);
// 	struct iio_device *tx = context_tx(argc,argv);
//     size_t rx_sample_sz,tx_sample_sz;

//     //rxmask = iio_create_channels_mask(iio_device_get_channels_count(rx));
//     //rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
// 	rx_sample_sz = iio_device_get_sample_size(rx, rxmask);
// 	tx_sample_sz = iio_device_get_sample_size(tx, txmask);

// 	printf("* Starting IO streaming (press CTRL+C to cancel)\n");
	
// 	stream(rx_sample_sz, tx_sample_sz, BLOCK_SIZE,
// 	      rxstream, txstream, rx0_i, tx0_i);
	
// 	shutdown();

// 	return 0;
// }