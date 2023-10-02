#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <WiFiManager.h>
#include <AsyncUDP.h>

#include <ArduinoOTA.h>

#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "freertos/queue.h"
// #include "wirelessWSerial.cpp"

#include "wirelessSerial.h"
#include "ota_task.h"

#include "cam/main.h"

#include "EEPROM.h"

const char* ssid = "ESP32";
const char* password = "murzynianka";

typedef unsigned char uchar;


// const char* ssid = "HR";
// const char* password = "assembler";

WSerialClass WSerial;
TaskHandle_t WSerial_handle;

// Message modes : 0 read variable; 1/2 r/w configuration; 3 - direct motor control; 4 - manouvers (eg. go forward);
WiFiManager wifiManager;

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
  return "UNKNOWN_ERROR";
}

void WSerialServerCli(void * parameters){
  WSerial.serialServer();
}
// void SDCamera(void * parameters){
//   while(1){
//     vTaskSuspend(NULL);
//     if(uploading){
//       vTaskDelete(NULL);
//     }
//     CameraPicture();
//   }
// }

AsyncUDP Audp;

// int WSerial_in(const char * format, va_list arg){
//     return WSerial.printf(format, arg);
// }

void blink(){
  bool now = gpio_get_level(GPIO_NUM_33);
  digitalWrite(33, !now);
  delay(100);
  digitalWrite(33, now);
}

void setup() {
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  delay(10);
  digitalWrite(33, HIGH);

	Serial.begin(115200);     // opens WSerial port, sets data rate to 9600 bps
  WSerial.println();


  configESPCamera();

  //Serial.printf("Connecting to %s ", ssid);
  WSerial.printf("Connecting to %s \n", ssid);

  //Connect to the WIFI or start AP
  wifiManager.setConnectRetries(3);
  wifiManager.setConnectTimeout(10);
  wifiManager.setConfigPortalTimeout(300);
  wifiManager.setHttpPort(85);
  wifiManager.autoConnect(ssid, password);
  //Once connected : 
  //1 : Blink and log
  WSerial.println("Connected!");
  blink();
  //2: Broadcast ip address
  String msg = "DEV:ESP32 at " + WiFi.localIP().toString();
  Audp.broadcastTo(msg.c_str(), 2555);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
//WSerial.print(".");
      WSerial.println(wl_status_to_string(WiFi.status()));
  }
  WSerial.printf("Now listening at IP %s\n", WiFi.localIP().toString().c_str());

  otaSetup();
  WSerial.begin();

  xTaskCreate(
    WSerialServerCli,
    "WSerial Server",
    2*2048+1024,
    NULL,
    1,
    &WSerial_handle
  );

  // xTaskCreate(
  //   SDCamera,
  //   "SDCamera",
  //   20480,
  //   NULL,
  //   1,
  //   &SDCam_handle
  // );
  digitalWrite(33, LOW);

  WebServerSetup();

	WSerial.println("Setup with camera Complete!");

  // delay(20000);
  //Take a Photo
}

int DeathCounter=140;
void loop() {
  ArduinoOTA.handle();
  // WSerial.println(wl_status_to_string(WiFi.status()));
  delay(100);
  // if(WiFi.status() != WL_CONNECTED){
  //   DeathCounter--;
  //   if(DeathCounter==0){ 
  //     wifiManager.autoConnect(); //After two minutes, recover from downtime (possibly some ESP-related error)
  //     DeathCounter=140;
  //   }
  // }
  // else{
  //   DeathCounter=140;
  // }
  //TODO : jeśli rozłączył się -> restart
}