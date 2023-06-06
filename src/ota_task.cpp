#include <Arduino.h>
#include <ArduinoOTA.h>
#include <freertos/FreeRTOS.h>
#include <wirelessSerial.h>


void otaSetup(){
  ArduinoOTA.setHostname("lilwsadbot");
  ArduinoOTA.setPassword("palindrom");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
            WSerial.println("sketch");
        } else { // U_FS
            type = "filesystem";
            WSerial.println("filesystem");
        }
        //WSerial.println("Suspending all other processes (including Wserial)");
        //delay(100); // Make sure he gets the logs.
        //vTaskSuspend(WSerial_handle);

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        WSerial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        WSerial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        WSerial.printf("Progress: %u%%\r", (progress*100 / total));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        WSerial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            WSerial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            WSerial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            WSerial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            WSerial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            WSerial.println("End Failed");
        }
    });
    ArduinoOTA.begin();

    WSerial.println("OTA Server initialized");

}
