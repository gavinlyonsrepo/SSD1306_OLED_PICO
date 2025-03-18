/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for Sh1106 OLED display, showing  "hello world" basic use case
	@test
		1. Test 100 Hello World 128x64 screen
*/

#include <cstdio>
#include "pico/stdlib.h"
#include "displaylib/sh110x.hpp"

/// @cond

// Screen Size 
#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
uint8_t  screenBuffer[FULLSCREEN];

SH110X myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate a OLED object

// I2C settings
const uint8_t ADDR = 0x3C; // I2C address
const uint16_t SPEED = 100; // I2C clock speed in kilo hertz
const uint8_t CLK_PIN = 19;
const uint8_t DATA_PIN = 18;

// Reset only needed if Hardware reset pin is present on device and used
int _RESET_PIN = -1; // set to -1 if not used


// =============== Function prototype ================
bool SetupTest(void);
void TestLoop(void);
void EndTest(void);

// ======================= Main ===================
int main(void)
{
	if(SetupTest()) TestLoop();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool SetupTest() 
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("OLED SH1106 :: Start!\r\n");
	while(myOLED.OLEDbegin(myOLED.SH1106_IC, _RESET_PIN, ADDR, i2c1, 
					SPEED, DATA_PIN, CLK_PIN) != DisplayRet::Success)
	{
		printf("SetupTest ERROR : Failed to initialize OLED!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : OLEDSetBufferPtr Failed!\r\n");
		return false;
	} // Initialize the buffer
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
	return true;
}

void EndTest()
{
	myOLED.OLEDPowerDown();
	myOLED.OLED_I2C_OFF();
	printf("OLED Test End\r\n");
}

void TestLoop()
{	
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer) != DisplayRet::Success) return;
	myOLED.OLEDclearBuffer();
	myOLED.setFont(pFontDefault);
	myOLED.setCursor(10, 10);
	myOLED.print("Hello World.");
	myOLED.OLEDupdate();
	busy_wait_ms(5000);
}

// ============== EOF =========

/// @endcond
