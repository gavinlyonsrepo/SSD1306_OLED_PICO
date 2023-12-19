/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing fps frame rate per second
	
	Project Name: SSD1306_OLED_PICO
	
	@test
		-# Test 601 FPS test frame rate per second
*/

// === Libraries ===
#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
const uint16_t I2C_Speed = 100; 
const uint8_t I2C_Address = 0x3C;
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

// vars for the test
uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight / 8) ]; 

// =============== Function prototype ================

void SetupTest(void);
void DisplayFPS(void);
void EndTests(void);

// ======================= Main ===================
int main()
{
	SetupTest();

	// Define a buffer to cover whole screen 
	uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)+1]; 
 	myOLED.buffer = (uint8_t*) &screenBuffer;  // set that to library buffer pointer
	myOLED.OLEDclearBuffer(); 
	
	DisplayFPS();
	EndTests();
}
// ======================= End of main  ===================

void EndTests()
{
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
	printf("OLED SSD1306 :: End\r\n");
}

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
	myOLED.OLEDFillScreen(0x01, 0); // splash screen bars
	busy_wait_ms(1000);
}

// Test 17 FPS frames per second test
void DisplayFPS()
{
	printf("OLED SSD1306 :: Frame rate per second test , ends at 1000\r\n");
	// Set text parameters
	myOLED.setFont(pFontDefault);

	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	bool colour = 1;

	while (1)
	{
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			colour = !colour;
			if (count >= 1000)
				return; // end if count gets to 3000
		}
		currentFramerate++;
		count++;

		myOLED.OLEDclearBuffer();
		myOLED.setCursor(0, 0);
		myOLED.print("SSD1306");

		myOLED.setCursor(0, 20);
		myOLED.print("G Lyons");

		myOLED.setCursor(0, 30);
		myOLED.print(count);

		myOLED.setCursor(0, 40);
		myOLED.print(fps);
		myOLED.print(" fps");
		myOLED.setCursor(0, 50);
		myOLED.print("V 1.0.0");
		myOLED.drawFastVLine(64, 0, 63, WHITE);

		myOLED.fillRect(70, 10, 20, 20, colour);
		myOLED.fillCircle(110, 20, 10, !colour);
		myOLED.drawRoundRect(80, 40, 40, 20, 10, WHITE);

		myOLED.OLEDupdate();
	}
}