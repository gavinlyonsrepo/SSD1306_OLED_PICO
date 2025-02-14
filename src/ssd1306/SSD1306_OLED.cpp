/*!
* @file SSD1306_OLED.cpp
* @brief   OLED driven by SSD1306 controller, Source file
* @author Gavin Lyons.
* @details <https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO>
*/

//#include <stdio.h> 
#include "pico/stdlib.h"
#include "../../include/ssd1306/SSD1306_OLED.hpp"

/*!
	@brief init the screen object
	@param oledwidth width of OLED in pixels 
	@param oledheight height of OLED in pixels 
 */
SSD1306  :: SSD1306(int16_t oledwidth, int16_t oledheight) :SSD1306_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_PAGE_NUM = (_OLED_HEIGHT/8); 
}

/*!
	@brief  begin Method initialise OLED I2C communication
	@param I2Caddress I2C Bus address by default 0x3C
	@param  i2c_type The I2C interface i2c0 or ic21 interface
	@param CLKspeed I2C Bus Clock speed in KHz. 
	@param SDApin I2C data GPIO pin  
	@param SCLKpin I2C clock GPIO pin 
	@return 
		-# Success if successful , init I2C communication
		-# I2CbeginFail 
*/
DisplayRet::Ret_Codes_e  SSD1306::OLEDbegin( uint8_t I2Caddress, i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t  SDApin, uint8_t  SCLKpin)
{
	_OLEDAddressI2C = I2Caddress;
	_i2c = i2c_type; 
	_CLKSpeed = CLKspeed;
	_SClkPin = SCLKpin;
	_SDataPin = SDApin;
	
	int ReturnCode = 0;
	uint8_t rxData = 0;

	// init I2c pins and interface
	gpio_set_function(_SDataPin, GPIO_FUNC_I2C);
	gpio_set_function(_SClkPin, GPIO_FUNC_I2C);
	gpio_pull_up(_SDataPin);
	gpio_pull_up(_SClkPin);
	i2c_init(_i2c, _CLKSpeed * 1000);

	// check connection?
	ReturnCode = i2c_read_timeout_us(_i2c, _OLEDAddressI2C , &rxData, 1, false, _TimeoutDelayI2C);
	if (ReturnCode < 1){ // no bytes read back from device or error issued
		if (_bSerialDebugFlag)
		{
			printf("1201 ERROR I2C_SSD1306 OLED :: begin: \r\n");
			printf("Check Connection, Return code :: %d ,RX data :: %u \r\n", ReturnCode , rxData);
		}
		_bIsConnected = false;
		return DisplayRet::I2CbeginFail;
	}
	_bIsConnected = true;
	OLEDinit();
	return DisplayRet::Success;
}

/*!
	@brief sets the buffer pointer to the users screen data buffer
	@param width width of buffer in pixels
	@param height height of buffer in pixels
	@param buffer span object of the buffer
	@return Will return:
		-# Success
		-# BufferSize Buffer size calculations are incorrect BufferSize = w * (h/8),
		-# BufferEmpty Not a valid span object.
*/
DisplayRet::Ret_Codes_e SSD1306::OLEDSetBufferPtr(uint8_t width, uint8_t height , std::span<uint8_t> buffer)
{
	if (buffer.size() != static_cast<size_t>(width * (height / 8)))
	{
		printf("SSD1306::OLEDSetBufferPtr Error 2: buffer size does not equal w * (h/8))\r\n");
		return DisplayRet::BufferSize;
	}
	_OLEDbuffer = buffer;

	if (buffer.empty())	{
		printf("SSD1306::OLEDSetBufferPtr Error 3: Problem assigning buffer, received empty buffer\r\n");
		return DisplayRet::BufferEmpty;
	}
	return DisplayRet::Success;
}

/*!
	@brief End I2C operations. I2C pins P1-03 (SDA) and P1-05 (SCL) 	
	are returned to their default INPUT behaviour. 
*/
void SSD1306::OLEDdeI2CInit(void)
{
	gpio_set_function(_SDataPin, GPIO_FUNC_NULL);
	gpio_set_function(_SClkPin, GPIO_FUNC_NULL);
	i2c_deinit(_i2c); 	
}

