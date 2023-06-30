#ifndef logger_H
#define logger_H

// #include "logger.cpp"
#include "logger/wirelessSerial.h"
#include <freertos/task.h>

class loggerClass{

    public:
    void printc(char *arr, int len);
    void begin();
    void print(String msg);
    void print(int i);
    void print(uint i);
    void print(uint8_t i);
    void print(int8_t i);
    void println(int i);
    void println(uint i);
    void println(uint8_t i);
    void println(int8_t i);
    void println();
    void println(String msg);
    void printf(String msg, ...);

};

extern WSerialClass WSerial;


// TaskHandle_t WSerial_handle;
void WSerialServerCli(void *);

#endif