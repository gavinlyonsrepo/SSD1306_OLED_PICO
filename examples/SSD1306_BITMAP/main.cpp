/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library,showing how to use  bitmap 128X64 screen 
	
	Project Name: SSD1306_OLED_PICO
	
	@test
		-# Test 301 bigImage 64x64 pixels
		-# Test 302 Small icons 16x8 pixels
		-# Test 303 full screen image 128x64 pixels 
		-# Test 304 image 40x16 pixels
		-# Test 305 image 88x48 pixels
*/

// === Libraries ===
#include <cstdio>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"
#include "ssd1306/Bitmap_test_data.hpp"

// Screen settings
#define myOLEDwidth  128
#define myOLEDheight 64
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize]; // Define a buffer to cover whole screen  128 * 64/8

// I2C settings
const uint16_t I2C_Speed = 100;
const uint8_t I2C_Address = 0x3C;

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

// =============== Function prototype ================
void SetupTest(void);
void myLoop(void);
void Test305(void);
void Test304(void);
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
	busy_wait_ms(500);
	printf("OLED SSD1306 :: Start!\r\n");
	while(myOLED.OLEDbegin(I2C_Address,i2c1,  I2C_Speed, 18, 19) != true)
	{
		printf("SetupTest ERROR : Failed to initialize OLED!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)/sizeof(uint8_t)) != 0)
	{
		printf("SetupTest : ERROR : OLEDSetBufferPtr Failed!\r\n");
		while(1){busy_wait_ms(1000);}
	} // Initialize the buffer
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
	Test304();
	Test305();
}

void Test301(void)
{

	printf("OLED Test 301 64x64 pixel Image\r\n");
	myOLED.OLEDBitmap(0, 0 , 64, 64, backupicon64x64, false,sizeof(backupicon64x64)/sizeof(uint8_t));
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test302(void)
{
	printf("OLED Test 302 16x8 pixel image\r\n");
	myOLED.OLEDBitmap(70, 10, 16, 8,  SignalIcon, false,sizeof(SignalIcon)/sizeof(uint8_t));
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}

void Test303(void)
{
	printf("OLED Test 303 128x64 pixel image, inverted color\r\n");
	myOLED.OLEDBitmap(0, 0, myOLEDwidth, myOLEDheight, fullscreenBitmap, true,sizeof(fullscreenBitmap)/sizeof(uint8_t));
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test304(void)
{
	printf("OLED Test 304 40x16 pixel image\r\n");
	myOLED.OLEDBitmap(5, 5, 40, 16,  SunTextImage, false, sizeof(SunTextImage)/sizeof(uint8_t));
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}
void Test305(void)
{
	printf("OLED Test 305 88x48 pixel image\r\n");
	myOLED.OLEDBitmap(0, 0, 88, 48, backupicon88x48, false, sizeof(backupicon88x48)/sizeof(uint8_t));
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}
