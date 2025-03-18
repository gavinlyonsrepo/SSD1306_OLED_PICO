/*!
	@file main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, testing graphics, functions, text.
	@test
		-# Test 501 Enable and disable Screen (Sleep mode)
		-# Test 502 Invert screen color
		-# Test 503 Contrast screen
		-# Test 504 Scroll Screen
		-# Test 505 Rotate
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
		-# Test 808 Error checking test functions
		-# Test 901 Simple graphics test
		-# Test 902 Advanced graphics
*/

// === Libraries ===
#include <cstdio>
#include "pico/stdlib.h"
#include <vector> // for test 808 and 902 only
#include "displaylib/ssd1306.hpp"

/// @cond

// Screen settings
#define myOLEDwidth  128
#define myOLEDheight 64
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize]; // Define a buffer to cover whole screen  128 * 64/8

// Test timing parameters
#define DisplayDelay4 4000
#define DisplayDelay2 2000

// I2C settings
const uint16_t I2C_Speed = 100;
const uint8_t I2C_GPIO_CLK = 19;
const uint8_t I2C_GPIO_DATA = 18;

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

// =============== Function prototype ================
void SetupTest(void);
void EndTest(void);
void TestReset(void);
void Test901(void);
#ifdef _ADVANCED_GRAPHICS_ENABLE
void Test902(void);
#endif
void Test50X(void);
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
void Test808(void);

// ==================== Main ===================
int main() 
{
	SetupTest();
	Test50X();
	Test901();
	#ifdef _ADVANCED_GRAPHICS_ENABLE
	Test902();
	#endif
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
	Test808();
	EndTest();
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void SetupTest() 
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("OLED SSD1306 :: Start!\r\n");
	while(myOLED.OLEDbegin(SSD1306::SSD1306_ADDR, i2c1, I2C_Speed, I2C_GPIO_DATA, I2C_GPIO_CLK) != DisplayRet::Success)
	{
		printf("SetupTest ERROR : Failed to initialize OLED!\r\n");
		busy_wait_ms(1500);
	} // initialize the OLED
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : OLEDSetBufferPtr Failed!\r\n");
		while(1){busy_wait_ms(1000);}
	} // Initialize the buffer
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
	busy_wait_ms(1000);
}

void Test50X()
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
	myOLED.setRotation(SSD1306::rDegrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);

	myOLED.setRotation(SSD1306::rDegrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);

	myOLED.setRotation(SSD1306::rDegrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);

	myOLED.setRotation(SSD1306::rDegrees_0); //default normal
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");
	myOLED.OLEDupdate();
	busy_wait_ms(3000);
	
	myOLED.OLEDFillScreen(0xF0, 0);
	
	// ** end of tests **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(15, 25);
	myOLED.print("End of function tests");
	myOLED.OLEDupdate();
	busy_wait_ms(1000);
	myOLED.OLEDFillScreen(0x00, 0);
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
	myOLED.print(47 , myOLED.DEC);
	myOLED.setCursor(0, 16);
	myOLED.print(47 , myOLED.HEX); 
	myOLED.setCursor(0, 32);
	myOLED.print(47, myOLED.BIN);
	myOLED.setCursor(0, 48);
	myOLED.print(47 , myOLED.OCT);
	TestReset();
}

