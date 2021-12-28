
// ****************************************************************************
//
//                                  VGA
//
// ****************************************************************************

#include "include.h"

// PIO command (jmp=program address, num=loop counter)
#define VGACMD(jmp, num) ( (u32)((jmp)+VGAOff) | ((u32)(num)<<5) )

// swap bytes of command
#define BYTESWAP(n) ((((n)&0xff)<<24)|(((n)&0xff00)<<8)|(((n)&0xff0000)>>8)|(((n)&0xff000000)>>24))

// display frame buffer
ALIGNED u8 FrameBuf[FRAMESIZE];

// VGA PIO
uint VGAOff;	// offset of VGA PIO program

// Scanline data buffers (commands sent to PIO)
u32 ScanLineImg[3];	// image: HSYNC ... back porch ... image command
u32 ScanLineFp;		// front porch (after image data)
u32 ScanLineDark[2];	// dark: HSYNC ... back porch + dark + front porch
u32 ScanLineSync[2];	// vertical sync: VHSYNC ... VSYNC(back porch + dark + front porch)

// Scanline control buffers
#define CB_MAX 8	// size of one scanline control buffer (1 link to data buffer requires 2x u32)
u32 ScanLineCB[2*CB_MAX]; // 2 control buffers
int VgaBufInx;		// current running control buffer
u32* ScanLineCBNext;	// next control buffer

// handler variables
volatile int VgaScanLine; // current processed scan line 0... (next displayed scan line)
volatile u32 VgaFrame;	// frame counter

// saved integer divider state
hw_divider_state_t SaveDividerState;

// VGA DMA handler - called on end of every scanline
void __not_in_flash_func(VgaLine)()
{
	// Clear the interrupt request for DMA control channel
	dma_hw->ints0 = (1u << VGA_DMA_PIO);

	// update DMA control channel and run it
	dma_channel_set_read_addr(VGA_DMA_CB, ScanLineCBNext, true);

	// save integer divider state
	hw_divider_save_state(&SaveDividerState);

	// switch current buffer index (bufinx = current preparing buffer, VgaBufInx = current running buffer)
	int bufinx = VgaBufInx;
	VgaBufInx = bufinx ^ 1;

	// prepare control buffer to be processed
	u32* cb = &ScanLineCB[bufinx*CB_MAX];
	ScanLineCBNext = cb;

	// increment scanline (1..)
	int line = VgaScanLine; // current scanline
	line++; 		// new current scanline
	if (line >= VGA_VTOT) // last scanline?
	{
		VgaFrame++;	// increment frame counter
		line = 0; 	// restart scanline
	}
	VgaScanLine = line;	// store new scanline

	// check scanline
	line -= VGA_VSYNC;
	if (line < 0)
	{
		// VSYNC
		*cb++ = 2;
		*cb++ = (u32)&ScanLineSync[0];
	}
	else
	{
		// front porch and back porch
		line -= VGA_VBACK;
		if ((line < 0) || (line >= VGA_VACT))
		{
			// dark line
			*cb++ = 2;
			*cb++ = (u32)&ScanLineDark[0];
		}

		// image scanlines
		else
		{
			// HSYNC ... back porch ... image command
			*cb++ = 3;
			*cb++ = (u32)&ScanLineImg[0];

			// image data
			*cb++ = WIDTHBYTE/4;
			*cb++ = (u32)&FrameBuf[line*WIDTHBYTE];

			// front porch
			*cb++ = 1;
			*cb++ = (u32)&ScanLineFp;
		}
	}

	// end mark
	*cb++ = 0;
	*cb = 0;

	// restore integer divider state
	hw_divider_restore_state(&SaveDividerState);
}

