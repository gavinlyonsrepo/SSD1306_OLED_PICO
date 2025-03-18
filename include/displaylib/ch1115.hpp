/*!
   @file ch1115.hpp
   @brief ER_OLEDM1 OLED driven by CH1115 controller header file,
	   Main file For OLED functions
   @author  Gavin Lyons
*/

#pragma once

// ** INCLUDES **
#include "hardware/spi.h"
#include "displaylib/display_graphics.hpp"


// ** CLASS SECTION **

/*! @brief class to drive the ERMCh1115 OLED */
class ERMCH1115 : public displaylib_graphics
{
private:
	/* CH1115 Command Set*/
	// Fundamental Commands
	static constexpr uint8_t ERMCH1115_ENTIRE_DISPLAY_ON  = 0xA4; /**< Forcibly turns the entire display on regardless of the contents of the display data RAM */
	static constexpr uint8_t ERMCH1115_ENTIRE_DISPLAY_OFF  = 0xA5; /**< Forcibly turns the entire display on regardless of the contents of the display data RAM */
	static constexpr uint8_t ERMCH1115_DISPLAY_ON  = 0xAF; /**< Turns on Display */
	static constexpr uint8_t ERMCH1115_DISPLAY_OFF  = 0xAE; /**< Turns off Display */
	static constexpr uint8_t ERMCH1115_DISPLAY_NORMAL  = 0xA6; /**< Display  Inversion off*/
	static constexpr uint8_t ERMCH1115_DISPLAY_INVERT  = 0xA7; /**< Display  Inversion on*/
	static constexpr uint8_t ERMCH1115_CONTRAST_CONTROL  = 0x81; /**<  Contrast Control Mode Set */
	static constexpr uint8_t ERMCH1115_CONTRAST_DATA_DEFAULT  = 0x80; /**<  Contrast Data Reg Set  = 0x00 to  = 0xFF */
	// Addressing Setting Commands
	static constexpr uint8_t ERMCH1115_SET_PAGEADD  = 0xB0; /**< Set Page Address: (B0H - B7H) */
	static constexpr uint8_t ERMCH1115_SET_COLADD_LSB  = 0x00; /**< Set Lower Column Address: (00H - 0FH) */
	static constexpr uint8_t ERMCH1115_SET_COLADD_MSB  = 0x10; /**< Set Higher Column Address: (10H – 1FH) */
	static constexpr uint8_t ERMCH1115_SET_DISPLAY_START_LINE  = 0x40; /**< Determine the initial display line */
	// Hardware Configuration Commands
	static constexpr uint8_t ERMCH1115_SET_PUMP_REG  = 0x30; /**< Output voltage (V PP ) internal charger pump. */
	static constexpr uint8_t ERMCH1115_SET_PUMP_SET    = 0x01; /**<  A1:A0[] 01 = 7.4v  */
	static constexpr uint8_t ERMCH1115_IREF_REG  = 0x82; /**< I REF Resistor Set  */
	static constexpr uint8_t ERMCH1115_IREF_SET  = 0x00; /**< I REF Resistor adjust */
	static constexpr uint8_t ERMCH1115_SEG_SET_REMAP   = 0xA0; /**< Set Segment Re-map: (A0H - A1H) */
	static constexpr uint8_t ERMCH1115_SEG_SET_PADS  = 0xA2; /**<Set SEG pads hw config: (A2H – A3H) */
	static constexpr uint8_t ERMCH1115_MULTIPLEX_MODE_SET  = 0xA8; /**<Multiplex Ration Mode Set */
	static constexpr uint8_t ERMCH1115_MULTIPLEX_DATA_SET   = 0x3F; /**<Multiplex Ration Data Set: (00H - 3FH) */
	static constexpr uint8_t ERMCH1115_DC_MODE_SET  = 0xAD; /**< DC-DC Control Mode Set */
	static constexpr uint8_t ERMCH1115_DC_ONOFF_SET  = 0x8B; /**< DC-DC ON/OFF Mode Set (8AH - 8BH) (a=off; b=on) */
	static constexpr uint8_t ERMCH1115_COMMON_SCAN_DIR  = 0xC0; /**< Set Common Op Scan Dir:C0H - C8H Flip vertical */
	static constexpr uint8_t ERMCH1115_OFFSET_MODE_SET  = 0xD3; /**<Display Offset Mode Set: */
	static constexpr uint8_t ERMCH1115_OFFSET_DATA_SET  = 0x00; /**<Display Offset Data Set: (00H~3FH) */
	static constexpr uint8_t ERMCH1115_BREATHEFFECT_SET  = 0x23; /**< Set Breathing Display Effect: */
	static constexpr uint8_t ERMCH1115_BREATHEFFECT_DATA  = 0x81; /**< Display Effect ON/OFF and Time Interval: */
	// Timing & Driving Scheme Setting Commands
	static constexpr uint8_t ERMCH1115_OSC_FREQ_MODE_SET   = 0xD5; /**<Divide Ratio/Oscillator Frequency Mode Set: (D5H) */
	static constexpr uint8_t ERMCH1115_OSC_FREQ_DATA_SET   = 0xA0; /**<Divide Ratio/Oscillator Frequency Data Set: */
	static constexpr uint8_t ERMCH1115_PRECHARGE_MODE_SET  = 0xD9; /**<Pre-charge Period Mode Set: */
	static constexpr uint8_t ERMCH1115_PRECHARGE_DATA_SET  = 0x22; /**<Discharge/Pre-charge Period Data Set: (00H - FFH) */
	static constexpr uint8_t ERMCH1115_COM_LEVEL_MODE_SET  = 0xDB; /**<COM Deselect Level Mode Set: (DBH) */
	static constexpr uint8_t ERMCH1115_COM_LEVEL_DATA_SET  = 0x40; /**<VCOM Deselect Level Data Set: (00H - FFH) */
	// HORIZONTAL Scrolling Commands
	static constexpr uint8_t ERMCH1115_HORIZONTAL_A_SCROLL_SETUP  = 0x24; /**<  Additional Horizontal Scroll Setup Mode Set: (24H) */
	static constexpr uint8_t ERMCH1115_HORIZONTAL_A_SCROLL_SET_SCOL  = 0x00; /**< Start Column Position Set: (A7 – A0) */
	static constexpr uint8_t ERMCH1115_HORIZONTAL_A_SCROLL_SET_ECOL  = 0x7F; /**< End Column Position Set: (B7 – B0) */
	static constexpr uint8_t ERMCH1115_SCROLL_SETUP   = 0x26; /**<Horizontal Scroll Setup: 26 right 27 left (A2 – A0)*/
	static constexpr uint8_t ERMCH1115_SPAGE_ADR_SET  = 0x00; /**< Start Page Address Set  = 0x00 = page 0 */
	static constexpr uint8_t ERMCH1115_TIME_SET  = 0x00; /**<Time Interval Set: (B2 – B0)  = 0x00 = 6 frames */
	static constexpr uint8_t ERMCH1115_EPAGE_ADR_SET   = 0x07; /**<End Page Address Set: (C2 – C0)  */
	static constexpr uint8_t ERMCH1115_SET_SCROLL_MODE  = 0x28; /**< Set Scroll Mode: (28H – 2BH)  = 0x28 = continuous */
	static constexpr uint8_t ERMCH1115_DEACTIVATE_SCROLL   = 0x2E; /**< De-active Scroll mode */
	static constexpr uint8_t ERMCH1115_ACTIVATE_SCROLL    = 0x2F; /**<active Scroll mode */

