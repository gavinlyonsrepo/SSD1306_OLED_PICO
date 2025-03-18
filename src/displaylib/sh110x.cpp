/*!
* @file sh110x.cpp
* @brief  OLED driven by controller. Sh1106 and Sh1107, Source file
*/

//#include <stdio.h> 
#include "pico/stdlib.h"
#include "../../include/displaylib/sh110x.hpp"

/*!
	@brief init the screen object
	@param oledwidth width of OLED in pixels
	@param oledheight height of OLED in pixels
*/
SH110X::SH110X(int16_t oledwidth, int16_t oledheight) :displaylib_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_PAGE_NUM = (_OLED_HEIGHT/8);
}


/*!
	@brief sets the buffer pointer to the users screen data buffer
	@param width width of buffer in pixels
	@param height height of buffer in pixels
	@param buffer the buffer span
	@return Will return DisplayRet::Ret_Codes_eenum
		-# Success DisplayRet::Success
		-# Error 1 DisplayRet::BufferSize
		-# Error 2 DisplayRet::BufferEmpty
*/
DisplayRet::Ret_Codes_e SH110X::OLEDSetBufferPtr(uint8_t width, uint8_t height , std::span<uint8_t> buffer)
{
	if(buffer.size() != static_cast<size_t>(width * (height / 8)))
	{
		printf("Error 1: OLEDSetBufferPtr: buffer size does not equal : width * (height/8))\n");
		return DisplayRet::BufferSize;
	}
	if(buffer.empty())
	{
		printf("Error 2: OLEDSetBufferPtr: Problem assigning buffer pointer, empty object\r\n");
		return DisplayRet::BufferEmpty;
	}
	_OLEDbuffer = buffer;
	return DisplayRet::Success;
}

