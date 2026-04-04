#ifndef __TFT_H
#define __TFT_H

#include "stm32f4xx.h"
#include <stdarg.h>
#include "delay.h"

#define TFT_CS_PORT      GPIOA
#define TFT_CS_PIN       GPIO_Pin_4
#define TFT_DC_PORT      GPIOA
#define TFT_DC_PIN       GPIO_Pin_3
#define TFT_RST_PORT     GPIOA
#define TFT_RST_PIN      GPIO_Pin_2
#define TFT_BL_PORT      GPIOA
#define TFT_BL_PIN       GPIO_Pin_1

#define TFT_CS_HIGH()    GPIO_SetBits(TFT_CS_PORT, TFT_CS_PIN)
#define TFT_CS_LOW()     GPIO_ResetBits(TFT_CS_PORT, TFT_CS_PIN)
#define TFT_DC_HIGH()    GPIO_SetBits(TFT_DC_PORT, TFT_DC_PIN)
#define TFT_DC_LOW()     GPIO_ResetBits(TFT_DC_PORT, TFT_DC_PIN)
#define TFT_RST_HIGH()   GPIO_SetBits(TFT_RST_PORT, TFT_RST_PIN)
#define TFT_RST_LOW()    GPIO_ResetBits(TFT_RST_PORT, TFT_RST_PIN)
#define TFT_BL_ON()      GPIO_SetBits(TFT_BL_PORT, TFT_BL_PIN)

#define TFTcmd_RGB_SELECT 0x3A
#define TFTcmd_MADCTL 0x36
#define TFTcmd_INV_OFF 0x21
#define TFTcmd_INV_ON 0x20
#define TFTcmd_DISP_ON 0x29
#define TFTcmd_SLEEP_OUT 0x11
#define TFTcmd_SLEEP_IN 0x10

void TFT_Init(void);

void TFT_SendByte(uint8_t Data);
void TFT_SendColor(uint16_t Data);
void TFT_Command(uint8_t Cmd,uint8_t num_params,...);
void TFT_SetBlock(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);

void TFT_FillBlock(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1,uint16_t color);
void TFT_ClearScreen(uint16_t color);
void TFT_DrawChar_16x16(uint16_t x, uint16_t y,const uint8_t *Char16x16, uint16_t fg_color, uint16_t bg_color);

#endif

