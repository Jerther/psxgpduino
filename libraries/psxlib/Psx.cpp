/*  PSX Controller Decoder Library (Psx.cpp)
	Written by: Kevin Ahrendt June 22nd, 2008
	
	Controller protocol implemented using Andrew J McCubbin's analysis.
	http://www.gamesx.com/controldata/psxcont/psxcont.htm
	
	Shift command is based on tutorial examples for ShiftIn and ShiftOut
	functions both written by Carlyn Maw and Tom Igoe
	http://www.arduino.cc/en/Tutorial/ShiftIn
	http://www.arduino.cc/en/Tutorial/ShiftOut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Psx.h"

#define DATA_PIN         15
#define DATA_DDR         DDRB
#define DATA_PORT        PINB       // PORTB input
#define DATA_PORT_INDEX  1          // PB1
#define DATA_MASK        B00000010  // index 1

#define CMD_PIN         14
#define CMD_DDR         DDRB
#define CMD_PORT        PORTB       // PORTB output
#define CMD_PORT_INDEX  3           // PB3

#define CLOCK_PIN        16
#define CLOCK_DDR        DDRB
#define CLOCK_PORT       PORTB      // PORTB output
#define CLOCK_PORT_INDEX 2          // PB2

#define ATT_PIN         10
#define ATT_DDR         DDRB
#define ATT_PORT        PORTB       // PORTB output
#define ATT_PORT_INDEX  6           // PB6



Psx::Psx()
{
	// pinMode and digitalWrite do all sorts of initial things.
	// it's WAY simpler just to use them instead of toying with registers.
	pinMode(DATA_PIN, INPUT_PULLUP);
	
	pinMode(CMD_PIN, OUTPUT);
	digitalWrite(CMD_PIN, LOW);

	pinMode(ATT_PIN, OUTPUT);
	digitalWrite(ATT_PIN, HIGH);

	pinMode(CLOCK_PIN, OUTPUT);
	digitalWrite(CLOCK_PIN, HIGH);
}

byte Psx::shift(byte _dataOut)							// Does the actual shifting, both in and out simultaneously
{
	for (_i = 0; _i <= 7; _i++)
	{
		bitClear(CLOCK_PORT, CLOCK_PORT_INDEX);
		if (bitRead(_dataOut, _i))
			bitSet(CMD_PORT, CMD_PORT_INDEX);
		else
			bitClear(CMD_PORT, CMD_PORT_INDEX);
		delayMicroseconds(10);
		bitSet(CLOCK_PORT, CLOCK_PORT_INDEX);
		bitWrite(_dataIn, 7 - _i, (DATA_PORT & DATA_MASK) == B00000010);
		delayMicroseconds(6);
	}
	return _dataIn;
}

unsigned int Psx::read()
{
	bitClear(ATT_PORT, ATT_PORT_INDEX);

	shift(0x01);
	shift(0x42);
	shift(0xFF);

	_data1 = ~shift(0xFF);
	_data2 = ~shift(0xFF);

	bitSet(ATT_PORT, ATT_PORT_INDEX);

	_dataOut = (_data2 << 8) | _data1;

	return _dataOut;
}