	void send_data(uint8_t data);
	void send_command(uint8_t command, uint8_t value);

	int8_t _display_CS;   /**< GPIO Chip select line*/
	int8_t _display_CD;   /**< GPIO Data or command line */
	int8_t _display_RST;  /**< GPIO Reset line */
	int8_t _display_SCLK; /**< GPIO Clock Line tied to chosen SPI interface*/
	int8_t _display_DIN;  /**< GPIO MOSI Line tied to chosen SPI interface*/

	bool _sleep = true;		  /**< False awake/ON , true sleep/OFF */
	uint8_t _OLEDcontrast;	  /**< Contrast default 0x80 datasheet 00-FF */
	spi_inst_t *spiInterface; /**< SPI interface instance */

	int16_t _OLED_WIDTH = 128;					/**< Width of OLED Screen in pixels */
	int16_t _OLED_HEIGHT = 64;					/**< Height of OLED Screen in pixels */
	int8_t _OLED_PAGE_NUM = (_OLED_HEIGHT / 8); /**< Number of byte size pages OLED screen is divided into */
	uint8_t *_OLEDbuffer = nullptr;				/**< Pointer to screen buffer data*/

public:
	ERMCH1115(int16_t width, int16_t height);
	~ERMCH1115(){};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;
	void OLEDupdate(void);
	void OLEDclearBuffer(void);
	void OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t *data);
	uint8_t OLEDSetBufferPtr(uint8_t width, uint8_t height, uint8_t *pBuffer, uint16_t sizeOfBuffer);

	void OLEDSPISetup(spi_inst_t *spi, uint32_t spiBaudRate, int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din);
	void OLEDinit(uint8_t OLEDcontrast);
	void OLEDReset(void);

	void OLEDFillScreen(uint8_t pixel, uint8_t mircodelay);
	void OLEDFillPage(uint8_t page_num, uint8_t pixels, uint8_t delay);
	void OLEDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t *data);

	void OLEDEnable(uint8_t on);
	void OLEDInvert(uint8_t on);
	void OLEDFlip(uint8_t on);
	void OLEDContrast(uint8_t contrast);
	void OLEDscroll(uint8_t bits);
	void OLEDscrollSetup(uint8_t Timeinterval = ERMCH1115_TIME_SET, uint8_t Direction = ERMCH1115_SCROLL_SETUP, uint8_t mode = ERMCH1115_SET_SCROLL_MODE);
	void OLEDfadeEffect(uint8_t bits = ERMCH1115_BREATHEFFECT_DATA);
	bool OLEDIssleeping(void);
	void OLEDPowerDown(void);
};// end of class
