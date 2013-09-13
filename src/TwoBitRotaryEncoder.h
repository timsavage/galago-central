#ifndef __TWO_BIT_ROTARY_ENCODER_H__
#define __TWO_BIT_ROTARY_ENCODER_H__

#include <GalagoAPI.h>

namespace GC {

using namespace Galago;

/*
 * Simple decoder for working with a rotary encode that outputs 2bit 
 * grey code.
 * 
 * Turning the encoder CW with produce a value from readPulse of 1, 
 * while turning the encoder CCW with produce a value of -1. If the 
 * encoder has not been turned a value of 0 will result.
 * 
 * Pins represent a high and low bit. If the value from readPulse is 
 * inverted swapping the pins will result in the correct value.
 * 
 */
class TwoBitRotaryEncoder 
{
public:
	TwoBitRotaryEncoder();
	
	void bind(IO::Pin &pinLow, IO::Pin &pinHigh);
	
	signed char readPulse();
	
private:
	IO::Pin _pinLow;
	IO::Pin _pinHigh;
	byte _v;
};

} // GC

#endif //__TWO_BIT_ROTARY_ENCODER_H__
