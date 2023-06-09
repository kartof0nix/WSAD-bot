#include <Arduino.h>

extern TaskHandle_t SDCam_handle;

// unsigned int pictureCount;
void CameraSetup() ;
void CameraPicture() ;
void takeNewPhoto(String path) ;
void initMicroSDCard() ;
void configESPCamera() ;
