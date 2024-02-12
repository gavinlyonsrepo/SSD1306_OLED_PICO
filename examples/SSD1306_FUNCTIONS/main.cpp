/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing use of various function
	
	Project Name: SSD1306_OLED_PICO
	
	@test
		-# Test 501 Enable and disable Screen (Sleep mode)
		-# Test 502 Invert screen color
		-# Test 503 Contrast screen
		-# Test 504 Scroll Screen
		-# Test 505 Rotate
*/

// === Libraries ===
#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"

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

// =============== Function prototypes ================
void SetupTest(void);
void myTests(void);
void EndTest(void);

// ======================= Main ===================
int main()
{
	SetupTest();
	myTests();
	EndTest();
}
// ======================= End of main  =====

// =============== Functions ================
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

void myTests()
{
	myOLED.OLEDclearBuffer(); // clear the buffer

	// Set text parameters
	myOLED.setFont(pFontDefault); 

	//  ** Test 501 OLED display enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("Disable test 501");
	printf("OLED Disable test 501\r\n");
	myOLED.OLEDupdate();

	busy_wait_ms(2000); 
	myOLED.OLEDEnable(0); 
	busy_wait_ms(2000); 
	myOLED.OLEDEnable(1); 
	busy_wait_ms(2000); 
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("Inverse test 502");
	printf("OLED Inverse test 502\r\n");
	myOLED.OLEDupdate();
	busy_wait_ms(1500);
	myOLED.OLEDInvert(1); // Inverted
	busy_wait_ms(1500);
	myOLED.OLEDInvert(0);
	busy_wait_ms(1500);

	// ** Test 503 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 503");
	printf("OLED Contrast test 503\r\n");
	myOLED.OLEDupdate();
	busy_wait_ms(1500);
	myOLED.OLEDFillScreen(0x77, 0); 
	myOLED.OLEDContrast(0x00);
	busy_wait_ms(1000);
	myOLED.OLEDContrast(0x80);
	busy_wait_ms(1000);
	myOLED.OLEDContrast(0xFF);
	busy_wait_ms(1000);
	myOLED.OLEDContrast(0x81);
	busy_wait_ms(1000);
	myOLED.OLEDclearBuffer();

	// ***** Test 504 Scroll **
	myOLED.setCursor(20,20 );
	myOLED.print("scroll test 504");
	printf("OLED scroll test 504\r\n");
	myOLED.OLEDupdate();
	busy_wait_ms(2500);

	myOLED.OLEDStartScrollRight(0, 0x0F);
	busy_wait_ms(3000);
	myOLED.OLEDStopScroll();
	
	myOLED.OLEDStartScrollLeft(0, 0x0F);
	busy_wait_ms(3000);
	myOLED.OLEDStopScroll();

	myOLED.OLEDStartScrollDiagRight(0, 0x07);
	busy_wait_ms(3000);
	myOLED.OLEDStopScroll();
 	
	myOLED.OLEDStartScrollDiagLeft(0, 0x07);
	busy_wait_ms(3000);
	myOLED.OLEDStopScroll();
 	
 	
 	// ** Test 505 rotate test **
 	
	myOLED.OLEDclearBuffer();
	printf("OLED Rotate test 505\r\n");
	myOLED.setRotation(displayBC_Degrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);

	myOLED.setRotation(displayBC_Degrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);

	myOLED.setRotation(displayBC_Degrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);

	myOLED.setRotation(displayBC_Degrees_0); //default normal
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	
	myOLED.OLEDFillScreen(0xF0, 0); // Clear the screen
	
	// ** end of tests **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(15, 25);
	myOLED.print("End of tests");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
}

void EndTest()
{
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
	printf("OLED SSD1306 :: End\r\n");
}
// =============== EOF ================
