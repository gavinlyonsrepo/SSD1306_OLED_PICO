 /*!
	@file  nokia5110.hpp
	@brief Header file for NOKIA 5110 library to communicate with PICO
	@author  Gavin Lyons
*/

#pragma once

// libraries 
#include <cstdint>
#include <cstdbool>
#include <cstdio>
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "display_graphics.hpp"

/*!
	@brief Class Controls SPI comms and LCD functionality
*/
class NOKIA_5110 : public displaylib_graphics
{

public:

	NOKIA_5110(int16_t width, int16_t height);
	~NOKIA_5110(){};
	
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) override;

	DisplayRet::Ret_Codes_e LCDSPISetup(spi_inst_t *spi, uint32_t spiBaudRate, int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din);
	void LCDInit(bool Inverse, uint8_t Contrast,uint8_t Bias);
	DisplayRet::Ret_Codes_e LCDSetBufferPtr(uint8_t width, uint8_t height, std::span<uint8_t> buffer);
	
	DisplayRet::Ret_Codes_e LCDupdate(void);
	DisplayRet::Ret_Codes_e LCDclearBuffer(void);
	void LCDBuffer(std::span<uint8_t> data);

	void LCDenableSleep(void);
	void LCDdisableSleep(void);
	bool LCDIsSleeping(void);

	void LCDSPIoff(void);
	void LCDPowerDown(void);

	void LCDSetContrast(uint8_t con);
	void LCDfillScreen(uint8_t fill = 0xFF);
	void LCDinvertDisplay(bool inv);
	void LCDgotoXY(uint8_t x, uint8_t y);
	void LCDfillBlock(uint8_t FillData = 0xFF , uint8_t RowBlockNum = 0);

private:

	void LCDWriteData(uint8_t data);
	void LCDWriteCommand(uint8_t command);

	// LCD Commands & registers list
	static constexpr uint8_t LCD_FUNCTIONSET           = 0x20; /**<LCD function set*/
	static constexpr uint8_t LCD_POWERDOWN             = 0x04; /**<LCD power off */
	static constexpr uint8_t LCD_ENTRYMODE             = 0x02; /**<LCD entry mode */
	static constexpr uint8_t LCD_EXTENDEDINSTRUCTION   = 0x01; /**<LCD get into the EXTENDED mode when combined with Function set*/

	static constexpr uint8_t LCD_DISPLAYCONTROL        = 0x08; /**<Set display control */
	static constexpr uint8_t LCD_DISPLAYBLANK          = 0x00; /**<Blank display */
	static constexpr uint8_t LCD_DISPLAYNORMAL         = 0x04; /**<normal mode display */
	static constexpr uint8_t LCD_DISPLAYALLON          = 0x01; /**<all pixels on */
	static constexpr uint8_t LCD_DISPLAYINVERTED       = 0x05; /**<display inverted */

	static constexpr uint8_t LCD_SETYADDR =  0x40; /**<Y axis address setting */
	static constexpr uint8_t LCD_SETXADDR =  0x80; /**<X axis address setting */

	static constexpr uint8_t LCD_SETTEMP   = 0x04; /**<set temperature coefficient */ 
	static constexpr uint8_t LCD_CONTRAST  = 0xB0; /**<default value set LCD VOP contrast range 0xB1-BF */
	static constexpr uint8_t LCD_BIAS      = 0x13; /**<LCD Bias mode 1:48 0x12 to 0x14 */

	// SPI
	int8_t _display_CS;		  /**< GPIO Chip select  line */
	int8_t _display_CD;		  /**< GPIO Data or command line */
	int8_t _display_RST;	  /**< GPIO Reset line */
	int8_t _display_SCLK;	  /**< GPIO Clock Line , Tied to SPI interface*/
	int8_t _display_DIN;	  /**< GPIO MOSI Line , Tied to SPI interface*/
	spi_inst_t *_spiInterface; /**< SPI instance spi0 or spi1*/

	// Display
	uint8_t  _contrast;     /**< LCD contrast */
	uint8_t  _bias;         /**< LCD bias*/
	bool _inverse = false;  /**< LCD inverted , false for off*/
	bool _sleep;            /**< LCD sleep mode*/

// Screen related
	int16_t _LCD_WIDTH = 84;				  /**< Width of LCD Screen in pixels */
	int16_t _LCD_HEIGHT = 48;				  /**< Height of LCD Screen in pixels */
	int8_t _LCD_PAGE_NUM = (_LCD_HEIGHT / 8); /**< Number of byte size pages LCD screen is divided into */
	int16_t _LCD_Display_size =  (_LCD_WIDTH * _LCD_PAGE_NUM); /**< Size of standard display 84 * 48/6 = 504 bytes */
	std::span<uint8_t> _LCDbuffer;           /**< Buffer to hold screen data */
}; //end of class


