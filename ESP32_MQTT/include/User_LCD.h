#ifndef USUER_LCD_
#define USUER_LCD_

#include <Arduino.h>
#include "stdlib.h"
#include "User_LCD_Bsp.h"





void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);                                          //画方块
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);                                                                       //画点
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t MOD);                     //单个字符
void LCD_ShowString(uint16_t x, uint16_t y, char *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t MOD);                     //字符串
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);                 //整数
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);                // 两位小数


#define WHITE 0xFFFF
#define BLACK 0x0000
#define RED   0X00F8


#endif
