/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for ER_OLEDM1_CH1115_PICO library, showing use of various functions + FPS test

	@test
		-# Test 501 Enable and disable Screen (Sleep mode)
		-# Test 502 Invert screen color
		-# Test 503 Flip screen with OLED command
		-# Test 504 Contrast screen
		-# Test 505 Scroll Screen
		-# Test 506 Fade breath effect
		-# Test 507 Rotate Screen by rotating buffer
		-# Test 601 Frame rate per second test. FPS.
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
uint8_t screenBuffer[myScreenSize];	// Define a buffer to cover whole screen  128 * 64/8

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

// === Function prototypes ===
void DisplayMiscTests(void);
void Test507(void);
void DisplayFPS(void);
void SetupTest(void);
void EndTest(void);

// === Main ===
int main(void)
{
	SetupTest();
	DisplayMiscTests();
	Test507();
	DisplayFPS();
	EndTest();
}
// === END OF MAIN ===

// === Function Space ===

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
	}																// Initialize the buffer
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void EndTest()
{
	myOLED.OLEDPowerDown(); // Switch off display
	spi_deinit(mySpiInst);	// Turn off SPI
	printf("OLED CH1115 :: End\r\n");
}

void DisplayMiscTests()
{
	myOLED.setFont(pFontDefault);

	//  ** Test 501 OLED enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("OLED Disable test 501");
	myOLED.OLEDupdate();

	busy_wait_ms(3000);		// display message
	myOLED.OLEDEnable(0); // turn off display (sleep mode 500uA measured)
	busy_wait_ms(3000);		// wait for 3
	myOLED.OLEDEnable(1); // turn on
	busy_wait_ms(2000);		// display message
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("OLED Inverse test 502");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	myOLED.OLEDInvert(1); // Inverted
	busy_wait_ms(5000);
	myOLED.OLEDInvert(0);
	busy_wait_ms(1000);

	// ** Test 503 OLED flip **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Flip LCD built-in     command test 503");
	myOLED.OLEDupdate();
	busy_wait_ms(4000);
	myOLED.OLEDFlip(1);
	busy_wait_ms(4000);
	myOLED.OLEDFlip(0);
	busy_wait_ms(2000);

	// ** Test 504 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 504");
	myOLED.OLEDupdate();
	busy_wait_ms(2500);
	myOLED.OLEDFillScreen(0x77, 0); // fill screen

	myOLED.OLEDContrast(0x00);
	busy_wait_ms(1000);
	myOLED.OLEDContrast(0x80);
	busy_wait_ms(1000);
	myOLED.OLEDContrast(0xFF);
	busy_wait_ms(1000);
	myOLED.OLEDContrast(OLEDcontrast);
	busy_wait_ms(1000);
	myOLED.OLEDclearBuffer();

	// ** Test 505 OLED scroll **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 40);
	myOLED.print("Scroll test 505");
	myOLED.OLEDupdate();
	busy_wait_ms(2500);

	// See .cpp file for more info on these parameters.
	uint8_t timeInterval = 0x00;		// 6 frames 0x00 - 0x07
	uint8_t scrollDirection = 0x26; // right 0x26 or 0x27
	uint8_t scrollMode = 0x28;			// continuous 0x28-0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection, scrollMode);
	myOLED.OLEDscroll(1); // start scroll
	busy_wait_ms(15000);
	myOLED.OLEDscroll(0); // stop Scroll

	busy_wait_ms(1);

	timeInterval = 0x02;		// 64 frames , 0x00 - 0x07
	scrollDirection = 0x27; // left , 0x26 or 0x27
	scrollMode = 0x29;			// continuous 0x28 one shot 0x29 , one col 0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection, scrollMode);
	myOLED.OLEDscroll(1); // start
	busy_wait_ms(15000);
	myOLED.OLEDscroll(0); // stop

	// ** TEST 506 fade ~ breath effect **
	myOLED.OLEDclearBuffer();
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Fade effect test 506");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	myOLED.OLEDfadeEffect();
	myOLED.OLEDFillScreen(0xE3, 0);
	busy_wait_ms(10000);
	myOLED.OLEDfadeEffect(0x00); // switch off fade

	myOLED.OLEDclearBuffer();
	myOLED.OLEDupdate();
}

void Test507(void)
{
	myOLED.OLEDclearBuffer();
	printf("OLED rotate buffer test 507\r\n");
	myOLED.setRotation(myOLED.rDegrees_0);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("r 90");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	
	myOLED.setRotation(myOLED.rDegrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("r 180");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	
	myOLED.setRotation(myOLED.rDegrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("r 270");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	
	myOLED.setRotation(myOLED.rDegrees_0); //default normal 
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("r 0");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	myOLED.OLEDclearBuffer();
}

// Test 5 FPS frames per second test
void DisplayFPS()
{

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
			if (count >= 3000)
				return; // end if count gets to 3000
		}
		currentFramerate++;
		count++;

		//  ** Code to test **
		myOLED.setCursor(0, 10);
		myOLED.print("CH1115");
		myOLED.setCursor(0, 20);
		myOLED.print("G Lyons");
		myOLED.setCursor(0, 30);
		myOLED.print(count);
		myOLED.setCursor(0, 40);
		myOLED.print(fps);
		myOLED.print(" fps");
		myOLED.setCursor(0, 50);
		myOLED.print(__LibVerNum__);
		myOLED.drawFastVLine(64, 0, 63, colour);

		myOLED.fillRect(70, 10, 20, 20, colour);
		myOLED.fillCircle(110, 20, 10, !colour);
		myOLED.drawRoundRect(70, 40, 40, 20, 10, !colour);
		myOLED.OLEDupdate();
	}
}

/// @endcond
