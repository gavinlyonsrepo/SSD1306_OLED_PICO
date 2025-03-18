 /*!
	@file display_graphics.hpp
	@brief header file for the graphics based functions, 1-bit color displays.
	@author  Gavin Lyons
*/

#pragma once


#include <cmath> // for "abs"
#include "display_fonts.hpp"
#include "display_print.hpp"

#define _ADVANCED_GRAPHICS_ENABLE

#ifdef _ADVANCED_GRAPHICS_ENABLE
#include <vector>
#endif

/*! @brief Graphics class to hold graphic related functions */
class displaylib_graphics : public displaylib_fonts , public Print 
{

 public:

	displaylib_graphics(int16_t w, int16_t h); // Constructor

	/*! @brief Display Pixel colours  definition */
	enum PixelColor : uint8_t {
		BG_COLOR  = 0,  /**< Background color */
		FG_COLOR  = 1,  /**< foreground color */
		INVERSE   = 2   /**< inverted color */
	};

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
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
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

	void setDrawBitmapAddr(bool mode);
	DisplayRet::Ret_Codes_e  drawBitmap(int16_t x, int16_t y, std::span<const uint8_t> bitmap,
		int16_t w, int16_t h, uint8_t color, uint8_t bg);

	int16_t height(void) const;
	int16_t width(void) const;
	display_rotate_e getRotation(void);
	void setRotation(display_rotate_e r);


#ifdef _ADVANCED_GRAPHICS_ENABLE
	void drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, 
		uint8_t length, int offset, uint8_t color);
	void drawQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, 
		int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
	void fillQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, 
		int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
	DisplayRet::Ret_Codes_e drawPolygon(int16_t x, int16_t y, uint8_t sides, 
		int16_t diameter, float rotation, bool fill , uint8_t color);
	void drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, 
		float startAngle, float endAngle, uint8_t color);
	void drawSimpleArc(int16_t cx, int16_t cy, int16_t radius, float startAngle, 
		float endAngle, uint8_t color);
	void drawEllipse(int16_t cx, int16_t cy, int16_t semiMajorAxis, 
			int16_t semiMinorAxis, bool fill, uint8_t color);
	DisplayRet::Ret_Codes_e drawDotGrid(int16_t x, int16_t y, int16_t w, 
			int16_t h, uint8_t DotGridGap, uint8_t color);
	float getArcAngleMax() const;
	void setArcAngleMax(float arcAngleMax);
	int getArcAngleOffset() const;
	void setArcAngleOffset(int arcAngleOffset);

#endif

 protected:
	
	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT;  /**< This is the 'raw' display h - never changes*/
	display_rotate_e  _display_rotate = rDegrees_0; /**< Enum to hold rotation */
	int16_t _width;  /**< Display w as modified by current _rotation*/
	int16_t _height;  /**< Display h as modified by current _rotation*/
	int16_t _cursor_x = 0; /**< Current X co-ord cursor position */
	int16_t _cursor_y = 0;  /**< Current Y co-ord cursor position */
	bool _drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */
	bool _textwrap = true;  /**< If set, text at right edge of display will wrap, print method*/
#ifdef _ADVANCED_GRAPHICS_ENABLE
	float _arcAngleMax = 360.0f; /**< Maximum angle of Arc , used by drawArc*/
	int _arcAngleOffset= 0; /**< used by drawArc, offset for adjusting the starting angle of arc. default positive X-axis (0°)*/
	float sineFromDegrees(float angle);
	float cosineFromDegrees(float angle);
	void ellipseHelper(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint8_t color);
	void drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint8_t color);
#endif	

		void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
							uint8_t color);
		void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
								int16_t delta, uint8_t color);
	private:
	/*!
		@brief Swaps the values of two int16_t variables.
		@param a Reference to the first integer.
		@param b Reference to the second integer.
		@note This function swaps the values in place without using additional memory.
	*/
	inline void swapInt16display(int16_t& a, int16_t& b) {
		int16_t t = a;
		a = b;
		b = t;
	}
};

