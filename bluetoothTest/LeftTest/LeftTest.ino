#include "stdlib.h"
#include "serialParse.hpp"

String receive_buf = "";
String str_to_parse = ""; 
boolean stringComplete = false;

char c_sensor_data[6][3][10] = {};
double sensor_data[6][3] = {};
double sensor_data_prev[6][3] = {};
int stop_count_f = 0;
const double TOLERANCE = 10;
const int FRAME_NUM = 15;

String buf_flex_data;
boolean flex_str_complete = false;

void setup(){
	pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

	Serial.begin(230400); // local port for debug
  Serial1.begin(230400); // port income from gyro sensors
  Serial2.begin(9600); // BT to receive flex data from right hand
  Serial3.begin(230400); // BT for smartphone 
  Serial.println("start!");

  receive_buf.reserve(200);
  str_to_parse.reserve(200);
  buf_flex_data.reserve(50);
}


void loop(){
	// receive flex data
	// while (Serial2.available() > 0) {
 //    char inByte = Serial2.read();
 //    if(inByte == '\n')
 //    	break; //대신 플래그를 사용하자 루프 돌면서 기다리지 말고  
    
 //    buf_flex_data.append(inByte);
 //  }

  // // print flex data
  // if(flex_str_complete == true){
  // 	Serial.println(buf_flex_data);
  // 	Serial3.println(buf_flex_data);
  // 	buf_flex_data = ""; // reset buf string
  //   flex_str_complete = false;
  // }
  
  // print gyro data
  update_data_prev();

  get_one_line();

  // Serial.println(str_to_parse);
  fill_c_sensor_data();
  // print_c_sensor_data();
  char2float_sensor_data();

  if(is_stop(FRAME_NUM, TOLERANCE) == false){
    // Serial.println("======float below");
    // print_float();
    print_float_two_set();
    // Serial3.println("====");
    print_float_two_set_BT();
  }
  else{
    // Serial.println("stop!!!!");
  }



	// delay(500);


}

/*
void serialEvent2() {
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    buf_flex_data += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      flex_str_complete = true;
    }
  }
}
*/