/*! 
	@brief Disables  OLED Call when powering down
*/
void SSD1306::OLEDPowerDown(void)
{
	OLEDEnable(0);
	busy_wait_ms(100);
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
*/
void SSD1306::OLEDinit()
 {
	const uint8_t  SSD1306_INITDELAY = 100 ;/**< Initialisation delay in mS */
	busy_wait_ms(SSD1306_INITDELAY);
	I2CWriteByte( SSD1306_DISPLAY_OFF);
	I2CWriteByte( SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
	I2CWriteByte( 0x80);
	I2CWriteByte( SSD1306_SET_MULTIPLEX_RATIO );
	I2CWriteByte( _OLED_HEIGHT - 1 );
	I2CWriteByte( SSD1306_SET_DISPLAY_OFFSET );
	I2CWriteByte(0x00);
	I2CWriteByte( SSD1306_SET_START_LINE|0x00);
	I2CWriteByte( SSD1306_CHARGE_PUMP );
	I2CWriteByte(0x14);
	I2CWriteByte( SSD1306_MEMORY_ADDR_MODE );
	I2CWriteByte(0x00);  //Horizontal Addressing Mode is Used
	I2CWriteByte( SSD1306_SET_SEGMENT_REMAP| 0x01);
	I2CWriteByte( SSD1306_COM_SCAN_DIR_DEC );

switch (_OLED_HEIGHT)
{
	case 64: 
		I2CWriteByte( SSD1306_SET_COM_PINS );
		I2CWriteByte( 0x12 );
		I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
		I2CWriteByte(0xCF);
	break;
	case 32: 
		I2CWriteByte( SSD1306_SET_COM_PINS );
		I2CWriteByte( 0x02 );
		I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
		I2CWriteByte(0x8F);
	break;
	case 16: // NOTE: not tested, lacking part. 
		I2CWriteByte( SSD1306_SET_COM_PINS );
		I2CWriteByte( 0x2 ); // ?
		I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
		I2CWriteByte(0xAF);
	break;
}

	I2CWriteByte( SSD1306_SET_PRECHARGE_PERIOD );
	I2CWriteByte( 0xF1 );
	I2CWriteByte( SSD1306_SET_VCOM_DESELECT );
	I2CWriteByte( 0x40 );
	I2CWriteByte( SSD1306_DISPLAY_ALL_ON_RESUME );
	I2CWriteByte( SSD1306_NORMAL_DISPLAY );
	I2CWriteByte( SSD1306_DEACTIVATE_SCROLL );
	I2CWriteByte( SSD1306_DISPLAY_ON );

	busy_wait_ms(SSD1306_INITDELAY);
}

/*!
	@brief Turns On Display
	@param bits   1  on , 0 off
*/
void SSD1306::OLEDEnable(uint8_t bits)
{
	bits ? I2CWriteByte(SSD1306_DISPLAY_ON) : I2CWriteByte(SSD1306_DISPLAY_OFF);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void SSD1306::OLEDContrast(uint8_t contrast)
{
	I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
	I2CWriteByte(contrast);
}

/*!
	@brief invert the display
	@param value true invert , false normal
*/
void SSD1306::OLEDInvert(bool value)
{
 value ? I2CWriteByte( SSD1306_INVERT_DISPLAY ) : I2CWriteByte( SSD1306_NORMAL_DISPLAY );
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay in milliseconds can be set to zero normally.
*/
void SSD1306::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		I2CWriteByte( 0xB0 | row);
		I2CWriteByte(SSD1306_SET_LOWER_COLUMN);
		I2CWriteByte(SSD1306_SET_HIGHER_COLUMN);
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			I2CWriteByte(dataPattern, SSD1306_DATA_CONTINUE);
			busy_wait_ms(delay);
		}
	}
}

/*!
	@brief Fill the chosen page(1-8)  with a datapattern
	@param page_num chosen page (1-8)
	@param dataPattern can be set to 0 to FF (not buffer)
	@param mydelay optional delay in milliseconds can be set to zero normally.
*/
void SSD1306::OLEDFillPage(uint8_t page_num, uint8_t dataPattern,uint8_t mydelay)
{
	uint8_t Result =0xB0 | page_num; 
	I2CWriteByte(Result);
	I2CWriteByte(SSD1306_SET_LOWER_COLUMN);
	I2CWriteByte(SSD1306_SET_HIGHER_COLUMN);
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		I2CWriteByte(dataPattern, SSD1306_DATA_CONTINUE);
		busy_wait_ms(mydelay);
	}
}

