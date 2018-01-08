/*
 * ePaper200x200_driver.c
 *
 *  Created on: Jan 7, 2018
 *      Author: Kevin Kuwata
 *		
 *		Summary: This is the parallel of Crystalfontz128x128_ST7735.c file but for the ePaper.
 *
 *
 */

#include "grlib.h"
#include "ePaper200x200_driver.h"
#include <driverlib.h>
#include "HAL_MSP_EXP432P401R_ePaper200x200.h"
#include <stdint.h>


uint8_t Lcd_Orientation;
uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;

extern uint8_t  Partial_LUT[30];
extern uint8_t  Full_LUT[30];

//*****************************************************************************
/*
 *  @name: ePaper200x200_Init
 *  @params: none
 *  @return: none
 *  @brief: this will initialize the display to use eUsci_B0.
 *
 *  for watch be sure to change the initialization.
 *  Modifies the GPIO and configures SPI module.
 * */
//*****************************************************************************
 void ePaper200x200_Init(uint8_t partial_full){
     HAL_LCD_PortInit();
     HAL_LCD_SpiInit();

     Lcd_ScreenWidth  = LCD_VERTICAL_MAX;
     Lcd_ScreenHeigth = LCD_HORIZONTAL_MAX;

     // Reset the controller, so it can be configured
     GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN);
     HAL_LCD_delay(50);
     GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);
     HAL_LCD_delay(120);

     HAL_LCD_writeCommand(CMD_DRIVER_OUTPUT_CONTROL);
     HAL_LCD_writeData((Lcd_ScreenWidth - 1 ) & 0xFF);
     HAL_LCD_writeData((Lcd_ScreenHeigth - 1 ) & 0xFF);
     HAL_LCD_writeData(0x00);

     HAL_LCD_writeCommand(CMD_BOOSTER_SOFT_START_CONTROL);
     HAL_LCD_writeData(0xD7);
     HAL_LCD_writeData(0xD6);
     HAL_LCD_writeData(0x9D);

     HAL_LCD_writeCommand(CMD_VCOM);
     HAL_LCD_writeData(0xA8);

     HAL_LCD_writeCommand(CMD_DUMMY_LINE);
     HAL_LCD_writeData(0x1A);

     HAL_LCD_writeCommand(CMD_GATE_TIME);
     HAL_LCD_writeData(0x08);

     HAL_LCD_writeCommand(CMD_DATA_ENTRY);
     HAL_LCD_writeData(0x03);

     //send the Look Up Table (LUT) depending on if partial or full
     HAL_LCD_writeCommand(CMD_WRITE_LUT);
     uint8_t index;
     if(partial_full == 0) {
         //this is partial
         for(index = 0; index <30; index++){
             HAL_LCD_writeData(lut_partial_update[index]);
         }
     }
     else if(partial_full == 1){
         //this is full
         for(index = 0; index <30; index++){
             HAL_LCD_writeData(lut_full_update[index]);
         }
     }
     else{
         //default is partial
         for(index = 0; index <30; index++){
             HAL_LCD_writeData(lut_partial_update[index]);
         }
     }
     //end of screen init
 }

 /*
  *  @name: ePaper200x200_SetFrameMemory
  *  @brief: takes an image and puts into display, does not update display
  *
  * */
 void ePaper200x200_SetFrameMemory(const unsigned char* image_buffer, uint16_t x, uint16_t y, uint8_t image_width, uint8_t image_height){
     uint8_t x_end;
     uint8_t y_end;

     x &= 0xF8;
     image_width &= 0xF8;
     if(x + image_width>= Lcd_ScreenWidth ){
         x_end = Lcd_ScreenWidth - 1;
     } else{
         x_end = x + image_width - 1;
     }

     if(y+image_height >= Lcd_ScreenHeigth){
         y_end = Lcd_ScreenHeigth -1;
     } else{
         y_end = y + image_height - 1;
     }

     ePaper200x200_SetMemoryArea(x, y, x_end, y_end);
     ePaper200x200_SetMemoryPointer(x,y);
     HAL_LCD_writeCommand(CMD_WRITE_RAM);

     //send image data from buffer
     uint8_t j;
     uint8_t i;
     for(j = 0; j < y_end - y +1; j++){
         for(i = 0; i < (x_end - x + 1) / 8 ; i++){
             HAL_LCD_writeData(image_buffer[i + j * (image_width / 8 )]);
         }
     }

 }

 /*
  * @brief: specify the memory area for the data to be written
  * @return: none
  */
 void  ePaper200x200_SetMemoryArea(uint8_t x_start,  uint8_t y_start, uint8_t x_end, uint8_t y_end){
     HAL_LCD_writeCommand(CMD_X_ADDR_START);
     HAL_LCD_writeData((x_start >> 3) & 0xFF);
     HAL_LCD_writeData((x_end >> 3) & 0xFF);

     HAL_LCD_writeCommand(CMD_Y_ADDR_START);
     HAL_LCD_writeData(y_start  & 0xFF);
     HAL_LCD_writeData((y_start >> 8) & 0xFF);
     HAL_LCD_writeData(y_end & 0xFF);
     HAL_LCD_writeData((y_end >> 8) & 0xFF);
 }

 /* @brief: start pont for the data to be written x and y ,probably call this each time you do a
  * partial update
  * @return: none
  *
  * */
void ePaper200x200_SetMemoryPointer(uint8_t x, uint8_t y){
    HAL_LCD_writeCommand(CMD_X_COUNTER);
    HAL_LCD_writeData((x >> 3) & 0xFF);

    HAL_LCD_writeCommand(CMD_Y_COUNTER);
    HAL_LCD_writeData(y & 0xFF);
    HAL_LCD_writeData((y >> 8) & 0xFF);
 //   while(LCD_BUSY_PIN  == 1) ; //wait until busy line goes low, aka not busy
   while(P3IN == BIT6); //while pin 3.6 is high wait ehre. TODO
    //TODO: come back and change this to allow for HAL

}
