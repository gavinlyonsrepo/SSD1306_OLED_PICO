/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for ERM19264_UC1609_PICO library, showing "hello world"
	@test
		-# Test 101 Hello World 128X64 screen
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib/erm19264.hpp"

/// @cond

// Screen settings
#define LCDcontrast 0x49 // Contrast
#define LCDRAMADDRCTRL 0x02  // RAM address control: Range 0-0x07, optional, default 0x02
#define myLCDwidth 192
#define myLCDheight 64
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

// instantiate  an LCD object
ERM19264 myLCD(myLCDwidth, myLCDheight);

// =============== Function prototype ================
void SetupTest(void);
void Test(void);
void EndTest(void);

// === Main ===
int main()
{
	SetupTest();
	Test();
	EndTest();
}
//=== end of main ===

// ===================== Function Space =====================
void SetupTest()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("LCD :: Start!\r\n");
	myLCD.LCDSPISetup(mySpiInst, mySPIBaudRate, dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);
	myLCD.LCDinit(LCDcontrast, LCDRAMADDRCTRL);
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : LCDSetBufferPtr Failed!\r\n");
		while (1)
		{
			busy_wait_ms(1000);
		}
	}								// Initialize the buffer
	myLCD.LCDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void Test()
{
	myLCD.LCDclearBuffer();
	myLCD.setFont(pFontDefault);
	myLCD.setCursor(5, 5);
	myLCD.print("Hello World!");
	myLCD.LCDupdate();
	busy_wait_ms(7000);
}

void EndTest()
{
	myLCD.LCDPowerDown(); // Switch off display
	spi_deinit(mySpiInst);	// Turn off SPI
	printf("LCD :: End\r\n");
}

/// @endcond
