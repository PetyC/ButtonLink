#include <Arduino.h>
#include <Ticker.h>
#include "User_Mqtt.h"
#include "User_LCD.h"


Ticker Tick;

#define SYS_LED_Pin 12
#define WIFI_LED_Pin 13
#define BEEP_PIN  1

uint32_t SYS_Tick_Count = 0;
void SYS_LED_Blink(void);
void Beep_DisEnable(void);
void User_Tick_Task(void);


void setup()
{
  pinMode(SYS_LED_Pin , OUTPUT);
  pinMode(WIFI_LED_Pin , OUTPUT);
  pinMode(BEEP_PIN , OUTPUT);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();


  User_LCD_Init();
  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  //LCD_ShowString(24 ,55 ,"Button_Link",BLACK ,WHITE, 16, 0);      
  LCD_ShowString(0 , 20 ,"Link:",BLACK ,WHITE, 16, 0);        



/*
  
  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  LCD_DrawPoint(10,10,BLACK);                 //画点
  LCD_DrawPoint(10,11,BLACK);
  LCD_DrawPoint(11,10,BLACK);
  LCD_DrawPoint(11,11,BLACK);
  LCD_ShowChar(15,15,'1',BLACK,WHITE,16,0);  //显示单个字符  X位置  Y位置  ‘要显示的字符’  字体颜色  背景颜色  字号  显示模式 0不重叠 1重叠
  LCD_ShowString(24,60,"lcd-1",BLACK ,WHITE,16,0); //显示字符串           “要显示的字符串”
  LCD_ShowString(24,80,"-i",BLACK ,WHITE,16,0); //显示字符串               
  LCD_ShowIntNum(72,60,67,3,BLACK,WHITE,16);  //显示整数变量                i=显示整数
  LCD_ShowFloatNum1(60,17,00.00,4,BLACK,WHITE,16); //显示两位小数
*/

  if( User_Wifi_Init() == 0)
  {
    Serial.println("Wifi connect is OK!");
    digitalWrite(WIFI_LED_Pin , 1);
  }

  User_Mqtt_Init();


  /*启动定时任务*/
  Tick.attach_ms(100 , User_Tick_Task);
}

void loop()
{
 static bool Link_Last = 0;
 static bool Button_Last = 0;

  if(Link_Last != Link_Status)
  {
    Link_Last = Link_Status;

    if(Link_Status == 0)
    {
      LCD_ShowString(45 ,20 ,"Connect",BLACK ,WHITE, 16, 0); 
    }
    else
    {
      LCD_ShowString(45 ,20 ,"Disconnect",BLACK ,WHITE, 16, 0); 
    }

  }

  if(Button_Last != Button_Status)
  {
    Button_Last = Button_Status;
    if(Button_Status == 0)
    {    
      LCD_ShowString(50 , 60 ,"Close" , RED , WHITE , 16, 0); 
    }
    else
    {  
      LCD_ShowString(50 , 60 ,"Open" , RED , WHITE , 16, 0); 
    }
  }


}


/**
 * @brief 用户定时函数
 * @param {*}
 * @return {*}
 */
void User_Tick_Task(void)
{
  User_Mqtt_Heartbeat();


  SYS_Tick_Count++;

  if(SYS_Tick_Count >= 8)
  {
    SYS_Tick_Count = 0;

    SYS_LED_Blink();

    Beep_DisEnable();

  }
}



/**
 * @brief 系统运行指示灯
 * @param {*}
 * @return {*}
 */
void SYS_LED_Blink(void)
{
  static bool Flag = 0;

  digitalWrite(SYS_LED_Pin , Flag);

  Flag = !Flag;

}



/**
 * @brief 蜂鸣器是能
 * @param {bool} DisEnable
 * @return {*}
 */
void Beep_DisEnable(void)
{

  static bool Flag = 0;

  if(Button_Status == 0 && Link_Status == 0)
  {
    digitalWrite(BEEP_PIN , Flag);
    Flag = !Flag;
  }
  else
  {
   digitalWrite(BEEP_PIN , 0);
  }

}