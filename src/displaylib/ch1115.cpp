/*!
	@file ch1115.cpp
	@brief ER_OLEDM1 OLED driven by CH1115 controller Source file
	@author  Gavin Lyons
*/

#include "pico/stdlib.h"
#include "../include/displaylib/ch1115.hpp"
#include "../include/displaylib/display_graphics.hpp"

/*!
	@brief init the OLED class object
	@param oledwidth width of oled in pixels
	@param oledheight height of oled in pixels
 */
ERMCH1115::ERMCH1115(int16_t oledwidth, int16_t oledheight) : displaylib_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_PAGE_NUM = (_OLED_HEIGHT / 8);
}

/*!
	@brief begin Method initialise OLED Sets pinmodes and SPI setup
	@param spiType  SPI instance
	@param spiSpeedKhz SPI bus speed in kiloHz
	@param cd GPIO command or data pin
	@param rst GPIO reset line
	@param cs GPIO chip select lin
	@param sclk GPIO SPI clock line tied to SPI instance
	@param din GPIO SPI data line tied to SPI instance(MOSI)
	@details Max SPI speed on the PICO is 62.5Mhz
*/
void ERMCH1115::OLEDSPISetup(spi_inst_t *spiType, uint32_t spiSpeedKhz, int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din)
{
	_display_CD = cd;
	_display_RST = rst;
	_display_CS = cs;
	_display_DIN = din;
	_display_SCLK = sclk;

	gpio_init(_display_CD);
	gpio_init(_display_RST);
	gpio_init(_display_CS);
	gpio_init(_display_SCLK);
	gpio_init(_display_DIN);

	gpio_set_dir(_display_CD, GPIO_OUT);
	gpio_set_dir(_display_RST, GPIO_OUT);
	gpio_set_dir(_display_CS, GPIO_OUT);
	gpio_put(_display_CS, true);

	spiInterface = spiType;
	// Initialize SPI port
	spi_init(spiInterface, spiSpeedKhz * 1000);
	// Initialize SPI pins
	gpio_set_function(_display_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(_display_DIN, GPIO_FUNC_SPI);

	// Set SPI format
	spi_set_format(spiInterface, // SPI instance
				   8,			 // Number of bits per transfer
				   SPI_CPOL_0,	 // Polarity (CPOL)
				   SPI_CPHA_0,	 // Phase (CPHA)
				   SPI_MSB_FIRST);
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
	@param OLEDcontrast Contrast of the OLED display default = 0x80 , range 0x00 to 0xFE
*/
void ERMCH1115::OLEDinit(uint8_t OLEDcontrast = ERMCH1115_CONTRAST_DATA_DEFAULT)
{
	const uint8_t ERMCH1115_INITDELAY = 100;/**< Init delay mS */
	_OLEDcontrast = OLEDcontrast;
	display_CS_SetLow;

	OLEDReset();

	send_command(ERMCH1115_DISPLAY_OFF, 0);

	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, 0);
	send_command(ERMCH1115_SET_DISPLAY_START_LINE, 0);

	send_command(ERMCH1115_CONTRAST_CONTROL, 0);
	send_command(_OLEDcontrast, 0);

	send_command(ERMCH1115_IREF_REG, 0);
	send_command(ERMCH1115_IREF_SET, 0);

	send_command(ERMCH1115_SEG_SET_REMAP, 0);
	send_command(ERMCH1115_SEG_SET_PADS, 0);
	send_command(ERMCH1115_ENTIRE_DISPLAY_ON, 0);
	send_command(ERMCH1115_DISPLAY_NORMAL, 0);

	send_command(ERMCH1115_MULTIPLEX_MODE_SET, 0);
	send_command(ERMCH1115_MULTIPLEX_DATA_SET, 0);

	send_command(ERMCH1115_COMMON_SCAN_DIR, 0);

	send_command(ERMCH1115_OFFSET_MODE_SET, 0);
	send_command(ERMCH1115_OFFSET_DATA_SET, 0);

	send_command(ERMCH1115_OSC_FREQ_MODE_SET, 0);
	send_command(ERMCH1115_OSC_FREQ_DATA_SET, 0);

	send_command(ERMCH1115_PRECHARGE_MODE_SET, 0);
	send_command(ERMCH1115_PRECHARGE_DATA_SET, 0);

	send_command(ERMCH1115_COM_LEVEL_MODE_SET, 0);
	send_command(ERMCH1115_COM_LEVEL_DATA_SET, 0);

	send_command(ERMCH1115_SET_PUMP_REG, ERMCH1115_SET_PUMP_SET);

	send_command(ERMCH1115_DC_MODE_SET, 0);
	send_command(ERMCH1115_DC_ONOFF_SET, 0);

	send_command(ERMCH1115_DISPLAY_ON, 0);
	_sleep = false;
	display_CS_SetHigh;
	busy_wait_ms(ERMCH1115_INITDELAY);
}

/*!
	@brief Sends a command to the display
	@param command Command to send
	@param value the values to change
	@note command and value  will be combined with OR
*/
void ERMCH1115::send_command(uint8_t command, uint8_t value)
{
	display_CD_SetLow;
	send_data(command | value);
	display_CD_SetHigh;
}

/*!
	 @brief Send data byte with SPI to ERMCH1115
	 @param data the data byte to send
*/
void ERMCH1115::send_data(uint8_t data)
{
	spi_write_blocking(spiInterface, &data, 1);
}

/*!
	@brief Resets OLED in a four wire setup called at start
*/
void ERMCH1115::OLEDReset()
{
	const uint8_t ERMCH1115_RST_DELAY1 = 10; /**< Reset Delay in mS*/
	const uint8_t ERMCH1115_RST_DELAY2 = 100; /**< Reset Delay 2 in mS */

	display_RST_SetHigh;
	busy_wait_ms(ERMCH1115_RST_DELAY1);
	display_RST_SetLow;
	busy_wait_ms(ERMCH1115_RST_DELAY1);
	display_RST_SetHigh;
	busy_wait_ms(ERMCH1115_RST_DELAY2);
}

/*!
	@brief Turns On Display
	@param bits  1  display on , 0 display off
*/
void ERMCH1115::OLEDEnable(uint8_t bits)
{

	display_CS_SetLow;
	if (bits)
	{
		_sleep = false;
		send_command(ERMCH1115_DISPLAY_ON, 0);
	}
	else
	{
		_sleep = true;
		send_command(ERMCH1115_DISPLAY_OFF, 0);
	}
	display_CS_SetHigh;
}

/*!
	@brief getting for _sleep member . is OLED in sleep mode
	@returns value of _sleep if true OLED is off and in sleep mode, 500uA.
*/
bool ERMCH1115::OLEDIssleeping() { return _sleep; }

/*!
	@brief Sets up Horizontal Scroll
	@param Timeinterval 0x00 -> 0x07 , 0x00 = 6 frames
	@param Direction 0x26 right 0x27 left (A2 – A0)
	@param mode Set Scroll Mode: (28H – 2BH)  0x28 = continuous
*/
void ERMCH1115::OLEDscrollSetup(uint8_t Timeinterval, uint8_t Direction, uint8_t mode)
{

	display_CS_SetLow;

	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SETUP, 0);
	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_SCOL, 0);
	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_ECOL, 0);

	send_command(Direction, 0);
	send_command(ERMCH1115_SPAGE_ADR_SET, 0);
	send_command(Timeinterval, 0);
	send_command(ERMCH1115_EPAGE_ADR_SET, 0);

	send_command(mode, 0);

	display_CS_SetHigh;
}

