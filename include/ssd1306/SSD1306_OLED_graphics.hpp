 /*!
	@file SSD1306_OLED_graphics.hpp
	@brief OLED driven by SSD1306 controller. header file
		for the graphics based functions.
	@details Project Name: SSD1306_OLED_PICO
		URL: https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO
	@author  Gavin Lyons
*/

#pragma once

#include <cstdio>
#include <cstdint>
#include <cmath> // for "abs"
#include "SSD1306_OLED_font.hpp"
#include "SSD1306_OLED_Print.hpp"

#define swapOLEDPICO(a, b) { int16_t t = a; a = b; b = t; }

/*! Enum to hold current screen rotation in degrees bi color display  */
enum displayBC_rotate_e : uint8_t
{
	displayBC_Degrees_0 =   0,    /**< display screen rotated 0 degrees */
	displayBC_Degrees_90 =  1,    /**< display screen rotated 90 degrees  */
	displayBC_Degrees_180 = 2,    /**< display screen rotated 180 degrees  */
	displayBC_Degrees_270 = 3     /**< display screen rotated 270 degrees */
};

/*! @brief Font class to hold font data object  */
class SSD1306_OLEDFonts
{
	public:
		
		SSD1306_OLEDFonts();
		~SSD1306_OLEDFonts(){};

		uint8_t setFont(const uint8_t * font);
		void setInvertFont(bool invertStatus);
		bool getInvertFont(void);

		const uint8_t *_FontSelect = pFontDefault; /**< Pointer to the active font,  Fonts Stored are Const */
	
	protected:
		uint8_t _Font_X_Size = 0x06; /**< Width Size of a Font character */
		uint8_t _Font_Y_Size = 0x08; /**< Height Size of a Font character */
		uint8_t _FontOffset = 0x00; /**< Offset in the ASCII table 0x00 to 0xFF, where font begins */
		uint8_t _FontNumChars = 0xFE; /**< Number of characters in font 0x00 to 0xFE */
	private:
		bool _FontInverted = false; /**< Is the font inverted , False = normal , true = inverted*/
};

/*! @brief Graphics class to hold graphic related functions */
class SSD1306_graphics : public SSD1306_OLEDFonts , public Print 
{

 public:

	SSD1306_graphics(int16_t w, int16_t h); // Constructor

	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;
	// Graphics functions
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillScreen(uint8_t color);

	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
	  uint8_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
	  int16_t delta, uint8_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	  int16_t x2, int16_t y2, uint8_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	  int16_t x2, int16_t y2, uint8_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
	  int16_t radius, uint8_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
	  int16_t radius, uint8_t color);
	void setCursor(int16_t x, int16_t y);	

	// Text related functions 
	virtual size_t write(uint8_t);
	uint8_t writeChar( int16_t x, int16_t y, char value );
	uint8_t writeCharString( int16_t x, int16_t y, char *text);
	void setTextWrap(bool w);

	int16_t height(void) const;
	int16_t width(void) const;
	displayBC_rotate_e getRotation(void);
	void setRotation(displayBC_rotate_e r);

 protected:
	
	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT;  /**< This is the 'raw' display h - never changes*/
	displayBC_rotate_e  _display_rotate = displayBC_Degrees_0; /**< Enum to hold rotation */
	int16_t _width;  /**< Display w as modified by current _rotation*/
	int16_t _height;  /**< Display h as modified by current _rotation*/
	int16_t _cursor_x = 0; /**< Current X co-ord cursor position */
	int16_t _cursor_y = 0;  /**< Current Y co-ord cursor position */
	
	bool _textwrap = true;  /**< If set, text at right edge of display will wrap, print method*/
};