/*!
	@brief Draw a bitmap  to the buffer 
	@param x x axis offset
	@param y y axis offset
	@param w width
	@param h height
	@param pBitmap span object to bitmap data
	@param invert color 
	@return Will return 
		-# success
		-# BitmapScreenBounds Bitmap co-ord out of bounds, check x and y
		-# BitmapLargerThanScreen Bitmap is larger than screen, check w and h
		-# BitmapEmpty Bitmap is an invalid object
		-# BitmapHorizontalSize Check Horizontal bitmap size
	@note bitmap data must be  horizontally addressed and height divisible by 8
*/
DisplayRet::Ret_Codes_e SSD1306::OLEDBitmap(int16_t x, int16_t y, int16_t w, int16_t h, std::span<const uint8_t>  pBitmap, bool invert)
{

// User error checks
// 1. Completely out of bounds?
if (x > _width || y > _height)
{
	printf("SSD1306::OLEDBitmap Error 1: Bitmap co-ord out of bounds, check x and y\r\n");
	return DisplayRet::BitmapScreenBounds;
}
// 2. bitmap weight and height
if (w > _width || h > _height)
{
	printf("SSD1306::OLEDBitmap Error 2: Bitmap is larger than screen, check w and h\r\n");
	return DisplayRet::BitmapLargerThanScreen;
}
// 3. bitmap is null
if(pBitmap.empty()) 
{
	printf("SSD1306::OLEDBitmap Error 3: Bitmap is is not valid \n");
	return DisplayRet::BitmapDataEmpty;
}

// 4. check Horizontal bitmap size
if(w % 8 != 0 )
{
	printf("SSD1306::OLEDBitmap Error 4: Bitmap width size is incorrect: w %i h %i \n", w , h);
	printf("Check is bitmap width divisible evenly by eight \n");
	return DisplayRet::BitmapHorizontalSize;
}

// 5. check  bitmap size
if(pBitmap.size() != static_cast<size_t>((w / 8) * h))
{
	printf("SSD1306::OLEDBitmap Error 5: Bitmap size is incorrect: w %i h %i \n", w , h);
	printf("Check bitmap size = ((w/8)*h)  \n");
	return DisplayRet::BitmapSize;
}

int16_t byteWidth = (w + 7) / 8; 
uint8_t byte = 0;
uint8_t color, bgcolor;
if (invert == false)
{
	color = WHITE;
	bgcolor = BLACK;
}else
{
	color = BLACK;
	bgcolor = WHITE;
}

for (int16_t j = 0; j < h; j++, y++) 
{
	for (int16_t i = 0; i < w; i++) 
	{
		if (i & 7)
			byte <<= 1;
		else
			byte = pBitmap[j * byteWidth + i / 8];
			
		drawPixel(x + i, y, (byte & 0x80) ? color : bgcolor );
	}
}
return DisplayRet::Success;
}

/*!
	@brief Writes a byte to I2C address,command or data, used internally
	@param value write the value to be written
	@param cmd command or data
	@note In the event of an error will loop 3 times each time.
*/
void SSD1306::I2CWriteByte(uint8_t value, uint8_t cmd)
{
	uint8_t  dataBuffer[2] = {cmd,value};
	uint8_t attemptI2Cwrite = 0;
	int16_t returnCode = 0;
	
	//returnCode = bcm2835_i2c_write(buf, 2); 
	returnCode = i2c_write_timeout_us(_i2c, _OLEDAddressI2C, dataBuffer, 2 , false, _TimeoutDelayI2C);
	
	while(returnCode < 1)
	{ // failure to write I2C byte 
		if (_bSerialDebugFlag)
		{
			printf("SSD1306::I2C_Write_Byte : Cannot Write byte : Retry Attempt = %u\n", attemptI2Cwrite);
			printf("Error code %i\n", returnCode);
		}
		if (attemptI2Cwrite >= _I2CRetryAttempts) break;
		returnCode = i2c_write_timeout_us(_i2c, _OLEDAddressI2C, dataBuffer, 2 , false, _TimeoutDelayI2C);
		busy_wait_ms(_I2CRetryDelay); // mS
		attemptI2Cwrite ++;
	}
	if (returnCode < 1) 
		_bIsConnected = false;
	else
		_bIsConnected = true;
}

