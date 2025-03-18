/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for ER_OLEDM1_CH1115_PICO library, showing "hello world"
	@test
		-# Test 101 Hello World 128X64 screen
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib/ch1115.hpp"

/// @cond

// Screen settings
#define OLEDcontrast 0x80 // Contrast 00 to FF , 0x80 is default. user adjust
#define myOLEDwidth 128
#define myOLEDheight 64
#define myScreenSize (myOLEDwidth * (myOLEDheight / 8)) // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize];						// Define a buffer to cover whole screen  128 * 64/8

// GPIO
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

// SPI configuration
uint32_t mySPIBaudRate = 8000;
spi_inst_t *mySpiInst = spi0;

// instantiate  an OLED object
ERMCH1115 myOLED(myOLEDwidth, myOLEDheight);

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
	printf("OLED Ch1115 :: Start!\r\n");
	myOLED.OLEDSPISetup(mySpiInst, mySPIBaudRate, dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);
	myOLED.OLEDinit(OLEDcontrast);
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != 0)
	{
		printf("SetupTest : ERROR : OLEDSetBufferPtr Failed!\r\n");
		while (1)
		{
			busy_wait_ms(1000);
		}
	}								// Initialize the buffer
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void Test()
{
	myOLED.OLEDclearBuffer();
	myOLED.setFont(pFontDefault);
	myOLED.setCursor(5, 5);
	myOLED.print("Hello World!");
	myOLED.OLEDupdate();
	busy_wait_ms(7000);
}

void EndTest()
{
	myOLED.OLEDPowerDown(); // Switch off display
	spi_deinit(mySpiInst);	// Turn off SPI
	printf("OLED CH1115 :: End\r\n");
}
/// @endcond
