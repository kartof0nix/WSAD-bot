
#include "loggerClass.h"
#include <Arduino.h>
#include "logger/wirelessSerial.h"

WSerialClass WSerial;

// Wserial
void WSerialServerCli(void * parameters){
  WSerial.serialServer();
}
// WSerialClass WSerial;
// TaskHandle_t WSerial_handle;


void loggerClass::printc(char *arr, int len){
    WSerial.printc(arr, len);
}

void loggerClass::begin(){

    // WSerial
    WSerial.begin();
    xTaskCreate(
        WSerialServerCli,
        "WWSerial Server",
        2048,
        NULL,
        1,
        &WSerial_handle
    );
    println("Wireless Serial initialized");

}

void loggerClass::print(String msg){
    char send[msg.length()];
    strcpy(send, msg.c_str());
    printc(send, msg.length());
}
void loggerClass::print(int i){
    print(String(i));
}
void loggerClass::print(uint i){
    print(String(i));
}
void loggerClass::print(uint8_t i){
    print(String(i));
}
void loggerClass::print(int8_t i){
    print(String(i));
}
void loggerClass::println(int i){
    print(String(i)+"\n");
}
void loggerClass::println(uint i){
    print(String(i)+"\n");
}
void loggerClass::println(uint8_t i){
    print(String(i)+"\n");
}
void loggerClass::println(int8_t i){
    print(String(i)+"\n");
}
void loggerClass::println(){
    print("\n");
}
void loggerClass::println(String msg){
    print(msg+'\n');
}
void loggerClass::printf(String msg, ...){
    char read[msg.length()];
    char send[2*msg.length()];
    strcpy(read, msg.c_str());
    va_list arg;
    va_start(arg, msg);
    uint len = vsnprintf(send, 2*msg.length(), read, arg);
    printc(send, min(len, 2*msg.length()));
}
