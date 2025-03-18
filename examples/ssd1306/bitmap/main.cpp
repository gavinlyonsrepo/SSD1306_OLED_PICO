/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library,showing how to display bitmap, 128X64 screen.
	@test
		-# Test 301 OLEDBitmap method, bigImage 64x64 pixels
		-# Test 302 OLEDBitmap method, Small icons 16x8 pixels
		-# Test 303 OLEDBitmap method, full screen image 128x64 pixels 
		-# Test 304 OLEDBitmap method, image 40x16 pixels
		-# Test 306 drawBitmap method, 16x16 pixels
		-# Test 307 drawBitmap method, 40x16 pixels
		-# Test 803 Bitmap error checking
*/

// === Libraries ===
#include <cstdio>
#include <vector> // For error test check Test 808 only
#include "pico/stdlib.h"

#include "displaylib/ssd1306.hpp"
#include "displaylib/bitmap_test_data.hpp"

/// @cond

// Screen settings
#define myOLEDwidth  128
#define myOLEDheight 64
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize]; // Define a data buffer to cover whole screen

// I2C settings
const uint16_t I2C_Speed = 100; // I2C baudrate in Khz
const uint8_t I2C_GPIO_CLK = 19;
const uint8_t I2C_GPIO_DATA = 18;

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

// =============== Function prototype ================
void SetupTest(void);
void myLoop(void);
void Test307(void);
void Test306(void);
void Test304(void);
void Test303(void);
void Test302(void);
void Test301(void);
void Test803(void);
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
	while(myOLED.OLEDbegin(SSD1306::SSD1306_ADDR, i2c1,  I2C_Speed, I2C_GPIO_DATA, I2C_GPIO_CLK) != DisplayRet::Success)
	{
		printf("SetupTest ERROR 1: Failed to initialize OLED!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest ERROR 2: OLEDSetBufferPtr Failed!\r\n");
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
	Test306();
	Test307();
	Test803();
}

void Test301(void)
{

	printf("OLED Test 301 64x64 pixel Image\r\n");
	myOLED.OLEDBitmap(0, 0 , 64, 64, backupicon64x64, false);
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
	myOLED.OLEDBitmap(0, 0, myOLEDwidth, myOLEDheight, backupicon128x64, true);
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test304(void)
{
	printf("OLED Test 304 40x16 pixel image\r\n");
	myOLED.OLEDBitmap(5, 5, 40, 16,  SunTextImage, false);
	myOLED.OLEDupdate();
	
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}

// test Draw a vertically addressed bitmap to screen with drawBitmap method
void Test306(void)
{
	printf("OLED Test306 16x16 drawBitmap method \r\n");
	myOLED.OLEDclearBuffer();				// Clear  buffer
	myOLED.setDrawBitmapAddr(true); // for Bitmap Data Vertical  addressed
	myOLED.drawBitmap(0, 0, TemperatureImageVa, 16, 16, myOLED.FG_COLOR, myOLED.BG_COLOR);
	myOLED.drawBitmap(30, 20, TemperatureImageVa, 16, 16, myOLED.BG_COLOR, myOLED.FG_COLOR);
	myOLED.OLEDupdate();
	busy_wait_ms(10000);
}

//   test Draw a Horizontally addressed bitmap to screen with drawBitmap method
void Test307(void)
{
	printf("OLED Test307 40x16 drawBitmap method \r\n");
	myOLED.OLEDclearBuffer();				 // Clear active buffer
	myOLED.setDrawBitmapAddr(false); // for Bitmap Data Horizontal addressed
	myOLED.drawBitmap(20, 10, SunTextImage, 40, 16, myOLED.FG_COLOR, myOLED.BG_COLOR);
	myOLED.drawBitmap(80, 20, SunTextImage, 40, 16, myOLED.BG_COLOR, myOLED.FG_COLOR);
	myOLED.OLEDupdate();
	busy_wait_ms(10000);
}
//  Bitmap error checking test
void Test803(void)
{
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		DisplayRet::Success,
		DisplayRet::BitmapScreenBounds, DisplayRet::BitmapScreenBounds,
		DisplayRet::BitmapLargerThanScreen, DisplayRet::BitmapDataEmpty,
		DisplayRet::BitmapHorizontalSize, DisplayRet::BitmapSize
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	char testString5[] = "Error Check Test 803";
	bool errorFlag = false;

	// === Tests, Perform function calls and store return values ===
	printf("=== START Error checking. Expecting errors ===\r\n");
	// Print message + sanity check for success
	myOLED.setFont(pFontDefault);
	returnValues.push_back(myOLED.writeCharString(5, 5, testString5)); 
	myOLED.OLEDupdate();
	busy_wait_ms(2000);
	myOLED.OLEDFillScreen(0x00, 0);

	returnValues.push_back(myOLED.OLEDBitmap(200, 0, 16, 8, SignalIcon, false));
	returnValues.push_back(myOLED.OLEDBitmap(0, 100, 16, 8,  SignalIcon, false));
	returnValues.push_back(myOLED.OLEDBitmap(0, 0, 180, 80, SignalIcon, false));
	returnValues.push_back(myOLED.OLEDBitmap(0, 0, 16, 8,  emptyBitmap, false));
	returnValues.push_back(myOLED.OLEDBitmap(0, 0, 17, 8, SignalIcon, false));
	returnValues.push_back(myOLED.OLEDBitmap(0, 0, 32, 20, SignalIcon, false));
	
	//== SUMMARY SECTION===
	printf("\nError Checking Summary.\n");
	// Check return values against expected errors
	for (size_t i = 0; i < returnValues.size(); ++i) {
		if (i >= expectedErrors.size() || returnValues[i] != expectedErrors[i]) {
			errorFlag = true;
			printf("Unexpected error code: %d at test case %zu (expected: %d)\n", 
				returnValues[i], i + 1, (i < expectedErrors.size() ? expectedErrors[i] : -1));
		}
	}

	// Print all expectedErrors for summary
	for (uint8_t value : expectedErrors ) 
	{
		printf("%d ", value);
	}
	printf("\n");
	// Print all returnValues for summary
	for (uint8_t value : returnValues) 
	{
		printf("%d ", value);
	}
	if (errorFlag == true ){
		printf("\nError Checking has FAILED.\n");
	}else{
		printf("\nError Checking has PASSED.\n");
	}
	printf("\n=== STOP Error checking. ===\r\n");
}
/// @endcond
