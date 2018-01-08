#include "msp.h"


/**
 * main.c
 * Author: Kevin Kuwata
 * Date: 1/7/2018
 * Version: 0.1
 *
 *
 * Summary: This is a basic driver for the ePAPER 200x200 display.
 * I am modeling this off of TI's example for the CrystalFontz 128x128 color display
 * I am strongly motivated because of TI's graphic library capabilities.
 *
 *      Files direclty modelled after:
 *      HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735
 *      Crystalfontz128x128_ST7735.h
 *
 *  Notes: The initiation sequence is derived from the weaveshare's example code for arduino.
 *
 *
 *
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
