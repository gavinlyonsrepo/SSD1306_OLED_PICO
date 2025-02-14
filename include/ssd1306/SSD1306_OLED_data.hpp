/*!
	@file SSD1306_OLED_data.hpp
	@brief file to hold common data to all classes and examples, return code enum
	@author Gavin Lyons.
*/

#pragma once

#include <cstdint>

namespace DisplayRet{
/*! Enum to define a standard return code for most functions in all files that return failures*/
enum Ret_Codes_e : uint8_t
{
	Success = 0,                /**< Success, Function ran without defined Error :)*/
	Reserved = 1,               /**< Reserved for future use*/
	FontDataEmpty = 2,          /**< The Font container is an invalid empty object */
	FontDataTooSmall = 3,       /**< Font is no small must be at least contain 5 bytes */
	CharScreenBounds = 4,       /**< Text Character is out of Screen bounds, Check x and y*/
	CharFontASCIIRange = 5,     /**< Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.*/
	CharArrayNullptr = 6,       /**< Text Character Array is an invalid pointer object*/
	BitmapDataEmpty= 7,         /**< The Bitmap container is an invalid object*/
	BitmapScreenBounds = 8,     /**< The Bitmap starting point is outside screen bounds, check x and y*/
	BitmapLargerThanScreen = 9, /**< The Bitmap is larger than screen, check  w and h*/
	BitmapVerticalSize = 10,    /**< A vertical Bitmap's height must be divisible by 8.*/
	BitmapHorizontalSize = 11,  /**< A horizontal Bitmap's width  must be divisible by 8*/
	BitmapSize = 12,            /**< Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h*/
	BufferSize = 13,            /**< Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)*/
	BufferEmpty = 14,           /**< The Buffer data container is an empty object*/
	I2CbeginFail = 15,          /**< Failed to open I2C*/
	I2CNotConnected = 16,       /**< I2C not connected as per checkConnection() tests */
	GenericError = 17           /**< Generic Error message */
};
}