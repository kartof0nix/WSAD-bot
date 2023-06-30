#include <Arduino.h>
#include <ArduinoOTA.h>
#include <freertos/FreeRTOS.h>
#include "logger/logger.h"

bool uploading=false;
void otaSetup(){
  ArduinoOTA.setHostname("lilwsadbot");
  ArduinoOTA.setPassword("palindrom");

    ArduinoOTA.onStart([]() {
        uploading=true;
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
            logger.println("sketch");
        } else { // U_FS
            type = "filesystem";
            logger.println("filesystem");
        }
        //logger.println("Suspending all other processes (including logger)");
        //delay(100); // Make sure he gets the logs.
        //vTaskSuspend(logger_handle);

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        logger.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        logger.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        logger.printf("Progress: %u%%\r", (progress*100 / total));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        logger.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            logger.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            logger.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            logger.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            logger.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            logger.println("End Failed");
        }
    });
    ArduinoOTA.begin();

    logger.println("OTA Server initialized");

}
