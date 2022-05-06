/*
 * @Description: MQTT相关
 * @Autor: Pi
 * @Date: 2022-05-01 22:42:23
 * @LastEditTime: 2022-05-05 23:10:51
 */
#include "User_Mqtt.h"


WiFiClient Wifi_Client;
PubSubClient mqttClient(Wifi_Client);


Wifi_Config_Srt User_Wifi;
Mqtt_Str User_Mqtt;

#define DEBUG

#ifdef DEBUG
#define Debug(Str)  (Serial.print(Str)) 
#else
#define Debug(Str)  
#endif


/**
 * @brief 初始化WIFI
 * @param {*}
 * @return {*}
 */
bool User_Wifi_Init()
{
  
  User_Wifi.Wifi_SSID = (char *)"CP02";
  User_Wifi.Wifi_Pass = (char *)"6231380dd";
  //User_Wifi.Wifi_SSID = (char *)"TNY";
  //User_Wifi.Wifi_Pass = (char *)"23333333";

  User_Wifi.MAC = WiFi.macAddress();

  WiFi.mode(WIFI_STA);

  WiFi.begin(User_Wifi.Wifi_SSID, User_Wifi.Wifi_Pass);
  

  Debug("WIFI Start to Connect!\r\n");
  Debug("SSID:");
  Debug(User_Wifi.Wifi_SSID);
  Debug("\r\n");
  Debug("MAC:");
  Debug(User_Wifi.MAC);
  Debug("\r\n");

  

  /*等待Wifi连接成功*/
  
  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Debug(".");
  }
  
  Debug("\r\n");
  Debug("WIFI IP:");
  Debug(WiFi.localIP());
  Debug("\r\n");
  
  return 0;

/*
  User_Wifi.MAC = WiFi.macAddress();
*/
  /*设置连接超时*/
 /* wifiManager.setConnectTimeout(60 * 5);

  // 打印调试内容
  wifiManager.setDebugOutput(true);
*/
/*
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  if(wifiManager.autoConnect("ButtonLink") == 0)
  {
    Debug("WIFI connect Error!\r\nMCU reset!\r\n");
*/
  /*清除WIFI连接信息*/
 /*     wifiManager.resetSettings();

    ESP.reset();
    delay(1000);
  }
*/

  /*清除WIFI连接信息*/
  //wifiManager.resetSettings();

 // return 0;
}




/**
 * @brief 连接MQTT服务器
 * @param {*}
 * @return {bool}0:成功    1:失败
 */
bool User_Mqtt_Init()
{
  User_Mqtt.IP = (char *)"qiandpi.com";
  User_Mqtt.Port = 1883;
  User_Mqtt.User = (char *)"cp";
  User_Mqtt.Paws = (char *)"2333";
  User_Mqtt.Client_Id = "Button_Link/" + User_Wifi.MAC;
  User_Mqtt.Client_Id = User_Mqtt.Client_Id.c_str();
  User_Mqtt.QOS = 1;
  User_Mqtt.CleanSession = false;

  String Temp = "Button_Link/" + User_Wifi.MAC + "/Will";
  User_Mqtt.Will_Topic = (char *)Temp.c_str();
  User_Mqtt.Will_MSG = (char *)"Disconnect\r\n";
  User_Mqtt.Will_QOS = 1;
  User_Mqtt.Will_Retain = true;


  /*设置IP地址和端口号*/
  mqttClient.setServer(User_Mqtt.IP, User_Mqtt.Port);

 
  /*连接Mqtt服务器*/
  if( mqttClient.connect(User_Mqtt.Client_Id.c_str(), User_Mqtt.User , User_Mqtt.Paws , User_Mqtt.Will_Topic, User_Mqtt.Will_QOS, User_Mqtt.Will_Retain, User_Mqtt.Will_MSG, User_Mqtt.CleanSession) == 1)
  {
    /*发布上线消息*/
    uint8_t Online_MSG[] = "Connect\r\n";
    mqttClient.publish(User_Mqtt.Will_Topic, Online_MSG , sizeof(Online_MSG) , 1); 

    return 0;
  }
  else
  {
    return 1;
  }

   
 
}





/**
 * @brief MQTT 心跳发送 需要定时发送
 * @param {*}
 * @return {*}
 */
void User_Mqtt_Heartbeat(void)
{

  User_Wifi_Detection();


  /*断开连接则重新尝试连接*/
  if(mqttClient.connected() == 0)
  { 
    if(mqttClient.connect(User_Mqtt.Client_Id.c_str(), User_Mqtt.User , User_Mqtt.Paws , User_Mqtt.Will_Topic, User_Mqtt.Will_QOS, User_Mqtt.Will_Retain, User_Mqtt.Will_MSG, User_Mqtt.CleanSession) == 1 )
    {
      uint8_t Online_MSG[] = "Connect\r\n";
      mqttClient.publish(User_Mqtt.Will_Topic, Online_MSG , sizeof(Online_MSG) , 1); 
      Debug("MQTT is Connect!\r\n");
    }
    else
    {
      Debug("MQTT connect Error!\r\n");
      return;
    }

   
  }

  mqttClient.loop();

}



/**
 * @brief 检测WIFI是否断开，断开则重启
 * @param {*}
 * @return {*}
 */
void User_Wifi_Detection(void)
{
 
  if(WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  Debug("Wifi is Disconnect!\r\n");
  Debug("MCU Restart!\r\n");

  /*复位*/
  //ESP.reset();

}




/**
 * @brief 发布MQTT消息
 * @param {*}
 * @return {*}
 */
void User_Mqtt_Publish(uint8_t *pData , uint8_t Len)
{
  /*主题名字*/
  String topicString = "Button_Link/" + WiFi.macAddress() + "/MSG";
  char publishTopic[topicString.length() + 1];  
  strcpy(publishTopic, topicString.c_str());

  /*发布消息 retained True*/
  mqttClient.publish(publishTopic, pData , Len , 1); 

}



