/*!
* @file SSD1306_OLED.cpp
* @brief   OLED driven by SSD1306 controller, Source file
* @author Gavin Lyons.
* @details <https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO>
*/

#include <stdio.h> // optional for printf debug messages
#include "pico/stdlib.h"
#include "../include/ssd1306/SSD1306_OLED.hpp"

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
	bufferWidth = _OLED_WIDTH;
	bufferHeight = _OLED_HEIGHT;
}

/*!
	@brief  begin Method initialise OLED I2C communication
	@param I2Caddress I2C Bus address by default 0x3C
	@param  i2c_type The I2C interface i2c0 or ic21 interface
	@param CLKspeed I2C Bus Clock speed in KHz. Set to 100
	@param SDApin I2C data GPIO pin  
	@param SCLKpin I2C clock GPIO pin 
	@return true if successful , init I2C communication
*/
bool SSD1306::OLEDbegin( uint8_t I2Caddress, i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t  SDApin, uint8_t  SCLKpin)
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
	ReturnCode = i2c_read_timeout_us(_i2c, _OLEDAddressI2C , &rxData, 1, false, _OLEDdelayI2C);
	if (ReturnCode < 1){ // no bytes read back from device or error issued
		if (_bSerialDebugFlag)
		{
			printf("1201 ERROR I2C_SSD1306 OLED :: begin: \r\n");
			printf("Check Connection, Return code :: %d ,RX data :: %u \r\n", ReturnCode , rxData);
		}
		_bIsConnected = false;
		return false;
	}
	_bIsConnected = true;
	OLEDinit();
	return true;
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

	busy_wait_ms(SSD1306_INITDELAY);
	SSD1306_command( SSD1306_DISPLAY_OFF);
	SSD1306_command( SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
	SSD1306_command( 0x80);
	SSD1306_command( SSD1306_SET_MULTIPLEX_RATIO );
	SSD1306_command( _OLED_HEIGHT - 1 );
	SSD1306_command( SSD1306_SET_DISPLAY_OFFSET );
	SSD1306_command(0x00);
	SSD1306_command( SSD1306_SET_START_LINE|0x00);
	SSD1306_command( SSD1306_CHARGE_PUMP );
	SSD1306_command(0x14);
	SSD1306_command( SSD1306_MEMORY_ADDR_MODE );
	SSD1306_command(0x00);  //Horizontal Addressing Mode is Used
	SSD1306_command( SSD1306_SET_SEGMENT_REMAP| 0x01);
	SSD1306_command( SSD1306_COM_SCAN_DIR_DEC );

switch (_OLED_HEIGHT)
{
	case 64: 
		SSD1306_command( SSD1306_SET_COM_PINS );
		SSD1306_command( 0x12 );
		SSD1306_command( SSD1306_SET_CONTRAST_CONTROL );
		SSD1306_command(0xCF);
	break;
	case 32: 
		SSD1306_command( SSD1306_SET_COM_PINS );
		SSD1306_command( 0x02 );
		SSD1306_command( SSD1306_SET_CONTRAST_CONTROL );
		SSD1306_command(0x8F);
	break;
	case 16: // NOTE: not tested, lacking part. 
		SSD1306_command( SSD1306_SET_COM_PINS );
		SSD1306_command( 0x2 ); // ?
		SSD1306_command( SSD1306_SET_CONTRAST_CONTROL );
		SSD1306_command(0xAF);
	break;
}

	SSD1306_command( SSD1306_SET_PRECHARGE_PERIOD );
	SSD1306_command( 0xF1 );
	SSD1306_command( SSD1306_SET_VCOM_DESELECT );
	SSD1306_command( 0x40 );
	SSD1306_command( SSD1306_DISPLAY_ALL_ON_RESUME );
	SSD1306_command( SSD1306_NORMAL_DISPLAY );
	SSD1306_command( SSD1306_DEACTIVATE_SCROLL );
	SSD1306_command( SSD1306_DISPLAY_ON );

	busy_wait_ms(SSD1306_INITDELAY);
}

