/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, testing text and fonts
	
	Project Name: SSD1306_OLED_PICO

	@test
		1. Test 701 Default font
		2. Test 702 Wide font
		3. Test 703 Pico font
		4. Test 704 Sinclair font
		5. Test 705 Mega font
		6. Test 706 Arial bold font
		7. Test 707 hallFetica font
		8. Test 708 Arial round font
		9. Test 709 GroTesk font
		10. Test 710 Sixteen Segment font
		11. Test 711 Display ASCII font 0-255 default font
		12. Test 712 Base number systems using print method
		13. Test 713 println + print & textwrap 
		14. Test 714 print method String object 
		15. Test 715 print method numbers 
*/

// === Libraries ===
#include <cstdio>
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"

// Test timing parameters
#define DisplayDelay4 4000
#define DisplayDelay2 2000

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

void EndTest(void);
void TestReset(void);
// ======================= Main ===================
int main() 
{
	SetupTest();
	myOLED.OLEDclearBuffer();
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
	
	EndTest();
}
// ======================= End of main  ===================

// ===================== Function Space =====================
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

void Test701(void)
{
	printf("OLED Test 701 Default font\r\n");
	myOLED.setFont(pFontDefault);
	char TestStr[] = "Default font";
	char InvertedStr[] = "Invertedfont";
	myOLED.writeChar(0,0 ,'a');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-45.98);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'5');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("print invert 3");
	TestReset();
}

void Test702(void)
{
	printf("OLED Test 702 Wide font, NO LOWERCASE LETTERS\r\n");
	myOLED.setFont(pFontWide);
	char TestStr[] = "WIDE FONT";
	char InvertedStr[] = "INVERT FONT.";
	myOLED.writeChar(0,0 ,'7');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-45.98);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'0');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("PRINT INV 2!");
	TestReset();
}

void Test703(void){
	printf("OLED Test 703 PICO font\r\n");
	myOLED.setFont(pFontPico);
	myOLED.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Inverted font";
	myOLED.writeChar(0,0 ,'p');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-3.98); 
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'6');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("print invert 3");
	TestReset();
}

void Test704(void){
	printf("OLED Test 704 Sinclair font\r\n");
	myOLED.setFont(pFontSinclairS);
	myOLED.setInvertFont(false);
	char TestStr[] = "Sinclair font";
	char InvertedStr[] = "Inverted font";
	myOLED.writeChar(0,0 ,'u');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-3.14);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'5');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("print invert 3");
	TestReset();
}

void Test705(void){
	printf("OLED Test 705 Mega font\r\n");
	myOLED.setFont(pFontMega);
	myOLED.setInvertFont(false);
	char TestStr[] = "Mega f.";
	char InvertedStr[] = "Invert";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(5,16);
	myOLED.print(-3.14);
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, InvertedStr);
	myOLED.setCursor(5,48);
	myOLED.print("invert");
	TestReset();
}

void Test706(void){
	printf("OLED Test 706 Arial bold font\r\n");
	myOLED.setFont(pFontArialBold);
	myOLED.setInvertFont(false);
	char TestStr[] = "Arial bo";
	char InvertedStr[] = "Invert 1";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(5,16);
	myOLED.print(-3.1415, 2); // print-3.14
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, InvertedStr);
	myOLED.setCursor(5,48);
	myOLED.print("invert");
	TestReset();
}

void Test707(void)
{
	printf("OLED Test 707 hallFetica font\r\n");
	myOLED.setFont(pFontHallfetica);
	myOLED.setInvertFont(false);
	char TestStr[] = "Hall f.";
	char InvertedStr[] = "Invert";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(5,16);
	myOLED.print(-3.17);
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, InvertedStr);
	myOLED.setCursor(5,48);
	myOLED.print("invert");
	TestReset();
}

void Test708(void){
	printf("OLED Test 708 Arial Round font\r\n");
	myOLED.setFont(pFontArialRound);
	myOLED.setInvertFont(false);
	char TestStr[] = "Arial R";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setInvertFont(true);
	myOLED.setCursor(1,32);
	myOLED.print("invert");
	TestReset();
}