// initialize VGA PIO
void VgaPioInit()
{
	int i;

	// load PIO program
	VGAOff = pio_add_program(VGA_PIO, &vga_program);

	// configure GPIOs for use by PIO
	for (i = VGA_GPIO_FIRST; i <= VGA_GPIO_LAST; i++) pio_gpio_init(VGA_PIO, i);

#if USE_HSYNC
	pio_gpio_init(VGA_PIO, VGA_CTRL_HSYNC);	// HSYNC output
#endif

#if USE_VSYNC
	pio_gpio_init(VGA_PIO, VGA_CTRL_VSYNC);	// VSYNC output
#endif

#if USE_DCLK
	pio_gpio_init(VGA_PIO, VGA_CTRL_DCLK);	// DCLK output
#endif

#if USE_DE
	pio_gpio_init(VGA_PIO, VGA_CTRL_DE);	// DE output
#endif

	// set pin direction to output
	pio_sm_set_consecutive_pindirs(VGA_PIO, VGA_SM, VGA_GPIO_FIRST, VGA_GPIO_NUM, true);
	pio_sm_set_consecutive_pindirs(VGA_PIO, VGA_SM, VGA_CTRL_FIRST, VGA_CTRL_NUM, true);

	// negate HSYNC and VSYNC outputs
#if USE_HSYNC
	gpio_set_outover(VGA_CTRL_HSYNC, GPIO_OVERRIDE_INVERT);
#endif

#if USE_VSYNC
	gpio_set_outover(VGA_CTRL_VSYNC, GPIO_OVERRIDE_INVERT);
#endif

#if USE_DCLK
	// maximize DCLK output current and slew rate
	//  Note: It is only needed when the output is loaded with a capacitive load.
/*
	gpio_set_pulls(VGA_CTRL_DCLK, false, false);
	hw_write_masked(
		&padsbank0_hw->io[VGA_CTRL_DCLK],
		(PADS_BANK0_GPIO0_DRIVE_VALUE_12MA << PADS_BANK0_GPIO0_DRIVE_LSB),
		PADS_BANK0_GPIO0_DRIVE_BITS);
	hw_write_masked(
		&padsbank0_hw->io[VGA_CTRL_DCLK],
		(1 << PADS_BANK0_GPIO0_SLEWFAST_LSB),
		PADS_BANK0_GPIO0_SLEWFAST_BITS);
*/
#endif

	// prepare default PIO program config
	pio_sm_config cfg = vga_program_get_default_config(VGAOff);

	// map state machine's OUT and MOV pins	
	sm_config_set_out_pins(&cfg, VGA_GPIO_FIRST, VGA_GPIO_NUM);

	// set sideset pins (control signals)
	sm_config_set_sideset_pins(&cfg, VGA_CTRL_FIRST);

	// join FIFO to send only
	sm_config_set_fifo_join(&cfg, PIO_FIFO_JOIN_TX);

	// PIO clock divider
	sm_config_set_clkdiv(&cfg, VGA_CLKDIV);

	// shift left, autopull, pull threshold
	sm_config_set_out_shift(&cfg, false, true, 32);

	// initialize state machine
	pio_sm_init(VGA_PIO, VGA_SM, VGAOff+vga_offset_entry, &cfg);
}

