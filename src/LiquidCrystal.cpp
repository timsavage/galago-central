#include "LiquidCrystal.h"

using namespace Galago;
using namespace GC;

inline void sleep(int interval) 
{ 
    int sleepTime = 75 * interval;
    // Perform a bunch of no-op instructions to simulate sleeping 
    for(int i = 0; i < sleepTime; i++) 
        __asm volatile("nop"::); 
}

LiquidCrystal::LiquidCrystal(bool twoLine)
    : _displayfunction(twoLine ? LCD_2LINE : 0)
    , _displaycontrol(LCD_DISPLAYON)
    , _displaymode(0)
{}

void LiquidCrystal::bind(IO::Pin &rs, IO::Pin &enable,
                         IO::Pin &d0, IO::Pin &d1, IO::Pin &d2, IO::Pin &d3)
{
    _rs.bind(rs);
    _enable.bind(enable);
    
    // Make this 4 Bit mode
    _data[0].bind(d0);
    _data[1].bind(d1);
    _data[2].bind(d2);
    _data[3].bind(d3); 
}

void LiquidCrystal::bind(IO::Pin &rs, IO::Pin &enable,
                         IO::Pin &d0, IO::Pin &d1, IO::Pin &d2, IO::Pin &d3,
                         IO::Pin &d4, IO::Pin &d5, IO::Pin &d6, IO::Pin &d7)
{
    _rs.bind(rs);
    _enable.bind(enable);

    // Make this 8 Bit mode
    _displayfunction |= LCD_8BITMODE;
    _data[0].bind(d0);
    _data[1].bind(d1);
    _data[2].bind(d2);
    _data[3].bind(d3); 
    _data[4].bind(d4);
    _data[5].bind(d5);
    _data[6].bind(d6);
    _data[7].bind(d7);
}

void LiquidCrystal::start()
{
    _rs.setOutput();
    _enable.setOutput();
    
    if (_displayfunction & LCD_8BITMODE) {
        command(LCD_FUNCTIONSET | _displayfunction);
        sleep(450);
    } else {
        write4Bits(0x3);
        sleep(450);
        write4Bits(0x3);
        sleep(450);
        write4Bits(0x3);
        sleep(150);
        write4Bits(0x2);
        write4Bits(0xC);
    }

    command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

/********** high level commands, for the user! */

void LiquidCrystal::enableDisplay(bool on)
{
    if (on) {
        _displaycontrol |= LCD_DISPLAYON;
    } else {
        _displaycontrol &= ~LCD_DISPLAYON;
    }
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::showCursor(bool on)
{
    if (on) {
        _displaycontrol |= LCD_CURSORON;
    } else {
        _displaycontrol &= ~LCD_CURSORON;
    }
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::blinkCursor(bool on)
{
    if (on) {
        _displaycontrol |= LCD_BLINKON;
    } else {
        _displaycontrol &= ~LCD_BLINKON;
    }
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::clear()
{
    command(LCD_CLEARDISPLAY);
    sleep(2000);  // This command takes a long time!
}

void LiquidCrystal::home()
{
    command(LCD_RETURNHOME);
    sleep(2000);  // this command takes a long time!
}

void LiquidCrystal::setCursor(byte location)
{
    command(LCD_SETDDRAMADDR | (location & 0x7F));
}

void LiquidCrystal::scrollDisplayLeft(byte step)
{
    for (byte i = 0; i <= step; i++) {
        command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE);
    }
}

void LiquidCrystal::scrollDisplayRight(byte step)
{
    for (byte i = 0; i <= step; i++) {
        command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    }
}

void LiquidCrystal::print(char const* string)
{
    char* offset = (char *)string;
    while (*offset != 0) {
        write(*(offset++));
    }
}

void LiquidCrystal::print(char const* string, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        write(*(string + i));
    }
}

/************ low level data pushing commands **********/

void LiquidCrystal::send(byte value, bool mode) {
    _rs = mode;

    if (_displayfunction & LCD_8BITMODE) {
        write8Bits(value);
    } else {
        write4Bits(value >> 4);
        write4Bits(value);
    }
}

void LiquidCrystal::pulseEnable(void)
{
    _enable = false;
    sleep(1);
    _enable = true;
    sleep(1);
    _enable = false;
    sleep(100);
}

void LiquidCrystal::write4Bits(byte value)
{
    for (int i = 0; i < 4; i++) {
        _data[i].setOutput();
        _data[i].write((value >> i) & 0x01);
    }
    
    pulseEnable();
}

void LiquidCrystal::write8Bits(byte value)
{
    for (int i = 0; i < 8; i++) {
        _data[i].setOutput();
        _data[i].write((value >> i) & 0x01);
    }
    
    pulseEnable();
}