/*!
	@brief  begin Method initialise OLED I2C communication
	@param OLEDtype enum type of display sh1106 or sh1107
	@param resetPin Used only if reset pin present on device, iF not = set to -1
	@param I2Caddress I2C Bus address by default 0x3C
	@param i2c_type The I2C interface i2c0 or ic21 interface
	@param CLKspeed I2C Bus Clock speed in KHz. 
	@param SDApin I2C data GPIO pin  
	@param SCLKpin I2C clock GPIO pin 
	@return 
		-# Success if successful , init I2C communication
		-# I2CbeginFail 
*/
DisplayRet::Ret_Codes_e SH110X::OLEDbegin(OLED_IC_type_e OLEDtype, int8_t resetPin, uint8_t I2Caddress, 
	i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t  SDApin, uint8_t  SCLKpin)
{
	_OLED_IC_type = OLEDtype;
	_Display_RST = resetPin;
	_OLEDAddressI2C = I2Caddress;
	_i2c = i2c_type; 
	_CLKSpeed = CLKspeed;
	_SClkPin = SCLKpin;
	_SDataPin = SDApin;
	int ReturnCode = 0;
	uint8_t rxData = 0;
	// If reset pin is used 
	if(_Display_RST >= 0)
	{
		gpio_init(_Display_RST);
		gpio_set_dir(_Display_RST, GPIO_OUT);
	}
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
	@brief  End I2C operations.
*/
void SH110X::OLED_I2C_OFF(void)
{
	// 1 Is reset pin being used?
	if(_Display_RST >= 0)
	{
		gpio_put(_Display_RST, false);
		gpio_deinit(_Display_RST);
	}
	gpio_set_function(_SDataPin, GPIO_FUNC_NULL);
	gpio_set_function(_SClkPin, GPIO_FUNC_NULL);
	i2c_deinit(_i2c); 	
}

/*!
	@brief Disables  OLED Call when powering down before OLED_I2C_OFF
*/
void SH110X::OLEDPowerDown(void)
{
	OLEDEnable(0);
	busy_wait_ms(100);
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
*/
void SH110X::OLEDinit()
{
	switch(_OLED_IC_type)
	{
		case SH1106_IC:
			SH1106_begin();
		break;
		case SH1107_IC:
			SH1107_begin();
		break;
		default:
			printf("Warning: OLEDinit: Unknown OLED type, Init Sh1106 by default\n");
			SH1106_begin();
		break;
	}
}

/*!
	@brief Handles OLED reset, if reset pin present on device.
*/
void SH110X::OLEDReset(void)
{
	const uint8_t resetDelay = 10; /**< reset delay in miliseconds*/

	gpio_put(_Display_RST, true);
	busy_wait_ms(resetDelay);
	gpio_put(_Display_RST, false);
	busy_wait_ms(resetDelay);
	gpio_put(_Display_RST, true);
}

/*!
 @brief Init routine for SH1106 OLED
*/
void SH110X::SH1106_begin(void)
{
	const uint16_t _INITDELAY = 100; /**< Initialisation delay in mS */
	if(_Display_RST >= 0)
	{
		OLEDReset(); // Hw reset pin
	}
	pageStartOffset = 2; // the SH1106 display  requires a small offset 

	I2CWriteByte(SH110X_DISPLAYOFF);
	I2CWriteByte(SH110X_SETDISPLAYCLOCKDIV);
	I2CWriteByte(0x80);

	I2CWriteByte(SH110X_SETMULTIPLEX);
	I2CWriteByte(0x3F);

	I2CWriteByte(SH110X_SETDISPLAYOFFSET);
	I2CWriteByte(0x00);
	I2CWriteByte(SH110X_SETSTARTLINE);

	I2CWriteByte(SH110X_DCDC);
	I2CWriteByte(0x8B);

	I2CWriteByte(SH110X_SEGREMAP + 1); // Left rotation +1
	I2CWriteByte(SH110X_COMSCANDEC);
	I2CWriteByte(SH110X_SETCOMPINS);
	I2CWriteByte(0x12);  // Alternative (POR)

	I2CWriteByte(SH110X_SETCONTRAST);
	I2CWriteByte(0xFF);

	I2CWriteByte(SH110X_SETPRECHARGE);
	I2CWriteByte(0x1F);

	I2CWriteByte(SH110X_SETVCOMDETECT);
	I2CWriteByte(0x40);
	I2CWriteByte(0x33);

	I2CWriteByte(SH110X_NORMALDISPLAY);
	I2CWriteByte(SH110X_MEMORYMODE);
	I2CWriteByte(0x10);
	I2CWriteByte(SH110X_DISPLAYALLON_RESUME);

	busy_wait_ms(_INITDELAY);
	I2CWriteByte(SH110X_DISPLAYON);
}

/*!
 @brief Init routine for SH1107 OLED
*/
void SH110X::SH1107_begin(void)
{
	const uint16_t _INITDELAY = 100; /**< Initialisation delay in mS */
	if(_Display_RST >= 0)
	{
		OLEDReset();
	}
	I2CWriteByte(SH110X_DISPLAYOFF);
	I2CWriteByte(SH110X_SETDISPLAYCLOCKDIV);
	I2CWriteByte(0x51);
	I2CWriteByte(SH110X_MEMORYMODE);
	I2CWriteByte(SH110X_SETCONTRAST);
	I2CWriteByte(0x4F);
	I2CWriteByte(SH110X_DCDC);
	I2CWriteByte(0x8A);
	I2CWriteByte(SH110X_SEGREMAP);
	I2CWriteByte(SH110X_COMSCANINC);
	I2CWriteByte(SH110X_SETDISPSTARTLINE);
	I2CWriteByte(0x0);
	I2CWriteByte(SH110X_SETDISPLAYOFFSET);
	I2CWriteByte(0x60);
	I2CWriteByte(SH110X_SETPRECHARGE);
	I2CWriteByte(0x22);
	I2CWriteByte(SH110X_SETVCOMDETECT);
	I2CWriteByte(0x35);
	I2CWriteByte(SH110X_SETMULTIPLEX);
	I2CWriteByte(0x3F);
	I2CWriteByte(SH110X_DISPLAYALLON_RESUME);
	I2CWriteByte(SH110X_NORMALDISPLAY);

	if (_width == 128 && _height == 128)
	{
		I2CWriteByte(SH110X_SETDISPLAYOFFSET);
		I2CWriteByte(0x00);
		I2CWriteByte(SH110X_SETMULTIPLEX);
		I2CWriteByte(0x7F);
	}

	busy_wait_ms(_INITDELAY);
	I2CWriteByte(SH110X_DISPLAYON);
}

/*!
	@brief Turns On Display
	@param bits   1  on , 0 off
*/
void SH110X::OLEDEnable(uint8_t bits)
{
	
	bits ? I2CWriteByte(SH110X_DISPLAYALLON ) : I2CWriteByte(SH110X_DISPLAYOFF);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void SH110X::OLEDContrast(uint8_t contrast)
{
	
	I2CWriteByte(SH110X_SETCONTRAST);
	I2CWriteByte(contrast);
}

/*!
	@brief invert the display
	@param value true invert , false normal
*/
void SH110X::OLEDInvert(bool value)
{
	value ? I2CWriteByte(SH110X_INVERTDISPLAY) : I2CWriteByte(SH110X_NORMALDISPLAY);
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay in milliseconds can be set to zero normally.
*/
void SH110X::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		I2CWriteByte( SH110X_SETPAGEADDR  | row);
		I2CWriteByte(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F));
		I2CWriteByte(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4 )); // 0x10 | 2 >>4 = 10
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			I2CWriteByte(dataPattern, SH110X_DATA_BYTE);
			busy_wait_ms(delay);
		}
	}
}

