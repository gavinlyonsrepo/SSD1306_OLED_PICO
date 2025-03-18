/*!
	@file main.cpp
	@brief Description library test file, carries out hello world test , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 101 Hello World Hardware SPI
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
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

// Hardware SPI setup
NOKIA_5110 myLCD(myLCDwidth, myLCDheight);

// ************ Function Headers ********
bool SetupTest(void);
void Test(void);
void EndTests(void);

// ************  MAIN ***************

int main(void)
{
	if (SetupTest()) Test();
	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device
bool SetupTest()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("LCD Start!\r\n");
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

void Test(void)
{
	printf("Nokia 5110 Hardware SPI, Hello World Test.\n");
	char testStr[]= "Hello World";
	myLCD.setCursor(0, 0);
	myLCD.print(testStr);
	myLCD.LCDupdate();
	busy_wait_ms(5000);
}


/// @endcond
