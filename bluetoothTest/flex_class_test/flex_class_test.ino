#include "flex.hpp"
#include <SoftwareSerial.h>

#define INT_PIN_MAX 3
#define INT_PIN_MIN 2

SoftwareSerial mySerial(10, 12); // RX, TX

Flex flex;

void setup(){
	pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(230400); // local port for debug
  mySerial.begin(9600); // BT port
 

  flex.readEEPROM();
  

  flex.attachInterrupt_MAX(INT_PIN_MAX, FALLING);
  flex.attachInterrupt_MIN(INT_PIN_MIN, FALLING);

  mySerial.println("start sending data");
  Serial.println("start sending data"); 
}

void loop(){

  flex.updateFlexData();

  Serial.println("Threshold");
  flex.printThreashold(Serial);

  flex.printFoldInfo(mySerial);
  flex.printFoldInfo(Serial);

  
  delay(300);

}