/*!
	@brief Fill the chosen page with a data pattern
	@param page_num chosen page (1-X)
	@param dataPattern can be set to 0 to FF (not buffer)
	@param mydelay optional delay in milliseconds can be set to zero normally.
*/
void SH110X::OLEDFillPage(uint8_t page_num, uint8_t dataPattern,uint8_t mydelay)
{
	uint8_t Result =SH110X_SETPAGEADDR | page_num;
	I2CWriteByte(Result);
	I2CWriteByte(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F)); // SH110X_SETLOWCOLUMN   = 0x00
	I2CWriteByte(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4));
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		I2CWriteByte(dataPattern, SH110X_DATA_BYTE);
		busy_wait_ms(mydelay);
	}
}

/*!
	@brief Writes a byte to I2C address,command or data, used internally
	@param value write the value to be written
	@param cmd command or data
	@note isDebugEnabled()  ,will output data on I2C failures.
*/
void SH110X::I2CWriteByte(uint8_t value, uint8_t cmd)
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
			printf("SH110X::I2C_Write_Byte : Cannot Write byte : Retry Attempt = %u\n", attemptI2Cwrite);
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
	@return 
		-# Success 
		-# BufferEmpty if buffer is empty object
*/
DisplayRet::Ret_Codes_e SH110X::OLEDupdate()
{
	if (_OLEDbuffer.empty())
	{
		printf("Error: OLEDupdate: Buffer is empty, cannot update screen\r\n");
		return DisplayRet::BufferEmpty;
	}
	uint8_t w = this->_OLED_WIDTH; 
	uint8_t h = this->_OLED_HEIGHT;
	OLEDBufferScreen(w,  h, this->_OLEDbuffer);
	return DisplayRet::Success;
}

/*!
	@brief clears the buffer memory i.e. does NOT write to the screen
	@return 
		-# Success 
		-# BufferEmpty is buffer empty object
*/
DisplayRet::Ret_Codes_e SH110X::OLEDclearBuffer()
{
	if (_OLEDbuffer.empty())
	{
		printf("Error: OLEDclearBuffer: Buffer is empty, cannot clear\r\n");
		return DisplayRet::BufferEmpty;
	}

	std::fill(_OLEDbuffer.begin(), _OLEDbuffer.end(), 0x00);
	return DisplayRet::Success;
}