void Test709(void)
{
	printf("OLED Test 709 GroTesk font\r\n");
	myOLED.setFont(pFontGroTesk);
	myOLED.setInvertFont(false);
	char TestStr[] = "GroTesk1";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setInvertFont(true);
	myOLED.setCursor(0,32);
	myOLED.print("inv 12!");
	TestReset();
}

void Test710(void)
{
	printf("OLED Test 710 Sixteen segment font\r\n");
	myOLED.setFont(pFontSixteenSeg);
	myOLED.setInvertFont(false);

	myOLED.setCursor(0,0);
	myOLED.print(-3.1, 1);
	TestReset();
	myOLED.setInvertFont(true);
	char TestStr1[] = "12:4";
	myOLED.writeCharString(0,0, TestStr1);
	TestReset();
}

void Test711() 
{
	printf("OLED Test 711-a print ASCII font 0-127 \r\n");
	myOLED.setFont(pFontDefault);
	myOLED.setInvertFont(false);
	myOLED.setCursor(0, 0);

	myOLED.print("ASCII font ");
	myOLED.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 0; i < 126; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myOLED.print(i);
	}
	TestReset();

	myOLED.setCursor(0, 0);
	myOLED.print("ASCII font 128-255");
	uint8_t x = 0;
	uint8_t y = 15;
	myOLED.setCursor(x, y);
	
	// Test 711-b
	printf("OLED Test 711-b print ASCII font 128-255 \r\n");
	for (uint8_t i = 128; i < 254; i++)
	{
		if (x > 110)
		{
			x = 0;
			y += 8;
		}
		myOLED.writeChar(x, y , i);
		x += 6;
	}
	TestReset();
}

void Test712(void)
{
	printf("OLED Test 712 Base number systems using print \r\n");
	myOLED.setFont(pFontDefault);
	myOLED.setCursor(0, 0);
	myOLED.print(47 , DEC);
	myOLED.setCursor(0, 16);
	myOLED.print(47 , HEX); 
	myOLED.setCursor(0, 32);
	myOLED.print(47, BIN);
	myOLED.setCursor(0, 48);
	myOLED.print(47 , OCT);
	TestReset();
}

void Test713(void)
{
	printf("OLED Test 713-a println + textwrap \r\n");
	myOLED.setCursor(0, 0);
	myOLED.println("first line");
	myOLED.print("2nd Line");
	myOLED.setCursor(0, 32);
	myOLED.setFont(pFontWide);
	myOLED.print("123456789ABCDEFGHIJ");
	TestReset();
	printf("OLED Test 713-b writeText new line \r\n");
	char TestStr1[] = "123456789ABCDEFGHIJ";
	myOLED.writeCharString(0,0, TestStr1);
	TestReset();
}

void Test714(void)
{
	printf("OLED Test 714 print method + string objects \r\n");
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myOLED.setFont(pFontDefault);
	myOLED.setCursor(5,5);
	myOLED.print(timeInfo);
	// print a new line with println 
	myOLED.setCursor(5,40);
	myOLED.println(newLine); // print a new line feed with println
	myOLED.print(newLine);
	TestReset();
}

void Test715(void)
{
	printf("OLED Test 715 print method numbers\r\n");	
	myOLED.setCursor(0, 0);
	myOLED.println(3.986,2 ); //print 3.99
	myOLED.println(4001);
	myOLED.println(-87.39);
	myOLED.println(-19);
	myOLED.println("hello");
	myOLED.setInvertFont(true);
	myOLED.print('P');
	TestReset();
}

void TestReset(void){
	myOLED.OLEDupdate();
	busy_wait_ms(DisplayDelay4);
	myOLED.OLEDclearBuffer();
}

void EndTest()
{
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
	printf("OLED SSD1306 :: End\r\n");
}

// ============== EOF =========