// initialize scanline buffers
void VgaBufInit()
{
	// image scanline data buffer: HSYNC ... back porch ... image command
	ScanLineImg[0] = BYTESWAP(VGACMD(vga_offset_hsync, VGA_HSYNC-2));	// HSYNC
	ScanLineImg[1] = BYTESWAP(VGACMD(vga_offset_blank, VGA_BP-2));		// back porch
	ScanLineImg[2] = BYTESWAP(VGACMD(vga_offset_output, WIDTH-2));		// image

	// front porch
	ScanLineFp = BYTESWAP(VGACMD(vga_offset_blank, VGA_FP-2));		// front porch

	// dark scanline: HSYNC ... back porch + dark + front porch
	ScanLineDark[0] = BYTESWAP(VGACMD(vga_offset_hsync, VGA_HSYNC-2));	// HSYNC
	ScanLineDark[1] = BYTESWAP(VGACMD(vga_offset_blank, VGA_TOTAL-VGA_HSYNC-2)); // back porch + dark + front porch

	// vertical sync: VHSYNC ... VSYNC(back porch + dark + front porch)
#if VGA_MODE == 0	// VGA mode (0=new analog, 1=old analog, 2=universal, 3=digital VH, 4=digital DE)
	ScanLineSync[0] = BYTESWAP(VGACMD(vga_offset_vhsync, VGA_TOTAL-VGA_HSYNC-2)); // VHSYNC ... invert HSYNC signal during VSYNC
	ScanLineSync[1] = BYTESWAP(VGACMD(vga_offset_vsync, VGA_HSYNC-2)); // VSYNC
#else
	ScanLineSync[0] = BYTESWAP(VGACMD(vga_offset_vhsync, VGA_HSYNC-2)); // VHSYNC
	ScanLineSync[1] = BYTESWAP(VGACMD(vga_offset_vsync, VGA_TOTAL-VGA_HSYNC-2)); // VSYNC (back porch + dark + front porch)
#endif

	// control buffer 1 - initialize to VSYNC
	ScanLineCB[0] = 2; // send 2x u32 (send ScanLineSync)
	ScanLineCB[1] = (u32)&ScanLineSync[0]; // VSYNC data buffer
	ScanLineCB[2] = 0; // stop mark
	ScanLineCB[3] = 0; // stop mark

	// control buffer 1 - initialize to VSYNC
	ScanLineCB[CB_MAX+0] = 2; // send 2x u32 (send ScanLineSync)
	ScanLineCB[CB_MAX+1] = (u32)&ScanLineSync[0]; // VSYNC data buffer
	ScanLineCB[CB_MAX+2] = 0; // stop mark
	ScanLineCB[CB_MAX+3] = 0; // stop mark
}

// initialize VGA DMA
//   control blocks aliases:
//                  +0x0        +0x4          +0x8          +0xC (Trigger)
// 0x00 (alias 0):  READ_ADDR   WRITE_ADDR    TRANS_COUNT   CTRL_TRIG
// 0x10 (alias 1):  CTRL        READ_ADDR     WRITE_ADDR    TRANS_COUNT_TRIG
// 0x20 (alias 2):  CTRL        TRANS_COUNT   READ_ADDR     WRITE_ADDR_TRIG
// 0x30 (alias 3):  CTRL        WRITE_ADDR    TRANS_COUNT   READ_ADDR_TRIG ... we use this!
void VgaDmaInit()
{

// ==== prepare DMA control channel

	// prepare DMA default config
	dma_channel_config cfg = dma_channel_get_default_config(VGA_DMA_CB);

	// increment address on read from memory
	channel_config_set_read_increment(&cfg, true);

	// increment address on write to DMA port
	channel_config_set_write_increment(&cfg, true);

	// each DMA transfered entry is 32-bits
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);

	// write ring - wrap to 8-byte boundary (TRANS_COUNT and READ_ADDR_TRIG of data DMA)
	channel_config_set_ring(&cfg, true, 3);

	// DMA configure
	dma_channel_configure(
		VGA_DMA_CB,		// channel
		&cfg,			// configuration
		&dma_hw->ch[VGA_DMA_PIO].al3_transfer_count, // write address
		&ScanLineCB[0],		// read address - as first, control buffer 1 will be sent out
		2,			// number of transfers in u32 (number of transfers per one request from data DMA)
		false			// do not start yet
	);

