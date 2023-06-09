# WSAD-bot
This is the source code for a simple project of mine. It uses two engines and a camera to act as a 'rover' robot.
Operating on FreeRTOS, the goal of this project is to make it some sort of a 'base' for future projects. Therefore, aside from reliability, I will probably focus on flexibility and (in the future) code legibility. I also implemented some development tools for one's convenience.

# Environment (board)
ESP32(cam) - however, the project is written in Arduino so it can be easily modified to fit other boards
# Libraries:
1. `Freertos`
2. `WiFi.h`
3. `ArduinoOTA.h`


# Features
1. OTA - Updete the board without connecting it to the computer
2. Wireless serial - See logs from your device without connecting it to the computer
3. Motor control via wifi
4. Live camera feed 
5. Camera save to SD card 
