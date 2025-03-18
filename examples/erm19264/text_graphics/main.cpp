/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for ERM19264_UC1609_PICO library,
		testing text, fonts & graphics.

	@test
		-# Test 701 Default font
		-# Test 702 Wide font
		-# Test 703 Pico font
		-# Test 704 Sinclair font
		-# Test 705 Mega font
		-# Test 706 Arial bold font
		-# Test 707 hallFetica font
		-# Test 708 Arial round font
		-# Test 709 GroTesk font
		-# Test 710 Sixteen Segment font
		-# Test 711 Display ASCII font 0-255 default font
		-# Test 712 Base number systems using print method
		-# Test 713 println + print & textwrap
		-# Test 714 print method String object
		-# Test 715 print method numbers
		-# Test 901 Graphics functions
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib/erm19264.hpp"

/// @cond

// Screen settings
#define LCDcontrast 0x49	// Contrast
#define LCDRAMADDRCTRL 0x02 // RAM address control: Range 0-0x07, optional, default 0x02
#define myLCDwidth 192
#define myLCDheight 64
#define myScreenSize (myLCDwidth * (myLCDheight / 8))
uint8_t screenBuffer[myScreenSize]; 

// GPIO
const uint mosi_pin = 19;
const uint sck_pin = 18;
const uint cs_pin = 17;
const uint res_pin = 3;
const uint dc_pin = 2;

// SPI configuration
uint32_t mySPIBaudRate = 8000;
spi_inst_t *mySpiInst = spi0;

// instantiate  an LCD object
ERM19264 myLCD(myLCDwidth, myLCDheight);

// Test timing parameters
#define DisplayDelay4 4000
#define DisplayDelay2 2000

// === Function prototypes ===
void SetupTest(void);
void TestReset(void);
void EndTest(void);

void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test705(void);
void Test706(void);
void Test707(void);
void Test708(void);
void Test709(void);
void Test710(void);
void Test711(void);
void Test712(void);
void Test713(void);
void Test714(void);
void Test715(void);

void Test901(void);

// === Main ===
int main()
{
	SetupTest();
	myLCD.LCDclearBuffer();
	Test701();
	Test702();
	Test703();
	Test704();
	Test705();
	Test706();
	Test707();
	Test708();
	Test709();
	Test710();
	Test711();
	Test712();
	Test713();
	Test714();
	Test715();

	Test901();

	EndTest();
}
// === END OF MAIN ===

// === Function Space ===

void SetupTest()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(750);
	printf("LCD ERM19264:: Start!\r\n");
	myLCD.LCDSPISetup(mySpiInst, mySPIBaudRate, dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);
	myLCD.LCDinit(LCDcontrast, LCDRAMADDRCTRL);
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : LCDSetBufferPtr Failed!\r\n");
		while (1){busy_wait_ms(1000);}
	}							  // Initialize the buffer
	myLCD.LCDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void EndTest()
{
	myLCD.LCDPowerDown();  // Switch off display
	myLCD.LCDSPIoff(); 
	printf("LCD ERM19264 :: End\r\n");
}

void Test701(void)
{
	printf("LCD Test 701 Default font\r\n");
	myLCD.setFont(pFontDefault);
	char TestStr[] = "Default font";
	char InvertedStr[] = "Invertedfont";
	myLCD.writeChar(0, 0, 'a');
	myLCD.writeCharString(0, 10, TestStr);
	myLCD.setCursor(5, 20);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0, 30, '5');
	myLCD.writeCharString(0, 40, InvertedStr);
	myLCD.setCursor(5, 50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test702(void)
{
	printf("LCD Test 702 Wide font, NO LOWERCASE LETTERS\r\n");
	myLCD.setFont(pFontWide);
	char TestStr[] = "WIDE FONT";
	char InvertedStr[] = "INVERT FONT.";
	myLCD.writeChar(0, 0, '7');
	myLCD.writeCharString(0, 10, TestStr);
	myLCD.setCursor(5, 20);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0, 30, '0');
	myLCD.writeCharString(0, 40, InvertedStr);
	myLCD.setCursor(5, 50);
	myLCD.print("PRINT INV 2!");
	TestReset();
}

