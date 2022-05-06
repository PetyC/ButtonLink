/*
 * @Description: MQTT相关
 * @Autor: Pi
 * @Date: 2022-05-01 22:42:23
 * @LastEditTime: 2022-05-05 23:31:47
 */
#include "User_Mqtt.h"

WiFiClient Wifi_Client;
PubSubClient mqttClient(Wifi_Client);

Wifi_Config_Srt User_Wifi;
Mqtt_Str User_Mqtt;

#define DEBUG

#ifdef DEBUG
#define Debug(Str) (Serial.print(Str))
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
  // User_Wifi.Wifi_SSID = (char *)"TNY";
  // User_Wifi.Wifi_Pass = (char *)"23333333";

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

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Debug(".");
  }

  Debug("\r\n");
  Debug("WIFI IP:");
  Debug(WiFi.localIP());
  Debug("\r\n");

  return 0;
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
  User_Mqtt.Client_Id = "Button_Link_Master/" + User_Wifi.MAC;
  User_Mqtt.Client_Id = User_Mqtt.Client_Id.c_str();
  User_Mqtt.QOS = 1;
  User_Mqtt.CleanSession = false;

  String Temp = "Button_Link_Master/" + User_Wifi.MAC + "/Will";
  User_Mqtt.Will_Topic = (char *)Temp.c_str();
  User_Mqtt.Will_MSG = (char *)"Disconnect\r\n";
  User_Mqtt.Will_QOS = 1;
  User_Mqtt.Will_Retain = true;

  /*设置IP地址和端口号*/
  mqttClient.setServer(User_Mqtt.IP, User_Mqtt.Port);

  /*设置MQTT订阅回调函数*/
  mqttClient.setCallback(User_Mqtt_Receive_Callback);

  /*连接Mqtt服务器*/
  if (mqttClient.connect(User_Mqtt.Client_Id.c_str(), User_Mqtt.User, User_Mqtt.Paws, User_Mqtt.Will_Topic, User_Mqtt.Will_QOS, User_Mqtt.Will_Retain, User_Mqtt.Will_MSG, User_Mqtt.CleanSession) == 1)
  {
    /*发布上线消息*/
    uint8_t Online_MSG[] = "Connect\r\n";
    mqttClient.publish(User_Mqtt.Will_Topic, Online_MSG, sizeof(Online_MSG), 1);

    /*订阅消息*/
    User_Mqtt_Subscribe_Topic();

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
  if (mqttClient.connected() == 0)
  {
    if (mqttClient.connect(User_Mqtt.Client_Id.c_str(), User_Mqtt.User, User_Mqtt.Paws, User_Mqtt.Will_Topic, User_Mqtt.Will_QOS, User_Mqtt.Will_Retain, User_Mqtt.Will_MSG, User_Mqtt.CleanSession) == 1)
    {
      uint8_t Online_MSG[] = "Connect\r\n";
      mqttClient.publish(User_Mqtt.Will_Topic, Online_MSG, sizeof(Online_MSG), 1);
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

  if (WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  Debug("Wifi is Disconnect!\r\n");
  Debug("MCU Restart!\r\n");

  /*复位*/
  ESP.restart();

  digitalWrite(13 , 0);
  
  delay(1000);
}

/**
 * @brief 发布MQTT消息
 * @param {*}
 * @return {*}
 */
void User_Mqtt_Publish(uint8_t *pData, uint8_t Len)
{
  /*主题名字*/
  String topicString = "Button_Link_Master/" + WiFi.macAddress() + "/MSG";
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  /*发布消息 retained True*/
  mqttClient.publish(publishTopic, pData, Len, 1);
}




/**
 * @brief 订阅指定主题
 * @param {*}
 * @return {*}
 */
void User_Mqtt_Subscribe_Topic()
{

  /*订阅主题名称*/
  String topicString = "Button_Link/08:3A:F2:94:3C:DC/#";
  char subTopic[topicString.length() + 1];
  strcpy(subTopic, topicString.c_str());

  // 通过串口监视器输出是否成功订阅主题以及订阅的主题名称
  if (mqttClient.subscribe(subTopic))
  {
    Debug("Subscrib Topic:");
    Debug(subTopic);
    Debug("\r\n");
  }
  else
  {
    Debug("Subscribe Fail...\r\n");
  }
}


bool Link_Status = 1;
bool Button_Status = 1;


/**
 * @brief 收到信息后的回调函数
 * @param {*}
 * @return {*}
 */
void User_Mqtt_Receive_Callback(char *topic, byte *payload, unsigned int length)
{

  Debug("Message Received [");
  Debug(topic);
  Debug("]:\r\n ");

  char Topic_Will[] = "Button_Link/08:3A:F2:94:3C:DC/Will";
  char Topic_MSG[] = "Button_Link/08:3A:F2:94:3C:DC/MSG";
  char MSG_Button_OFF[] = "Button:Open\r\n";
  char MSG_Button_ON[] = "Button:Close\r\n";


  /*如果是遗嘱消息*/
  if(strstr(topic , Topic_Will) )
  {
    /*设备上线*/
    if( strstr("Connect\r\n" , (char *)payload) )
    {
      Debug("Button_Link Connect\r\n");
      Link_Status = 0;
    }
    /*设备下线 */
    else if( strstr("Disconnect\r\n" , (char *)payload) )
    {
      Debug("Button_Link Disconnect\r\n");
      Link_Status = 1;
    }
  }
  /*如果是正常消息*/
  else if(strstr(topic , Topic_MSG) )
  {
    /*按钮未被按下*/
    if( strstr(MSG_Button_OFF , (char *)payload) )
    {
      Debug("Button is Open\r\n");
      Button_Status = 1;
    }
    /*按钮被按下*/
    else if( strstr(MSG_Button_ON , (char *)payload) )
    {
      Debug("Button is Close\r\n");
      Button_Status = 0;
    }

  }


}