/*!
	@brief updates the buffer i.e. writes it to the screen
*/
DisplayRet::Ret_Codes_e SSD1306::OLEDupdate()
{
	if (_OLEDbuffer.empty())
	{
		printf("SSD1306::OLEDupdate Error: Buffer is empty, cannot update screen\r\n");
		return DisplayRet::BufferEmpty;
	}
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = this->_OLED_WIDTH;
	uint8_t h = this->_OLED_HEIGHT;

	OLEDBuffer(x, y, w, h, _OLEDbuffer);
	return DisplayRet::Success;
}

/*!
	@brief clears the buffer memory i.e. does NOT write to the screen
*/
DisplayRet::Ret_Codes_e SSD1306::OLEDclearBuffer()
{
	if (_OLEDbuffer.empty())
	{
		printf("SSD1306::OLEDclearBuffer Error: Buffer is empty, cannot clear\r\n");
		return DisplayRet::BufferEmpty;
	}

	std::fill(_OLEDbuffer.begin(), _OLEDbuffer.end(), 0x00);
	return DisplayRet::Success;
}

/*!
	@brief Draw a bitmap directly to the screen
	@param x x axis  offset
	@param y y axis offset
	@param w width
	@param h height
	@param data the buffer data
	@note Called by OLEDupdate internally
*/
void SSD1306::OLEDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, std::span<uint8_t> data)
{
	uint8_t tx, ty;
	uint16_t offset = 0;
		
	I2CWriteByte( SSD1306_SET_COLUMN_ADDR );
	I2CWriteByte(0);   // Column start address (0 = reset)
	I2CWriteByte( _OLED_WIDTH-1 ); // Column end address (127 = reset)

	I2CWriteByte( SSD1306_SET_PAGE_ADDR );
	I2CWriteByte(0); // Page start address (0 = reset)
	
	switch (_OLED_HEIGHT)
	{
		case 64: I2CWriteByte(7); break;
		case 32: I2CWriteByte(3); break;
		case 16: I2CWriteByte(1); break;
	}
	
	for (ty = 0; ty < h; ty = ty + 8)
		{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT) {continue;}
		for (tx = 0; tx < w; tx++)
		{

			if (x + tx < 0 || x + tx >= _OLED_WIDTH) {continue;}
			offset = (w * (ty /8)) + tx;
			I2CWriteByte(data[offset++], SSD1306_DATA_CONTINUE);
		}
	}

}

