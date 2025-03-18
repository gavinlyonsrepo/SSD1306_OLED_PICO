/*!
	@file examples/nokia5110/bitmap/main.cpp
	@brief Description library test file, carries out series of Bitmap tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 301 Bitmap full screen
		-# Test 302 Vertically addressed data small bitmaps
		-# Test 303 horizontal addressed data small bitmaps

*/

// libraries
#include "pico/stdlib.h"
#include "displaylib/bitmap_test_data.hpp"
#include "displaylib/nokia5110.hpp"

/// @cond

// Screen settings
//  LCD
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14
#define myLCDwidth 84
#define myLCDheight 48
#define myScreenSize (myLCDwidth * (myLCDheight / 8)) 
uint8_t screenBuffer[myScreenSize];	

// GPIO
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

// SPI configuration
uint32_t mySPIBaudRate = 8000;
spi_inst_t *mySpiInst = spi0;

NOKIA_5110 myLCD(myLCDwidth, myLCDheight);

// ************ Function Headers ****************
bool Setup(void);
void screenReset(void);
void EndTests(void);

void testBitMap(void);
void testCustomChar(void);
void testSmallBitmap(void);
void testSmallBitmapHa(void);

// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	myLCD.LCDfillScreen(0x00);
	myLCD.setDrawBitmapAddr(true);
	testBitMap();
	testSmallBitmap();
	testSmallBitmapHa();

	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device

// Initialize the device
bool Setup()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("LCD Bitmap test Start!\r\n");
	myLCD.LCDSPISetup(mySpiInst, mySPIBaudRate, dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);
	myLCD.LCDInit(LCD_INV, LCD_CST, LCD_BIAS);
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : LCDSetBufferPtr Failed!\r\n");
		return false;
	}								// Initialize the buffer
	myLCD.LCDfillScreen(0xF0); // splash screen bars
	busy_wait_ms(1000);
	return true;
}

void EndTests(void)
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	printf("LCD End\n");
}

void testBitMap(void)
{
	printf("Test 301 Bitmap lighting image X2 \n");
	myLCD.drawBitmap(0,0, lightingImage, myLCDwidth, myLCDheight/2, myLCD.BG_COLOR, myLCD.FG_COLOR); // Splash screen
	myLCD.drawBitmap(0,24, lightingImage, myLCDwidth, myLCDheight/2, myLCD.FG_COLOR , myLCD.BG_COLOR);
	screenReset();
}

void screenReset(void) {
	myLCD.LCDupdate();
	busy_wait_ms(5000);
	myLCD.LCDclearBuffer();
}


void testSmallBitmap(void)
{
	printf("Test 302 Vertically addressed data bitmaps \n");
	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, SignalIconVa, 16, 8, myLCD.BG_COLOR, myLCD.FG_COLOR);
	myLCD.drawBitmap(60, 5, TemperatureImageVa, 16, 16, myLCD.BG_COLOR,  myLCD.FG_COLOR);
	screenReset();
}

void testSmallBitmapHa(void)
{
	printf("Test 303 Horizontally addressed data bitmaps \n");
	myLCD.setDrawBitmapAddr(false);
	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, BatIcon, 16, 8, myLCD.BG_COLOR, myLCD.FG_COLOR);
	myLCD.drawBitmap(25, 5, BatIcon, 16, 8, myLCD.FG_COLOR,myLCD.BG_COLOR);
	screenReset();
}
// *************** EOF ****************

/// @endcond
