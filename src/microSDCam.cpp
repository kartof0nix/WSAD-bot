#include "microSDCam.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
 
// MicroSD Libraries
#include "FS.h"
#include "SD_MMC.h"
 
// EEPROM Library
#include "EEPROM.h"
 
//Wireless Serial
#include <wirelessSerial.h>

TaskHandle_t SDCam_handle;

// Use 1 byte of EEPROM space
#define EEPROM_SIZE 1
 

// Counter for picture number
unsigned int pictureCount = 0;
 
// Pin definitions for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
 
void configESPCamera() {
  // Configure Camera parameters
 
  // Object to store the camera configuration parameters
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Initialize the Camera
  WSerial.println("Camera init 1 succesfull");
  delay(100);
  esp_err_t err = esp_camera_init(&config);
  WSerial.println("Camera init 2 succesfull");
  delay(100);
  if (err != ESP_OK) {
    WSerial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  WSerial.println("Camera init 3 succesfull");
  delay(100);
  // Camera quality adjustments
  sensor_t * s = esp_camera_sensor_get();
 
  WSerial.println("Camera init 4 succesfull");
  delay(100);
  // BRIGHTNESS (-2 to 2)
  s->set_brightness(s, 0);
  // CONTRAST (-2 to 2)
  s->set_contrast(s, 0);
  // SATURATION (-2 to 2)
  s->set_saturation(s, 0);
  // SPECIAL EFFECTS (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_special_effect(s, 0);
  // WHITE BALANCE (0 = Disable , 1 = Enable)
  s->set_whitebal(s, 1);
  // AWB GAIN (0 = Disable , 1 = Enable)
  s->set_awb_gain(s, 1);
  // WB MODES (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_wb_mode(s, 0);
  // EXPOSURE CONTROLS (0 = Disable , 1 = Enable)
  s->set_exposure_ctrl(s, 1);
  // AEC2 (0 = Disable , 1 = Enable)
  s->set_aec2(s, 0);
  // AE LEVELS (-2 to 2)
  s->set_ae_level(s, 0);
  // AEC VALUES (0 to 1200)
  s->set_aec_value(s, 300);
  // GAIN CONTROLS (0 = Disable , 1 = Enable)
  s->set_gain_ctrl(s, 1);
  // AGC GAIN (0 to 30)
  s->set_agc_gain(s, 0);
  // GAIN CEILING (0 to 6)
  s->set_gainceiling(s, (gainceiling_t)0);
  // BPC (0 = Disable , 1 = Enable)
  s->set_bpc(s, 0);
  // WPC (0 = Disable , 1 = Enable)
  s->set_wpc(s, 1);
  // RAW GMA (0 = Disable , 1 = Enable)
  s->set_raw_gma(s, 1);
  // LENC (0 = Disable , 1 = Enable)
  s->set_lenc(s, 1);
  // HORIZ MIRROR (0 = Disable , 1 = Enable)
  s->set_hmirror(s, 0);
  // VERT FLIP (0 = Disable , 1 = Enable)
  s->set_vflip(s, 0);
  // DCW (0 = Disable , 1 = Enable)
  s->set_dcw(s, 1);
  // COLOR BAR PATTERN (0 = Disable , 1 = Enable)
  s->set_colorbar(s, 0);
 
}
 
void initMicroSDCard() {
  // Start the MicroSD card
 
  WSerial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin()) {
    WSerial.println("MicroSD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    WSerial.println("No MicroSD Card found");
    return;
  }
 
}
 
void takeNewPhoto(String path) {
  // Take Picture with Camera
 
  // Setup frame buffer
  camera_fb_t  * fb = esp_camera_fb_get();
 
  if (!fb) {
    WSerial.println("Camera capture failed");
    return;
  }
 
  // Save picture to microSD card
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    WSerial.println("Failed to open file in write mode");
  }
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    WSerial.printf("Saved file to path: %s\n", path.c_str());
  }
  // Close the file
  file.close();
 
  // Return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);
}
 
void CameraSetup() {
 
  // Initialize the camera
  WSerial.println("Initializing the camera module...");
  configESPCamera();
  WSerial.println("Camera OK!");
 
  // Initialize the MicroSD
  // WSerial.print("Initializing the MicroSD card module... ");
  // initMicroSDCard();
  // EEPROM.begin(EEPROM_SIZE);
}

void CameraPicture(){
  // initialize EEPROM with predefined size
  pictureCount = EEPROM.read(0) + 1;
 
  // Path where new picture will be saved in SD Card
  String path = "/image" + String(pictureCount) + ".jpg";
  WSerial.printf("Picture file name: %s\n", path.c_str());
 
  // Take and Save Photo
  takeNewPhoto(path);
    
  // Update EEPROM picture number counter
  EEPROM.write(0, pictureCount);
  EEPROM.commit();
 
  // Bind Wakeup to GPIO13 going LOW
  WSerial.println("I dont't care I'm done");
}
 