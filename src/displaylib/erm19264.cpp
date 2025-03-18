/*!
	@file erm19264.cpp
	@brief ERM19264 LCD driven by UC1609C controller, src file. 
	@author Gavin Lyons.
*/

#include "../../include/displaylib/erm19264.hpp"

/*!
	@brief init the LCD class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
 */
ERM19264::ERM19264(int16_t lcdwidth, int16_t lcdheight) : displaylib_graphics(lcdwidth, lcdheight)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;
	_LCD_PAGE_NUM = (_LCD_HEIGHT / 8);
}

/*!
	@brief initialise LCD pinmodes and SPI setup
	@param spiType SPi instance to initialize
    @param spiSpeedKhz SPI baud rate in Khz
	@param cd GPIO data or command 
	@param rst GPIO reset
	@param cs GPIO Chip select
	@param sclk GPIO SPI Clock
	@param din GPIO MOSI
 */
void ERM19264::LCDSPISetup(spi_inst_t* spiType, uint32_t spiSpeedKhz,int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din)
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

	_spiInterface = spiType;
	// Initialize SPI port
	spi_init(_spiInterface, spiSpeedKhz * 1000);
	// Initialize SPI pins
	gpio_set_function(_display_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(_display_DIN, GPIO_FUNC_SPI);

	// Set SPI format
	spi_set_format(_spiInterface,		   // SPI instance
				   8,		   // Number of bits per transfer
				   SPI_CPOL_0, // Polarity (CPOL)
				   SPI_CPHA_0, // Phase (CPHA)
				   SPI_MSB_FIRST);

}

/*!
	@brief begin Method initialise LCD
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
 */
