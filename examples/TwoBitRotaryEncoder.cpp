#include <GalagoAPI.h>
#include "TwoBitRotaryEncoder.h"

using namespace Galago;
using namespace GC;

TwoBitRotaryEncoder encoder;

void readEncoder(void*, Task, bool) 
{
	static byte counter = 0;

	signed char val = encoder.readPulse();
	if (val) {
		counter += val;
		
		io.serial.write("\n\rCounter:");
		io.serial.write(counter);
	}
	
	system.when(system.delay(1), readEncoder);
}

int main(void)
{
	io.serial.start(4800);
	
	// Bind Hi and Low pins off of the encoder.
	encoder.bind(io.p4, io.p5);
	
	system.when(system.delay(1), readEncoder);

	while(true)
		system.sleep();
}
