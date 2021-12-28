
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************
// Logic analyzer and noise generator.
//  Analyzer input: GPIO15, Pico pin 20
//  Analyzer sample rate: 126 MHz
//  Displays: 38400 samples
//  Total sampling time: 305 us
//  Noise output: GPIO14, Pico pin 19
//  Noise sample rate: 15.75 MHz

#include "include.h"

// ====== analyzer input
// Time to load whole sample buffer: 1200*32/126 = 305 us

#define SAMP_DIV	1	// clock divider to divide clk_sys=126 MHz to sample rate
#define SAMPLES		1200	// number of 32-bit samples
#define SAMP_GPIO	15	// used GPIO input
#define SAMP_PIO	pio1	// used PIO
#define SAMP_SM		0	// used state machine
#define SAMP_DMA	8	// used DMA channel

u32 SampOff;			// PIO program offset
u32 Samples[SAMPLES];		// input samples from analyzer

// configure state machine
void SampSM()
{
	// configure state machine
	pio_sm_config cfg = analyzer_program_get_default_config(SampOff); // prepare default config
	sm_config_set_in_pins(&cfg, SAMP_GPIO); // set pin as input
	sm_config_set_clkdiv(&cfg, SAMP_DIV); // set clock divider
	sm_config_set_in_shift(&cfg, true, true, 32); // shift right, auto-push, push threshold = 32 bits
	sm_config_set_fifo_join(&cfg, PIO_FIFO_JOIN_RX); // join FIFO to receive only
	pio_sm_init(SAMP_PIO, SAMP_SM, SampOff, &cfg); // initialize state machine
}

// configure DMA
void SampDMA()
{
	dma_channel_config cfg = dma_channel_get_default_config(SAMP_DMA); // prepare default config
	channel_config_set_read_increment(&cfg, false); // no increment on read (read from PIO port)
	channel_config_set_write_increment(&cfg, true); // increment on write (write into sample buffer)
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32); // transfer size 32-bits
	channel_config_set_dreq(&cfg, pio_get_dreq(SAMP_PIO, SAMP_SM, false)); // data request from PIO

	dma_channel_configure(
		SAMP_DMA,			// DMA channel
		&cfg,				// configuration
		Samples,			// destination buffer
		&SAMP_PIO->rxf[SAMP_SM],	// read from PIO data port
		SAMPLES,			// number of samples to read
		true);				// start immediately
}

// capture samples
void SampCapt()
{
	// stop state machine and clear FIFO
	pio_sm_set_enabled(SAMP_PIO, SAMP_SM, false);
	pio_sm_clear_fifos(SAMP_PIO, SAMP_SM);

	// configure state machine
	SampSM();

	// configure DMA
	SampDMA();

	// start state machine
	pio_sm_set_enabled(SAMP_PIO, SAMP_SM, true);

	// wait for transfer
	dma_channel_wait_for_finish_blocking(SAMP_DMA);
}

// display samples
//  1 row = 8 lines
//  1 row = 640 samples
//  total 480 lines -> 60 rows = 38400 bits = 1200 u32 samples
void SampDisp()
{
	int x, y;
	u8 col = 1;
	u8 col2;
	u32* src = Samples;
	u32 s;
	int i = 0;

	for (y = 0; y < HEIGHT; y += 8)
	{
		for (x = 0; x < WIDTH; x++)
		{
			DrawPoint(x, y+4, col);
			col2 = col;
			if (i == 0) s = *src++;
			i = (i+1) & 0x1f;
			if ((s & 1) == 0) col2 = COL_BLACK;
			s >>= 1;
			DrawPoint(x, y+0, col2);
			DrawPoint(x, y+1, col2);
			DrawPoint(x, y+2, col2);
			DrawPoint(x, y+3, col2);
		}
		col++;
		if (col > MAXCOL) col = 1;
	}
}

