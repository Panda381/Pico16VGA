
Pico16VGA - 16-color VGA display on Raspberry Pico
--------------------------------------------------
Miroslav Nemecek, panda38.sweb.cz, Panda38@seznam.cz

project: http://www.breatharian.eu/hw/pico16vga/index_en.html

How to install:
- Install compiler gcc-arm-none-eabi-10-2020 into folder C:\ARM10
- Unpack this package into some folder

If you have gcc compiler installed in another folder,
 edit PATH in files _c1.bat and _boot2\c.bat

I recommend FAR Manager as program editor - it contains
 very useful console output.

To compile demo project, run c.bat (or c_all.bat to compile all).
To clear demo project, run d.bat (or d_all.bat to clear all).
To write demo project to the Pico board:
 - press and hold BOOTSEL button on the Pico board
 - connect USB cable
 - release BOOTSEL button
 - edit destination drive (now there is r:) in file _e1.bat, command:
       copy /b program.uf2 r:
   and then run e.bat 
   ... or drag and drop file 'program.uf2' to Pico drive.


How to connect VGA display
--------------------------
GP8 ... VGA blue ... resistor 330 to Blue output
GP9 ... VGA green ... resistor 330 to Green output
GP10 ... VGA red ... resistor 330 to Red output
GP11 ... VGA intensity ... 3 resistors 1K0 to outputs Red, Green and Red
GP16 ... VGA HSYNC synchronization ... resistor 100 to HSYNC
GP17 ... VGA VSYNC synchronization ... resistor 100 to VSYNC

GP28 ... PWM sound output (RC LOW filter 1K5 resistor + 10n capacitor)


How to connect Pico to PC with serial port
------------------------------------------
Some demo projects are prepared to control via USB serial port adapter.
USB interface is selected in Makefile with this parameter:
DEFINE += -DPICO_STDIO_USB=1

Instead of it, you can use serial port. Change Makefile parameter to:
DEFINE += -DPICO_STDIO_UART=1

To connect Pico to PC via serial port, you need either convertor from
RS232 COM port from 3.3V levels of Pico board, or you need convertor
to USB port. Good choise is USB-serial adaptor PL2303TA.

   How to connect UART to Pico:
                    +---USB---+
   <-- UART0_TX GP0 |.1    40.|VBUS --- +5V power output to USB device
   --> UART0_RX GP1 |.2    39.|VSYS ---|<|--- +5V power input
   ---          GND |.3    38.| GND  K diode A
                GP2 |.4    37.| 3V3_EN
                GP3 |.5    36.| 3V3_OUT
                      ......
   Note: in simple case, you need not diode, you can connect +5V directly on board.

   connect USB/UART adaptor PL2303TA:
       red ......... +5V VSYS (39)
       black ....... GND (3)
       white RxD ... UART0_TX GP0 (1)
       green TxD ... UART0_RX GP1 (2)

   You need console program (as PuTTY) to see demo text.
   Setup port to 115200 Baud, 8 bits, 1 stop bit, no parity,
   flow control None.

   In Device Manager, USB adaptor can be found under name:
           "Prolific USB-to-Serial Comm Port (COM10)".


Pico16VGA examples
----------------
Analyzer - Simple logic analyzer with sampling rate 126 MHz, with noise test pattern generator.

Balloons - sprite demo, floating balloons

Draw - draw graphics demo

Earth - rotating Earth globe

Eggs - game of Eggs+Chickens+Hens, variant of the Reversi game
	keyboard control: 'L' right, 'I' up, 'J' left, 'K' down, 'H' help,
	'Q' quit, 'P' 2 players, 'D' demo, space stone, Enter ok

Fifteen - game
	keyboard control: 'L' right, 'I' up, 'J' left, 'K' down, 'Q' new game

Flag - animation of flag fluttering in the wind

FreqMeter - Frequency meter in the range of 2.5 Hz to 63 MHz, with 4 digit accuracy and with display of signal waveform.

Hello - simple demonstration, print text "Hello world!" on the screen

Life - game, reproducing cells
	keyboard control: 'L' right, 'I' up, 'J' left, 'K' down, 'C' copy, 'V' paste, 'D' clear,
		space flip state, Enter run life, 0..9 select slot

Lines - moving line patterns

Mandelbrot - fractal pattern generator
	keyboard control: 'E' up, 'S' left, 'D' right, 'X' down, 'Q' zoom in, 'A' zoom out,
	'I' integer arithmetics, 'F' float arithmetics, 'B' double arithmetics,
	'O' decrease number of steps, 'P' increase number of steps, 'U' increase system clock,
	'T' decrease system clock, space redraw

Matrix Rain - simulation of Matrix code rain

Maze - game, simple maze with maze generator
	keyboard control: 'J' left, 'I' up, 'L' right, 'K' down, 'H' help (show door)

Monoscope - show monoscope

Pi - calculate Pi at 4800 digits

Pixels - random pixels

Raytrace - ray tracing 3D demonstration of mirror spheres

RGBTest - color test

Spheres - draw random spheres

Spots - draw random dot spots

Twister - deformable block covered with 4 textures

Water surface - simulation of a rippling water surface
