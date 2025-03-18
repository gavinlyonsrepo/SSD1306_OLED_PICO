# CH1115 OLED README

[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ all ](https://github.com/gavinlyonsrepo/displaylib_1bit_PICO/blob/main/extra/image/all.jpg)

## Table of contents


  * [Overview](#overview)
  * [Output](#output)
  * [Hardware](#hardware)
  * [Software](#software)
	* [SPI](#spi)
	* [Bitmaps](#bitmaps)
  * [File system](#file-system)
  
## Overview

* Name : ER_OLEDM1_CH1115_PICO
* Description :

Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module driven by the CH1115 controller 

1. Raspberry pi PICO RP2040 library.      
2. Invert colour, vertical flip command, rotate  screen,  sleep, fade effect, horizontal scroll and contrast control  functions supported.
3. 10 ASCII fonts included.
4. Graphics support included.
5. Bitmaps supported.
6. Hardware SPI.
7. Polymorphic print class included to print many data types.

## Output

Output Screenshots.

![ p ](https://github.com/gavinlyonsrepo/displaylib_1bit_PICO/blob/main/extra/image/all.jpg)

## Hardware

CH1115 is a single-chip CMOS OLED driver with controller for organic light emitting diode dot-matrix graphic display system. CH1115 consists of 128 segments, 64 commons that can support a maximum display resolution of 128 X 64. It is designed for Common Cathode type OLED panel. ER-OLEDM1.09-1W-SPI is a White 1.09" OLED Display Panel with Breakout Board. This module is a combination of the two.(controller and OLED)

| pin no  | pin name | pin desc |
|---|---|---|
| 1 | Gnd | Ground |
| 2 | VCC | voltage supply |
| 3 | SCL | Serial clock input |
| 4 | SDA | Serial data input |
| 5 | RES | This pin is reset signal input. When the pin is low, initialization of the chip is executed. |
| 6 | DC | This pin is Data or Command control pin. |
| 7 | CS | This pin is chip select input. The chip is enabled for MCU comms  when CS is pulled low|
 
This wiring Diagram from the manufacturer datasheet showing hardware setup connected to a 3.3 volt system MCU.

![ wiring ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/wiring.jpg)

## Software

### SPI

Hardware SPI. The hardware SPI speed is set at 8MHz you can increase this if necessary by changing OLEDSPIsetup method arguments.
Spi0 is used in example files but can be changed by passing a different SPI channel. 

### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |  
| 1 | OLEDBitmap() |  Vertical |  Writes directly to screen , no buffer used. | 
| 2 | OLEDBufferScreen() |  Vertical  | For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

The drawBitmap method will return an error if : The Bitmap is completely off screen , 
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height calculation
( This helps prevents buffer overflow).
A vertical addressed Bitmap's height must be divisible by 8. eg, for a bitmap with width=128 and height=64.
Bitmap excepted size = 128 * (64/8) = 1024 bytes.
A horizontal addressed Bitmap's width must be divisible by 8. eg, for a bitmap with width=88 and height=48.
Bitmap excepted size = (88/8) * 48 = 528 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) 
See example file "BITMAP" for more details.


### User adjustments

When the user calls OLEDinit() to start OLED they can specify a contrast setting from 0x00 to 0xFF. Datasheet says 0x80 is default. User can also change contrast on the fly.
Screen can be disabled to enter a sleep type mode where OLED draws 500uA.
Screen pixel colour can be inverted and also screen can be vertically rotated. 

There is a "fade or breath display effect". Whose parameters(time brightness) can be adjusted by passing data to function see "OLEDfadeEffect" function header  in .cpp and datasheet for details.
default is 0x81.

There is a  Horizontal scroll effect. Whose parameters: Time-interval , direction , mode,
can be adjusted by passing data to function see "OLEDscrollSetup" function header in .cpp and datasheet for details. defaults are : 6 frames , right , continuous mode.

## Files system

| Src Files | Description  |
| ------ | ------ |
| ch1115.hpp | library header file  |
| ch1115.cpp |  library  source file  |
| display_graphics.hpp | graphics header file |
| display_graphics.cpp |  graphics source file |
| display_fonts.hpp |  font header file |
| display_fonts.cpp |  font data file |
| display_print.hpp | used for printing data types |
| display_print.cpp |  used for printing data types |

The example files are in example folder.
To build the one you want, edit the Cmaketxt file 
add_executable(${PROJECT_NAME} section, comment in one example file path and one only.
The default is the Hello world one.

| Examples files main.cpp  | Description |
| ------ | ------ | 
| hello | Hello world , Basic usage | 
| FPS_functions | FPS test & misc functions, rotate , scroll,  etc | 
| text_graphics  | Fonts + graphics  |
| bitmap| Shows use of bitmaps methods  | 
