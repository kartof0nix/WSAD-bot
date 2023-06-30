#include <Arduino.h>
#include <WiFi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#ifndef WSERIAL_H
#define WSERIAL_H

extern TaskHandle_t WSerial_handle;

class WSerialClass{
    int it_front=0;
    int it_back=0;
    WiFiServer serverD;
    // WiFiUDP Udp; 
    
    QueueHandle_t xQueue;
    char item;

    public:
        WSerialClass();

        void printc(char *msg, int len);
        void serialServer();
        void begin();


};

#endif
