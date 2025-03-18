# ERM19264 LCD README

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [SPI](#spi)
	* [Bitmaps](#bitmaps)
	* [User adjustments](#user-adjustments)
	* [File system](#file-system)
  * [Notes and Issues](#notes-and-issues)

## Overview

* Name : ERM19264_UC1609
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller)
* Description : 

1. Raspberry pi PICO RP2040 library.       
2. Inverse, Scroll, rotate and contrast control. 
3. 10 ASCII fonts included.
4. Graphics class included.
5. Sleep mode.
6. Bitmaps supported.
7. Hardware SPI options
8. Polymorphic print class included to print many data types.
9. Port of my arduino [library](https://github.com/gavinlyonsrepo/ERM19264_UC1609) , More details there. 

* Author: Gavin Lyons
* Developed on
	1. Raspberry pi PICO RP2040
	2. SDK C++ compiler G++ for arm-none-eabi
	3. CMAKE , VScode


## Hardware

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The backlight control is left up to user. The CLK and Data lines are fixed to the SPI interface but any GPIO can be used for reset, DC and CS. 

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)
 
1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The library was tested on 1 and 2. 
This wiring Diagram from the manufacturer showing hardware setup connected to an ~8051 MCU, showing both 3.3 volt and 5 volt systems. 

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)

## Software

### SPI

Hardware SPI. The hardware SPI speed is set at 8MHz you can change this if necessary by changing LCDSPISetup method arguments.
Spi0 is used in example files but can be changed by passing a different SPI channel.(Spi0 or Spi1) 


### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |  
| 1 | LCDBitmap() |  Vertical |  Writes directly to screen , no buffer used. | 
| 2 | LCDBufferScreen() |  Vertical  | For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

The drawBitmap method will return an error if : The Bitmap is completely off screen , 
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height calculation
( This helps prevents buffer overflow).
A vertical addressed Bitmap's height must be divisible by 8. eg, for a  bitmap with width=128 and height=64.
Bitmap excepted size = 128 * (64/8) = 1024 bytes.
A horizontal addressed Bitmap's width must be divisible by 8. eg, for a bitmap with width=88 and height=48.
Bitmap excepted size = (88/8) * 48 = 528 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) 
See example file "BITMAP" for more details.


### User adjustments

If the LCD does not initialize correctly.
Try adjusting the RAM address control setting from 0x02(default) to 0x01.
See github issue 4(arduino Source port) for details. Users can adjust setting in the "begin" method argument list. This setting changes bits 2-0 in AC register, see diagram below for details.

![ ERM19264 ram image ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/ram.png)

When the user calls LCDinit() to start LCD they can specify a contrast setting from 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT). Lower contrast works better on the blue color version.

It is also possible for user to change LCD bias, Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker. See Data sheet for range of values here. Defaults where found to be fine during all testing of this library.

| Parameter | default Values |  Define | Register bits |
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + Internal VLCD (7x charge pump) |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49 |  Set by user with LCDinit| PM 7:0 |
| Ram Address Control | 0x02 |  Set by user with LCDinit  | AC 2:0 |


### File system

| Src Files | Description  |
| ------ | ------ |
| erm19264.hpp | library header file  |
| erm19264.cpp |  library  source file  |
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
| text_graphics | Text + graphics  tests |
| FPS_functions | FPS test & misc functions, rotate , scroll,  etc |
| bitmap | Shows use of bitmaps methods  |

## Notes and Issues


