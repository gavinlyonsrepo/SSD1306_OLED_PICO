/*!
	@file examples/nokia5110/text_graphics_functions/main.cpp
	@author Gavin Lyons
	@brief Test file for Nokia 5110 display showing use of Text,graphics & functions.
		Project Name: Display_Lib_RPI

	@test
		-# Test 501 fill + clear screen
		-# Test 502 Sleep
		-# Test 503 Rotation
		-# Test 504 invert screen command
		-# Test 701 Default font
		-# Test 702 gll font
		-# Test 703 Pico font
		-# Test 704 Sinclair font
		-# Test 707 Mega font
		-# Test 708 Arial bold font
		-# Test 709 hallFetica font
		-# Test 710 Arial round font
		-# Test 711 GroTesk font
		-# Test 712 Sixteen Segment font
		-# Test 713 Display ASCII font 32-90 default font
		-# Test 714 Base number systems using print method
		-# Test 715 println + print & textwrap
		-# Test 716 print method String object
		-# Test 717 print method numbers
		-# Test 901 Graphics shapes
		-# Test 902 Draw pixel
		-# Test 903 Draw lines
		-# Test 904 Advanced Graphic shapes
*/

// === Libraries ===
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib/nokia5110.hpp"

#ifdef _ADVANCED_GRAPHICS_ENABLE
#include <vector>
#endif

/// @cond

// Test setup
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// Screen settings
//  LCD
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14
#define myLCDwidth 84
#define myLCDheight 48
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

// Hardware SPI setup
NOKIA_5110 myLCD(myLCDwidth, myLCDheight);

// ************ Function Headers ****************
bool Setup(void);
void screenReset(void);
void EndTests(void);

void Test501(void);
void Test502(void);
void Test503(void);
void Test504(void);

void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test707(void);
void Test708(void);
void Test709(void);
void Test710(void);
void Test711(void);
void Test712(void);
void Test713(void);
void Test714(void);
void Test715(void);
void Test716(void);
void Test717(void);

void Test901(void);
void Test902(void);
void Test903(void);
#ifdef _ADVANCED_GRAPHICS_ENABLE
void Test904(void);
#endif
// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	Test501();
	Test502();
	Test503();
	Test504();

	Test701();
	Test702();
	Test703();
	Test704();
	Test707();
	Test708();
	Test709();
	Test710();
	Test711();
	Test712();
	Test713();
	Test714();
	Test715();
	Test716();
	Test717();

	Test901();
	Test902();
	Test903();
	#ifdef _ADVANCED_GRAPHICS_ENABLE
	Test904();
	#endif
	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device
bool Setup(void)
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(500);
	printf("LCD Start!\r\n");
	myLCD.LCDSPISetup(mySpiInst, mySPIBaudRate, dc_pin, res_pin, cs_pin, sck_pin, mosi_pin);
	myLCD.LCDInit(LCD_INV, LCD_CST, LCD_BIAS);
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer) != DisplayRet::Success)
	{
		printf("SetupTest : ERROR : LCDSetBufferPtr Failed!\r\n");
		return false;
	}								// Initialize the buffer
	myLCD.LCDfillScreen(0x31); // splash screen bars
	busy_wait_ms(2000);
	return true;
}

void EndTests(void)
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	printf("\nLCD End\n");
}

void Test701(void)
{
	printf("Test 701 Default font \n");
	myLCD.setFont(pFontDefault);
	char TestStr[] = "Default";
	char InvertedStr[] = "Invert";
	myLCD.writeChar(0,0 ,'a');
	myLCD.writeCharString(0,9, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,29 ,'5');
	myLCD.writeCharString(0,29, InvertedStr);
	myLCD.setCursor(5,37);
	myLCD.print("invert p");
	screenReset();
}

void Test702(void)
{
	printf("Test 702 Wide font, NO LOWERCASE LETTERS \n");
	myLCD.setFont(pFontWide);
	char TestStr[] = "GLL FONT";
	char InvertedStr[] = "INVERT";
	myLCD.writeChar(0,0 ,'7');
	myLCD.writeCharString(0,9, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,29 ,'0');
	myLCD.writeCharString(0,29, InvertedStr);
	myLCD.setCursor(5,37);
	myLCD.print("INV 2!");
	screenReset();
}

void Test703(void){
	printf("Test 703 PICO font\n");
	myLCD.setFont(pFontPico);
	myLCD.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(0,14);
	myLCD.print(-3.98);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,30, InvertedStr);
	screenReset();
}

void Test704(void){
	printf("Test 704 Sinclair font \n");
	myLCD.setFont(pFontSinclairS);
	myLCD.setInvertFont(false);
	char TestStr[] = "Sinclair";
	char InvertedStr[] = "Invert";
	myLCD.writeChar(0,0 ,'u');
	myLCD.writeCharString(10,10, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-3.14);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,29 ,'5');
	myLCD.writeCharString(10,29, InvertedStr);
	myLCD.setCursor(0,37);
	myLCD.print("p inv 3");
	screenReset();
}