/*!
	@brief Draw the buffer to screen directly to the screen
	@param w width
	@param h height
	@param data the buffer data
	@note Called by OLEDupdate internally
*/
void SH110X::OLEDBufferScreen(uint8_t w, uint8_t h, std::span<uint8_t> data)
{
	uint8_t page;

	for (page = 0; page < (h/8); page++) 
	{
		I2CWriteByte(SH110X_SETPAGEADDR + page);
		I2CWriteByte(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F)); // SH110X_SETLOWCOLUMN   = 0x00
		I2CWriteByte(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4));
		//write data
		for(int i = 0; i< w; i++ ) 
		{
			I2CWriteByte(data[i+page*w], SH110X_DATA_BYTE);
		}
	}

}

/*!
	@brief Draws a Pixel to the screen overrides the graphics lib
	@param x x axis  position
	@param y y axis  position
	@param color color of pixel.
*/
void SH110X::drawPixel(int16_t x, int16_t y, uint8_t color)
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
		case FG_COLOR:  this->_OLEDbuffer[tc] |= (1 << (y & 7)); break;
		case BG_COLOR:  this->_OLEDbuffer[tc] &= ~(1 << (y & 7)); break;
		case INVERSE: this->_OLEDbuffer[tc] ^= (1 << (y & 7)); break;
	}
}

/*!
	@brief Check Connection Function
		Check if device is on the bus asks for one byte
	@return int16_t if less than 1 = error 
*/
int16_t SH110X::CheckConnection()
{
	int16_t returnValue = 0;
	uint8_t rxData = 0;
	returnValue = i2c_read_timeout_us(_i2c, _OLEDAddressI2C , &rxData, 1, false, _TimeoutDelayI2C);
	if (_bSerialDebugFlag)
	{
		printf("SH110X::CheckConnection. Info \r\n");
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
bool SH110X::GetIsConnected(void)
{return _bIsConnected;}

/*!
	@brief setter for is connected status 
	@param connected Set device connected , true yes, false no
*/
void SH110X::SetIsConnected(bool connected)
{_bIsConnected = connected;}

/*!
	@brief getter for debug status
	@return  true debug is on 
*/
bool SH110X::GetDebugMode(void)
{return _bSerialDebugFlag;}

/*!
	@brief setter for debug status 
	@param debugMode  true yes on , false off
*/
void SH110X::SetDebugMode(bool debugMode)
{_bSerialDebugFlag = debugMode;}


/*!
	@brief Gets the Number of I2C retry Attempts in event of I2C bus error
		Set to zero for NO retry attempts. default 3
	@return uint8_t Max number of I2C retry attempts
 */
uint8_t SH110X::GetI2CRetryAttemptsNo(void){return _I2CRetryAttempts;}

/*!
	@brief Sets the Number of I2C retry Attempts in event of I2C bus error
		Set to zero for NO retry attempts.
	@param Attempts  Max number of I2C retry attempts
 */
void SH110X::SetI2CRetryAttemptsNo(uint8_t Attempts)
{_I2CRetryAttempts = Attempts;}

/*!
	@brief Gets delay in mS between retry attempts in event of I2C error
	@return Delay mS
 */
uint16_t SH110X::GetI2CRetryDelay(void){return _I2CRetryDelay;}

/*!
	@brief Sets delay in mS between retry attempts in event of I2C error
	@param Delay  delay in mS between retry attempts in event of I2C error
*/
void SH110X::SetI2CRetryDelay(uint16_t Delay)
{_I2CRetryDelay = Delay;}

/*!
	@brief Gets I2C timeout value used in I2C functions calls
	@return Delay uS, default 50000
 */
uint32_t SH110X::GetI2CTimeout(void){return _TimeoutDelayI2C;}

/*!
	@brief Sets delay the I2C timeout in uS 
	@param timeout delay in uS
*/
void SH110X::SetI2CTimeout(uint32_t timeout)
{_TimeoutDelayI2C = timeout;}

// --- EOF ---
