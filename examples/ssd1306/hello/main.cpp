/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing  "hello world" basic use case
	@test
		1. Test 101 Hello World 128X64 screen
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "displaylib/ssd1306.hpp"

/// @cond

// Screen settings
#define myOLEDwidth  128
#define myOLEDheight 64
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize];

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

// I2C settings
const uint16_t SPEED = 100;
const uint8_t CLK_PIN = 19;
const uint8_t DATA_PIN = 18;

// =============== Function prototype ================
void SetupTest(void);
void Test(void);
void EndTest(void);

// ======================= Main ===================
int main() 
{
	SetupTest();
	Test();
	EndTest();
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void SetupTest() 
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("OLED SSD1306 :: Start!\r\n");
	while(myOLED.OLEDbegin(SSD1306::SSD1306_ADDR, i2c1,  SPEED, DATA_PIN, CLK_PIN) != DisplayRet::Success)
	{
		printf("SetupTest ERROR : Failed to initialize OLED!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : OLEDSetBufferPtr Failed!\r\n");
		while(1){busy_wait_ms(1000);}
	} // Initialize the buffer
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void Test() 
{
	myOLED.OLEDclearBuffer(); 
	myOLED.setFont(pFontDefault);
	myOLED.setCursor(5,5);
	myOLED.print("Hello World!");
	myOLED.OLEDupdate();  
	busy_wait_ms(7000);
}

void EndTest()
{
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
	printf("OLED SSD1306 :: End\r\n");
}

/// @endcond