/*!
	@brief Turns on Horizontal scroll
	@param bits 1  Horizontal scroll on , 0 Horizontal scroll off
	@note OLEDscrollSetup must be called before it
*/
void ERMCH1115::OLEDscroll(uint8_t bits)
{

	display_CS_SetLow;
	bits ? send_command(ERMCH1115_ACTIVATE_SCROLL, 0) : send_command(ERMCH1115_DEACTIVATE_SCROLL, 0);
	display_CS_SetHigh;
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void ERMCH1115::OLEDContrast(uint8_t contrast)
{

	display_CS_SetLow;
	send_command(ERMCH1115_CONTRAST_CONTROL, 0);
	send_command(contrast, 0);
	display_CS_SetHigh;
}

/*!
	@brief Rotates the display vertically, A 180 degree 'flip'
	@param bits 1  on , 0 off
*/
void ERMCH1115::OLEDFlip(uint8_t bits)
{

	display_CS_SetLow;
	bits ? send_command(ERMCH1115_COMMON_SCAN_DIR, 0x08) : send_command(ERMCH1115_COMMON_SCAN_DIR, 0x00); // C0H - C8H
	bits ? send_command(ERMCH1115_SEG_SET_REMAP, 0x01) : send_command(ERMCH1115_SEG_SET_REMAP, 0x00);	  //(A0H - A1H)
	display_CS_SetHigh;
}

/*!
	@brief Turns on fade effect
	@param bits 0x00 to stop
	@note bits values: (see datasheet breatheffect P25 for more details)
		-# ON/OFF * * A4 A3 A2 A1 A0
		-# When ON/OFF =”H”, Breathing Light ON.
		-# Breathing Display Effect Maximum Brightness Adjust Set: (A4 – A3)
		-# Breathing Display Effect Time Interval Set: (A2 – A0)
		-# Default on is 0x81
*/
void ERMCH1115::OLEDfadeEffect(uint8_t bits)
{

	display_CS_SetLow;
	send_command(ERMCH1115_BREATHEFFECT_SET, 0);
	send_command(bits, 0);
	display_CS_SetHigh;
}

/*!
	@brief Power down function
	@details Disables screen and sets all independent GPIO low.
		call when powering down before end of operations
*/
void ERMCH1115::OLEDPowerDown(void)
{
	OLEDEnable(0);
	display_CD_SetLow;
	display_RST_SetLow;
	display_CS_SetLow;
	display_SCLK_SetLow;
	display_SDA_SetLow;
	_sleep = true;
}

/*!
	@brief invert the display
	@param bits 1 invert , 0 normal
*/
void ERMCH1115::OLEDInvert(uint8_t bits)
{

	display_CS_SetLow;
	bits ? send_command(ERMCH1115_DISPLAY_INVERT, 0) : send_command(ERMCH1115_DISPLAY_NORMAL, 0);
	display_CS_SetHigh;
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay A delay in milliseconds can be set to zero normally.
*/
void ERMCH1115::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		OLEDFillPage(row, dataPattern, delay);
	}
}

