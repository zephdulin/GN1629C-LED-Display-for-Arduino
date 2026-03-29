/**********************************************************************************************
 * Arduino 14-Segment LED Display Library for GN1629C device
 * Zeph Dulin
 *
 * Austin, Texas USA
 **********************************************************************************************/

#include <Arduino.h>
#include <GN1629C.h>

/* Constructor (...) */
GN1629C::GN1629C(int startDigit, int numDigits, uint8_t CS_Pin)
{
	CS_PinSet = CS_Pin;
	numDigitsSet = numDigits;
	startDigitSet = startDigit;

}		

/* begin() ******************************************************************/
bool GN1629C::begin()
{
	pinMode(CS_PinSet, OUTPUT);
	digitalWrite(CS_PinSet, HIGH);
	SPI.begin();
	SPI.beginTransaction(SPISettings(400000, LSBFIRST, SPI_MODE3));
  
	GN1629C::setFonts();
	GN1629C::clearDisplay();
	GN1629C::displayON(1);
  
  return true;
}

/* clearDisplay ***********************************************************/
void GN1629C::clearDisplay()
{
	int val[16] = { 0 };
	GN1629C::displayTransfer(192, val);
}

/* displayON **************************************************************/
void GN1629C::displayON(int brightness) {  // brightness value 1 = minimum, 8 = maximum
  int val = 135 + brightness;
  cmdTransfer(val);
}

/* displayOFF ************************************************************/
void GN1629C::displayOFF() {
  cmdTransfer(128);
}

/* cmdTransfer ***********************************************************/
void GN1629C::cmdTransfer(int cmd)
{
	digitalWrite(CS_PinSet, LOW);
	SPI.transfer(cmd);
	digitalWrite(CS_PinSet, HIGH);
}

/* displayTransfer ***********************************************************/
/* take the prepared data and shoot it to the display module using SPI transfer */
void GN1629C::displayTransfer(int addr, int displayData[]) 
{
	digitalWrite(CS_PinSet, LOW);
	SPI.transfer(addr);
	for (int i = 0; i < 16; i++) {
		SPI.transfer(displayData[i]);
	}
	digitalWrite(CS_PinSet, HIGH);
}

/* writeDisplay ***********************************************************/
/* take the incoming string and arrange it into the format needed for transfer to the display module */
void GN1629C::writeDisplay(char displayData[], int justifyText)
{
	int displayVal[256] = { 0 };
	int displayStr[256] = { 0 };
	int displayBytes[256] = { 0 };
	int dispStrLen = strlen(displayData);
	int dispDataLen = strlen(displayData);
	int paddingLen;
	int j = 0;
	bool lastWasDecimal = false;
	
	// select the characters from the font array
	for (int i = 0; i < dispStrLen; i++) { 
		int asciiNum = displayData[i];
		
		if ((asciiNum != 46) || (i==0) || (lastWasDecimal == true)) {
			displayStr[j] = font_14Seg[asciiNum];
			j++;
			if (asciiNum == 46) { 
				lastWasDecimal = true;
			} else {
				lastWasDecimal = false;
			}
		} else { //handle the period or decimal
			displayStr[j-1] = displayStr[j-1] + font_14Seg[asciiNum];
			dispDataLen = dispDataLen - 1;
			lastWasDecimal = true;
		}
	}
	
	if (dispDataLen > 32) {
		while(1);
	}
	
	// set the padding length to right-justify the text
	if ((dispDataLen < numDigitsSet)&& justifyText) {
		paddingLen = numDigitsSet - dispDataLen;
	} else {
		paddingLen = 0;
	}
	
	// apply padding length to the display string
	for (int i = 0; i < dispDataLen; i++) {
		displayVal[i+paddingLen] = displayStr[i];
	}
	
	// separate the bytes hi and lo
	for (int i = 0; i < numDigitsSet*2; i+=2) { 

		displayBytes[i] = highByte(displayVal[i/2]);
		displayBytes[i+1] = lowByte(displayVal[i/2]);
	}	
	
	// send the prepared display array
	int tempDisplayBytes4 = displayBytes[4];
	int tempDisplayBytes5 = displayBytes[5];
	displayBytes[4] = displayBytes[6];
	displayBytes[5] = displayBytes[7];
	displayBytes[6] = tempDisplayBytes4;
	displayBytes[7] = tempDisplayBytes5;
	
	GN1629C::displayTransfer(192, displayBytes);
}

/* writeDisplay - default to left-justify text ***********************************************************/
void GN1629C::writeDisplay(char displayData[]) {
	justifyText = LEFT_JUSTIFY;
	writeDisplay(displayData, justifyText);
}

