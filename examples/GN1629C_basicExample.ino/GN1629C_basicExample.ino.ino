/*******************************************************************************/
/* GN1621C 14-Segment Display Control Example                                  */
/*  Zeph Dulin                                                                 */
/*  Austin, Texas USA                                                          */
/*  2026-03-29                                                                 */
/*                                                                             */
/*******************************************************************************/

#include <GN1629C.h>

#define startDigit 0
#define numDigits 8
#define CS_Pin D10

GN1629C LED_displayTEMP(startDigit, numDigits, CS_Pin);

/**********************************************************************************/
void setup() {

  LED_displayTEMP.begin();
  LED_displayTEMP.displayON(5); // set the display brightness from 1 (dim) to 7 (bright)
}

/**********************************************************************************/
void loop() {

  LED_displayTEMP.writeDisplay("HELLO"); // default to left-justify text
  delay(500);
  LED_displayTEMP.writeDisplay("WORLD", RIGHT_JUSTIFY); // right-justify text with parameter
  delay(500);
}
