#include "stdlib.h"
#include "serialParse.hpp"
#include <EEPROM.h>

#define FLEX_NUM 4
#define INT_PIN_MAX 11
#define INT_PIN_MIN 2
#define START_SW_PIN 3

// 4 LED +
// 5 LED -

/*
  variable for gyro value
*/
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

/*
  variable for flex data
*/
volatile int flex_sensor[FLEX_NUM] = {};
volatile int threshold[FLEX_NUM] = {330,330,330,330};
volatile int MAX_threshold[FLEX_NUM] = {700,700,700,700};
volatile int MIN_threshold[FLEX_NUM] = {};
volatile int update_flag = 0;
char fold[FLEX_NUM] = {};


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

  /*
    setting for flex data
  */
  // reading threashold from EEPROM
  Serial.println("reading EEPROM value");
  Serial.print("loaded threshold value: ");
  for(int i=0;i<FLEX_NUM;i++){
    threshold[i] = EEPROM.read(i)*4;
    Serial.print(threshold[i]); Serial.print(" ");
  }
  Serial.println("\n=================");
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  pinMode(INT_PIN_MAX,INPUT_PULLUP);
  pinMode(INT_PIN_MIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN_MAX), INT_set_max_threshold, FALLING);
  attachInterrupt(digitalPinToInterrupt(INT_PIN_MIN), INT_set_min_threshold, FALLING); 
}


void loop(){
  /*
    get flex data
  */
  for(int i=0; i<FLEX_NUM; i++){
    flex_sensor[i] = analogRead(i);
    if(flex_sensor[i] > threshold[i]){
      fold[i] = '0'; // unfold
    }
    else
      fold[i] = '1'; // fold
  }

  // print threashold
  // Serial.print("threshold: ");
  // for(int i=0;i<FLEX_NUM;i++){
  //   Serial.print(threshold[i]); Serial.print(" ");
  // }
  // Serial.print("\n");
  // for(int i=0;i<FLEX_NUM;i++){
  //     Serial.print(fold[i]);
  //     Serial.print(",");
  //     Serial3.print(fold[i]); // BT
  //     Serial3.print(","); // BT
  // }


  // print flex data
  if(flex_str_complete == true){
    
    Serial.print("F:");
    Serial3.print("F:"); // BT
    for(int i=FLEX_NUM-1;i>=0;i--){
      Serial.print(fold[i]);
      Serial.print(",");
      Serial3.print(fold[i]); // BT
      Serial3.print(","); // BT
    }
    Serial.println(buf_flex_data);
    Serial3.println(buf_flex_data); // BT
    buf_flex_data = ""; // reset buf string
    flex_str_complete = false;
  }
  
  /*
   print gyro data
  */
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
    print_float_two_set();
    print_float_two_set_BT();
    Serial.println("Stop");
    Serial3.println("Stop");
  }



  delay(100);


}

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

void INT_set_max_threshold(){
  int sum_value = 0;

  // update flex data & sum_value
  for(int i=0;i<FLEX_NUM;i++){
    flex_sensor[i] = analogRead(i);
    sum_value += flex_sensor[i];
  }

  // all unfold
  for(int i=0;i<FLEX_NUM;i++){
    MAX_threshold[i] = flex_sensor[i];
  }
  update_flag++;

  if(update_flag == 2){ // when set MAX & MIN value
    // update threshold & write to EEPROM 
    for(int i=0;i<FLEX_NUM;i++){
      threshold[i] = (MAX_threshold[i] + MIN_threshold[i])/2;
      EEPROM.write(i, threshold[i]/4);
    }
    update_flag = 0;
  }
}

void INT_set_min_threshold(){
  int sum_value = 0;

  // update flex data & sum_value
  for(int i=0;i<FLEX_NUM;i++){
    flex_sensor[i] = analogRead(i);
    sum_value += flex_sensor[i];
  }

  // all fold
  for(int i=0;i<FLEX_NUM;i++){
    MIN_threshold[i] = flex_sensor[i];
    
  }
  update_flag++;

  if(update_flag == 2){ // when set MAX & MIN value
    // update threshold & write to EEPROM 
    for(int i=0;i<FLEX_NUM;i++){
      threshold[i] = (MAX_threshold[i] + MIN_threshold[i])/2;
      EEPROM.write(i, threshold[i]/4);
    }
    update_flag = 0;
  }
}


