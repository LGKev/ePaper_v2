#include "msp.h"
#include "ePaper200x200_driver.h"
#include "HAL_MSP_EXP432P401R_ePaper200x200.h"
#include "gpio.h"

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
 *      To change from LAUNCHPAD to WATCH_V2
 *                   uncomment the define at the top of  HAL_MSP_EXP432P401R_ePaper200x200.h
 *                      launchpad uses B0
 *                      watch v2 uses A2
 *
 */



uint8_t testImage[] = {0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0xFF, 0XFF, 0XFF, 0XFF, 0XFF};

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
ePaper200x200_Init(0); //0 being partial and 1 being full

uint8_t count = 0;
while(1){
	ePaper200x200_ClearFrameMemory(WHITE);
	ePaper200x200_DisplayFrame2();
	P3OUT |=BIT2;
	for(count = 0; count <200; count++);
//	ePaper200x200_Load_Image(50, 50, testImage);
    P3OUT &=~BIT2;

	ePaper200x200_ClearFrameMemory(BLACK);



	ePaper200x200_DisplayFrame2();
  //  for(count = 0; count < 2000; count++);

}



}