void Test703(void)
{
	printf("LCD Test 703 PICO font\r\n");
	myLCD.setFont(pFontPico);
	myLCD.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Inverted font";
	myLCD.writeChar(0, 0, 'p');
	myLCD.writeCharString(0, 10, TestStr);
	myLCD.setCursor(5, 20);
	myLCD.print(-3.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0, 30, '6');
	myLCD.writeCharString(0, 40, InvertedStr);
	myLCD.setCursor(5, 50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test704(void)
{
	printf("LCD Test 704 Sinclair font\r\n");
	myLCD.setFont(pFontSinclairS);
	myLCD.setInvertFont(false);
	char TestStr[] = "Sinclair font";
	char InvertedStr[] = "Inverted font";
	myLCD.writeChar(0, 0, 'u');
	myLCD.writeCharString(0, 10, TestStr);
	myLCD.setCursor(5, 20);
	myLCD.print(-3.14);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0, 30, '5');
	myLCD.writeCharString(0, 40, InvertedStr);
	myLCD.setCursor(5, 50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test705(void)
{
	printf("LCD Test 705 Mega Font\r\n");
	myLCD.setFont(pFontMega);
	myLCD.setInvertFont(false);
	char TestStr[] = "Mega font";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0, 0, TestStr);
	myLCD.setCursor(5, 16);
	myLCD.print(-3.14);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0, 32, InvertedStr);
	myLCD.setCursor(5, 48);
	myLCD.print("invert");
	TestReset();
}

void Test706(void)
{
	printf("LCD Test 706 Arial bold font\r\n");
	myLCD.setFont(pFontArialBold);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial Bold!";
	char InvertedStr[] = "Invert 1";
	myLCD.writeCharString(0, 0, TestStr);
	myLCD.setCursor(5, 16);
	myLCD.print(-3.1415, 2); // print-3.14
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0, 32, InvertedStr);
	myLCD.setCursor(5, 48);
	myLCD.print("invert");
	TestReset();
}

void Test707(void)
{
	printf("LCD Test 707 hallFetica font\r\n");
	myLCD.setFont(pFontHallfetica);
	myLCD.setInvertFont(false);
	char TestStr[] = "Hall Fetica";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0, 0, TestStr);
	myLCD.setCursor(5, 16);
	myLCD.print(-3.17);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0, 32, InvertedStr);
	myLCD.setCursor(5, 48);
	myLCD.print("invert");
	TestReset();
}

void Test708(void)
{
	printf("LCD Test 708 Arial Round font\r\n");
	myLCD.setFont(pFontArialRound);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial Round";
	myLCD.writeCharString(0, 0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1, 32);
	myLCD.print("invert");
	TestReset();
}

void Test709(void)
{
	printf("LCD Test 709 GroTesk font\r\n");
	myLCD.setFont(pFontGroTesk);
	myLCD.setInvertFont(false);
	char TestStr[] = "GroTesk Font";
	myLCD.writeCharString(0, 0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(0, 32);
	myLCD.print("inv 12!");
	TestReset();
}

void Test710(void)
{
	printf("LCD Test 710 Sixteen segment font\r\n");
	myLCD.setFont(pFontSixteenSeg);
	myLCD.setInvertFont(false);

	myLCD.setCursor(0, 0);
	myLCD.print(-3.14, 2);
	TestReset();
	myLCD.setInvertFont(true);
	char TestStr1[] = "12:43";
	myLCD.writeCharString(0, 0, TestStr1);
	TestReset();
}

void Test711()
{
	printf("LCD Test 711-a print ASCII font 0-127 \r\n");
	myLCD.setFont(pFontDefault);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);

	myLCD.print("ASCII font ");
	myLCD.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 0; i < 126; i++)
	{
		if (i == '\n' || i == '\r')
			continue;
		myLCD.print(i);
	}
	TestReset();

	myLCD.setCursor(0, 0);
	myLCD.print("ASCII font 128-255");
	uint8_t x = 0;
	uint8_t y = 15;
	myLCD.setCursor(x, y);

	// Test 711-b
	printf("LCD Test 711-b print ASCII font 128-255 \r\n");
	for (uint8_t i = 128; i < 254; i++)
	{
		if (x > 180)
		{
			x = 0;
			y += 8;
		}
		myLCD.writeChar(x, y, i);
		x += 6;
	}
	TestReset();
}

