/*!
	@file erm19264.hpp
	@brief ERM19264 LCD driven by UC1609C controller, header file.
	@author Gavin Lyons.
*/

#pragma once

// ** INCLUDES **
#include "display_graphics.hpp"
#include "hardware/spi.h"
#include "pico/stdlib.h"

// class
class ERM19264 : public displaylib_graphics
{

public:
	ERM19264(int16_t width, int16_t height);
	~ERM19264(){};
	// Rotate LCD commands
	enum LCD_rotate_cmd_e : uint8_t {
		ROTATION_FLIP_THREE = 0x00,
		ROTATION_FLIP_ONE =   0x02,
		ROTATION_NORMAL =     0x04,
		ROTATION_FLIP_TWO =   0x06
	};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;

	DisplayRet::Ret_Codes_e LCDupdate(void);
	DisplayRet::Ret_Codes_e LCDSetBufferPtr(uint8_t width, uint8_t height, std::span<uint8_t> buffer);
	DisplayRet::Ret_Codes_e LCDclearBuffer(void);
	void LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, std::span<uint8_t> data);
	void LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, std::span<const uint8_t> data);

	void LCDSPISetup(spi_inst_t *spi, uint32_t spiBaudRate, int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din);
	void LCDinit(uint8_t VbiasPot = UC1609_DEFAULT_GN_PM, uint8_t AddressSet = UC1609_ADDRESS_SET);
	void LCDEnable(uint8_t on);
	void LCDFillScreen(uint8_t pixel, uint8_t mircodelay);
	void LCDFillPage(uint8_t pixels);
	void LCDsetRotateCmd(LCD_rotate_cmd_e  rotatevalue);
	void LCDInvertDisplay(uint8_t on);
	void LCDAllpixelsOn(uint8_t bits);
	void LCDGotoXY(uint8_t column , uint8_t page);
	void LCDscroll(uint8_t bits);
	void LCDReset(void);
	void LCDPowerDown(void);
	void LCDSPIoff(void);

private:
	void SendData(uint8_t data);
	void SendCommand(uint8_t command, uint8_t value);

	// GPIO & SPI
	int8_t _display_CS;		  /**< GPIO Chip select  line */
	int8_t _display_CD;		  /**< GPIO Data or command line */
	int8_t _display_RST;	  /**< GPIO Reset line */
	int8_t _display_SCLK;	  /**< GPIO Clock Line , Tied to SPI interface*/
	int8_t _display_DIN;	  /**< GPIO MOSI Line , Tied to SPI interface*/
	spi_inst_t *_spiInterface; /**< SPI instance spi0 or spi1*/

	// Screen related
	int16_t _LCD_WIDTH = 192;				  /**< Width of LCD Screen in pixels */
	int16_t _LCD_HEIGHT = 64;				  /**< Height of LCD Screen in pixels */
	int8_t _LCD_PAGE_NUM = (_LCD_HEIGHT / 8); /**< Number of byte size pages LCD screen is divided into */
	std::span<uint8_t> _LCDbuffer;           /**< Buffer to hold screen data */
	uint8_t _VbiasPOT;	  /**< Contrast default 0x49 datasheet 00-FE, lower works better on Blue*/
	uint8_t _AddressCtrl; /**< Set AC [2:0] Program registers  for RAM address control. 0x00 to 0x07*/

	// UC1609 registers
	static constexpr uint8_t UC1609_SYSTEM_RESET = 0xE2; /**< System SW Reset */
	static constexpr uint8_t UC1609_POWER_CONTROL = 0x28; /**< Power control Address 0*/
	static constexpr uint8_t UC1609_PC_SET = 0x06; /**< PC[2:0] 110, Internal V LCD (7x charge pump) + 10b: 1.4mA*/
	static constexpr uint8_t UC1609_ADDRESS_CONTROL = 0x88; /**< Set RAM address control*/
	static constexpr uint8_t UC1609_ADDRESS_SET = 0x02; /**< Set AC [2:0] Program registers for RAM address control.*/
	static constexpr uint8_t UC1609_SET_PAGEADD = 0xB0; /**< Page address Set PA[3:0]*/
	static constexpr uint8_t UC1609_SET_COLADD_LSB = 0x00; /**< Column Address Set CA [3:0]*/
	static constexpr uint8_t UC1609_SET_COLADD_MSB = 0x10; /**< Column Address Set CA [7:4]*/
	static constexpr uint8_t UC1609_TEMP_COMP_REG = 0x27; /**< Temperature Compensation Register */
	static constexpr uint8_t UC1609_TEMP_COMP_SET = 0x00; /**< TC[1:0] = 00b= -0.00%/ C */
	static constexpr uint8_t UC1609_FRAMERATE_REG = 0xA0; /**< Frame rate */
	static constexpr uint8_t UC1609_FRAMERATE_SET = 0x01; /**< Set Frame Rate LC [4:3] 01b: 95 fps */
	static constexpr uint8_t UC1609_BIAS_RATIO = 0xE8; /**< Bias Ratio. The ratio between V-LCD and V-D.*/
	static constexpr uint8_t UC1609_BIAS_RATIO_SET = 0x03; /**< Set BR[1:0] = 11 (set to 9 default)*/
	static constexpr uint8_t UC1609_GN_PM = 0x81; /**< Set V BIAS Potentiometer to fine-tune V-D and V-LCD (double-byte command)*/
	static constexpr uint8_t UC1609_DEFAULT_GN_PM = 0x49; /**< Default only used if user does not specify Vbias*/
	static constexpr uint8_t UC1609_LCD_CONTROL = 0xC0; /**< Rotate map control*/
	static constexpr uint8_t UC1609_DISPLAY_ON = 0xAE; /**< Enables display*/
	static constexpr uint8_t UC1609_ALL_PIXEL_ON = 0xA4; /**< Sets all Pixels on*/
	static constexpr uint8_t UC1609_INVERSE_DISPLAY = 0xA6; /**< Inverts display*/
	static constexpr uint8_t UC1609_SCROLL = 0x40; /**< Scrolls, Set the scroll line number. 0-64 */

	// Delays
	static constexpr uint8_t UC1609_RESET_DELAY =  3;  /**< ms Delay ,datasheet >3uS*/
	static constexpr uint8_t UC1609_RESET_DELAY2 = 0; /**< mS delay datasheet says > 5mS, does not work?*/
	static constexpr uint8_t UC1609_INIT_DELAY = 100; /**<  mS delay ,after init*/
	static constexpr uint8_t UC1609_INIT_DELAY2 =  3;  /**< mS delay,  before reset called datasheet <3mS*/
};
