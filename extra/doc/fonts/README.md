# Fonts

There are 10 fonts packaged with library. 
Fonts can be easily added or removed by user.
All the Font data is in file display_fonts.cpp and display_fonts.hpp

Font data table: 

| num | Font pointer name | character size XxY |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | pFontDefault | 6x8 |  0 - 0xFE, Full Extended  | 1534 |
| 2 | pFontWide | 9x8 | 0x20 - 0x5A, NO lowercase letters | 535 |
| 3 | pFontPico | 4x6 | 0x20 - 0x7E  | 289 | 
| 4 | pFontSinclairS  | 8x8 | 0x20 - 0x7E | 764 |
| 5 | pFontMega | 16x16 | 0x20 - 0x7E | 3044 |
| 6 | pFontArialBold  | 16x16 | 0x20 - 0x7E |  3044 |
| 7 | pFontHallfetica | 16x16 | 0x20 - 0x7E | 3044 |
| 8 | pFontArialRound| 16x24 | 0x20 - 0x7E | 4564 |
| 9 | pFontGroTesk | 16x32 | 0x20 - 0x7A |  5828 |
| 10 | pFontSixteenSeg | 32x48 | 0x2D-0x3A , 0-10  :  .  / - only | 2692 |

Font size in bytes = ((X * (Y/8)) * numberOfCharacters) + (4*ControlByte)

| Font class Function | Notes |
| ------ | ------ | 
| writeChar| draws single character |
| writeCharString | draws character array |
| print | Polymorphic print class which will print out many data types |

These methods return an error code in event of an error.

**Remove a font**

To remove an unwanted font from project simply comment out or delete.

1. The Font data in display_fonts.cpp file
2. The span to font at middle(search 'Font spans definitions') of display_fonts.cpp file
3. The associated extern declaration in the display_fonts.hpp file

**Adding a new font**

1. Add the Font data in display_fonts.cpp file
2. Add a span to font at middle(search 'Font spans definitions') of display_fonts.cpp file
3. Add an associated extern declaration in the display_fonts.hpp file

The new ASCII font must have following font structure.
First 4 bytes are control bytes followed 
by *vertically addressed* font data.

```
// An 4 by 8 character size font starting at 
// ASCII offset 0x30 in ASCII table with 2 characters in font. 
// '0' and '1' 
static const std::array<uint8_t, 12>FontBinaryExample[] =
{
0x04, 0x08, 0x30, 0x01,   // x-size, y-size, offset, total characters-1
(data),(data),(data),(data) // font data '0' , 0x30
(data),(data),(data),(data) // font data '1' , 0x31
}
```

*Sources*

1. Some of the fonts packaged with library came from [URL LINK](http://rinkydinkelectronics.com/)
2. These fonts have horizontally addressed data by default.
3. They must be converted to vertically addressed font data if user wants them for library.
4. There is a monochrome font maker there at [URL](http://rinkydinkelectronics.com/t_make_font_file_mono.php),
5. Just upload the picture of font from URL links on step 1 to it. 
6. The font you pick MUST have : Height(or y-size) must be divisible evenly by 8. (Width X Height)

Another source for vertical fonts is [URL LINK](https://jared.geek.nz/2014/01/custom-fonts-for-microcontrollers/)