/*!
	 @brief Fill the chosen page(0-7)  with a datapattern
	 @param pageNum  page 0-7 divides 64 pixel screen into 8 pages or blocks  64/8
	 @param dataPattern can be set to 0 to FF (not buffer)
	 @param mydelay optional delay in milliseconds can be set to zero normally.
*/
void ERMCH1115::OLEDFillPage(uint8_t pageNum, uint8_t dataPattern, uint8_t mydelay)
{
	if (pageNum >= 8)
	{
		printf("Error OLEDFillPage 1 :page number must be between 0 and 7 \n");
		return;
	}

	display_CS_SetLow;
	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, pageNum);
	display_CS_SetHigh;
	busy_wait_us(2);
	display_CS_SetLow;
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		send_data(dataPattern);
		busy_wait_ms(mydelay);
	}
	display_CS_SetHigh;
}

/*!
	 @brief Draw a bitmap  to the screen
	 @param x offset 0-128
	 @param y offset 0-64
	 @param w width 0-128
	 @param h height 0-64
	 @param data  pointer to the bitmap
	 @note data is const. writes direct to screen , no buffer.
*/
void ERMCH1115::OLEDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t *data)
{
	display_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >> 3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT)
		{
			continue;
		}
		send_command(ERMCH1115_SET_COLADD_LSB, (column & 0x0F));
		send_command(ERMCH1115_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(ERMCH1115_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _OLED_WIDTH)
			{
				continue;
			}
			offset = (w * (ty >> 3)) + tx;
			send_data(data[offset]);
		}
	}
	display_CS_SetHigh;
}