void Test707(void){
	printf("Test 707 Mega font\n");
	myLCD.setFont(pFontMega);
	myLCD.setInvertFont(false);
	char TestStr[] = "Mega";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(99);
	myLCD.setInvertFont(true);
	myLCD.setCursor(5,32);
	myLCD.print("inv");
	screenReset();
}

void Test708(void){
	printf("Test 708 Arial bold font\n");
	myLCD.setFont(pFontArialBold);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial";
	char InvertedStr[] = "Inv";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.1415, 1); // print-3.1
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	screenReset();
}

void Test709(void)
{
	printf("Test 709 hallFetica font\n");
	myLCD.setFont(pFontHallfetica);
	myLCD.setInvertFont(false);
	char TestStr[] = "Hall";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.17,1);
	myLCD.setInvertFont(true);
	myLCD.setCursor(5,32);
	myLCD.print("inv");
	screenReset();
}

void Test710(void){
	printf("Test 710 Arial Round font\n");
	myLCD.setFont(pFontArialRound );
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,25);
	myLCD.print("inv");
	screenReset();
}

void Test711(void)
{
	printf("Test 711 GroTesk font\n");
	myLCD.setFont(pFontGroTesk );
	myLCD.setInvertFont(false);
	char TestStr[] = "GroT";
	myLCD.writeCharString(0,0, TestStr);
	screenReset();
}

void Test712(void)
{
	printf("Test 712 Sixteen segment font\n");
	myLCD.setFont(pFontSixteenSeg );
	myLCD.setInvertFont(false);

	myLCD.setCursor(0,0);
	myLCD.print(-3);
	screenReset();
	myLCD.setInvertFont(true);
	char TestStr1[] = "12";
	myLCD.writeCharString(0,0, TestStr1);
	screenReset();
}

void Test713()
{
	printf("Test 713 print ASCII font 32-90 \n");
	char i;
	myLCD.setFont(pFontDefault);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);
	// Print out the full ASCII font for default font
	for (i = 32; i < 90; i++)
	{
		myLCD.print(i);
	}
	screenReset();

}

void Test714(void)
{
	printf("Test 714 Base number systems using print \n");
	myLCD.setFont(pFontDefault);
	myLCD.setCursor(0, 0);
	myLCD.print(47, myLCD.DEC);
	myLCD.setCursor(0, 9);
	myLCD.print(47, myLCD.HEX);
	myLCD.setCursor(0, 20);
	myLCD.print(47, myLCD.BIN);
	myLCD.setCursor(0, 29);
	myLCD.print(47 ,myLCD.OCT);
	screenReset();
}

void Test715(void)
{
	printf("Test 715-a println + textwrap \n");
	myLCD.setCursor(0, 0);
	myLCD.println("1st line");
	myLCD.print("2nd Line");
	myLCD.setCursor(0, 20);
	myLCD.setFont(pFontWide);
	myLCD.print("123456789ABCDEFGHIJ");
	screenReset();
	printf("Test 715-b writeText new line \n");
	char TestStr1[] = "123456789ABF";
	myLCD.writeCharString(0,0, TestStr1);
	screenReset();
}

void Test716(void)
{
	printf("Test 716 print method + string objects \n");
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myLCD.setFont(pFontDefault);
	myLCD.setCursor(5,5);
	myLCD.print(timeInfo);
	// print a new line with println
	myLCD.setCursor(5,32);
	myLCD.println(newLine); // print a new line feed with println
	myLCD.print(newLine);
	screenReset();
}

void Test717(void)
{
	printf("Test 717-a print method numbers\n");
	myLCD.setCursor(0, 0);
	myLCD.println(3.986,2 ); //print 3.99
	myLCD.println(4001);
	myLCD.println(-87.39);
	myLCD.println(-19);
	myLCD.println("hello");
	myLCD.setInvertFont(true);
	myLCD.print('P');
	myLCD.setInvertFont(false);
	screenReset();

}

void Test901(void){
	printf("Test 901 Draw  shapes\n");
	myLCD.drawCircle(20, 20, 10, myLCD.FG_COLOR);
	screenReset();
	myLCD.fillCircle(60, 20, 10, myLCD.FG_COLOR);
	screenReset();
	myLCD.fillRect(0, 20, 10, 30, myLCD.FG_COLOR);
	screenReset();
	myLCD.drawTriangle(30, 20, 50, 5, 80, 20, myLCD.FG_COLOR);
	screenReset();
	myLCD.fillTriangle(10, 20, 40, 5, 80, 20, myLCD.FG_COLOR);
	screenReset();
	myLCD.drawRoundRect(20, 20, 15 , 15, 5, myLCD.FG_COLOR);
	screenReset();
	myLCD.fillRoundRect(60, 20, 15, 15, 5, myLCD.FG_COLOR);
	screenReset();
	myLCD.drawRect(20, 15, 20, 15, myLCD.FG_COLOR);
	screenReset();
}

void Test902(void) {
	printf("Test 902 Draw pixels\n");
	myLCD.drawPixel(2, 5, myLCD.FG_COLOR);
	myLCD.drawPixel(10, 10, myLCD.FG_COLOR);
	myLCD.drawPixel(20, 20, myLCD.FG_COLOR);
	myLCD.drawPixel(30, 30, myLCD.FG_COLOR);
	myLCD.drawPixel(32, 35, myLCD.FG_COLOR);
	screenReset();
}

