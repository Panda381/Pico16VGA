
// ****************************************************************************
//
//                               VGA configuration
//
// ****************************************************************************
// By default, MCU is configured to system frequency 126 MHz
// with display 640 x 480 pixels / 16 colors (4 bits per pixel).
// HSYNC frequency: 31500 Hz, VSYNC frequency: 60 Hz, pixel clock: 25.2 MHz.

// color formats
#define COLOR_FORMAT_RGB	0	// RGB: bit2 red, bit1 green, bit0 blue (recommended)
#define COLOR_FORMAT_BGR	1	// BGR: bit2 blue, bit1 green, bit0 red
#define COLOR_FORMAT_GRB	2	// GRB: bit2 green, bit1 red, bit0 blue

// ========= base configuration

// VGA mode (0=new analog, 1=old analog, 2=universal, 3=digital VH, 4=digital DE)
//	0 = new analog VGA (uses HSYNC as composite sync)
//	1 = old analog VGA (uses HSYNC, VSYNC)
//	2 = universal mode (uses HSYNC, VSYNC, DCLK, DE)
//	3 = digital parallel VGA in HV mode (uses HSYNC, VSYNC, DCLK)
//	4 = digital parallel VGA in DE mode (uses DCLK, DE)
// Note: If you change VGA mode, go to vga.pio and uncomment corresponding side-set section.
#define VGA_MODE	2	// if modify - go to vga.pio to update side-set !

// center image on the screen
#define VGA_SHIFTX	0	// shift image in horizontal direction (in pixels: -12 left .. +12 right)
#define VGA_SHIFTY	0	// shift image in vertical direction (in lines: -9 up ... +9 down))

// Color format (0=RGB, 1=BGR, 2=GRB)
//   Determines COL_* constants and function COLRGB.
//   Images must be imported using corresponding palettes.
//	COLOR_FORMAT_RGB: RGB, bit2 red, bit1 green, bit0 blue (recommended)
//	COLOR_FORMAT_BGR: BGR, bit2 blue, bit1 green, bit0 red
//	COLOR_FORMAT_GRB: GRB, bit2 green, bit1 red, bit0 blue
#define COLOR_FORMAT	COLOR_FORMAT_RGB

// First VGA GPIO color signal (color output uses 4 output pins: B, G, R, and Y)
//	GP8 ... VGA B blue
//	GP9 ... VGA G green
//	GP10 ... VGA R red
//	GP11 ... VGA Y intensity
// Note: All four color output pins must form a continuous GPIO sequence.
#define VGA_GPIO_FIRST	8

// VGA GPIO control signals (control signals uses 4 output pins: HSYNC, VSYNC, DCLK and DE)
//	GP16 ... VGA HSYNC (negative, active HSYNC on LOW, used with modes 0, 1, 2 and 3)
//	GP17 ... VGA VSYNC (negative, active VSYNC on LOW, used with modes 1, 2 and 3)
//	GP18 ... VGA DCLK data clock 25 MHz (active data on rising edge from LOW to HIGH, used with modes 2, 3 and 4)
//	GP19 ... VGA DE data enable (active on HIGH, used with modes 2 and 4)
// Note: All used control output pins must form a continuous GPIO sequence.
// Note: Unused pins need not to be set, they will be ignored.
#define VGA_CTRL_HSYNC	16	// HSYNC output, used with modes 0, 1, 2 and 3
#define VGA_CTRL_VSYNC	17	// VSYNC output, used with modes 1, 2 and 3
#define VGA_CTRL_DCLK	18	// DCLK output, used with modes 2, 3 and 4
#define VGA_CTRL_DE	19	// DE output, used with modes 2 and 4

// Pin to control LED intensity via PWM
//	GP27 ... VGA LED PWM background intensity control (100..300 Hz)
// Note: Compatible with Pimoroni VGA board (AUDIO_R output).
#define USE_PWMLED	1	// 1 = use PWM LED support, or 0 = do not use PWM LED
#define PWMLED_GPIO	27	// PWM LED GPIO

// PWM sound output GPIO pin
//	GP28 ... PWM sound output
// Note: Compatible with Pimoroni VGA board (AUDIO_L output).
#define USE_PWMSOUND	1	// 1 = use PWM sound support, or 0 = do not use PWM sound
#define PWMSND_GPIO	28	// PWM output GPIO pin

// ========= derived configuration

// VGA port color pins
#define VGA_GPIO_NUM	4	// number of VGA color GPIOs signasl
#define VGA_GPIO_LAST	(VGA_GPIO_FIRST+VGA_GPIO_NUM-1) // last VGA color GPIO