/*!
	 @brief updates the OLED i.e. writes  buffer to the screen
*/
void ERMCH1115::OLEDupdate()
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = this->_OLED_WIDTH;
	uint8_t h = this->_OLED_HEIGHT;
	OLEDBufferScreen(x, y, w, h, this->_OLEDbuffer);
}

/*!
	 @brief clears the active shared buffer i.e. does NOT write to the screen
*/
void ERMCH1115::OLEDclearBuffer()
{
	memset(this->_OLEDbuffer, 0x00, (this->_OLED_WIDTH * (this->_OLED_HEIGHT / 8)));
}

/*!
	 @brief Draw buffer data to the screen
	 @param x offset 0-128
	 @param y offset 0-64
	 @param w width 0-128
	 @param h height 0-64
	 @param data pointer to the data array
	 @note Called by OLEDupdate, used internally mostly
*/
void ERMCH1115::OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t *data)
{
	display_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y / 8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT)
		{
			continue;
		}

		send_command(ERMCH1115_SET_COLADD_LSB, (column & 0x0F));
		send_command(ERMCH1115_SET_COLADD_MSB, (column & 0XF0) >> 4);
		send_command(ERMCH1115_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _OLED_WIDTH)
			{
				continue;
			}
			offset = (w * (ty / 8)) + tx;
			send_data(data[offset++]);
		}
	}
	display_CS_SetHigh;
}

/*!
	@brief Draws a Pixel to the screen, overides the  graphics library
	@param x x co-ord of pixel
	@param y y co-ord of pixel
	@param colour colour of  pixel
	@note virtual function overides the  graphics library
*/
void ERMCH1115::drawPixel(int16_t x, int16_t y, uint8_t colour)
{
	if ((x < 0) || (x >= this->_width) || (y < 0) || (y >= this->_height))
	{
		return;
	}
	int16_t temp;
	uint8_t RotateMode = getRotation();
	switch (RotateMode) {
	case 1:
		temp = x;
		x = WIDTH - 1 - y;
		y = temp;
	break;
	case 2:
		x = WIDTH - 1 - x;
		y = HEIGHT - 1 - y;
	break;
	case 3:
		temp = x;
		x = y;
		y = HEIGHT - 1 - temp;
	break;
	}
	uint16_t tc = (_OLED_WIDTH * (y / 8)) + x;
	switch (colour)
	{
	case FG_COLOR:
		this->_OLEDbuffer[tc] |= (1 << (y & 7));
		break;
	case BG_COLOR:
		this->_OLEDbuffer[tc] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		this->_OLEDbuffer[tc] ^= (1 << (y & 7));
		break;
	}
}

/*!
	@brief sets the buffer pointer to the users screen data buffer
	@param width width of buffer in pixels
	@param height height of buffer in pixels
	@param pBuffer the buffer array which decays to pointer
	@param sizeOfBuffer size of buffer
	@return Will return:
		-# 0. Success
		-# 2. Buffer size calculations are incorrect BufferSize = w * (h/8),
		-# 3. Not a valid pointer object.
*/
uint8_t ERMCH1115::OLEDSetBufferPtr(uint8_t width, uint8_t height, uint8_t *pBuffer, uint16_t sizeOfBuffer)
{
	if (sizeOfBuffer != width * (height / 8))
	{
		printf("ERMCH1115::::OLEDSetBufferPtr Error 2: buffer size does not equal: w * (h/8))\r\n");
		return 2;
	}
	_OLEDbuffer = pBuffer;
	if (_OLEDbuffer == nullptr)
	{
		printf("ERMCH1115::::OLEDSetBufferPtr Error 3: Problem assigning buffer pointer\r\n");
		return 3;
	}
	return 0;
}


//************** EOF *************************
