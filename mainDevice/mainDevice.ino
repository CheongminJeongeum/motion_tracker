#include "stdlib.h"
#include "serialParse.hpp"

String receive_buf = "";
String str_to_parse = ""; 
boolean stringComplete = false;

char c_sensor_data[6][3][10] = {};
double sensor_data[6][3] = {};
double sensor_data_prev[6][3] = {};

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(230400);
  Serial1.begin(230400);
  receive_buf.reserve(200);
  str_to_parse.reserve(200);
  Serial.println("Serial Start!");

  pinMode(13, OUTPUT); 
}

void loop() { // run over and over

  get_one_line();
  // Serial.println(str_to_parse);
  fill_data();
  print_c_sensor_data();
  char2float_sensor_data();
  Serial.println("======");
  print_float();


  // delay(500);
  // blink();
  
}