void ERM19264::LCDinit(uint8_t VbiasPOT, uint8_t AddressSet)
{
	_VbiasPOT = VbiasPOT;
	if (AddressSet > 7 ) AddressSet = 0x02;
	_AddressCtrl =  AddressSet;
	display_CD_SetHigh;
	display_CS_SetHigh;
	busy_wait_ms(UC1609_INIT_DELAY2);

	LCDReset();

	display_CS_SetLow;

	SendCommand(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET);
	SendCommand(UC1609_ADDRESS_CONTROL, _AddressCtrl);
	SendCommand(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	SendCommand(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);
	SendCommand(UC1609_POWER_CONTROL, UC1609_PC_SET);
	busy_wait_ms(UC1609_INIT_DELAY);

	SendCommand(UC1609_GN_PM, 0);
	SendCommand(UC1609_GN_PM, _VbiasPOT); //  changed by user

	SendCommand(UC1609_DISPLAY_ON, 0x01);					  // turn on display
	SendCommand(UC1609_LCD_CONTROL, ROTATION_NORMAL); // rotate to normal

	display_CS_SetHigh;
}

/*!
	 @brief Sends a command to the display
	 @param command Command to send
	 @param value the values to change
	 @note command and value  will be combined with OR
*/
void ERM19264::SendCommand(uint8_t command, uint8_t value)
{
	display_CD_SetLow;
	SendData(command | value);
	display_CD_SetHigh;
}

/*!
	@brief Resets LCD in a four wire setup called at start
	and  should also be called in a controlled power down setting
*/
void ERM19264::LCDReset()
{
	display_RST_SetLow;
	busy_wait_ms(UC1609_RESET_DELAY);
	display_RST_SetHigh;
	busy_wait_ms(UC1609_RESET_DELAY2);
}

/*!
	@brief Powerdown procedure for LCD see datasheet P40
*/
void ERM19264::LCDPowerDown(void)
{
	LCDReset();
	LCDEnable(0);
	display_CD_SetLow;
	display_RST_SetLow;
	display_CS_SetLow;
	display_SCLK_SetLow;
	display_SDA_SetLow;
}

/*! @brief End SPI operations. */
void ERM19264::LCDSPIoff(void){
	spi_deinit(_spiInterface);
}

/*!
	 @brief Turns On Display
	 @param bits  1  display on , 0 display off
*/
void ERM19264::LCDEnable(uint8_t bits)
{
	display_CS_SetLow;
	SendCommand(UC1609_DISPLAY_ON, bits);
	display_CS_SetHigh;
}

/*!
	@brief Scroll the displayed image up by SL rows.
	@details The valid SL value is between 0 (for no
		scrolling) and (64).
		Setting SL outside of this range causes undefined effect on the displayed image.
	@param bits 0-64 line number y-axis
*/
void ERM19264::LCDscroll(uint8_t bits)
{
	display_CS_SetLow;
	SendCommand(UC1609_SCROLL, bits);
	display_CS_SetHigh;
}

/*!
	@brief Rotates the display using LCD built-in rotate commands
	@details Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
	@param rotatevalue enum 4 possible values 000 010 100 110 (defined)
	@note If Mx is changed the buffer must BE updated see examples. 
		This rotates the display based on LCD commands. It is also 
		possible to rotate software buffer using setRotation method
*/
void ERM19264::LCDsetRotateCmd(LCD_rotate_cmd_e  rotatevalue)
{
	display_CS_SetLow;
	switch (rotatevalue)
	{
		case ROTATION_FLIP_THREE:
			SendCommand(UC1609_LCD_CONTROL, ROTATION_FLIP_THREE);
		break;
		case ROTATION_FLIP_ONE:
			SendCommand(UC1609_LCD_CONTROL, ROTATION_FLIP_ONE);
		break;
		case ROTATION_NORMAL:
			SendCommand(UC1609_LCD_CONTROL, ROTATION_NORMAL);
		break;
		case ROTATION_FLIP_TWO:
			SendCommand(UC1609_LCD_CONTROL, ROTATION_FLIP_TWO);
		break;
	default:
		SendCommand(UC1609_LCD_CONTROL, ROTATION_NORMAL);
		break;
	}
	display_CS_SetHigh;
}

/*!
	 @brief invert the display
	 @param bits 1 invert , 0 normal
*/
void ERM19264::LCDInvertDisplay(uint8_t bits)
{
	display_CS_SetLow;
	SendCommand(UC1609_INVERSE_DISPLAY, bits);
	display_CS_SetHigh;
}

/*!
	@brief turns on all Pixels
	@param bits Set DC[1] to force all SEG drivers to output ON signals
	1 all on ,  0 all off
*/
void ERM19264::LCDAllpixelsOn(uint8_t bits)
{
	display_CS_SetLow;
	SendCommand(UC1609_ALL_PIXEL_ON, bits);
	display_CS_SetHigh;
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay optional delay in microseconds can be set to zero normally.
*/
void ERM19264::LCDFillScreen(uint8_t dataPattern = 0, uint8_t delay = 0)
{
	display_CS_SetLow;
	uint16_t numofbytes = _LCD_WIDTH * (_LCD_HEIGHT / 8); // width * height
	for (uint16_t i = 0; i < numofbytes; i++)
	{
		SendData(dataPattern);
		busy_wait_us(delay);
	}
	display_CS_SetHigh;
}

/*!
	 @brief Fill the chosen page at cursor  with a datapattern
	 @param dataPattern can be set to 0 to FF (not buffer)
*/
void ERM19264::LCDFillPage(uint8_t dataPattern = 0)
{
	display_CS_SetLow;
	uint16_t numofbytes = ((_LCD_WIDTH * (_LCD_HEIGHT / 8)) / 8); // (width * height/8)/8 = 192 bytes
	for (uint16_t i = 0; i < numofbytes; i++)
	{
		SendData(dataPattern);
	}
	display_CS_SetHigh;
}

/*!
	 @brief Draw a bitmap in direct to screen to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data  span to the bitmap data
	 @note No buffer used. Data drawn onto screen directly
*/
void ERM19264::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, std::span<const uint8_t>  data)
{
	display_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >> 3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT)
		{
			continue;
		}
		SendCommand(UC1609_SET_COLADD_LSB, (column & 0x0F));
		SendCommand(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
		SendCommand(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _LCD_WIDTH)
			{
				continue;
			}
			offset = (w * (ty >> 3)) + tx;
			SendData(data[offset]);
		}
	}
	display_CS_SetHigh;
}

/*!
	 @brief Send data byte with SPI to UC1609
	 @param data the data byte to send 
*/
void ERM19264::SendData(uint8_t data){
	spi_write_blocking(_spiInterface, &data, 1);
}

