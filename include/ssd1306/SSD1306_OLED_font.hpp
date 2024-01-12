/*!
	@file SSD1306_OLED_font.hpp
	@brief OLED driven by SSD1306 controller. font data file 10 fonts.
	@author Gavin Lyons.
	@details 
		-#  1. pFontDefault  6 by 8
		-#  2. pFontWide  9 by 8 (NO LOWERCASE)
		-#  3. pFontpico 3 by 6
		-#  4. pFontSinclairS 8 by 8
		-#  5. pFontMega  Mega 16 by 16
		-#  6. pFontArialBold 16 by 16
		-#  7. pFontHallfetica 16 by 16
		-#  8. pFontArialRound 16 by 24
		-#  9. pFontGroTesk 16 by 32
		-#  10. pFontSixteenSeg 32 by 48 (NUMBERS ONLY + : . -)
*/

#pragma once

#include <cstdint>

// Font data is in the cpp file accessed thru extern pointers.
extern const uint8_t * pFontDefault;
extern const uint8_t * pFontWide;
extern const uint8_t * pFontPico;
extern const uint8_t * pFontSinclairS;
extern const uint8_t * pFontMega;
extern const uint8_t * pFontArialBold;
extern const uint8_t * pFontHallfetica;
extern const uint8_t * pFontArialRound;
extern const uint8_t * pFontGroTesk;
extern const uint8_t * pFontSixteenSeg;
