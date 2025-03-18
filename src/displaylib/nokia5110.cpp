/*!
 * @file nokia5110.cpp
 * @brief   Source file for NOKIA 5110 library to communicate with LCD
 * @author Gavin Lyons.
 */

#include "../../include/displaylib/nokia5110.hpp"

/*!
	@brief init the LCD class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
 */
NOKIA_5110::NOKIA_5110(int16_t lcdwidth, int16_t lcdheight) : displaylib_graphics(lcdwidth, lcdheight)
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
	@return At present just success.
 */
DisplayRet::Ret_Codes_e NOKIA_5110::LCDSPISetup(spi_inst_t *spiType, uint32_t spiSpeedKhz, int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din)
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
	auto baudRateReturned = spi_init(_spiInterface, spiSpeedKhz * 1000);
	if (baudRateReturned  == 0)
	{
		printf("Warning :: the actual baud rate set of spi_init is zero\n");
	}
	// Initialize SPI pins
	gpio_set_function(_display_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(_display_DIN, GPIO_FUNC_SPI);

	// Set SPI format
	spi_set_format(_spiInterface, // SPI instance
				   8,			  // Number of bits per transfer
				   SPI_CPOL_0,	  // Polarity (CPOL)
				   SPI_CPHA_0,	  // Phase (CPHA)
				   SPI_MSB_FIRST);

	return DisplayRet::Success;
}

/*!
	@brief Init the LCD command sequence, called from begin
			This sends the commands to the PCD8544 to init LCD
	@param Inverse false normal mode true display inverted
	@param Contrast Set LCD VOP contrast range 0xB1-BF
	@param Bias LCD Bias mode 1:48 0x12 to 0x14
*/
void NOKIA_5110::LCDInit(bool Inverse, uint8_t Contrast, uint8_t Bias)
{
	_inverse = Inverse;
	_bias = Bias;
	_contrast = Contrast;
	busy_wait_ms(100);
	display_RST_SetHigh;
	display_RST_SetLow;
	busy_wait_ms(50);
	display_RST_SetHigh;
	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION);
	LCDWriteCommand(_bias);
	LCDWriteCommand(LCD_SETTEMP);
	LCDWriteCommand(_contrast);
	LCDWriteCommand(LCD_FUNCTIONSET); // We must send 0x20 before modifying the display control mode
	// set LCDdisplayUpdate to normal mode or inverted
	if (_inverse == false)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
}

/*!
	@brief End SPI operations.
*/
void NOKIA_5110::LCDSPIoff(void)
{
	spi_deinit(_spiInterface);
}

/*!
	@brief Powerdown procedure for LCD see datasheet P40
*/
void NOKIA_5110::LCDPowerDown(void)
{
	LCDfillScreen(0x00);
	display_CD_SetLow;
	display_RST_SetLow;
	display_CS_SetLow;
	display_SCLK_SetLow;
	display_SDA_SetLow;
}

/*!
	@brief  Writes a byte to the PCD8544
	@param dataByte byte will be sent as command or data depending on status of DC line
*/
void NOKIA_5110::LCDWriteData(uint8_t dataByte)
{
	spi_write_blocking(_spiInterface, &dataByte, 1);
}

/*!
	@brief  Writes a command byte to the PCD8544
	@param command The command byte to send
*/
void NOKIA_5110::LCDWriteCommand(uint8_t command)
{
	display_CD_SetLow;
	display_CS_SetLow;
	LCDWriteData(command);
	display_CS_SetHigh;
}

/*!
	@brief Function to set contrast passed a byte
	@param contrast Set LCD VOP Contrast B0 to BF
*/
void NOKIA_5110::LCDSetContrast(uint8_t contrast)
{
	_contrast = contrast;
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION);
	LCDWriteCommand(_contrast);
	LCDWriteCommand(LCD_FUNCTIONSET);
}

/*!
	@brief Set a single pixel in the buffer
	@param x  x coordinate
	@param y  y coordinate
	@param color Color of pixel
*/
void NOKIA_5110::drawPixel(int16_t x, int16_t y, uint8_t color)
{
	if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)){
		return;}

	uint8_t rotation = getRotation();
	switch (rotation)
	{
	case 1:
		std::swap(x, y);
		y = HEIGHT - 1 - y;
		break;
	case 2:
		x = WIDTH - 1 - x;
		y = HEIGHT - 1 - y;
		break;
	case 3:
		std::swap(x, y);
		x = WIDTH - 1 - x;
	}
	uint16_t tc = (_LCD_WIDTH * (y / 8)) + x;
	switch (color)
	{
	case FG_COLOR: // 1
		this->_LCDbuffer[tc] |= (1 << (y & 7));
		break;
	case BG_COLOR: // 0
		this->_LCDbuffer[tc] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		this->_LCDbuffer[tc] ^= (1 << (y & 7));
		break;
	}
}

/*!
	@brief Writes the buffer (with pattern) to the LCD
	@param Pattern The pattern to send 0x00 to 0xFF
*/
void NOKIA_5110::LCDfillScreen(uint8_t Pattern)
{
	uint16_t i;
	LCDWriteCommand(LCD_SETYADDR); // set y = 0
	LCDWriteCommand(LCD_SETXADDR); // set x = 0
	display_CD_SetHigh;			   // Data send
	display_CS_SetLow;
	for (i = 0; i < _LCD_Display_size; i++)
	{
		LCDWriteData(Pattern);
	}
	display_CS_SetHigh;
}

