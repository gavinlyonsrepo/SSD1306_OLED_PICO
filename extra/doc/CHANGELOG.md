# Changelog

* Version 1.0.0 Dec 2023
	* first release
* Version 1.0.1 Jan 24
	* Added error handling & return values to Text and 
		Bitmap related functions.
	* Added new I2C parameters for testing & error handling. 
		Timeout, retry attempts amd retry delay.
	* Wrapped the buffer pointer in a function.
	* Added I2C test example file. 
* Version 1.0.2 Feb 24
	* minor update
	* added rotate enum.
	* corrected number of characters parameter for sixteen segment font 15 to 14.
	* corrected 'drawPixel' boundary scan for 90 and 270 degree rotation. 
* Version 1.1.0 Feb 25
	* Added Error enum and improved error handling 
* Version 2.0.0 Mar 2025
	* Changed project name from SSD1306_OLED_PICO to displaylib_1bit_PICO
	* Added support for erm19264, nokia5110 , Sh1106 sh1107 and ch1115 displays
	* Added Advanced graphics options.
