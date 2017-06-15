
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define FLEX_NUM 4

volatile int flex_sensor[FLEX_NUM] = {};
volatile int threshold[FLEX_NUM] = {330,330,330,330};
volatile int MAX_threshold[FLEX_NUM] = {700,700,700,700};
volatile int MIN_threshold[FLEX_NUM] = {};
volatile int update_flag = 0;
char fold[FLEX_NUM] = {};
SoftwareSerial mySerial(11, 12); // RX, TX

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(230400); // local port for debug
  mySerial.begin(9600); // BT port
 

 
  mySerial.println("start sending data");
  Serial.println("start sending data"); 
}

void loop(){


  
  Serial.println("L,0,0,0,0,");
  mySerial.println("L,0,0,0,0,");
  Serial.println("R,1,1,1,1,");
  mySerial.println("R,1,1,1,1,");

  Serial.println("1: 12.54 24.35 12.43");
  mySerial.println("1: 12.54 24.35 12.43");
  Serial.println("4: 12.54 24.35 12.43");
  mySerial.println("4: 12.54 24.35 12.43");
  
  delay(500);

}