void Test903(void) {
	printf("Test 903 Draw lines \n");
	myLCD.drawLine(0, 0, 40, 20, myLCD.FG_COLOR);
	myLCD.drawFastHLine(50, 20, 10 , myLCD.FG_COLOR);
	myLCD.drawFastVLine(70, 20 ,10, myLCD.FG_COLOR);
	screenReset();
}

#ifdef _ADVANCED_GRAPHICS_ENABLE
void Test904(void) {
	printf("Test 904 Draw more shapes\n");
	printf("Polygons\n");
	// Draw hexagon 
	myLCD.drawPolygon(42, 24, 6, 18, 0, false, myLCD.FG_COLOR);
	screenReset();
	// fill hexagon
	myLCD.drawPolygon(42, 24, 6, 18, 0, true, myLCD.FG_COLOR);
	busy_wait_ms(200);
	screenReset();

	printf("Dot grid\n");
	myLCD.drawDotGrid(0, 0, myLCDwidth, myLCDheight, 5, myLCD.FG_COLOR);
	screenReset();

	printf("Quadrilateral\n");
	int16_t x8 = 20, y8 = 20;
	int16_t x9 = 25, y9 = 40;
	int16_t x10 = 55, y10 = 40;
	int16_t x11 = 64, y11 = 20;
	myLCD.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myLCD.FG_COLOR);
	screenReset();
	myLCD.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myLCD.FG_COLOR);
	screenReset();

	printf("Ellipse\n");
	myLCD.drawEllipse(42, 24, 18, 12, false, myLCD.FG_COLOR);
	screenReset();
	myLCD.drawEllipse(42, 24, 18, 12, true, myLCD.FG_COLOR);
	screenReset();

	printf("Drawing Arc: drawArc and drawSimpleArc\n");
	int16_t centerX = 42;
	int16_t centerY = 30;
	int16_t radius = 20;

	printf("  Angle offset default: %i/n ", myLCD.getArcAngleOffset());
	// Draw upper half arc (180° to 360°)
	myLCD.drawArc(centerX, centerY, radius, 5, 180.0f, 360.0f, myLCD.FG_COLOR);
	screenReset();
	busy_wait_ms(2000);

	// Draw third quarter arc (180° to 270°)
	myLCD.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myLCD.FG_COLOR);
	screenReset();

	// Draw simple arc
	myLCD.drawSimpleArc(30, 24, 15, 0.0f, 300.0f, myLCD.FG_COLOR);
	screenReset();
}
#endif 

void Test502(void) {

	printf("Test 502 Sleep mode\n");
	char testStr1[]= "Sleep 5 secs!\r\n";
	char testStr2[]= "Awake!\r\n";

	myLCD.LCDfillScreen(0x00);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.LCDupdate();

	busy_wait_ms(TEST_DELAY2);
	myLCD.LCDenableSleep();
	busy_wait_ms(TEST_DELAY5);
	myLCD.LCDdisableSleep();

	myLCD.print(testStr2);
	myLCD.LCDupdate();
	busy_wait_ms(TEST_DELAY2);
	screenReset();
}

void Test503(void) {
	printf("Test 503 rotate mode\n");
	char testStr0[]= "Rotate  0\r\n";
	char testStr1[]= "Rotate   90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate   270\r\n";
	char testStr4[]= "Bottom  .\n";

	// rotation example
	myLCD.LCDfillScreen(0x00);
	myLCD.setCursor(0, 0);

	myLCD.setRotation(myLCD.rDegrees_90); // rotate 90 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.rDegrees_180); // rotate 180 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	myLCD.setCursor(0,39);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.rDegrees_270); // rotate 270 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	// revert back to no rotation
	myLCD.setRotation(myLCD.rDegrees_0);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr0);
	myLCD.setCursor(0,39);
	myLCD.print(testStr4);
	screenReset();
}

void Test504(void)
{
	printf("Test 504 invert screen command\n");

	screenReset();
	myLCD.setCursor(5, 5);
	myLCD.print("INVERT 12");
	screenReset();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	busy_wait_ms(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	busy_wait_ms(TEST_DELAY5);
}

void Test501(void){
	printf("Test 501 fill screen\n");
	myLCD.LCDfillScreen(0xFF);
	busy_wait_ms(TEST_DELAY5);
	myLCD.LCDfillScreen(0x00);
	busy_wait_ms(TEST_DELAY5);
	printf("Test 501-b fill Block\n");
	myLCD.LCDfillBlock(0x0F, 0);
	busy_wait_ms(TEST_DELAY5);
	myLCD.LCDfillBlock(0xFF, 3);
	busy_wait_ms(TEST_DELAY5);
	myLCD.LCDfillScreen(0x00);
}

void screenReset(void) {
	myLCD.LCDupdate();
	busy_wait_ms(TEST_DELAY5);
	myLCD.LCDclearBuffer();

}

// *************** EOF ****************

/// @endcond
