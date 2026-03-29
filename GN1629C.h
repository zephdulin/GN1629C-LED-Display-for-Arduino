/*
 * 	SPI driver for the GN1629C 15 segment, 8 grid  LED driver manufactured by GN Semiconductor (Shanghai) co. ltd.
 *		
 *	Zeph Dulin
 *	zephdulin.com
 *	Austin, Texas USA
 *	2026-03-01
 */
 
#ifndef _GN1629C_H
#define _GN1629C_H

#include <Arduino.h>
#include <SPI.h>
 
class GN1629C
{

  public:
	 
  #define LEFT_JUSTIFY 0
  #define RIGHT_JUSTIFY 1
  #define NORMAL 0
  #define INVERTED 1
  #define MARQUEE 1
	
	GN1629C(int, int, uint8_t); 					// * Constructor. Start Digit, # of Digits, CS pin
	
	bool begin();									// * Initializes the display:
													//		- starts the SPI
													//		- prepares the display

	void writeDisplay(char displayData[]);			// show the input string on the LED diplay. default to left-justify text.
	
	void writeDisplay(char displayData[], int justifyText);			// show the input string on the LED diplay. overload for setting text justify
	
	void displayON(int Brightness);					// turn on the LED display with input brightness 1-7
	
	void displayOFF();								// turn off the LED display
	
	void clearDisplay();							// clear the display buffer in the driver
	
	void displayTransfer(int addr, int displayData[]);
	
	void cmdTransfer(int);
  
  private:

	void setFonts();
	
	uint8_t CS_PinSet;
	int numDigitsSet;
	int startDigitSet;
	int justifyText;

	word font_14Seg[128];

};
#endif