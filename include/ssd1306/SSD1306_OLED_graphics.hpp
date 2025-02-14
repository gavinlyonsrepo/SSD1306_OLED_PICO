 /*!
	@file SSD1306_OLED_graphics.hpp
	@brief OLED driven by SSD1306 controller. header file
		for the graphics based functions.
	@details Project Name: SSD1306_OLED_PICO
		URL: https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO
	@author  Gavin Lyons
*/

#pragma once

#include <cmath> // for "abs"
#include "SSD1306_OLED_font.hpp"
#include "SSD1306_OLED_Print.hpp"



/*! @brief Graphics class to hold graphic related functions */
class SSD1306_graphics : public SSD1306_OLEDFonts , public Print 
{

 public:

	SSD1306_graphics(int16_t w, int16_t h); // Constructor

	/*! Enum to hold current screen rotation in degrees bi color display  */
	enum display_rotate_e : uint8_t
	{
		rDegrees_0 =   0,    /**< display screen rotated 0 degrees */
		rDegrees_90 =  1,    /**< display screen rotated 90 degrees  */
		rDegrees_180 = 2,    /**< display screen rotated 180 degrees  */
		rDegrees_270 = 3     /**< display screen rotated 270 degrees */
	};

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
	DisplayRet::Ret_Codes_e writeChar( int16_t x, int16_t y, char value );
	DisplayRet::Ret_Codes_e writeCharString( int16_t x, int16_t y, char *text);
	void setTextWrap(bool w);

	int16_t height(void) const;
	int16_t width(void) const;
	display_rotate_e getRotation(void);
	void setRotation(display_rotate_e r);

 protected:
	
	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT;  /**< This is the 'raw' display h - never changes*/
	display_rotate_e  _display_rotate = rDegrees_0; /**< Enum to hold rotation */
	int16_t _width;  /**< Display w as modified by current _rotation*/
	int16_t _height;  /**< Display h as modified by current _rotation*/
	int16_t _cursor_x = 0; /**< Current X co-ord cursor position */
	int16_t _cursor_y = 0;  /**< Current Y co-ord cursor position */
	
	bool _textwrap = true;  /**< If set, text at right edge of display will wrap, print method*/

	private:
	/*!
		@brief Swaps the values of two int16_t variables.
		@param a Reference to the first integer.
		@param b Reference to the second integer.
		@note This function swaps the values in place without using additional memory.
	*/
	inline void swapInt16_OLED(int16_t& a, int16_t& b) {
		int16_t t = a;
		a = b;
		b = t;
	}
};

