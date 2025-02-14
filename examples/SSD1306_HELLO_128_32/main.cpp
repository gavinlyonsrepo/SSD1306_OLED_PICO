/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing  "hello world" basic use case
	for a 128X32 size screen
	
	Project Name: SSD1306_OLED_PICO

	@test
		1. Test 102 Hello World 128X32 screen
*/

// === Libraries ===
#include <cstdio>

#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"

// Screen settings
#define myOLEDwidth  128
#define myOLEDheight 32
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // eg 512 bytes = 128 * 32/8
uint8_t screenBuffer[myScreenSize]; // Define a buffer to cover whole screen  128 * 64/8

// I2C settings
const uint16_t I2C_Speed = 100;
const uint8_t I2C_GPIO_CLK = 19;
const uint8_t I2C_GPIO_DATA = 18;

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

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
	while(myOLED.OLEDbegin(SSD1306::SSD1306_ADDR, i2c1,  I2C_Speed, I2C_GPIO_DATA, I2C_GPIO_CLK) != DisplayRet::Success)
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
	myOLED.setFont(pFontGroTesk); 
	myOLED.setCursor(0, 0);
	myOLED.print("HELLO WD");
	myOLED.OLEDupdate();  
	busy_wait_ms(7000);
}

void EndTest()
{
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
	printf("OLED SSD1306 :: End\r\n");
}
// ============== EOF =========
