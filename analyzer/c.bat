@echo off
rem Compilation...

if exist analyzer.pio.h del analyzer.pio.h
..\_exe\pioasm.exe -o c-sdk src\analyzer.pio analyzer.pio.h
if not exist analyzer.pio.h goto err

if exist vga.pio.h del vga.pio.h
..\_exe\pioasm.exe -o c-sdk ..\_vga\vga.pio vga.pio.h
if exist vga.pio.h goto start
:err
pause
goto stop

:start
call ..\_c1.bat

:stop