/* setFonts ********************************************************************************/
void GN1629C::setFonts() {

/*	segment assignments: 	0b| L | K | E | C | dp| D | G2| M | nc| B | A | G1| J | I | H | F | */
  font_14Seg[31] = 0b0000000000000000;

  font_14Seg[32] = 0b0000000000000000;  // 32 - space
  font_14Seg[33] = 0b0000100001000000;  // 33 - exclamation mark

  font_14Seg[46] = 0b0000100000000000;  // 46 - period .

  font_14Seg[48] = 0b0011010011100001;  // 48 - 0
  font_14Seg[49] = 0b0001000001001000;  // 49 - 1
  font_14Seg[50] = 0b0010011001110000;  // 50 - 2
  font_14Seg[51] = 0b0001011001100000;  // 51 - 3
  font_14Seg[52] = 0b0001001001010001;  // 52 - 4
  font_14Seg[53] = 0b0001011000110001;  // 53 - 5
  font_14Seg[54] = 0b0011011000110001;  // 54 - 6
  font_14Seg[55] = 0b0001000001100000;  // 55 - 7
  font_14Seg[56] = 0b0011011001110001;  // 56 - 8
  font_14Seg[57] = 0b0001011001110001;  // 57 - 9
  
  font_14Seg[63] = 0b1000001001100000;  // 63 - ?

  font_14Seg[65] = 0b0011001001110001;  // 65 - A
  font_14Seg[66] = 0b1001011001100100;  // 66 - B
  font_14Seg[67] = 0b0010010000100001;  // 67 - C
  font_14Seg[68] = 0b1001010001100100;  // 68 - D
  font_14Seg[69] = 0b0010010000110001;  // 69 - E
  font_14Seg[70] = 0b0010000000110001;  // 70 - F
  font_14Seg[71] = 0b0011011000100001;  // 71 - G
  font_14Seg[72] = 0b0011001001010001;  // 72 - H
  font_14Seg[73] = 0b1000010000100100;  // 73 - I
  font_14Seg[74] = 0b0011010001000000;  // 74 - J
  font_14Seg[75] = 0b0010000100011001;  // 75 - K
  font_14Seg[76] = 0b0010010000000001;  // 76 - L
  font_14Seg[77] = 0b0011000001001011;  // 77 - M
  font_14Seg[78] = 0b0011000101000011;  // 78 - N
  font_14Seg[79] = 0b0011010001100001;  // 79 - O
  font_14Seg[80] = 0b0010001001110001;  // 80 - P
  font_14Seg[81] = 0b0011010101100001;  // 81 - Q
  font_14Seg[82] = 0b0010001101110001;  // 82 - R
  font_14Seg[83] = 0b0001011000110001;  // 83 - S
  font_14Seg[84] = 0b1000000000100100;  // 84 - T
  font_14Seg[85] = 0b0011010001000001;  // 85 - U
  font_14Seg[86] = 0b0110000000001001;  // 86 - V
  font_14Seg[87] = 0b0111000101000001;  // 87 - W
  font_14Seg[88] = 0b0100000100001010;  // 88 - X
  font_14Seg[89] = 0b1000000000001010;  // 89 - Y
  font_14Seg[90] = 0b0100010000101000;  // 90 - Z
/*
  font[97] = { 0b11011111, 0b00000000 };   // 97 - a
  font[98] = { 0b11111100, 0b00000000 };   // 98 - b
  font[99] = { 0b00111001, 0b00000000 };   // 99 - c
  font[100] = { 0b11011110, 0b00000000 };  // 100 - d
  font[101] = { 0b11111011, 0b00000000 };  // 101 - e
  font[102] = { 0b01110001, 0b00000000 };  // 102 - f
  font[103] = { 0b11101111, 0b00000000 };  // 103 - g
  font[104] = { 0b11110100, 0b00000000 };  // 104 - h
  font[105] = { 0b00000000, 0b00010000 };  // 105 - i
  font[106] = { 0b00011110, 0b00000000 };  // 106 - j
  font[107] = { 0b01110000, 0b00100100 };  // 107 - k
  font[108] = { 0b00000000, 0b00010010 };  // 108 - l
  font[109] = { 0b11010100, 0b00010000 };  // 109 - m
  font[110] = { 0b01010000, 0b00010000 };  // 110 - n
  font[111] = { 0b11011100, 0b00000000 };  // 111 - o
  font[112] = { 0b11110011, 0b00000000 };  // 112 - p
  font[113] = { 0b11100111, 0b00000000 };  // 113 - q
  font[114] = { 0b01010000, 0b00000000 };  // 114 - r
  font[115] = { 0b11101101, 0b00000000 };  // 115 - s
  font[116] = { 0b01111000, 0b00000000 };  // 116 - t
  font[117] = { 0b00011100, 0b00000000 };  // 117 - u
  font[118] = { 0b00010000, 0b00001000 };  // 118 - v
  font[119] = { 0b00011100, 0b00010000 };  // 119 - w
  font[120] = { 0b00000000, 0b00101101 };  // 120 - x
  font[121] = { 0b00000000, 0b00010101 };  // 121 - y
  font[122] = { 0b00001001, 0b00001100 };  // 122 - z
*/
}
