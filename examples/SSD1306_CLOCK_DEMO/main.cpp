/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, Test file showing a "clock demo" 128X64 screen 
	Project Name: SSD1306_OLED_PICO
	
	@details One possible method you can also use strings objects
	
	@test
		-# Test 401 Clock Demo
*/

// === Libraries ===
#include <time.h>
#include <stdio.h>
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
void DisplayClock(void);
void SplashScreen(void);
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
	SplashScreen();  
	DisplayClock();  
}


void DisplayClock(void)
{
	uint8_t Sec = 50;
	uint8_t Hour = 10;
	uint8_t Min = 59;
	uint16_t count = 0;
	char strTime[12];
	unsigned long previousMillis = 0;  // will store last time display was updated
	const long interval = 1000;  //   interval at which to update display (milliseconds)
	
	myOLED.setFont(pFontDefault);
	printf("OLED Clock Demo 30 seconds.\r\n");
		
	while (count < 30)
	{
		myOLED.OLEDBitmap(0, 0, 16, 8,  SignalIcon, false, sizeof(SignalIcon)/sizeof(uint8_t));
		myOLED.OLEDBitmap(20, 0, 16, 8,  MsgIcon, false, sizeof(MsgIcon)/sizeof(uint8_t));
		myOLED.OLEDBitmap(37, 0, 8, 8,  AlarmIcon, false, sizeof(AlarmIcon)/sizeof(uint8_t));
		myOLED.OLEDBitmap(110, 0, 16, 8,  BatIcon, false, sizeof(BatIcon)/sizeof(uint8_t));

		myOLED.drawLine(0,10,128,10,WHITE);
		myOLED.drawLine(0,35,128,35,WHITE);
		myOLED.drawLine(0,63,128,63,WHITE);

		myOLED.drawLine(0,35,0,63,WHITE);
		myOLED.drawLine(127,35,127,63,WHITE);

		myOLED.drawLine(40,35,40,63,WHITE);
		myOLED.drawLine(75,35,75,63,WHITE);
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= interval) // rolls over every interval (1 sec)
		{
			count++;
			previousMillis = currentMillis;
			Sec++;
			if (Sec == 60)
			{
				Min++;
				Sec = 0;
				if (Min == 60)
				{
					Hour++;
					Min = 0;
					if (Hour == 24)
					{
						Hour = 0;
					}
				}
			}
			myOLED.setFont(pFontHallfetica );
			snprintf(strTime, sizeof(strTime), "%02u:%02u:%02u", Hour , Min , Sec);
			myOLED.writeCharString(0,16, strTime);
			myOLED.setCursor(55,44);
			myOLED.setFont(pFontDefault);
			myOLED.print(count);
			myOLED.OLEDBitmap(80, 40, 16, 8,  MsgIcon, false,sizeof(MsgIcon)/sizeof(uint8_t)); 
			myOLED.OLEDupdate();
			myOLED.OLEDclearBuffer();
		} //sec
	}
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}


void SplashScreen(void)
{
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)/sizeof(uint8_t)) != 0)
	{
		printf("Test: ERROR : OLEDSetBufferPtr Failed!\r\n");
		while(1){busy_wait_ms(1000);}
	}
	myOLED.OLEDBitmap(0, 0, myOLEDwidth, myOLEDheight, fullscreenBitmap, true, sizeof(fullscreenBitmap)/sizeof(uint8_t));
	myOLED.OLEDupdate();
	busy_wait_ms(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

