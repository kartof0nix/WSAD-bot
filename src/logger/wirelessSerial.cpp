#include <Arduino.h>
#include "wirelessSerial.h"
// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>
#include <WiFi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#ifndef wirelessSerial_CPP
#define wirelessSerial_CPP
// #include "logger/logger.h"


// #define BUFF 1024
#define Q_SIZE 1024
#define debugPort 1186
#define echoSerial false

TaskHandle_t WSerial_handle;

WSerialClass::WSerialClass(){
    xQueue = xQueueCreate(Q_SIZE, sizeof(item));
}

void WSerialClass::begin(){
    //Wifi Should be initialized by main function
    serverD.begin(debugPort);
    // Udp.begin(debugPort);
}
void WSerialClass::serialServer(){
    for(;;){
        WiFiClient client = serverD.available();
        for(;;){
            if(!client || !client.connected()){
                vTaskDelay(50 / portTICK_PERIOD_MS);
                break;
            }
            else if(xQueueReceive(xQueue, &item, 100000 / portTICK_PERIOD_MS)){
                client.write(item);
            }
        }
    }
}

// public:
void WSerialClass::printc(char *msg, int len){
    // char send[BUFF];
    // strcpy(send, msg.c_str());
    for(int i = 0; i < len; i++){
        xQueueSend(xQueue, msg+i, 0);
    }
}



    // Serial.println(replyPacket);
#endif