/*
 * @Description: MQTT相关
 * @Autor: Pi
 * @Date: 2022-05-01 22:42:33
 * @LastEditTime: 2022-05-05 23:04:36
 */
#ifndef USER_MQTT_H
#define USER_MQTT_H

#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"


typedef struct 
{
  char *IP;
  uint16_t Port;
  char *User;
  char *Paws;
  String Client_Id;
  uint8_t QOS;
  bool CleanSession;

  char *Will_Topic;
  char *Will_MSG;
  uint8_t Will_QOS;
  bool Will_Retain;

}Mqtt_Str;



typedef struct 
{
  char *Wifi_SSID;
  char *Wifi_Pass;
  String MAC;
  //char *
}Wifi_Config_Srt;


extern Wifi_Config_Srt User_Wifi;
extern Mqtt_Str User_Mqtt;



bool User_Wifi_Init();
bool User_Mqtt_Init();


void User_Mqtt_Heartbeat(void);
void User_Wifi_Detection(void);
void User_Mqtt_Publish(uint8_t *pData , uint8_t Len);

#endif