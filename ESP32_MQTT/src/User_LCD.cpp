#include "User_LCD.h"
#include "font.h"




/**
 * @brief 颜色填充区域
 * @param {uint16_t} xsta
 * @param {uint16_t} ysta
 * @param {uint16_t} xend
 * @param {uint16_t} yend
 * @param {uint16_t} color
 * @return {*}
 */
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
  uint16_t i, j;
  LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
  for (i = ysta; i < yend; i++)
  {
    for (j = xsta; j < xend; j++)
    {
      LCD_WR_DATA(color);
    }
  }
}


/**
 * @brief 画点
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} color
 * @return {*}
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
  LCD_Address_Set(x, y, x, y); 
  LCD_WR_DATA(color);
}




/**
 * @brief 显示单个字母
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint8_t} num
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint8_t} sizey
 * @param {uint8_t} MOD
 * @return {*}
 */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t MOD)
{
  uint8_t temp, sizex, t;
  uint16_t i, TypefaceNum;
  uint16_t x0 = x;
  sizex = sizey / 2;
  TypefaceNum = sizex / 8 * sizey;
  num = num - ' ';
  LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1);
  for (i = 0; i < TypefaceNum; i++)
  {
    if (sizey == 16)
      temp = ascii_1608[num][i];
    else if (sizey == 32)
      temp = ascii_3216[num][i];
    else
      return;
    for (t = 0; t < 8; t++)
    {
      if (!MOD)
      {
        if (temp & (0x01 << t))
          LCD_WR_DATA(fc);
        else
          LCD_WR_DATA(bc);
      }
      else
      {
        if (temp & (0x01 << t))
          LCD_DrawPoint(x, y, fc);
        x++;
        if ((x - x0) == sizex)
        {
          x = x0;
          y++;
          break;
        }
      }
    }
  }
}



/**
 * @brief 显示字符串
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {char} *p
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint8_t} sizey
 * @param {uint8_t} MOD
 * @return {*}
 */
void LCD_ShowString(uint16_t x, uint16_t y, char *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t MOD)
{
  while (*p != '\0')
  {
    LCD_ShowChar(x, y, *p, fc, bc, sizey, MOD);
    x += sizey / 2;
    p++;
  }
}



/**
 * @brief 显示数字
 * @param {uint8_t} m
 * @param {uint8_t} n
 * @return {*}
 */
uint32_t mypow(uint8_t m, uint8_t n)
{
  uint32_t result = 1;
  while (n--)
    result *= m;
  return result;
}




/**
 * @brief 显示整数变量
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} num
 * @param {uint8_t} len
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint8_t} sizey
 * @return {*}
 */
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
  uint8_t t, temp;
  uint8_t enshow = 0;
  uint8_t sizex = sizey / 2;
  for (t = 0; t < len; t++)
  {
    temp = (num / mypow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1))
    {
      if (temp == 0)
      {
        LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
        continue;
      }
      else
        enshow = 1;
    }
    LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}



/**
 * @brief 显示两位小数
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {float} num
 * @param {uint8_t} len
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint8_t} sizey
 * @return {*}
 */
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
  uint8_t t, temp, sizex;
  uint16_t num1;
  sizex = sizey / 2;
  num1 = num * 100;
  for (t = 0; t < len; t++)
  {
    temp = (num1 / mypow(10, len - t - 1)) % 10;
    if (t == (len - 2))
    {
      LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
      t++;
      len += 1;
    }
    LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}
