#include "TwoBitRotaryEncoder.h"

using namespace Galago;
using namespace Controller;

static char ENC_STATES[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

TwoBitRotaryEncoder::TwoBitRotaryEncoder(IO::Pin &pinLow, IO::Pin &pinHigh)
	: _v(0)
{
	_pinLow.bind(pinLow);
	_pinLow.setInput();
	_pinHigh.bind(pinHigh);
	_pinHigh.setInput();
}

signed char TwoBitRotaryEncoder::readPulse()
{
	byte low = _pinLow ? 0x01 : 0;
	byte high = _pinHigh ? 0x02 : 0;
	
	_v = (_v << 2);
	_v |= (high | low);
	
	return (ENC_STATES[_v & 0x0f]); 
}
