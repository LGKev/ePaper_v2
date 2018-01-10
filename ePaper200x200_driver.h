/*
 * ePaper200x200_driver.h
 *
 *  Created on: Jan 7, 2018
 *      Author: Kevin Kuwata
 *
 *      This file is essentially the parallel of Crystalfontz128x128_ST7735.h but for the ePaper.
 *		
 */

#ifndef EPAPER200X200_DRIVER_H_
#define EPAPER200X200_DRIVER_H_

#include <stdint.h>
#include "driverlib.h"
#include "grlib.h"
#include "msp.h"
#include "HAL_MSP_EXP432P401R_ePaper200x200.h"
// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   200
#define LCD_HORIZONTAL_MAX                 200

//TODO: check that this is truly White  OR BLACK
#define BLACK   0XFF
#define WHITE    0X00

#define LCD_ORIENTATION_UP    0
#define LCD_ORIENTATION_LEFT  1
#define LCD_ORIENTATION_DOWN  2
#define LCD_ORIENTATION_RIGHT 3

//ePaper 200x200 LCD controller Command Table
#define CMD_DRIVER_OUTPUT_CONTROL 0X01
#define CMD_BOOSTER_SOFT_START_CONTROL 0X0C
#define CMD_GATE_SCAN_SP            0X0F
#define CMD_DEEP_SLEEP          0X10
#define CMD_DATA_ENTRY  0X11
#define CMD_SW_RESET     0X12
#define CMD_MASTER_ACTV  0X20
#define CMD_WRITE_RAM 0X24
#define CMD_VCOM    0X2C
#define CMD_WRITE_LUT 0X32
#define CMD_DUMMY_LINE 0X3A
#define CMD_GATE_TIME 0X3B
#define CMD_BORDER 0X3C
#define CMD_DISPLAY_UPDATE_CTRL2    0X22
#define CMD_DISP_UPDATE_SEQ1            0XC4 // ENABLE CLOCK, ENABLE CP, TO PATTERN DISPLAY.

#define CMD_X_ADDR_START 0X44
#define CMD_Y_ADDR_START 0X45
#define CMD_X_COUNTER 0X4E
#define CMD_Y_COUNTER 0X4F
#define CMD_NOP 0XFF //GOOD FOR ENDING A COMMAND    /   DATA

 uint8_t Lcd_Orientation;
 uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;

extern const Graphics_Display g_ePAPER_200x200;
 void ePaper200x200_Init( uint8_t partial_full);
 void ePaper200x200_SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
 void ePaper200x200_SetOrientation(uint8_t orientaiton);

 void ePaper200x200_SetFrameMemory(const unsigned char* image_buffer, uint16_t x, uint16_t y, uint8_t image_width, uint8_t image_height);
void  ePaper200x200_SetMemoryArea(uint8_t x_start,  uint8_t y_start, uint8_t x_end, uint8_t y_end);
void ePaper200x200_SetMemoryPointer(uint8_t x, uint8_t y);

void ePaper200x200_ClearFrameMemory(uint8_t color);

void ePaper200x200_DisplayFrame2(void);

void ePaper200x200_Load_Image(uint8_t width, uint16_t height, uint8_t* image);


//TODO:
/*
 *  TODO:
 *   need the follwoing funciton defintions / prototypes
 *   sleep()
 *   waitUntilIdle <-- HAL
 *   ePaper200x200_SetFrame_ClearFrameMemory(unsigned char color)
 *   ePaper200x200_SetFrame_DisplayFrame(void)
 *
 *
 * */


#endif /* EPAPER200X200_DRIVER_H_ */
