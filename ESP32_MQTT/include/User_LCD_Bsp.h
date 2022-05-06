#ifndef USER_LCD_BSP_H
#define USER_LCD_BSP_H


#include <Arduino.h>
#include "stdlib.h"


#define SCL_PIN 2
#define SDA_PIN 3
#define RES_PIN 10
#define DC_PIN 6
#define CS_PIN 7
#define BL_PIN 11


#define LCD_SCLK_Clr() digitalWrite(SCL_PIN, LOW); // SCL=SCLK
#define LCD_SCLK_Set() digitalWrite(SCL_PIN, HIGH);

#define LCD_MOSI_Clr() digitalWrite(SDA_PIN, LOW); // SDA=MOSI
#define LCD_MOSI_Set() digitalWrite(SDA_PIN, HIGH);

#define LCD_RES_Clr() digitalWrite(RES_PIN, LOW); // RES
#define LCD_RES_Set() digitalWrite(RES_PIN, HIGH);

#define LCD_DC_Clr() digitalWrite(DC_PIN, LOW); // DC
#define LCD_DC_Set() digitalWrite(DC_PIN, HIGH);

#define LCD_CS_Clr() digitalWrite(CS_PIN, LOW); // CS
#define LCD_CS_Set() digitalWrite(CS_PIN, HIGH);

#define LCD_BLK_Clr() digitalWrite(BL_PIN, LOW); // BLK
#define LCD_BLK_Set() digitalWrite(BL_PIN, HIGH);


#define USE_HORIZONTAL 3 //显示模式  1 竖屏  2好像是横屏旋转90

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 128
#define LCD_H 160
#else
#define LCD_W 160
#define LCD_H 128
#endif


void User_LCD_Init(void);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);



#endif
