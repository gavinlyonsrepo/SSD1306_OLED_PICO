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
#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"

#define myOLEDwidth  128
#define myOLEDheight 32
const uint16_t I2C_Speed = 100; 
const uint8_t I2C_Address = 0x3C;
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object 

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
	busy_wait_ms(100);
	printf("OLED SSD1306 :: Start!\r\n");
	while(myOLED.OLEDbegin(I2C_Address,i2c1,  I2C_Speed, 18, 19) != true)
	{
		printf("OLED SSD1306 :: Failed to initialize OLED.!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
}

void Test() 
{
	// Define a buffer to cover whole screen 
	uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
 	myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer
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