/*!
	@brief Draws a Pixel to the screen overides the gfx lib if defined
	@param x x axis  position  
	@param y y axis  position
	@param color color of pixel.
*/
void SSD1306::drawPixel(int16_t x, int16_t y, uint8_t color)
{

	if ((x < 0) || (x >= this->_width) || (y < 0) || (y >= this->_height)) {
	return;
	}
	int16_t temp;
	uint8_t rotation = getRotation();
	switch (rotation) {
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
		uint16_t tc = (_OLED_WIDTH * (y /8)) + x;
		switch (color)
		{
			case WHITE:  this->_OLEDbuffer[tc] |= (1 << (y & 7)); break;
			case BLACK:  this->_OLEDbuffer[tc] &= ~(1 << (y & 7)); break;
			case INVERSE: this->_OLEDbuffer[tc] ^= (1 << (y & 7)); break;
		}
}

/*!
	@brief Scroll OLED data to the right
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLEDStartScrollRight(uint8_t start, uint8_t stop) 
{
	I2CWriteByte(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);  // start page
	I2CWriteByte(0X00);
	I2CWriteByte(stop);   // end page
	I2CWriteByte(0X00);
	I2CWriteByte(0XFF);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data to the left
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLEDStartScrollLeft(uint8_t start, uint8_t stop) 
{
	I2CWriteByte(SSD1306_LEFT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);
	I2CWriteByte(0X00);
	I2CWriteByte(stop);
	I2CWriteByte(0X00);
	I2CWriteByte(0XFF);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data diagonally to the right
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLEDStartScrollDiagRight(uint8_t start, uint8_t stop) 
{
	I2CWriteByte(SSD1306_SET_VERTICAL_SCROLL_AREA);
	I2CWriteByte(0X00);
	I2CWriteByte(_OLED_HEIGHT);
	I2CWriteByte(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);
	I2CWriteByte(0X00);
	I2CWriteByte(stop);
	I2CWriteByte(0X01);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data diagonally to the left
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLEDStartScrollDiagLeft(uint8_t start, uint8_t stop) 
{
	I2CWriteByte(SSD1306_SET_VERTICAL_SCROLL_AREA);
	I2CWriteByte(0X00);
	I2CWriteByte(_OLED_HEIGHT);
	I2CWriteByte(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);
	I2CWriteByte(0X00);
	I2CWriteByte(stop);
	I2CWriteByte(0X01);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief  Stop scroll mode
*/
void SSD1306::OLEDStopScroll(void) 
{
	I2CWriteByte(SSD1306_DEACTIVATE_SCROLL);
}

/*!
	@brief Check Connection Function
		Check if device is on the bus asks for one byte
	@return int16_t if less than 1 = error 
*/
int16_t SSD1306::CheckConnection()
{
	int16_t returnValue = 0;
	uint8_t rxData = 0;
	returnValue = i2c_read_timeout_us(_i2c, _OLEDAddressI2C , &rxData, 1, false, _TimeoutDelayI2C);
	if (_bSerialDebugFlag)
	{
		printf("SSD1306::CheckConnection. Info \r\n");
		printf("I2C Return value = %d , RxData =  %u \r\n", returnValue , rxData);
		if (returnValue >= 1)
			printf("Connected.\r\n ");
		else
		 	printf("Not Connected. \r\n");	
	}
	if (returnValue < 1) {
		_bIsConnected = false;
		return DisplayRet::I2CNotConnected;
	}
	else{
		_bIsConnected = true;
		return DisplayRet::Success;
	}
}

/*!
	@brief getter for is connected status
	@return is connected or disconnected
*/
bool SSD1306::GetIsConnected(void)
{return _bIsConnected;}

/*!
	@brief setter for is connected status 
	@param connected Set device connected , true yes, false no
*/
void SSD1306::SetIsConnected(bool connected)
{_bIsConnected = connected;}

/*!
	@brief getter for debug status
	@return  true debug is on 
*/
bool SSD1306::GetDebugMode(void)
{return _bSerialDebugFlag;}

/*!
	@brief setter for debug status 
	@param debugMode  true yes on , false off
*/
void SSD1306::SetDebugMode(bool debugMode)
{_bSerialDebugFlag = debugMode;}

/*!
	@brief getter for Library version number
	@return the version number 101 is 1.0.1
*/
uint16_t SSD1306::GetLibVerNum(void){return _OLEDLibVerNum;}

/*!
	@brief Gets the Number of I2C retry Attempts in event of I2C bus error
		Set to zero for NO retry attempts. default 3
	@return uint8_t Max number of I2C retry attempts
 */
uint8_t SSD1306::GetI2CRetryAttemptsNo(void){return _I2CRetryAttempts;}

/*!
	@brief Sets the Number of I2C retry Attempts in event of I2C bus error
		Set to zero for NO retry attempts.
	@param Attempts  Max number of I2C retry attempts
 */
void SSD1306::SetI2CRetryAttemptsNo(uint8_t Attempts)
{_I2CRetryAttempts = Attempts;}

/*!
	@brief Gets delay in mS between retry attempts in event of I2C error
	@return Delay mS
 */
uint16_t SSD1306::GetI2CRetryDelay(void){return _I2CRetryDelay;}

/*!
	@brief Sets delay in mS between retry attempts in event of I2C error
	@param Delay  delay in mS between retry attempts in event of I2C error
*/
void SSD1306::SetI2CRetryDelay(uint16_t Delay)
{_I2CRetryDelay = Delay;}

/*!
	@brief Gets I2C timeout value used in I2C functions calls
	@return Delay uS, default 50000
 */
uint32_t SSD1306::GetI2CTimeout(void){return _TimeoutDelayI2C;}

/*!
	@brief Sets delay the I2C timeout in uS 
	@param timeout delay in uS
*/
void SSD1306::SetI2CTimeout(uint32_t timeout)
{_TimeoutDelayI2C = timeout;}

// ---  EOF ---