/*!
	@brief Turns On Display
	@param bits   1  on , 0 off
*/
void SSD1306::OLEDEnable(uint8_t bits)
{
	bits ? SSD1306_command(SSD1306_DISPLAY_ON) : SSD1306_command(SSD1306_DISPLAY_OFF);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void SSD1306::OLEDContrast(uint8_t contrast)
{
	SSD1306_command( SSD1306_SET_CONTRAST_CONTROL );
	SSD1306_command(contrast);
}

/*!
	@brief invert the display
	@param value true invert , false normal
*/
void SSD1306::OLEDInvert(bool value)
{
 value ? SSD1306_command( SSD1306_INVERT_DISPLAY ) : SSD1306_command( SSD1306_NORMAL_DISPLAY );
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
		SSD1306_command( 0xB0 | row);
		SSD1306_command(SSD1306_SET_LOWER_COLUMN);
		SSD1306_command(SSD1306_SET_HIGHER_COLUMN);
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			SSD1306_data(dataPattern);
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
	SSD1306_command(Result);
	SSD1306_command(SSD1306_SET_LOWER_COLUMN);
	SSD1306_command(SSD1306_SET_HIGHER_COLUMN);
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		SSD1306_data(dataPattern);
		busy_wait_ms(mydelay);
	}
}

/*!
	@brief Draw a bitmap  to the buffer 
	@param x x axis offset 0-128
	@param y y axis offset 0-64
	@param w width 0-128
	@param h height 0-64
	@param data pointer to bitmap data
	@param invert color 
	@note bitmap data must be  horizontally addressed.
*/
void SSD1306::OLEDBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* data, bool invert)
{

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
			byte = data[j * byteWidth + i / 8];
			
		drawPixel(x + i, y, (byte & 0x80) ? color : bgcolor );
	}
}

}

/*!
	@brief Writes a byte to I2C address,command or data, used internally
	@param value write the value to be written
	@param cmd command or data
	@note In the event of an error will loop 3 times each time.
*/
void SSD1306::I2C_Write_Byte(uint8_t value, uint8_t cmd)
{
	uint8_t  dataBuffer[2] = {cmd,value};
	uint8_t attemptI2Cwrite = 0;
	uint8_t returnCode = 0;
	
	//returnCode = bcm2835_i2c_write(buf, 2); 
	returnCode = i2c_write_timeout_us(_i2c, _OLEDAddressI2C, dataBuffer, 2 , false, _OLEDdelayI2C);
	
	while(returnCode < 1)
	{ // failure to write I2C byte 
		attemptI2Cwrite ++;
		if (_bSerialDebugFlag)
		{
			printf("Error 1202 I2C OLED: Cannot Write byte :: %u\n", attemptI2Cwrite);
			printf("Error code %u\n", returnCode);
		}
		returnCode = i2c_write_timeout_us(_i2c, _OLEDAddressI2C, dataBuffer, 2 , false, _OLEDdelayI2C);
		busy_wait_ms(100); // mS
		if (attemptI2Cwrite >= 3) break;
	}
	if (returnCode < 1) _bIsConnected = false;
}

/*!
	@brief updates the buffer i.e. writes it to the screen
*/
void SSD1306::OLEDupdate()
{
	uint8_t x = 0; uint8_t y = 0; uint8_t w = this->bufferWidth; uint8_t h = this->bufferHeight;
	OLEDBuffer( x,  y,  w,  h, (uint8_t*) this->buffer);
}

/*!
	@brief clears the buffer memory i.e. does NOT write to the screen
*/
void SSD1306::OLEDclearBuffer()
{
	memset( this->buffer, 0x00, (this->bufferWidth * (this->bufferHeight /8))  );
}