/*!
	@brief inverts color on display
	@param invert True = Inverted mode , False = Display control mode normal
*/
void NOKIA_5110::LCDinvertDisplay(bool invert)
{
	if (invert == false){
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
		_inverse = false;
	}else
	{
		_inverse = true;
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
	}
}

/*!
	@brief Turn on sleep mode
*/
void NOKIA_5110::LCDenableSleep(void)
{
	_sleep = true;
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_POWERDOWN);
}

/*!
	@brief Turn off sleep mode
*/
void NOKIA_5110::LCDdisableSleep(void)
{
	_sleep = false;
	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION);
	LCDWriteCommand(_bias);
	LCDWriteCommand(LCD_SETTEMP);
	LCDWriteCommand(_contrast);
	LCDWriteCommand(LCD_FUNCTIONSET); // normal mode
	// set LCDdisplayUpdate to normal mode
	if (_inverse == false)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
}

/*!
	@brief LCDisSleeping
	@return  value of _sleep, if true LCD is in sleep mode.
*/
bool NOKIA_5110::LCDIsSleeping() { return _sleep; }

/*!
	 @brief updates the LCD  i.e. writes the  shared buffer to the active screen
		pointed to by ActiveBuffer
	@return
		-# Success
		-# BufferEmpty if buffer is empty object
*/
DisplayRet::Ret_Codes_e NOKIA_5110::LCDupdate()
{
	if (_LCDbuffer.empty())
	{
		printf("NOKIA_5110 ::LCDupdate Error Buffer is empty, cannot update screen\r\n");
		return DisplayRet::BufferEmpty;
	}
	LCDBuffer(_LCDbuffer);
	return DisplayRet::Success;
}

/*!
	@brief clears the buffer of the active screen pointed to by ActiveBuffer
	@return
		-# Success
		-# BufferEmpty if buffer is empty object
	 @note Does NOT write to the screen
*/
DisplayRet::Ret_Codes_e NOKIA_5110::LCDclearBuffer()
{
	if (_LCDbuffer.empty())
	{
		printf("NOKIA_5110::LCDclearBuffer Error: Buffer is empty, cannot clear\r\n");
		return DisplayRet::BufferEmpty;
	}

	std::fill(_LCDbuffer.begin(), _LCDbuffer.end(), 0x00);
	return DisplayRet::Success;
}

/*!
	 @brief Draw an  array  to the screen
	 @param data span to the buffer array
	 @note Called by LCDupdate internally to write buffer to screen
*/
void NOKIA_5110::LCDBuffer(std::span<uint8_t> data)
{
	LCDWriteCommand(LCD_SETYADDR); // set y = 0
	LCDWriteCommand(LCD_SETXADDR); // set x = 0
	display_CD_SetHigh;			   // Data send
	display_CS_SetLow;
	for (uint16_t i = 0; i < _LCD_Display_size; i++)
	{
		LCDWriteData(data[i]);
	}
	display_CS_SetHigh;
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
DisplayRet::Ret_Codes_e NOKIA_5110::LCDSetBufferPtr(uint8_t width, uint8_t height, std::span<uint8_t> buffer)
{
	if (buffer.size() != static_cast<size_t>(width * (height / 8)))
	{
		printf("NOKIA_5110::LCDSetBufferPtr Error 2: buffer size does not equal w * (h/8))\r\n");
		return DisplayRet::BufferSize;
	}
	_LCDbuffer = buffer;

	if (buffer.empty())
	{
		printf("NOKIA_5110::LCDSetBufferPtr Error 3: Problem assigning buffer, received empty buffer\r\n");
		return DisplayRet::BufferEmpty;
	}
	return DisplayRet::Success;
}

/*!
	@brief  LCDgotoXY gotoXY routine to position cursor (x,y)
	@param x range: 0 to 83 (0 to 0x53)
	@param y range: 0 to 5 (6 blocks one byte each 6*8 = 48)
	@note only works in standard  84x48 rotation
*/
void NOKIA_5110::LCDgotoXY(uint8_t x, uint8_t y)
{
	LCDWriteCommand(LCD_SETXADDR  | x); // Column. (result 0x80 to 0xD3)
	LCDWriteCommand(LCD_SETYADDR  | y); // Row.
}

/*!
	@brief  LCDFillBlock Clears one of the 6 row blocks(one byte height) on LCD
	@param FillData data to write 0-0xFF
	@param RowBlockNum row block number 0-5
	@details The pattern of data can be reversed by changing filldata
	i.e 0x0F will half fill a block with ones.
	@note only works in standard 84x48 rotation
*/
void NOKIA_5110::LCDfillBlock(uint8_t FillData, uint8_t RowBlockNum)
{
	LCDgotoXY(0, RowBlockNum);
	display_CD_SetHigh;			   // Data send
	display_CS_SetLow;
	for (uint8_t index = 0; index < _LCD_WIDTH ; index++)
	{
		LCDWriteData(FillData);
	}
	display_CS_SetHigh;

}
/* ------------- EOF ------------------ */
