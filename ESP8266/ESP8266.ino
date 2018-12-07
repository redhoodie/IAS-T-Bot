#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

/*
	This is an initial sketch to be used as a "blueprint" to create apps which can be used with IOTappstory.com infrastructure
	Your code can be filled wherever it is marked.

	Copyright (c) [2017] [Andreas Spiess]

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/

// TODO:
// [ ] LED Control from Slack

//#define CFG_AUTHENTICATE  true

#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 0                                        // Button pin on the esp for selecting modes. D3 for the Wemos!

#include <IOTAppStory.h>                                    // IotAppStory.com library
IOTAppStory IAS(COMPDATE, MODEBUTTON);                      // Initialize IOTAppStory

#include "debug.h"
#include "arduino_ota.h"

char* telegram_bot_token = "";

#include "telegram.h"


#include "lwip/dns.h"

// ================================================ SETUP ================================================
void setup() {

  IAS.preSetDeviceName("iastbot");                         // preset deviceName this is also your MDNS responder: http://iasled.local

  IAS.addField(telegram_bot_token, "TelegramBotToken", 46);                   // These fields are added to the config wifimanager and saved to eeprom. Updated values are returned to the original variable.

  // You can configure callback functions that can give feedback to the app user about the current state of the application.
  // In this example we use serial print to demonstrate the call backs. But you could use leds etc.
  IAS.onModeButtonShortPress([]() {
    Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });

  IAS.onModeButtonLongPress([]() {
    Serial.println(F(" If mode button is released, I will enter in configuration mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });
  
  IAS.begin('L');                                           // Optional parameter: What to do with EEPROM on First boot of the app? 'F' Fully erase | 'P' Partial erase(default) | 'L' Leave intact

  IAS.setCallHome(true);                                    // Set to true to enable calling home frequently (disabled by default)
  IAS.setCallHomeInterval(7200);                              // Call home interval in seconds, use 60s only for development. Please change it to at least 2 hours in production
  
	
  //-------- Your Setup starts from here ---------------

  telegram_setup();
  debug_setup();
  arduino_ota_start();
}


// ================================================ LOOP =================================================
void loop() {
  IAS.loop();																// this routine handles the calling home on the configured itnerval as well as reaction of the Flash button. If short press: update of skethc, long press: Configuration

  //-------- Your Sketch starts from here ---------------

   arduino_ota_loop();
   telegram_loop();
}
