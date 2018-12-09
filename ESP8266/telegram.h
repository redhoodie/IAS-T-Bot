
#ifndef telegram_h
#define telegram_h

#include <Thread.h>

#include "string_utils.h"

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

WiFiClientSecure net_ssl;
UniversalTelegramBot *telegram_bot;

Thread telegram_thread = Thread();

String telegram_last_chat_id = "";

void telegram_run() {
  if (Serial.available()) {
    while(Serial.available()) {
      if (Serial.read() == '=') {
        String chat_id = Serial.readStringUntil(':');
        String message = Serial.readStringUntil('\n');
        telegram_bot->sendMessage(chat_id, message, "");
      }
    }
  }

  // Read incoming Telegram messages
  int numNewMessages = telegram_bot->getUpdates(telegram_bot->last_message_received + 1);
  while(numNewMessages) {
    Serial.println("got response");
    for (int i=0; i<numNewMessages; i++) {
      telegram_last_chat_id = telegram_bot->messages[i].chat_id;

      size_t lines = 10;
      String data = telegram_bot->messages[i].text;
      for (int l = 0; l <= lines; l++) {
        String message = getValue(data, '\n', l);
        if (message.length() > 0) {
          Serial.print('=');
          Serial.print(telegram_last_chat_id);
          Serial.print(":");
          Serial.println(message);
        }
      }
    }
    numNewMessages = telegram_bot->getUpdates(telegram_bot->last_message_received + 1);
  }
}

void telegram_setup() {
  telegram_bot = new UniversalTelegramBot(telegram_bot_token, net_ssl);
  telegram_thread.onRun(telegram_run);
  telegram_thread.setInterval(500);
}

void telegram_loop() {
  if(telegram_thread.shouldRun()){
    telegram_thread.run();
  }
}

#endif
