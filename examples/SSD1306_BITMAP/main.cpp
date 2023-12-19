/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library,showing how to use  bitmap 128X64 screen 
	
	Project Name: SSD1306_OLED_PICO
	
	@test
		-# Test 301 bigImage 64x64 pixels
		-# Test 302 Small icons 16x8 pixels
		-# Test 303 full screen image 128x64 pixels 
*/

// === Libraries ===
#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"
#include "ssd1306/Bitmap_test_data.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
uint8_t fullscreenBuffer[1024]; // buffer 128 * 64/8
const uint16_t I2C_Speed = 100;
const uint8_t I2C_Address = 0x3C;
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

// =============== Function prototype ================
void SetupTest(void);
void myLoop(void);
void Test303(void);
void Test302(void);
void Test301(void);
void EndTests(void);

// ======================= Main ===================
int main()
{
	SetupTest();
	myLoop();
	EndTests();
}
// ======================= End of Main ===================


// ======================= Function space ===================
void SetupTest() 
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(100);
	printf("OLED SSD1306 :: Start!\r\n");
	while(myOLED.OLEDbegin(I2C_Address,i2c1,  I2C_Speed, 18, 19) != true)
	{
		printf("OLED SSD1306 :: Failed to initialize OLED.!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void EndTests()
{
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
	printf("OLED SSD1306 :: End\r\n");
}

void myLoop()
{
	Test301();  
	Test302();  
	Test303();  
}

void Test301(void)
{
	printf("OLED Test 301 64x64 pixel Image\r\n");
	myOLED.buffer = (uint8_t*) &fullscreenBuffer; // buffer to the pointer
	myOLED.OLEDBitmap(0, 0 , 64, 64, bigImage, false);
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test302(void)
{
	printf("OLED Test 302 16x8 pixel image\r\n");
	myOLED.OLEDBitmap(70, 10, 16, 8,  SignalIcon, false);
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}

void Test303(void)
{
	printf("OLED Test 303 128x64 pixel image, inverted color\r\n");
	myOLED.OLEDBitmap(0, 0, myOLEDwidth, myOLEDheight, fullscreenBitmap, true);
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
}

