# WSAD-bot
This is the source code for a simple project of mine. It uses two engines and a camera to act as a 'rover' robot.
Operating on FreeRTOS, the goal of this project is to make it some sort of a 'base' for future projects. Therefore, aside from reliability, I will probably focus on flexibility and (in the future) code legibility. I also implemented some development tools for one's convenience.

# Environment 
IDE: Platformio IDE

Board : `AI-Thinker ESP32-cam` - however, the project is written in Arduino so it can be easily modified to fit other boards

Platform : Linux / Windows / MacOS (Mac not tested)
# Libraries:
1. `Freertos`
2. `WiFi.h`
3. `ArduinoOTA.h`
4. EEPROM libraries
5. MicroSD + Filesystem
6. All the libraries required by the camera

All of the above should be preinstalled with Arduino.


# Features
1. OTA - Updete the board without connecting it to the computer
2. Wireless serial - See logs from your device without connecting it to the computer
3. Motor control via wifi
4. Live camera feed 
5. Camera save to SD card 

# Change of Plans

This project will now be based and inmplement all of the fetures of this one:
https://dronebotworkshop.com/esp32cam-robot-car/

# Future plans

Hence the code is now complete (all features are present), I will temporary step back from writing this repo and instead catch up frontend repo to suit all present features and create a PCB. Minor fixes might get pushed for now. Once I'm 
done with all the above, this are the future plans:
1. As mentioned, improve reliability. This means saving logs to BOTH EEPROM and SD Card (user enabled) and a recover mechanism.
2. If possible, add a battery meter ( though I have no pins left :( )
3. Saving video to SD card (only photos supported now).
4. Clean up the code : put everything in tasks and organize it nicely. 


If you have any ideas, you may add a pull request or open up an issue and I will try to reply ASAP.

