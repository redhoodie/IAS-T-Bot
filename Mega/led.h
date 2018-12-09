#ifndef led_h
#define led_h

#include <FastLED.h>
#include <Thread.h>
#include "StringSplitter.h"


#define LED_PIN     5
#define NUM_LEDS    57

CRGB leds[NUM_LEDS];
int led_brightness = 64;
String led_current_mode;
int led_phase = 0;
int led_speed = 500;

int led_hue;
Thread led_thread = Thread();

void flash_hue(int hue) {
  for(int j = 255; j > 0; j--){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue, 255, j);
    }
    FastLED.show();
    delay(1);
  }

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

String processCommand(String command) {
  command.toLowerCase();
  int param_split = command.indexOf(' ');

  String params;

  if (param_split != -1) {
    params = command.substring(param_split + 1);
    command = command.substring(0, param_split);
  }

  if (command == "/strobe") {
    led_current_mode = "strobe";
    if (params != "") {
      led_hue = params.toInt();
    } else {
      led_hue = 95;
    }
    return "Mode set to strobe.";
  }
  else if (command == "/random") {
    led_current_mode = "random";
    return "Mode set to random.";
  }
  else if (command == "/rainbow") {
    led_current_mode = "rainbow";
    if (params == "") {
      led_speed = 40;
      led_thread.setInterval(led_speed);
    } else {
      led_speed = params.toInt();
      led_thread.setInterval(led_speed);
    }
    return "Mode set to rainbow.";
  }
  else if (command == "/police") {
    led_current_mode = "police";
    if (params == "") {
      led_speed = 40;
      led_thread.setInterval(led_speed);
    } else {
      led_speed = params.toInt();
      led_thread.setInterval(led_speed);
    }
    return "Mode set to police.";
  }
  else if (command == "/stop") {
    led_current_mode = "";
    return "stopped.";
  }
  else if (command == "/speed" ) {
    if (params == "") {
      return String("Current speed is ") + led_speed + ".";
    } else {
      led_speed = params.toInt();
      led_thread.setInterval(led_speed);
      return String("Set speed to ") + led_speed + ".";
    }
  }
  else if (command == "/brightness" ) {
    if (params == "") {
      return String("Current brightness is ") + led_brightness + ".";
    } else {
      led_brightness = params.toInt();
      return String("Brightness set to ") + led_brightness + ".";
    }
  }

  return "invalid command: " + command;
}

void led_run() {
  if (led_current_mode == "strobe") {
    if (led_phase) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(led_hue, 255, led_brightness);
      }
      FastLED.show();
    } else {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
    led_phase = (led_phase + 1) % 2;
  } else if (led_current_mode == "police") {
    CRGB colour;

    for (int i = 0; i < NUM_LEDS; i++) {
      // left
      if (i <= (NUM_LEDS / 2)) {
        // Red
        if (led_phase == 0 || led_phase == 4) {
          colour = CHSV(160, 255, led_brightness);
        } else {
          colour = CRGB::Black;
        }
        // Right
      } else {
        // Blue
        if (led_phase == 1 || led_phase == 3) {
          colour = CHSV(0, 255, led_brightness);
        } else {
          colour = CRGB::Black;
        }
      }

      leds[i] = colour;
    }

    led_phase = (led_phase + 1) % 6;
    FastLED.show();
  } else if (led_current_mode == "rainbow") {
    for (int i = 0; i < NUM_LEDS; i++) {
      int hue = (255 / NUM_LEDS) * (i + led_phase);
      int saturation = random(200, 255); // Shimmer
      leds[i] = CHSV(hue, saturation, led_brightness);
    }
    led_phase = (led_phase + 1) % NUM_LEDS;
    FastLED.show();
  } else if (led_current_mode == "random") {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(random(0, 255), 255, led_brightness);
    }
    FastLED.show();
  } else if (led_current_mode == "") {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
}

void led_setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  flash_hue(160);

  led_thread.onRun(led_run);
  led_thread.setInterval(led_speed);
}

void led_loop() {
  if (led_thread.shouldRun()) {
    led_thread.run();
  }
}

#endif