/*!
	@brief Draw a bitmap directly to the screen
	@param x x axis  offset 0-128
	@param y y axis offset 0-64
	@param w width 0-128
	@param h height 0-64
	@param data the buffer data
	@note Called by OLEDupdate internally 
*/
void SSD1306::OLEDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data)
{
	uint8_t tx, ty;
	uint16_t offset = 0;
		
	SSD1306_command( SSD1306_SET_COLUMN_ADDR );
	SSD1306_command(0);   // Column start address (0 = reset)
	SSD1306_command( _OLED_WIDTH-1 ); // Column end address (127 = reset)

	SSD1306_command( SSD1306_SET_PAGE_ADDR );
	SSD1306_command(0); // Page start address (0 = reset)
	
	switch (_OLED_HEIGHT)
	{
		case 64: SSD1306_command(7); break;
		case 32: SSD1306_command(3); break;
		case 16: SSD1306_command(1); break;
	}
	
	for (ty = 0; ty < h; ty = ty + 8)
		{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT) {continue;}
		for (tx = 0; tx < w; tx++)
		{

			if (x + tx < 0 || x + tx >= _OLED_WIDTH) {continue;}
			offset = (w * (ty /8)) + tx;
			SSD1306_data(data[offset++]);
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

	if ((x < 0) || (x >= this->bufferWidth) || (y < 0) || (y >= this->bufferHeight)) {
	return;
	}
	int16_t temp;
	switch (_rotation) {
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
		uint16_t tc = (bufferWidth * (y /8)) + x;
		switch (color)
		{
			case WHITE:  this->buffer[tc] |= (1 << (y & 7)); break;
			case BLACK:  this->buffer[tc] &= ~(1 << (y & 7)); break;
			case INVERSE: this->buffer[tc] ^= (1 << (y & 7)); break;
		}
}

/*!
	@brief Scroll OLED data to the right
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLED_StartScrollRight(uint8_t start, uint8_t stop) 
{
	SSD1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	SSD1306_command(0X00);
	SSD1306_command(start);  // start page
	SSD1306_command(0X00);
	SSD1306_command(stop);   // end page
	SSD1306_command(0X00);
	SSD1306_command(0XFF);
	SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data to the left
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLED_StartScrollLeft(uint8_t start, uint8_t stop) 
{
	SSD1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
	SSD1306_command(0X00);
	SSD1306_command(start);
	SSD1306_command(0X00);
	SSD1306_command(stop);
	SSD1306_command(0X00);
	SSD1306_command(0XFF);
	SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data diagonally to the right
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLED_StartScrollDiagRight(uint8_t start, uint8_t stop) 
{
	SSD1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	SSD1306_command(0X00);
	SSD1306_command(_OLED_HEIGHT);
	SSD1306_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	SSD1306_command(0X00);
	SSD1306_command(start);
	SSD1306_command(0X00);
	SSD1306_command(stop);
	SSD1306_command(0X01);
	SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data diagonally to the left
	@param start start position
	@param stop stop position 
*/
void SSD1306::OLED_StartScrollDiagLeft(uint8_t start, uint8_t stop) 
{
	SSD1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	SSD1306_command(0X00);
	SSD1306_command(_OLED_HEIGHT);
	SSD1306_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	SSD1306_command(0X00);
	SSD1306_command(start);
	SSD1306_command(0X00);
	SSD1306_command(stop);
	SSD1306_command(0X01);
	SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief  Stop scroll mode
*/
void SSD1306::OLED_StopScroll(void) 
{
	SSD1306_command(SSD1306_DEACTIVATE_SCROLL);
}

/*!
	@brief Check Connection Function
		Check if device is on the bus asks for one byte
	@return int16_t if less than zero = error 
*/
int16_t SSD1306::CheckConnection()
{
	int16_t returnValue;
	uint8_t rxData;
	returnValue = i2c_read_timeout_us(_i2c, _OLEDAddressI2C , &rxData, 1, false, _OLEDdelayI2C);
	if (_bSerialDebugFlag) printf("Error 1203 I2C OLED:: CheckConnection %d , %u \r\n ", returnValue , rxData);
	if (returnValue < 1) _bIsConnected = false;
	return returnValue;
}

/*!
	@brief getter for is connected status
	@return is connected or disconnected
*/
bool SSD1306::GetIsConnected(void)
{return _bIsConnected;}

/*!
	@brief setter for is connected status 
	@param connected is device connected , true yes, false no
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

// ---  EOF ---
