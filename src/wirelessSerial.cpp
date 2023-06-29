#include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>



#define BUFF 256
#define Q_SIZE 26
#define debugPort 1186
#define echoSerial false


class WSerialClass{


    int it_front=0;
    int it_back=0;
    WiFiServer serverD;
    // WiFiUDP Udp; 
    
    QueueHandle_t xQueue;
    char item[BUFF];
    public:
    WSerialClass(){
        xQueue = xQueueCreate(Q_SIZE, sizeof(item));
    }

    void begin(){
        //Wifi Should be initialized by main function
        serverD.begin(debugPort);
        // Udp.begin(debugPort);
        println("Wireless Serial initialized");
    }
    void serialServer(){
        for(;;){
            WiFiClient client = serverD.available();
            for(;;){
                if(!client || !client.connected()){
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                    break;
                }
                else if(xQueueReceive(xQueue, item, 100000 / portTICK_PERIOD_MS)){
                    if(echoSerial){
                        Serial.print(item);
                    }
                    client.write(item);

                }
            }
        }
    }
    // public:

    void print(String msg){
        char send[BUFF];
        strcpy(send, msg.c_str());
        xQueueSend(xQueue, send, 5 / portTICK_PERIOD_MS);
    }
    void println(){
        print("\n");
    }
    void println(int val){
        print(String(val)+'\n');
    }
    void println(String msg){
        print(msg+'\n');
    }
    void printf(String msg, ...){
        va_list arg;
        va_start(arg, msg);
        char read[BUFF];
        char send[BUFF];
        strcpy(read, msg.c_str());
        vsnprintf(send, BUFF, read, arg);
        xQueueSend(xQueue, send, 5 / portTICK_PERIOD_MS);
    }

};


    // Serial.println(replyPacket);
    