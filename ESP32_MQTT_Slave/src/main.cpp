/*
 * @Description: MQTT
 * @Autor: Pi
 * @Date: 2022-05-01 22:42:03
 * @LastEditTime: 2022-05-05 23:03:01
 */
#include <Arduino.h>
#include <Ticker.h>
#include "User_Mqtt.h"

#define BUTTON_PIN 16
Ticker Tick;


void User_Tick_Task(void);
void User_Button_Detection(void);


void setup() {

  Serial.begin(115200);


  Serial.println();
  Serial.println();
  Serial.println();


  /*初始化按钮*/
  pinMode(BUTTON_PIN , INPUT_PULLUP);


  if( User_Wifi_Init() == 0)
  {
    Serial.println("Wifi connect is OK!");
  }



  if(User_Mqtt_Init() == 0)
  {
    Serial.println("Mqtt connect is OK!");
  }
  else
  {
    Serial.println("Mqtt connect is Error!");
  }


  /*启动定时任务*/
  Tick.attach_ms(50 , User_Tick_Task);

}



void loop() 
{


   
}





/**
 * @brief 用户定时函数
 * @param {*}
 * @return {*}
 */
void User_Tick_Task(void)
{

  User_Mqtt_Heartbeat();


  User_Button_Detection();


}





/**
 * @brief 按钮检测
 * @param {*}
 * @return {bool}
 */
void User_Button_Detection(void)
{
  static bool Button_Last = 1;
  bool Button = 1;

  Button = digitalRead(BUTTON_PIN);

  /*状态不同*/
  if(Button != Button_Last)
  {
    Button_Last = Button;

    /*被按下*/
    if(Button == 0)
    {
      uint8_t Mqtt_MSG[] = "Button:Close\r\n";

      User_Mqtt_Publish(Mqtt_MSG , sizeof(Mqtt_MSG));

      Serial.println("Button is Close!");
    }
    else
    {
      uint8_t Mqtt_MSG[] = "Button:Open\r\n";

      User_Mqtt_Publish(Mqtt_MSG , sizeof(Mqtt_MSG));

      Serial.println("Button is Open!");
    }
  }

  
}