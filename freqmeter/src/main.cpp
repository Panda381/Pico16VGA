
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************
// Frequency meter (can measure 2.5 Hz .. 63 MHz)
//  Input: GPIO15, Pico pin 20
//  Sample rate: 126 MHz
//  Number of bit samples: 24000*32 = 768000
//  Load samples at DIV=1 (sample rate 126 MHz): 6095 us (= 24000*32/126)
//    DIV=2: 12 ms
//    DIV=4: 24 ms
//    DIV=8: 49 ms
//    DIV=16: 98 ms
//    DIV=32: 195 ms
//    DIV=64: 390 ms
//    DIV=128: 780 ms

#include "include.h"

#define SAMPLES		24000	// number of 32-bit samples (= 96 KB)
#define FREQ_GPIO	15	// used GPIO input
#define FREQ_PIO	pio1	// used PIO
#define FREQ_SM		0	// used state machine
#define FREQ_DMA	8	// used DMA channel

u32 FreqOff;			// PIO program offset
u32 Samples[SAMPLES];		// input samples

// configure state machine
void FreqPIO(int div)
{
	// configure state machine
	pio_sm_config cfg = freqmeter_program_get_default_config(FreqOff); // prepare default config
	sm_config_set_in_pins(&cfg, FREQ_GPIO); // set pin as input
	sm_config_set_clkdiv(&cfg, div); // set clock divider
	sm_config_set_in_shift(&cfg, true, true, 32); // shift right, auto-push, push threshold = 32 bits
	sm_config_set_fifo_join(&cfg, PIO_FIFO_JOIN_RX); // join FIFO to receive only
	pio_sm_init(FREQ_PIO, FREQ_SM, FreqOff, &cfg); // initialize state machine
}

// configure DMA
void FreqDMA()
{
	dma_channel_config cfg = dma_channel_get_default_config(FREQ_DMA); // prepare default config
	channel_config_set_read_increment(&cfg, false); // no increment on read (read from PIO port)
	channel_config_set_write_increment(&cfg, true); // increment on write (write into sample buffer)
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32); // transfer size 32-bits
	channel_config_set_dreq(&cfg, pio_get_dreq(FREQ_PIO, FREQ_SM, false)); // data request from PIO

	dma_channel_configure(
		FREQ_DMA,			// DMA channel
		&cfg,				// configuration
		Samples,			// destination buffer
		&FREQ_PIO->rxf[FREQ_SM],	// read from PIO data port
		SAMPLES,			// number of samples to read
		true);				// start immediately
}

int first;
int last10;
int delay; // interval of first and last sample

// capture samples
int FreqCapt(int div)
{
	int i, j, n, f, del;
	u32 s;
	bool bit, last;

	// stop state machine and clear FIFO
	pio_sm_set_enabled(FREQ_PIO, FREQ_SM, false);
	pio_sm_clear_fifos(FREQ_PIO, FREQ_SM);

	// configure state machine
	FreqPIO(div);

	// configure DMA
	FreqDMA();

	// start state machine
	pio_sm_set_enabled(FREQ_PIO, FREQ_SM, true);

	// wait for transfer
	dma_channel_wait_for_finish_blocking(FREQ_DMA);

	// count samples
	n = 0;
	f = -1;
	del = -1;
	last10 = -1;
	last = ((Samples[1] & 1) != 0);
	for (i = 1; i < SAMPLES; i++) // skip 1st sample, can be corrupted
	{
		s = Samples[i];
		for (j = 0; j < 32; j++)
		{
			// count positive edges
			bit = ((s & 1) != 0);
			if (bit && !last)
			{
				n++;
				del = i*32 + j;
				if (n == 10) last10 = del;
				if (f == -1) f = del;
			}
			s >>= 1;
			last = bit;
		}
	}
	first = f;
	delay = del - f; // interval of first and last sample
	return n;
}

// display diagram
void Disp(int s)
{
	int s1, del, x, y, bit;
	bool b, oldb;
	u8 col;
	if (s < 3)
	{
		s1 = 0;
		del = 10;
	}
	else
	{
		s1 = first;
		if (last10 < 0)
			del = delay;
		else
			del = last10 - s1;
	}

	oldb = false;
	for (x = 0; x < WIDTH; x++)
	{
		bit = (int)((double)x/WIDTH*del + s1 + 0.5);
		b = ((Samples[bit/32] & (1 << (bit & 0x1f))) != 0);

		if (b != oldb)
		{
			for (y = 20; y <= 40; y++) DrawPoint(x, y, COL_GREEN);
		}
		else
		{
			if (b)
			{
				DrawPoint(x, 20, COL_GREEN);
				for (y = 21; y <= 40; y++) DrawPoint(x, y, COL_BLACK);
			}
			else
			{
				for (y = 20; y < 40; y++) DrawPoint(x, y, COL_BLACK);
				DrawPoint(x, 40, COL_GREEN);
			}
		}
		oldb = b;
	}
}

char buf[50];

// main function
int main()
{
	int i, div;
	u32 s;
	double f, t;

	// clear display
	DrawClear();

	// load PIO program
	FreqOff = pio_add_program(FREQ_PIO, &freqmeter_program);

	// configure GPIO for use by PIO
	pio_gpio_init(FREQ_PIO, FREQ_GPIO);

	// main loop
	while (true)
	{
		f = 0;
		t = 0;

		// capture samples (measure time 6 .. 780 ms)
		for (div = 1; div <= 128; div *= 2)
		{
			// measure
			s = FreqCapt(div);

			// calculate frequency in Hz
			if (s >= 3)
			{
				f = (double)(s-1)*SYSFREQ/delay/div;
				t = 1/f; // period in seconds
			}

			// we can measure frequency with precision 4 digits
			//  (lowest measurable frequency is 13.1 kHz = 10200 * 126000 / 768 / 128 )
			if (s >= 10200) break;

			// we can measure interval with precision 4 digits
			//  (highest measurable frequency is 12.1 kHz = 74 * 126000 / 768 / 1 )
			//  (lowest measurable frequency is 2.5 Hz = 2 * 126000 / 768 / 128 )
			if ((s >= 3) && (s <= 75)) // interval is min. 10200 samples
			{
				// calculate period in seconds
				t = (double)delay/(s-1)*div/SYSFREQ;
				f = 1/t;
				break;
			}
		}

		// display
		if (f == 0)
			memcpy(buf, "no signal     ", 10);
		else
		{
			if (f >= 1000000)
				sprintf(buf, "%.4g MHz    ", f/1000000);
			else if (f > 1000)
				sprintf(buf, "%.4g kHz    ", f/1000);
			else
				sprintf(buf, "%.4g Hz     ", f);
		}
		DrawTextBg(buf, 200, 200, 4, 5, COL_WHITE, COL_BLACK);

		if (t == 0)
			memcpy(buf, "            ", 10);
		else
		{
			if (t >= 0.001)
				sprintf(buf, "%.4g ms     ", t*1000);
			else if (t >= 0.000001)
				sprintf(buf, "%.4g us     ", t*1000000);
			else
				sprintf(buf, "%.4g ns     ", t*1000000000);
		}
		DrawTextBg(buf, 200, 270, 4, 5, COL_WHITE, COL_BLACK);

		// display diagram
		Disp(s);
	}
}
