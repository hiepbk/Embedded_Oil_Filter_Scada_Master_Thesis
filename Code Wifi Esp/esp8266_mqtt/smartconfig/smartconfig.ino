#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;

void setup() {
  int cnt = 0;
  //Khởi tạo baud 115200
  Serial.begin(115200);
  //Mode wifi là station
  WiFi.mode(WIFI_STA);
  //Chờ kết nối
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(cnt++ >= 10){
       WiFi.beginSmartConfig();
       while(1){
           delay(1000);
           //Kiểm tra kết nối thành công in thông báo
           if(WiFi.smartConfigDone()){
             Serial.println("SmartConfig Success");
             break;
           }
       }
    }
  }
  
  Serial.println("");
  Serial.println("");
  
  WiFi.printDiag(Serial);
  
  // Khởi tạo server
  Udp.begin(49999);
  Serial.println("Server started");

  // In địa chỉ IP
  Serial.println(WiFi.localIP());
}

void loop() {
  // Nhận gói tin gửi từ ESPTouch
  Udp.parsePacket();
  //In IP của ESP8266
  while(Udp.available()){
    Serial.println(Udp.remoteIP());
    Udp.flush();
    delay(5);
  }
 
}
