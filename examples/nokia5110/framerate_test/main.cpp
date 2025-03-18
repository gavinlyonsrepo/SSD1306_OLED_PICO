/*!
	@file examples/nokia5110/framerate_test/main.cpp
	@brief Description library test file,carries out FPS test  , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@details 8 Mhz 318 frames 03-2025 recorded
	@test
		-# Test 601 FPS test (frame rate per second)
*/

// libraries
#include "pico/stdlib.h"
#include "displaylib/nokia5110.hpp"

/// @cond

// Test Setup
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
uint16_t count = 0;
bool colour = 1;
uint64_t  previousCounter = 0;
uint16_t countLimit = 1500;

/// Screen settings
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
uint32_t mySPIBaudRate = 1000;
spi_inst_t *mySpiInst = spi0;

NOKIA_5110 myLCD(myLCDwidth, myLCDheight);

// =============== Function prototype ================
bool Setup(void);
void DisplayFPS(void);
void EndTests(void);

// ======================= Main ===================
int main()
{

	if (Setup()) {DisplayFPS();}
	EndTests();
	return 0;
}
// ======================= End of main  ===================

bool Setup(void)
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("LCD FPS test Start!\r\n");
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


// Test 601 FPS frames per second test
void DisplayFPS()
{
	myLCD.setFont(pFontDefault);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 5000;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	bool colour = 1;

	while (count >= 5)
	{
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			colour = !colour;
		}
		currentFramerate++;
		count--;

		//  ** Code to test **
		myLCD.setCursor(0, 0);
		myLCD.print("504 bytes");
		myLCD.setCursor(0, 10);
		myLCD.print("G Lyons");
		myLCD.setCursor(0, 20);
		myLCD.fillRect(0, 20, 48, 10, myLCD.BG_COLOR);
		myLCD.print(count);
		myLCD.setCursor(0, 30);
		myLCD.print(fps);
		myLCD.print(" fps");
		myLCD.setCursor(0, 40);
		myLCD.print(__LibVerNum__); 

		myLCD.fillRect(60, 1, 20, 20, colour);
		myLCD.drawRect(60, 1, 20, 20, !colour);
		myLCD.fillCircle(60, 35, 10, !colour);
		myLCD.drawCircle(60, 35, 10, colour);
		myLCD.LCDupdate();
	}
}

/// @endcond