void Test713(void)
{
	printf("OLED Test 713-a println + textwrap \r\n");
	myOLED.setCursor(0, 0);
	myOLED.println("first line");
	myOLED.print("2nd Line");
	myOLED.setCursor(40, 32);
	myOLED.setFont(pFontWide);
	myOLED.print("123456789ABCDEFGHIJ");
	TestReset();
	printf("OLED Test 713-b writeText new line \r\n");
	char TestStr1[] = "123456789ABCDEFGHIJ";
	myOLED.writeCharString(40,0, TestStr1);
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

void Test808(void)
{
	// Error checking
	printf("==== Test 808 Start Error checking ====\n");
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		3, DisplayRet::CharFontASCIIRange, 3, DisplayRet::CharFontASCIIRange, 5, DisplayRet::Success, 0, DisplayRet::CharArrayNullptr,
		DisplayRet::Success, DisplayRet::CharFontASCIIRange, DisplayRet::CharFontASCIIRange,
		DisplayRet::CharFontASCIIRange, DisplayRet::CharScreenBounds, DisplayRet::CharScreenBounds,
		DisplayRet::CharScreenBounds, DisplayRet::CharScreenBounds, DisplayRet::CharArrayNullptr
	};
	
	// Vector to store return values
	std::vector<uint8_t> returnValues; 

	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-;A";
	bool errorFlag = false;
	myOLED.setFont(pFontWide);

	// (1) Print statement test, wide font lower case letters
	// We check print class error flag as print statement does not return errors it returns num of characters
	// it attempted to print. Always clear error flag before checking getWriteError after new prin
	myOLED.setCursor(40,40);
	returnValues.push_back(myOLED.print("ABc")); // Print AB , return 3 (num of characters)
	returnValues.push_back(myOLED.getWriteError()); // return error
	returnValues.push_back(myOLED.print("abC"));  // print C , return 3 (num of characters)
	returnValues.push_back(myOLED.getWriteError()); // return error
	returnValues.push_back(myOLED.print("12345")); // print 12345 , return 5 (num of characters)
	returnValues.push_back(myOLED.getWriteError()); // return pass
	returnValues.push_back(myOLED.print(nullptr));  // return 0 (num of characters)
	returnValues.push_back(myOLED.getWriteError());  // return error
	TestReset();
	printf("========\r\n");
	// (2) writeChar + writeCharString
	// lower case + ] character out of font bounds
	returnValues.push_back(myOLED.writeChar(32, 0, '!')); //success
	returnValues.push_back(myOLED.writeCharString(5,  5, testlowercase)); //throw font error 2
	TestReset();
	// Numeric extended bounds ; , A errors
	myOLED.setFont(pFontSixteenSeg);
	returnValues.push_back(myOLED.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myOLED.writeChar(32, 0, ',')); //throw error 2
	TestReset();
	printf("========\r\n");
	// screen out of bounds
	myOLED.setFont(pFontDefault);
	returnValues.push_back(myOLED.writeChar(0, 400, 'e')); //throw error 1
	returnValues.push_back(myOLED.writeChar(400, 0, 'f')); //throw error 1
	TestReset();
	myOLED.setFont(pFontPico);
	returnValues.push_back(myOLED.writeChar(0, 400, 'A')); //throw error 1
	returnValues.push_back(myOLED.writeChar(400, 0, 'B')); //throw error 1
	TestReset();
	
	returnValues.push_back(myOLED.writeCharString(5, 5, nullptr)); //throw error 
	
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

// Function to display Graphics.
void  Test901()
{
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	TestReset(); // Clear the buffer
	printf("OLED Test 901 graphics test \r\n");
	bool colour = 1;
	uint8_t count = 0;
	while (count < 15)
	{
		colour = !colour;
		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, myOLED.FG_COLOR);
		myOLED.drawFastVLine(62, 0, 64, myOLED.FG_COLOR);
		myOLED.drawFastHLine(0, 32, 128, myOLED.FG_COLOR);
		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, myOLED.FG_COLOR);
		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawRect(105, 10, 15, 15, myOLED.FG_COLOR);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, myOLED.FG_COLOR);
			myOLED.OLEDupdate();
			busy_wait_ms(25);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
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

#ifdef _ADVANCED_GRAPHICS_ENABLE
void Test902()
{
	printf("OLED Test 902 Advanced graphics test \r\n");
	printf( "Test : polygons\r\n");
	//drawPolygon( x,  y, sides,  diameter, rotation,fill, color);
	// hexagon 6 sides
	myOLED.drawPolygon(60, 30, 6, 25, 0, false, myOLED.FG_COLOR);
	TestReset();
	myOLED.drawPolygon(60, 30, 6, 25, 0, true, myOLED.FG_COLOR);
	busy_wait_ms(200);	
	TestReset();
	printf( "Test : dot grid\r\n");
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myOLED.drawDotGrid(0, 0, myOLEDwidth, myOLEDheight, 5, myOLED.FG_COLOR);
	TestReset();
	printf( "Test : quadrilateral\r\n");
	// Trapezoid 1 
	int16_t x8 = 30, y8 = 30;   // First vertex
	int16_t x9 = 35, y9 = 50;
	int16_t x10 = 60, y10 = 50;
	int16_t x11 = 75, y11 = 30;
	myOLED.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myOLED.FG_COLOR);
	TestReset();
	myOLED.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myOLED.FG_COLOR);
	TestReset();
	printf( "Test : Ellipse\r\n");
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myOLED.drawEllipse(60, 30, 25, 20, false, myOLED.FG_COLOR);  // Top-left
	TestReset();
	myOLED.drawEllipse(60, 30, 25, 20, true, myOLED.FG_COLOR);
	TestReset();
	printf( "Test : Drawing Arc: drawArc and drawSimpleArc\r\n");
	int16_t centerX = 50;  // X-coordinate of the circle center
	int16_t centerY = 50;  // Y-coordinate of the circle center
	int16_t radius = 45;    // Radius of the circle
	printf( " Angle offset default : %i \r\n", myOLED.getArcAngleOffset());
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full upper half (180° to 360°)
	myOLED.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myOLED.FG_COLOR);
	TestReset();
	busy_wait_ms(2000);
	// Draw the third quarter (180° to 270°)
	myOLED.drawArc(centerX, centerY, radius, 3, 180.0f, 270.0f, myOLED.FG_COLOR);
	TestReset();
	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myOLED.drawSimpleArc(30, 30, 25, 0.0f, 300.0f, myOLED.FG_COLOR);
	TestReset();
}
#endif

/// @endcond
