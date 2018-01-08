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

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   200
#define LCD_HORIZONTAL_MAX                 200

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

#define CMD_X_ADDR_START 0X44
#define CMD_Y_ADDR_START 0X45
#define CMD_X_COUNTER 0X4E
#define CMD_Y_COUNTER 0X4F
#define CMD_NOP 0XFF //GOOD FOR ENDING A COMMAND    /   DATA

//provided by vendor
 const unsigned char lut_full_update[] = {
                                          0x02, //C221 25C Full update waveform
                                          0x02,
                                          0x01,
                                          0x11,
                                          0x12,
                                          0x12,
                                          0x22,
                                          0x22,
                                          0x66,
                                          0x69,
                                          0x69,
                                          0x59,
                                          0x58,
                                          0x99,
                                          0x99,
                                          0x88,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0xF8,
                                          0xB4,
                                          0x13,
                                          0x51,
                                          0x35,
                                          0x51,
                                          0x51,
                                          0x19,
                                          0x01,
                                          0x00
 };
 //provided by vendor
 const unsigned char lut_partial_update[] ={
                                            0x10, //C221 25C partial update waveform
                                            0x18,
                                            0x18,
                                            0x08,
                                            0x18,
                                            0x18,
                                            0x08,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x13,
                                            0x14,
                                            0x44,
                                            0x12,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00,
                                            0x00
 };

 uint8_t Lcd_Orientation;
 uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;

extern const Graphics_Display g_ePAPER_200x200;
 void ePaper200x200_Init(uint8_t partial_full);
 void ePaper200x200_SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
 void ePaper200x200_SetOrientation(uint8_t orientaiton);

 void ePaper200x200_SetFrameMemory(const unsigned char* image_buffer, uint16_t x, uint16_t y, uint8_t image_width, uint8_t image_height);
void  ePaper200x200_SetMemoryArea(uint8_t x_start,  uint8_t y_start, uint8_t x_end, uint8_t y_end);
void ePaper200x200_SetMemoryPointer(uint8_t x, uint8_t y);


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
