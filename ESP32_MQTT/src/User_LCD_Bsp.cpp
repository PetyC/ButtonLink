#include "User_LCD_Bsp.h"

/*内部调用函数*/
static void LCD_GPIO_Init(void);


/**
 * @brief 写入1字节数据
 * @param {uint8_t} dat
 * @return {*}
 */
void LCD_Writ_Bus(uint8_t dat)
{
  uint8_t i;
  LCD_CS_Clr();
  for (i = 0; i < 8; i++)
  {
    LCD_SCLK_Clr();
    if (dat & 0x80)
    {
      LCD_MOSI_Set();
    }
    else
    {
      LCD_MOSI_Clr();
    }
    LCD_SCLK_Set();
    dat <<= 1;
  }
  LCD_CS_Set();
}


/**
 * @brief 写入1字节
 * @param {uint8_t} dat
 * @return {*}
 */
void LCD_WR_DATA8(uint8_t dat)
{
  LCD_Writ_Bus(dat);
}


/**
 * @brief 写入2个字节
 * @param {uint16_t} dat
 * @return {*}
 */
void LCD_WR_DATA(uint16_t dat)
{
  LCD_Writ_Bus(dat >> 8);
  LCD_Writ_Bus(dat);
}


/**
 * @brief 写入1字节命令
 * @param {uint8_t} dat
 * @return {*}
 */
void LCD_WR_REG(uint8_t dat)
{
  LCD_DC_Clr();
  LCD_Writ_Bus(dat);
  LCD_DC_Set();
}


/**
 * @brief 设置起始地址
 * @param {uint16_t} x1
 * @param {uint16_t} y1
 * @param {uint16_t} x2
 * @param {uint16_t} y2
 * @return {*}
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  if (USE_HORIZONTAL == 0)
  {
    LCD_WR_REG(0x2a);
    LCD_WR_DATA(x1 + 2);
    LCD_WR_DATA(x2 + 2);
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(y1 + 1);
    LCD_WR_DATA(y2 + 1);
    LCD_WR_REG(0x2c);
  }
  else if (USE_HORIZONTAL == 1)
  {
    LCD_WR_REG(0x2a);
    LCD_WR_DATA(x1 + 2);
    LCD_WR_DATA(x2 + 2);
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(y1 + 1);
    LCD_WR_DATA(y2 + 1);
    LCD_WR_REG(0x2c);
  }
  else if (USE_HORIZONTAL == 2)
  {
    LCD_WR_REG(0x2a);
    LCD_WR_DATA(x1 + 1);
    LCD_WR_DATA(x2 + 1);
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(y1 + 2);
    LCD_WR_DATA(y2 + 2);
    LCD_WR_REG(0x2c);
  }
  else
  {
    LCD_WR_REG(0x2a);
    LCD_WR_DATA(x1 + 1);
    LCD_WR_DATA(x2 + 1);
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(y1 + 2);
    LCD_WR_DATA(y2 + 2);
    LCD_WR_REG(0x2c);
  }
}


/**
 * @brief GPIO初始化
 * @param {*}
 * @return {*}
 */
static void LCD_GPIO_Init()
{
  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDA_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(RES_PIN, OUTPUT);
  pinMode(BL_PIN, OUTPUT);
}



/**
 * @brief LCD初始化
 * @param {*}
 * @return {*}
 */
void User_LCD_Init(void)
{
  LCD_GPIO_Init();

  LCD_RES_Clr();
  delay(100);
  LCD_RES_Set();
  delay(100);

  LCD_BLK_Set();
  delay(100);

  //************* Start Initial Sequence **********//
  LCD_WR_REG(0x11); // Sleep out
  delay(120);       // Delay 120ms
  //------------------------------------ST7735S Frame Rate-----------------------------------------//
  LCD_WR_REG(0xB1);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3C);
  LCD_WR_DATA8(0x3C);
  LCD_WR_REG(0xB2);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3C);
  LCD_WR_DATA8(0x3C);
  LCD_WR_REG(0xB3);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3C);
  LCD_WR_DATA8(0x3C);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3C);
  LCD_WR_DATA8(0x3C);
  //------------------------------------End ST7735S Frame Rate---------------------------------//
  LCD_WR_REG(0xB4); // Dot inversion
  LCD_WR_DATA8(0x03);
  //------------------------------------ST7735S Power Sequence---------------------------------//
  LCD_WR_REG(0xC0);
  LCD_WR_DATA8(0x28);
  LCD_WR_DATA8(0x08);
  LCD_WR_DATA8(0x04);
  LCD_WR_REG(0xC1);
  LCD_WR_DATA8(0XC0);
  LCD_WR_REG(0xC2);
  LCD_WR_DATA8(0x0D);
  LCD_WR_DATA8(0x00);
  LCD_WR_REG(0xC3);
  LCD_WR_DATA8(0x8D);
  LCD_WR_DATA8(0x2A);
  LCD_WR_REG(0xC4);
  LCD_WR_DATA8(0x8D);
  LCD_WR_DATA8(0xEE);
  //---------------------------------End ST7735S Power Sequence-------------------------------------//
  LCD_WR_REG(0xC5); // VCOM
  LCD_WR_DATA8(0x1A);
  LCD_WR_REG(0x36); // MX, MY, RGB mode
  if (USE_HORIZONTAL == 0)
    LCD_WR_DATA8(0x00);
  else if (USE_HORIZONTAL == 1)
    LCD_WR_DATA8(0xC0);
  else if (USE_HORIZONTAL == 2)
    LCD_WR_DATA8(0x70);
  else
    LCD_WR_DATA8(0xA0);
  //------------------------------------ST7735S Gamma Sequence---------------------------------//
  LCD_WR_REG(0xE0);
  LCD_WR_DATA8(0x04);
  LCD_WR_DATA8(0x22);
  LCD_WR_DATA8(0x07);
  LCD_WR_DATA8(0x0A);
  LCD_WR_DATA8(0x2E);
  LCD_WR_DATA8(0x30);
  LCD_WR_DATA8(0x25);
  LCD_WR_DATA8(0x2A);
  LCD_WR_DATA8(0x28);
  LCD_WR_DATA8(0x26);
  LCD_WR_DATA8(0x2E);
  LCD_WR_DATA8(0x3A);
  LCD_WR_DATA8(0x00);
  LCD_WR_DATA8(0x01);
  LCD_WR_DATA8(0x03);
  LCD_WR_DATA8(0x13);
  LCD_WR_REG(0xE1);
  LCD_WR_DATA8(0x04);
  LCD_WR_DATA8(0x16);
  LCD_WR_DATA8(0x06);
  LCD_WR_DATA8(0x0D);
  LCD_WR_DATA8(0x2D);
  LCD_WR_DATA8(0x26);
  LCD_WR_DATA8(0x23);
  LCD_WR_DATA8(0x27);
  LCD_WR_DATA8(0x27);
  LCD_WR_DATA8(0x25);
  LCD_WR_DATA8(0x2D);
  LCD_WR_DATA8(0x3B);
  LCD_WR_DATA8(0x00);
  LCD_WR_DATA8(0x01);
  LCD_WR_DATA8(0x04);
  LCD_WR_DATA8(0x13);
  //------------------------------------End ST7735S Gamma Sequence-----------------------------//
  LCD_WR_REG(0x3A); // 65k mode
  LCD_WR_DATA8(0x05);
  LCD_WR_REG(0x29); // Display on
}