// ====== random test samples output
// Time to send whole sample buffer: 8192*32/126*8 = 16.644 ms

#define RAND_DIV	8	// clock divider to divide clk_sys=126 MHz to sample rate
#define RAND_BITS	15	// number of bits of random sample buffer size in bytes (max. 15 allowed)
#define RAND_BYTES	(1<<RAND_BITS) // size of random sample buffer in bytes (=32768)
#define RANDS		(RAND_BYTES/4) // number of output random samples in u32 (=8192)
#define RAND_GPIO	14	// used GPIO input
#define RAND_PIO	pio1	// used PIO
#define RAND_SM		1	// used state machine
#define RAND_DMA	9	// used DMA channel
#define RAND_LOOP	30	// number of repeats sending whole buffer (= time 50 ms)

u32 RandOff;			// PIO program offset
u32 Rands[RANDS];		// output random test samples (=65536 bytes)

// configure state machine
void RandSM()
{
	// configure state machine
	pio_sm_config cfg = analyzer_program_get_default_config(RandOff); // prepare default config
	sm_config_set_out_pins(&cfg, RAND_GPIO, 1); // set pin as output
	sm_config_set_clkdiv(&cfg, RAND_DIV); // set clock divider
	sm_config_set_out_shift(&cfg, true, true, 32); // shift right, auto-pull, pull threshold = 32 bits
	sm_config_set_fifo_join(&cfg, PIO_FIFO_JOIN_TX); // join FIFO to send only
	pio_sm_init(RAND_PIO, RAND_SM, RandOff, &cfg); // initialize state machine

	// set pin direction to output
	pio_sm_set_consecutive_pindirs(RAND_PIO, RAND_SM, RAND_GPIO, 1, true);
}

// configure DMA
void RandDMA()
{
	dma_channel_config cfg = dma_channel_get_default_config(RAND_DMA); // prepare default config
	channel_config_set_read_increment(&cfg, true); // increment on read (read from sample buffer)
	channel_config_set_write_increment(&cfg, false); // no increment on write (frite to PIO port)
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32); // transfer size 32-bits
	channel_config_set_ring(&cfg, false, RAND_BITS); // set ring to the buffer
	channel_config_set_dreq(&cfg, pio_get_dreq(RAND_PIO, RAND_SM, true)); // data request from PIO

	dma_channel_configure(
		RAND_DMA,			// DMA channel
		&cfg,				// configuration
		&RAND_PIO->txf[RAND_SM],	// write to PIO data port
		Rands,				// source buffer
		RANDS*RAND_LOOP,		// number of samples to write
		true);				// start immediately
}

// send samples
void RandSend()
{
	// stop state machine and clear FIFO
	pio_sm_set_enabled(RAND_PIO, RAND_SM, false);
	pio_sm_clear_fifos(RAND_PIO, RAND_SM);

	// configure state machine
	RandSM();

	// configure DMA
	RandDMA();

	// start state machine
	pio_sm_set_enabled(RAND_PIO, RAND_SM, true);
}

// main function
int main()
{
	int i;

	// clear display
	DrawClear();

	// load PIO program
	SampOff = pio_add_program(SAMP_PIO, &analyzer_program);
	RandOff = pio_add_program(RAND_PIO, &sampler_program);

	// configure GPIO for use by PIO
	pio_gpio_init(SAMP_PIO, SAMP_GPIO);
	pio_gpio_init(RAND_PIO, RAND_GPIO);

	// main loop
	while (true)
	{
		// send samples
		RandSend();

		// capture samples at 126 MHz
		SampCapt();

		// display samples
		SampDisp();

		// fill random sample buffer with output samples
		//  (why just now - to minimize the delay until the new transmission)
		for (i = 0; i < RANDS; i++) Rands[i] = RandU32();

		// wait for finish sending test samples (= wait 50 ms)
		dma_channel_wait_for_finish_blocking(RAND_DMA);
	}
}