/*!
	 @brief updates the LCD  i.e. writes the  shared buffer to the active screen
		pointed to by ActiveBuffer
	@return 
		-# Success 
		-# BufferEmpty if buffer is empty object
*/
DisplayRet::Ret_Codes_e ERM19264::LCDupdate()
{
	if (_LCDbuffer.empty())
	{
		printf("ERM19264_UC1609::LCDupdate Error Buffer is empty, cannot update screen\r\n");
		return DisplayRet::BufferEmpty;
	}
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = this->_LCD_WIDTH;
	uint8_t h = this->_LCD_HEIGHT;
	LCDBuffer(x, y, w, h, _LCDbuffer);
	return DisplayRet::Success;
}

/*!
	@brief clears the buffer of the active screen pointed to by ActiveBuffer 
	@return 
		-# Success 
		-# BufferEmpty if buffer is empty object
	 @note Does NOT write to the screen
*/
DisplayRet::Ret_Codes_e ERM19264::LCDclearBuffer()
{
	if (_LCDbuffer.empty())
	{
		printf("ERM19264_UC1609::LCDclearBuffer Error: Buffer is empty, cannot clear\r\n");
		return DisplayRet::BufferEmpty;
	}

	std::fill(_LCDbuffer.begin(), _LCDbuffer.end(), 0x00);
	return DisplayRet::Success;
}

/*!
	 @brief Draw an  array  to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data span to the buffer  array
	 @note Called by LCDupdate internally to write buffer to screen , can be called standalone 	as well
*/
void ERM19264::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, std::span<uint8_t> data)
{
	display_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y / 8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT)
		{
			continue;
		}

		SendCommand(UC1609_SET_COLADD_LSB, (column & 0x0F));
		SendCommand(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4);
		SendCommand(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _LCD_WIDTH)
			{
				continue;
			}
			offset = (w * (ty / 8)) + tx;
			SendData(data[offset++]);
		}
	}

	display_CS_SetHigh;
}

/*!
	@brief Draws a Pixel to the screen , overrides the  graphics library 
	@param x x co-ord of pixel
	@param y y co-ord of pixel
	@param colour colour of  pixel
*/
void ERM19264::drawPixel(int16_t x, int16_t y, uint8_t colour)
{
	if ((x < 0) || (x >= this->_width) || (y < 0) || (y >= this->_height)){
		return;}

	int16_t temp;
	uint8_t RotateMode = getRotation();
	switch (RotateMode) 
	{
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
	uint16_t tc = (_LCD_WIDTH * (y / 8)) + x;
	switch (colour)
	{
		case FG_COLOR: //1
			this->_LCDbuffer[tc] |= (1 << (y & 7));
		break;
		case BG_COLOR: //0
			this->_LCDbuffer[tc] &= ~(1 << (y & 7));
		break;
		case INVERSE:
			this->_LCDbuffer[tc] ^= (1 << (y & 7));
		break;
	}
}
/*!
	@brief sets the buffer pointer to the users screen data buffer
	@param width width of buffer in pixels
	@param height height of buffer in pixels
	@param buffer the buffer array which decays to pointer
	@return Will return:
		-# 0. Success
		-# 2. Buffer size calculations are incorrect BufferSize = w * (h/8),
		-# 3. Not a valid pointer object.
*/
DisplayRet::Ret_Codes_e ERM19264::LCDSetBufferPtr(uint8_t width, uint8_t height,std::span<uint8_t> buffer)
{
	if (buffer.size() != static_cast<size_t>(width * (height / 8)))
	{
		printf("ERM19264_UC1609::LCDSetBufferPtr Error 2: buffer size does not equal w * (h/8))\r\n");
		return DisplayRet::BufferSize;
	}
	_LCDbuffer = buffer;

	if (buffer.empty())	{
		printf("ERM19264_UC1609::LCDSetBufferPtr Error 3: Problem assigning buffer, received empty buffer\r\n");
		return DisplayRet::BufferEmpty;
	}
	return DisplayRet::Success;
}

/*!
	 @brief Goes to X Y position
	 @param  column Column 0-192
	 @param page 0-7
*/
void ERM19264::LCDGotoXY(uint8_t column , uint8_t page)
{
	display_CS_SetLow;
	SendCommand(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
	SendCommand(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
	SendCommand(UC1609_SET_PAGEADD, page++); 
	display_CS_SetHigh;
}


//***********************************************
