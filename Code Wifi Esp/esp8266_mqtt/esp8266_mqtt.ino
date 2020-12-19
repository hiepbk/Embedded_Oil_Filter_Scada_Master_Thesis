#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include "ESP8266WebServer.h"
#include <string.h>
#include "uart.h"
// Cập nhật thông tin
// Thông tin về wifi
//#define ssid "Hiep"
//#define password "mot2ba45"
// Thông tin về MQTT Broker
#define mqtt_server "tailor.cloudmqtt.com" // Tai khoan cua HIepbk
#define mqtt_topic_pub "ESP_PUB"   //ESP_PUB
#define mqtt_topic_sub "ESP_SUB"
#define mqtt_user "hiepbk"    //hiepbk
#define mqtt_pwd "8286597"
//SoftwareSerial mySerial(4,5);//Rx,tx (D2,D1)
//const int rs = 0, en = 2, d4 = 14, d5 = 12, d6 = 13, d7 = 16;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const uint16_t mqtt_port = 16370; //Port của CloudMQTT

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);
long lastMsg = 0;
char msg[50];
int value = 0;
char bfrStm[200];
String bfr;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin(115200);
//  mySerial.begin(115200);
  WiFi.begin();
  setup_wifi(); 
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback); 
}
// Hàm kết nối wifi
void setup_wifi() {
  int cnt = 0;
  delay(10);
  WiFi.mode(WIFI_STA);
    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(cnt++ >= 10){
       WiFi.beginSmartConfig();
       while(1){
           delay(1000);
           //Kiểm tra kết nối thành công in thông báo
           if(WiFi.smartConfigDone()){
             Serial.print("SmartConfig Success \n");
             break;
           }
       }
    }
  }
    // Khởi tạo server 
  Serial.print("Server started");
  // In địa chỉ IP
//  Serial.print(WiFi.localIP());
//  Serial.print("Connecting to");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
 //Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
  for(int i=0;i<length;i++){
  Serial.print((char)payload[i]);
  //mySerial.print((char)payload[i]);
  }
}
 //Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Đang kết nối MQTT...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
      Serial.println("connected MQTT");
      // Khi kết nối sẽ publish thông báo
      client.publish(mqtt_topic_pub, "ESP8266 connected");
      // ... và nhận lại thông tin này
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("Không thể kết nối MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void loop() {  
  reicevedStm();       
   //Kiểm tra kết nối MQTT
  if (!client.connected()) {
    reconnect();
  } 
  //  sentDataToMqtt();
  client.loop();
  
}
void reicevedStm(){ 
  if(Serial.available()>3){  
    memset(bfrStm,0,200);
    Serial.readBytesUntil('\n',bfrStm,200);
      //bfr=Serial.readString();
      //bfr.toCharArray(bfrStm,500);
    if(bfrStm[0]='{'){
      client.publish(mqtt_topic_pub,bfrStm);
//      Serial.print("Uart_Stm32_Sent:");
//      Serial.print(bfrStm);
//      Serial.print("\n");

 
      }  
  }
 }


    
