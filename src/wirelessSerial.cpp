#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <freertos/FreeRTOS.h>

#include <cppQueue.h>

#define BUFF 256
#define debugPort 1186

Queue

class WSerial{


    uint8_t queue[BUFF];
    int it_front=0;
    int it_back=0;
    WiFiUDP Udp; 

    WSerial(){
        Udp.begin(debugPort);
    }

    void serialServer(void * parameters){
        for(;;){
            if(it_front != it_back){
                Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            }
            for(;it_front != it_back; it_front=(it_front+1)%BUFF;){
                Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                Udp.write(queue[it_front]);
                Udp.endPacket();

            }
        }
    }

}
    // Serial.println(replyPacket);