void Test712(void)
{
	printf("LCD Test 712 Base number systems using print \r\n");
	myLCD.setFont(pFontDefault);
	myLCD.setCursor(0, 0);
	myLCD.print(47, ERM19264::DEC);
	myLCD.setCursor(0, 16);
	myLCD.print(47, ERM19264::HEX);
	myLCD.setCursor(0, 32);
	myLCD.print(47, ERM19264::BIN);
	myLCD.setCursor(0, 48);
	myLCD.print(47, ERM19264::OCT);
	TestReset();
}

void Test713(void)
{
	printf("LCD Test 713-a println + textwrap \r\n");
	myLCD.setCursor(0, 0);
	myLCD.println("first line");
	myLCD.print("2nd Line");
	myLCD.setCursor(60, 32);
	myLCD.setFont(pFontWide);
	myLCD.print("123456789ABCDEFGHIJ");
	TestReset();
	printf("LCD Test 713-b writeText new line \r\n");
	char TestStr1[] = "123456789ABCDEFGHIJ";
	myLCD.writeCharString(60, 0, TestStr1);
	TestReset();
}

void Test714(void)
{
	printf("LCD Test 714 print method + string objects \r\n");
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myLCD.setFont(pFontDefault);
	myLCD.setCursor(5, 5);
	myLCD.print(timeInfo);
	// print a new line with println
	myLCD.setCursor(5, 40);
	myLCD.println(newLine); // print a new line feed with println
	myLCD.print(newLine);
	TestReset();
}

void Test715(void)
{
	printf("LCD Test 715 print method numbers\r\n");
	myLCD.setCursor(0, 0);
	myLCD.println(3.986, 2); // print 3.99
	myLCD.println(4001);
	myLCD.println(-87.39);
	myLCD.println(-19);
	myLCD.println("hello");
	myLCD.setInvertFont(true);
	myLCD.println('P');
	myLCD.print("PIE");
	myLCD.setInvertFont(false);
	TestReset();
}

void TestReset(void)
{
	myLCD.LCDupdate();
	busy_wait_ms(DisplayDelay4);
	myLCD.LCDclearBuffer();
}
// Test 801 Function to display Graphics.
void Test901()
{
	// Q1 ||  Q2
	//---------
	// Q3 ||  Q4
	printf("LCD Test 901 Graphic functions test \r\n");
	bool colour = 1;
	uint8_t count = 50;

	myLCD.LCDclearBuffer(); // Clear the buffer
	while (count > 1)
	{
		colour = !colour;

		// Draw the X
		myLCD.drawLine(96, 0, 96, 64, ERM19264::FG_COLOR);
		myLCD.drawFastVLine(94, 0, 64, ERM19264::FG_COLOR);
		myLCD.drawFastHLine(0, 32, 192, ERM19264::FG_COLOR);

		// Q1
		myLCD.fillRect(0, 10, 20, 20, colour);
		myLCD.fillCircle(40, 20, 10, ERM19264::FG_COLOR);
		myLCD.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
		// Q2
		myLCD.drawRect(100, 10, 20, 20, ERM19264::FG_COLOR);
		myLCD.drawCircle(140, 20, 10, colour);
		myLCD.drawTriangle(160, 30, 170, 10, 180, 30, ERM19264::FG_COLOR);
		// Q3
		myLCD.fillRoundRect(0, 40, 40, 20, 10, !colour);
		myLCD.drawRoundRect(45, 40, 40, 20, 10, colour);
		// Q4
		char i;
		for (i = 0; i < 10; i++)
		{
			myLCD.drawRect(100 + i, 40 + i, 80 - i * 2, 20 - i * 2, ERM19264::FG_COLOR);
			myLCD.LCDupdate();
			busy_wait_ms(50);
		}
		myLCD.LCDclearBuffer();
		count--;
		myLCD.setCursor(0, 0);

		myLCD.print(count);
	}
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
}

/// @endcond
