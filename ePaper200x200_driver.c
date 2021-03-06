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
#include "driverlib.h"
#include "HAL_MSP_EXP432P401R_ePaper200x200.h"
#include <stdint.h>
#include "msp.h"


uint8_t Lcd_Orientation;
uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;

//provided by vendor
 const uint8_t lut_full_update[] = {
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
 const uint8_t lut_partial_update[] ={
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
 void ePaper200x200_Init( uint8_t partial_full){
     HAL_LCD_PortInit();
     HAL_LCD_SpiInit();

     Lcd_ScreenWidth  = LCD_VERTICAL_MAX;
     Lcd_ScreenHeigth = LCD_HORIZONTAL_MAX;

     // Reset the controller, so it can be configured
     GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN);
     HAL_LCD_delay(50);
     GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);
     HAL_LCD_delay(120);

     //:TODO check that this is a valid command what about
     //CS?! because we only have 1 display the chip select is always set low. aka always enabled.
     // the ti library does the same thing.
     // so the way i got the SPI to decode on the logica anlyzer was probably using the D/C

     GPIO_setOutputLowOnPin(LCD_CS_PORT, LCD_CS_PIN);

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

     GPIO_setOutputHighOnPin(LCD_CS_PORT, LCD_CS_PIN);

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

/*
 *  @name: epaper200x200_ClearFrameMemory
 *  @brief: changes the frame memoery to be all black or all white remember its only 0x00 or 0xFF.
      @parameter: input a color 0x00 or 0xFF; TODO come back and define which is which, black white.
     @return: none
 */
void ePaper200x200_ClearFrameMemory(uint8_t color){
    ePaper200x200_SetMemoryArea(0 , 0, LCD_VERTICAL_MAX - 1, LCD_HORIZONTAL_MAX - 1);
    ePaper200x200_SetMemoryPointer(0,0);
    HAL_LCD_writeCommand(CMD_WRITE_RAM);
    //send in color: white or black
    uint8_t pixel;
//   for(pixel = 0; pixel < (LCD_HORIZONTAL_MAX/8) * LCD_VERTICAL_MAX; pixel++){
    for(pixel = 0; pixel < 1; pixel++){
    HAL_LCD_writeData(color);
    }
}

/*
 *  @name: ePaper200x200_DisplayFrame2
 *  @brief: this will issue display command   2 (0x22) but will only use the init display, init clock (C0)
 *  and then do To display pattern (0xC4)
 *  @summary: this will show and update the display.
 *  QUESTION: is this a partial update or full or only depends on the LUT table.
 * */
void ePaper200x200_DisplayFrame2(void){
    HAL_LCD_writeCommand(CMD_DISPLAY_UPDATE_CTRL2);
    HAL_LCD_writeData(CMD_DISP_UPDATE_SEQ1);
    HAL_LCD_writeCommand(CMD_MASTER_ACTV);
    while(P3IN == BIT6);
    HAL_LCD_writeCommand(CMD_NOP);
    //TODO: fix thiS TO HAL
while(P3IN  == BIT6); //wait until not busy.
}

void ePaper200x200_Load_Image(uint8_t width, uint16_t height, uint8_t* image){
uint8_t width_size = (width + 7) >> 3;
uint16_t height_size = 0;
uint16_t index = 0;
while(P3IN == BIT6);

HAL_LCD_writeCommand(CMD_WRITE_RAM);

for(height_size = 0; height_size < height; height_size++ ){
    for(width_size = 0; width_size < width; width_size++){
        HAL_LCD_writeData(image);
    }
}


}

