#include "serialParse.hpp"

String receive_buf = "";   
boolean stringComplete = false;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(230400);
  Serial1.begin(230400);
  receive_buf.reserve(200);
  Serial.println("Serial Start!");

  pinMode(13, OUTPUT); 
}

void loop() { // run over and over
  readSerial1();
  
  if (stringComplete) {
    Serial.println(receive_buf);
    // Serial.print("===end");
    // clear the string:
    receive_buf = "";
    stringComplete = false;
  }
  // delay(500);
  // blink();
  
}

void readSerial1() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    receive_buf += inChar;
   
    if (inChar == '\r') {
      receive_buf.remove(receive_buf.indexOf('\r'));
    }
    else if(inChar == '\n'){
      stringComplete = true;
      receive_buf.remove(receive_buf.indexOf('\n'));
      break;
    }
  }
}
