#ifndef __LIQUID_CRYSTAL_H__
#define __LIQUID_CRYSTAL_H__

#include <GalagoAPI.h>

namespace GC {
	
using namespace Galago;

// commands
#define LCD_CLEARDISPLAY	0x01
#define LCD_RETURNHOME 		0x02
#define LCD_ENTRYMODESET 	0x04
#define LCD_DISPLAYCONTROL 	0x08
#define LCD_CURSORSHIFT 	0x10
#define LCD_FUNCTIONSET 	0x20
#define LCD_SETCGRAMADDR 	0x40
#define LCD_SETDDRAMADDR 	0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 	0x00
#define LCD_ENTRYLEFT 	0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 	0x04
#define LCD_DISPLAYOFF 	0x00
#define LCD_CURSORON 	0x02
#define LCD_CURSOROFF 	0x00
#define LCD_BLINKON 	0x01
#define LCD_BLINKOFF 	0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 	0x00
#define LCD_MOVERIGHT 	0x04
#define LCD_MOVELEFT 	0x00

// flags for function set
#define LCD_8BITMODE 	0x10
#define LCD_4BITMODE 	0x00
#define LCD_2LINE 		0x08
#define LCD_1LINE 		0x00
#define LCD_5x10DOTS 	0x04
#define LCD_5x8DOTS 	0x00


/**
 * Interface for simple LCD
 */
class LiquidCrystal 
{
public:
	LiquidCrystal(bool twoLine=true);

    void bind(IO::Pin &rs, IO::Pin &enable,
              IO::Pin &d0, IO::Pin &d1, IO::Pin &d2, IO::Pin &d3);
    void bind(IO::Pin &rs, IO::Pin &enable,
              IO::Pin &d0, IO::Pin &d1, IO::Pin &d2, IO::Pin &d3,
              IO::Pin &d4, IO::Pin &d5, IO::Pin &d6, IO::Pin &d7);

	/**
	 * Initialise the LCD
	 */
	void start();
	
	/**
	 * Enable the LCD
	 */
	void enableDisplay(bool on=true);
	
	/**
	 * Turn on cursor
	 */
	void showCursor(bool on=true);
	
	/**
	 * Set the cursor to blink mode
	 */
	void blinkCursor(bool on=true);
	
	/**
	 * Clear display, set cursor position to zero
	 */
	void clear();
	
	/**
	 * Set cursor position to zero
	 */
	void home();
		
	/**
	 * Move the cursor to a particular location.
	 * 
	 * The location is actually a location in the LCD memory that might
	 * not be visible on screen. The typical LCD uses a virtual display
	 * of 80 characters, depending on the number of lines the display is
	 * configured to display these 80 characters are either in one long
	 * 80 character line or split into two 40 character lines.
	 * 
	 * To place a character on the second line you need to specify the 
	 * location by adding 40. So for the 5 column on the second line 
	 * move the cursor to location 45.
	 */
	void setCursor(byte location);
	
	/**
	 * Move the entire display left
	 */
	void scrollDisplayLeft(byte step=1);

	/**
	 * Move the entire display right
	 */
	void scrollDisplayRight(byte step=1);
	
	/**
	 * Print a string to the display
	 */
	void print(char const* cStr);
	
	/**
	 * Print a fixed length string to the display
	 */
	void print(char const* cStr, size_t length);
	
	/**
	 * Write a command to the LCD
	 */
	inline void command(byte value) { send(value, false); }
	
	/**
	 * Write a single "Character" to the LCD
	 */
	inline void write(char value) { send(value, true); }
	
private:
	void send(byte value, bool mode);
	void pulseEnable(void);
	void write4Bits(byte value);
	void write8Bits(byte value);

    IO::Pin _rs;  // LOW: command.  HIGH: character.
    IO::Pin _enable;  // activated by a HIGH pulse.
    IO::Pin _data[8];
    
	byte _displayfunction;
	byte _displaycontrol;
	byte _displaymode;
};

} // GC

#endif //__LIQUID_CRYSTAL_H__