// VGA port control pins
#if VGA_MODE == 0
#define VGA_CTRL_FIRST	VGA_CTRL_HSYNC		// first control pin
#define VGA_CTRL_NUM	1			// number of control pins
#elif VGA_MODE == 1
#define VGA_CTRL_FIRST	VGA_CTRL_HSYNC		// first control pin
#define VGA_CTRL_NUM	2			// number of control pins
#elif VGA_MODE == 2
#define VGA_CTRL_FIRST	VGA_CTRL_HSYNC		// first control pin
#define VGA_CTRL_NUM	4			// number of control pins
#elif VGA_MODE == 3
#define VGA_CTRL_FIRST	VGA_CTRL_HSYNC		// first control pin
#define VGA_CTRL_NUM	3			// number of control pins
#else
#define VGA_CTRL_FIRST	VGA_CTRL_DCLK		// first control pin
#define VGA_CTRL_NUM	2			// number of control pins
#endif

#define VGA_CTRL_LAST	(VGA_CTRL_FIRST+VGA_CTRL_NUM-1) // last VGA control GPIO

#define USE_HSYNC	(VGA_MODE < 4)				// use HSYNC output
#define USE_VSYNC 	((VGA_MODE > 0) && (VGA_MODE < 4))	// use VSYNC output
#define USE_DCLK	(VGA_MODE > 1)				// use DCLK output
#define USE_DE		((VGA_MODE == 2) || (VGA_MODE == 4))	// use DE output

// Pin to control PWM LED intensity.
#define PWMLED_SLICE	((PWMLED_GPIO >> 1) & 7) // PWM slice index
#define PWMLED_CHAN	(PWMLED_GPIO & 1) // PWM channel index

// PWM sound configuration
#define PWMSND_SLICE	((PWMSND_GPIO >> 1) & 7) // PWM slice index
#define PWMSND_CHAN	(PWMSND_GPIO & 1) // PWM channel index

// VGA PIO and state machines
#define VGA_PIO	pio0	// VGA PIO
#define VGA_SM		0	// VGA state machine

// VGA DMA channel
#define VGA_DMA_CB	0	// DMA control block of base layer
#define VGA_DMA_PIO	1	// DMA copy data to PIO (raises IRQ0 on quiet)

// VGA display resolution
#define WIDTH	640		// display width (must be multiply of 8)
#define HEIGHT	480		// display height
#define WIDTHBYTE (WIDTH/2)	// scanline length (byte offset between image rows, = 320)
#define FRAMESIZE (WIDTHBYTE*HEIGHT) // display frame size in bytes (=153600)

// VGA horizonal timings
#define VGA_CLKDIV	1	// SM divide clock ticks
#define VGA_CPP	5	// state machine clock ticks per pixel
#define VGA_SYSCPP	(VGA_CLKDIV*VGA_CPP) // system clock ticks per pixel (=5)

// 126 MHz timings
#define QUARTZ		12000000.0	// quartz frequency in Hz (= 12 MHz)
#define FBDIV		84		// FBDIV multiplier
#define VGA_VCO	(QUARTZ*FBDIV)		// PLL VCO frequency in Hz (= 12 * 84 = 1008 MHz)
#define VGA_PD1	4			// PLL PD1
#define VGA_PD2	2			// PLL PD2
#define SYSFREQ		(VGA_VCO/VGA_PD1/VGA_PD2) // system frequency in Hz (= 1008 / 4 / 2 = 126 MHz)
#define PIO_CLK		(SYSFREQ/VGA_CLKDIV) // PIO clock in Hz (= 126 / 1 = 126 MHz)
#define PIXEL_CLK	(PIO_CLK/VGA_CPP) // pixel clock in Hz (126 / 5 = 25.2 MHz)
#define PIXEL_TIME	(1.0/PIXEL_CLK)	// pixel time in seconds (1000/25.2 = 39.68 ns)

// VGA horizontal timings (HSYNC: 31.746 us = 31.50 kHz)
#define VGA_TOTAL	800		// total in pixels
#define VGA_FP		(16-VGA_SHIFTX)	// front porch in pixels
#if VGA_MODE < 3	// VGA mode (0=new analog, 1=old analog, 2=universal, 3=digital VH, 4=digital DE)
#define VGA_HSYNC	96		// horizontal sync in pixels - analog VGA
#else
#define VGA_HSYNC	10		// horizontal sync in pixels - digital VGA
#endif
#define VGA_BP		(VGA_TOTAL-VGA_FP-VGA_HSYNC-WIDTH) // back porch in pixels (= 48 analog or 134 digital)

// VGA vertical timings (VSYNC: 16.67 ms = 60 Hz)
#define VGA_VTOT	525	// total scanlines (= VGA_VSYNC + VGA_VBACK + VGA_VACT + VGA_VFRONT)
#define VGA_VFRONT	(10-VGA_SHIFTY)	// V front porch
#define VGA_VSYNC	2	// length of V sync (number of scanlines)
#define VGA_VACT	HEIGHT	// V active scanlines
#define VGA_VBACK	(VGA_VTOT-VGA_VFRONT-VGA_VSYNC-VGA_VACT) // V back porch
