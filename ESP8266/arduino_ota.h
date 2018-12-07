
#ifndef arduono_ota_h
#define arduono_ota_h

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

int percent = 0;

void arduino_ota_start() {
  ArduinoOTA.setPasswordHash("5e7be9512068c04c27350b6eb9e5982d");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    debug_println("Start updating " + type);
  }); 
  ArduinoOTA.onEnd([]() {
    debug_println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char buffer[32];
    int new_percent = (progress / (total / 100));
    int diff = new_percent - percent;
    
    if (diff > 4.9) {
      percent = new_percent;
      sprintf(buffer, "Progress: %u%%\r", percent);
      debug_println(String(buffer));
    }
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) debug_println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) debug_println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) debug_println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) debug_println("Receive Failed");
    else if (error == OTA_END_ERROR) debug_println("End Failed");
  });

  ArduinoOTA.begin();
}

void arduino_ota_loop() {
  ArduinoOTA.handle();
}

#endif
