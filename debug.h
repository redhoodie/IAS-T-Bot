#ifndef debug_h
#define debug_h

#define SERIAL_DEBUG_BAUD 115200

void debug_setup() {
   // Debug console
  Serial.begin(SERIAL_DEBUG_BAUD);

  // within your code, wherever necessary:
  #ifdef BOARD
   Serial.println(BOARD);
  #endif

}

void debug_print(String message){
  Serial.print(message);
}

void debug_println(String message){
  debug_print(message + "\n");
}


void debug_print(int message){
  debug_print(String(message));
}
void debug_println(int message){
  debug_println(String(message));
}

void debug_loop() {
}

#endif
