/*
 * HAL_MSP_EXP432P401R_ePaper200x200.h
 *
 *  Created on: Jan 7, 2018
 *      Author: Kevin Kuwata
 *		Summary: This file configures the SPI module B0 for use with the ePaper display.
 *		It also identifies which gpio pins will be used for the CS, Reset, D/C pins .
 *
 */

#ifndef HAL_MSP_EXP432P401R_EPAPER200X200_H_
#define HAL_MSP_EXP432P401R_EPAPER200X200_H_

#include <stdint.h>
#include "driverlib.h"
#include "msp.h"

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// System clock speed (in Hz)
#define LCD_SYSTEM_CLOCK_SPEED                 48000000
// SPI clock speed (in Hz)
#define LCD_SPI_CLOCK_SPEED                    16000000

// Ports from MSP432 connected to LCD
#define LCD_SCK_PORT          GPIO_PORT_P1
#define LCD_SCK_PIN_FUNCTION  GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_MOSI_PORT         GPIO_PORT_P1
#define LCD_MOSI_PIN_FUNCTION GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_RST_PORT          GPIO_PORT_P5
#define LCD_CS_PORT           GPIO_PORT_P5
#define LCD_DC_PORT           GPIO_PORT_P3
#define LCD_BUSY_PORT       GPIO_PORT_P3

// Pins from MSP432 connected to LCD, all available on the launchpad breakout headers.
#define LCD_SCK_PIN           GPIO_PIN5 //valid 1.5
#define LCD_MOSI_PIN          GPIO_PIN6 //valid 1.6
#define LCD_RST_PIN           GPIO_PIN4 // 5.4
#define LCD_CS_PIN            GPIO_PIN5 //5.5
#define LCD_DC_PIN            GPIO_PIN7//3.7
#define LCD_BUSY_PIN        GPIO_PIN6 //3.6

// Definition of USCI base address to be used for SPI communication
#define LCD_EUSCI_BASE        EUSCI_B0_BASE //accessible from the launchpad header. good choice.
//*****************************************************************************
// Look up tables: LUT partial and full


//*****************************************************************************



//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void HAL_LCD_writeCommand(uint8_t command);
extern void HAL_LCD_writeData(uint8_t data);
extern void HAL_LCD_PortInit(void);
extern void HAL_LCD_SpiInit(void);

// Custom __delay_cycles() for non CCS Compiler
#if !defined(ccs)
#undef __delay_cycles
#define __delay_cycles(x)     SysCtlDelay(x)
void SysCtlDelay(uint32_t);
#endif

#define HAL_LCD_delay(x)      __delay_cycles(x * 48)


#endif /* HAL_MSP_EXP432P401R_EPAPER200X200_H_ */