// ==== prepare DMA data channel

	// prepare DMA default config
	cfg = dma_channel_get_default_config(VGA_DMA_PIO);

	// increment address on read from memory
	channel_config_set_read_increment(&cfg, true);

	// do not increment address on write to PIO
	channel_config_set_write_increment(&cfg, false);

	// each DMA transfered entry is 32-bits
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);

	// DMA data request for sending data to PIO
	channel_config_set_dreq(&cfg, pio_get_dreq(VGA_PIO, VGA_SM, true));

	// chain channel to DMA control block
	channel_config_set_chain_to(&cfg, VGA_DMA_CB);

	// raise the IRQ flag when 0 is written to a trigger register (end of chain)
	channel_config_set_irq_quiet(&cfg, true);

	// set byte swapping
	channel_config_set_bswap(&cfg, true);

	// set high priority
	cfg.ctrl |= DMA_CH0_CTRL_TRIG_HIGH_PRIORITY_BITS;

	// DMA configure
	dma_channel_configure(
		VGA_DMA_PIO,		// channel
		&cfg,			// configuration
		&VGA_PIO->txf[VGA_SM], // write address
		NULL,			// read address
		0,			// number of transfers in u32
		false			// do not start immediately
	);

// ==== initialize IRQ0, raised from DMA data channel

	// enable DMA channel IRQ0
	dma_channel_set_irq0_enabled(VGA_DMA_PIO, true);

	// set DMA IRQ handler
	irq_set_exclusive_handler(DMA_IRQ_0, VgaLine);

	// set highest IRQ priority
	irq_set_priority(DMA_IRQ_0, 0);
}

// initialize VGA (will change system clock to 126 MHz) (auto-initialized at startup)
void VgaInit()
{
	// initialize system clock
	set_sys_clock_pll((u32)VGA_VCO, VGA_PD1, VGA_PD2);

	// initialize PIO
	VgaPioInit();

	// initialize scanline buffers
	VgaBufInit();

	// initialize DMA
	VgaDmaInit();

	// initialize parameters
	VgaScanLine = 0; // currently processed scanline
	VgaBufInx = 0; // at first, control buffer 1 will be sent out
	VgaFrame = 0; // current frame
	ScanLineCBNext = &ScanLineCB[CB_MAX]; // send control buffer 2 next

	// enable DMA IRQ
	irq_set_enabled(DMA_IRQ_0, true);

	// start DMA
	dma_channel_start(VGA_DMA_CB);

	// run state machine
	pio_sm_set_enabled(VGA_PIO, VGA_SM, true);
}

void (* volatile Core1Fnc)() = NULL; // core 1 remote function

// VGA core
void VgaCore()
{
	void (*fnc)();

	// initialize VGA
	Core1Fnc = VgaInit;
	__dmb();
	VgaInit();
	__dmb();
	Core1Fnc = NULL;

	// infinite loop
	while (true)
	{
		// data memory barrier
		__dmb();

		// execute remote function
		fnc = Core1Fnc;
		if (fnc != NULL)
		{
			fnc();
			__dmb();
			Core1Fnc = NULL;
		}
	}
}

// execute core 1 remote function
void Core1Exec(void (*fnc)())
{
	__dmb();
	Core1Fnc = fnc;
	__dmb();
}

// check if core 1 is busy (executing remote function)
Bool Core1Busy()
{
	__dmb();
	return Core1Fnc != NULL;
}

// wait if core 1 is busy (executing remote function)
void Core1Wait()
{
	while (Core1Busy()) {}
}

// check VSYNC
Bool IsVSync()
{
	return ((VgaScanLine < VGA_VSYNC+VGA_VBACK) ||
		(VgaScanLine >= VGA_VTOT-VGA_VFRONT));
}

// wait for VSync scanline
void WaitVSync()
{
	// wait for end of VSync
	while (IsVSync()) { __dmb(); }

	// wait for start of VSync
	while (!IsVSync()) { __dmb(); }
}

// auto initialization
Bool VgaAutoInit()
{
	multicore_launch_core1(VgaCore);

	// waiting for clk_sys to become valid
	Core1Wait();
	WaitVSync();
	WaitVSync();

	return True;
}

Bool VgaAutoInitOK = VgaAutoInit();
