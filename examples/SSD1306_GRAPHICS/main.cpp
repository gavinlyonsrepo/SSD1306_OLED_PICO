/*!
	@file /main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing use of graphics.
	
	Project Name: SSD1306_OLED_PICO
	
	@test
		-# Test 901 Graphics test
*/

// === Libraries ===
#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64

#define DisplayDelay1 4000
#define DisplayDelay2 0

const uint16_t I2C_Speed = 100; 
const uint8_t I2C_Address = 0x3C;
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

// =============== Function prototype ================
void SetupTest(void);
void DisplayGraphics(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv)
{
	SetupTest();
	DisplayGraphics();
	EndTest();
}
// ======================= End of main  ===================

// ===================== Function Space =====================

void SetupTest() 
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(50);
	printf("OLED SSD1306 :: Start!\r\n");
	while(myOLED.OLEDbegin(I2C_Address,i2c1,  I2C_Speed, 18, 19) != true)
	{
		printf("OLED SSD1306 :: Failed to initialize OLED.!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void EndTest()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLEDdeI2CInit();
	printf("OLED SSD1306 :: End!\r\n");
}

// Function to display Graphics.
void  DisplayGraphics()
{
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	// Define a full screen buffer
	uint8_t  screenBuffer[(myOLEDwidth * (myOLEDheight / 8)) + 1];
	myOLED.buffer = (uint8_t*) &screenBuffer;  
	myOLED.OLEDclearBuffer(); // Clear the buffer
	
	printf("OLED Test 901 graphics test \r\n");
	bool colour = 1;
	uint8_t count = 0;
	
	while (count < 15)
	{
		colour = !colour;

		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, WHITE);
		myOLED.drawFastVLine(62, 0, 64, WHITE);
		myOLED.drawFastHLine(0, 32, 128, WHITE);

		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, WHITE);

		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawRect(105, 10, 15, 15, WHITE);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, WHITE);
			myOLED.OLEDupdate();
			busy_wait_ms(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
}
