#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "motor.h"
#include "ota_task.h"

#define MOTOR1_FWD 4
#define MOTOR1_BCK 12
#define MOTOR1_ENB 14

#define MOTOR2_FWD 15
#define MOTOR2_BCK 13
#define MOTOR2_ENB 12

typedef unsigned char uchar;


// const char* ssid = "HR";
// const char* password = "assembler";

const char* ssid = "HOME1";
const char* password = "J2U8GLe7W8";
byte incoming_Byte = 0;   // for incoming serial data
uchar incoming_Message[256] = "";
byte index_Message = 0;

WiFiUDP Udp; 
constexpr int localUdpPort = 1185;
uchar incomingPacket[256]; // For incoming network data
char replyPacket[256] = "I will not hesitate.\n";
uint8_t replyPacketB[256] = "I will not hesitate.\n";

Motor Motor1(MOTOR1_FWD, MOTOR1_BCK, MOTOR1_ENB);
Motor Motor2(MOTOR2_FWD, MOTOR2_BCK, MOTOR2_ENB);
// Message modes : 0 read variable; 1/2 r/w configuration; 3 - direct motor control; 4 - manouvers (eg. go forward);

String replies[4] = {"OK", "Wrong ussage.", "Wrong message type.", "Argument error."};

int read_Message(uchar *message){
  if(message[0] == '4'){
    switch (message[1])
    {
      case 'w':
        Motor1.changeMotorDirection(1);
        Motor2.changeMotorDirection(1);
        Serial.println("Onward!");
        return 0;
        break;
      case 's':
        Motor1.changeMotorDirection(-1);
        Motor2.changeMotorDirection(-1);
        Serial.println("Backward!");
        return 0;
        break;
      case 'a':
        Motor1.changeMotorDirection(-1);
        Motor2.changeMotorDirection(1);
        Serial.println("Left");
        return 0;
        break;
      case 'd':
        Motor1.changeMotorDirection(1);
        Motor2.changeMotorDirection(-1);
        Serial.println("Right");
        return 0;
        break;
      case 'e':
        Motor1.changeMotorDirection(0);
        Motor2.changeMotorDirection(0);
        Serial.println("Noneward!");
        return 0;
        break;
      case '+':
        Motor1.changeMotorSpeed(min( Motor1.rt_Speed + 10, 255 ));
        Motor2.changeMotorSpeed(min( Motor2.rt_Speed + 10, 255 ));
        Serial.print("Current speed : ");
        Serial.println(Motor1.rt_Speed);
        return 0;
        break;
      case '-':
        Motor1.changeMotorSpeed( max ( Motor1.rt_Speed - 10, 90 ));
        Motor2.changeMotorSpeed( max ( Motor2.rt_Speed - 10, 90 ));
        Serial.print("Current speed : ");
        Serial.println(Motor1.rt_Speed);
        return 0;
        break;
      case 'h':
        {
          int speed = message[2];
          if(speed < 90) return 3;
          Motor1.changeMotorSpeed( speed );
          Motor2.changeMotorSpeed( speed );
          Serial.print("Current speed : ");
          Serial.println(Motor1.rt_Speed);
          return 0;
          break;
        }
        //Serial.write("Wrong operation! Use w/s/+/-\n");
      default:
        return 1;
    }
  }
  return 2;
}

void otaSetup(){
  ArduinoOTA.setHostname("lilwsadbot");
  ArduinoOTA.setPassword("palindrom");

    ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
    } else { // U_FS
        type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
    }
    });
    ArduinoOTA.begin();
}

void setup() {
	Serial.begin(115200);     // opens serial port, sets data rate to 9600 bps
  Serial.println();

  Motor1.setup();
  Motor2.setup();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      //Serial.print(".");
      Serial.print(WiFi.status());
  }
  Serial.println(" connected");
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  otaIntSetup();
  xTaskCreate(
    otaRun,
    "Ota Interface",
    1000,
    NULL,
    1,
    NULL
  );

	Serial.println("Begun!");
  Serial.println();

}

void loop() {
	
	//digitalWrite(MOTOR1_SPD, HIGH);
	// send data only when you receive data:

	// if (Serial.available() > 0) {
	// 	// read the incoming byte:
	// 	incoming_Byte = Serial.read();
  //   if(incoming_Byte != '\n' ){
  //     incoming_Message[index_Message++] = incoming_Byte;
  //   }
  //   else{
  //     for(int i = 0; i < index_Message; i++)
  //       Serial.print((unsigned char)(incoming_Message[i]));
  //     Serial.println();
  //     int c = read_Message();
  //     Serial.println(replies[c]);
  //     index_Message = 0;
  //   }
  // }
  
  int packet_Size = Udp.parsePacket();
  if(packet_Size){
    Serial.printf("Received %d bytes from %s, port %d\n", packet_Size, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
        incomingPacket[len] = '\0';
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    int c = read_Message(incomingPacket);
    replies[c].toCharArray(replyPacket, 256);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacketB, 23);
    Udp.endPacket();
    Serial.println(replyPacket);
  }
  delay(10);
}




