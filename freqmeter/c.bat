@echo off
rem Compilation...

if exist freqmeter.pio.h del freqmeter.pio.h
..\_exe\pioasm.exe -o c-sdk src\freqmeter.pio freqmeter.pio.h
if not exist freqmeter.pio.h goto err

if exist vga.pio.h del vga.pio.h
..\_exe\pioasm.exe -o c-sdk ..\_vga\vga.pio vga.pio.h
if exist vga.pio.h goto start
:err
pause
goto stop

:start
call ..\_c1.bat

:stop
