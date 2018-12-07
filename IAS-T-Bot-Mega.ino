void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
}

String processCommand(String command) {
  return "got you";
}

void processSerialCommand(String command) {
  Serial.println(processCommand(command));
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()) {
    // Only pass messages starting with an = to the TelegramBot (ESP8266)
    if (Serial.peek() == '=') {
      // Discard '='
      Serial.read();
      String chat_id = Serial.readStringUntil(':');
      String message = Serial.readString();
      message.trim();
      Serial.println("Sending message \"" + message + "\" to chat_id:" + chat_id);
      Serial3.print("=" + chat_id + ":" + message);
    } else {
      processSerialCommand(Serial.readString());
    }
  }

  while(Serial3.available()) {
    if (Serial3.peek() == '=') {
      // Discard '='
      Serial3.read();
      String chat_id = Serial3.readStringUntil(':');
      String message = Serial3.readString();
      message.trim();
      Serial.print("Got message: \"");
      Serial.print(message);
      Serial.println("\" from:" + chat_id);
      String response = processCommand(message);
      Serial.println("Sending reply:" + response);
      if (!response.equals("")) {
        Serial3.print("=" + chat_id + ":" + response);
      }
    } else {
      Serial.write(Serial3.read());
    }
  }
}
