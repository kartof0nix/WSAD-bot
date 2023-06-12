#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <ArduinoOTA.h>

#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "freertos/queue.h"
// #include "wirelessWSerial.cpp"

#include "wirelessSerial.h"
#include "ota_task.h"
#include "microSDCam.h"

#include "cam/main.h"
#include "wheels/wheels.h"



typedef unsigned char uchar;


// const char* ssid = "HR";
// const char* password = "assembler";

WSerialClass WSerial;
TaskHandle_t WSerial_handle;

const char* ssid = "HOME1";
const char* password = "J2U8GLe7W8";
byte incoming_Byte = 0;   // for incoming WSerial data
uchar incoming_Message[256] = "";
byte index_Message = 0;

WiFiUDP Udp; 
constexpr int localUdpPort = 1185;
uchar incomingPacket[256]; // For incoming network data
char replyPacket[256] = "I will not hesitate.\n";
uint8_t replyPacketB[256] = "I will not hesitate.\n";


// Message modes : 0 read variable; 1/2 r/w configuration; 3 - direct motor control; 4 - manouvers (eg. go forward);

String replies[4] = {"OK", "Wrong ussage.", "Wrong message type.", "Argument error."};

int read_Message(uint len, uchar *message){
  if(message[0] == '4'){
    wheelsReadMessage(len-1, message+1);    
  }
  else if(message[0] == '3'){
    vTaskResume(SDCam_handle);
    return 0;
  }
  return 2;
}

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
void SDCamera(void * parameters){
  while(1){
    vTaskSuspend(NULL);
    if(uploading){
      vTaskDelete(NULL);
    }
    CameraPicture();
  }
}

void setup() {
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  delay(10);
  digitalWrite(33, HIGH);

	// Serial.begin(115200);     // opens WSerial port, sets data rate to 9600 bps
  WSerial.println();

  wheelsSetup();

  CameraSetup();

  //Serial.printf("Connecting to %s ", ssid);
  WSerial.printf("Connecting to %s \n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      //WSerial.print(".");
      WSerial.println(wl_status_to_string(WiFi.status()));
  }
  WSerial.println("Connected!");
  Udp.begin(localUdpPort);
  WSerial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  // Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  otaSetup();

  WSerial.begin();

  xTaskCreate(
    WSerialServerCli,
    "WWSerial Server",
    2048,
    NULL,
    1,
    &WSerial_handle
  );

  xTaskCreate(
    SDCamera,
    "SDCamera",
    20480,
    NULL,
    1,
    &SDCam_handle
  );
  digitalWrite(33, LOW);

  WebServerSetup();

	WSerial.println("Setup with camera Complete!");

  // delay(20000);
  //Take a Photo
}

void loop() {
	
	//digitalWrite(MOTOR1_SPD, HIGH);
	// send data only when you receive data:

	// if (WSerial.available() > 0) {
	// 	// read the incoming byte:
	// 	incoming_Byte = WSerial.read();
  //   if(incoming_Byte != '\n' ){
  //     incoming_Message[index_Message++] = incoming_Byte;
  //   }
  //   else{
  //     for(int i = 0; i < index_Message; i++)
  //       WSerial.print((unsigned char)(incoming_Message[i]));
  //     WSerial.println();
  //     int c = read_Message();
  //     WSerial.println(replies[c]);
  //     index_Message = 0;
  //   }
  // }
  int packet_Size = Udp.parsePacket();
  if(packet_Size){
    WSerial.printf("Received %d bytes from %s, port %d\n", packet_Size, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
        incomingPacket[len] = '\0';
    }
    WSerial.printf("UDP packet contents: %s\n", incomingPacket);

    int c = read_Message(len, incomingPacket);
    replies[c].toCharArray(replyPacket, 256);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacketB, 23);
    Udp.endPacket();
    WSerial.println(replyPacket);
  }
  ArduinoOTA.handle();

  delay(100);
}




