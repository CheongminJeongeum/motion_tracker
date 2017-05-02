#include "serialParse.hpp"

String receive_buf = "";  
String str_to_parse = ""; 
boolean stringComplete = false;

char sensor_data[6][3][10] = {};

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
  print_sensor_data();


  // delay(500);
  // blink();
  
}
void print_sensor_data(){
  for(int i=0;i<6;i++){
    Serial.print(i+1);
    Serial.print(": ");
    for(int j=0;j<3;j++){
      for(int k=0;;k++){
        if(sensor_data[i][j][k] == '\0')
          break;
        Serial.print(sensor_data[i][j][k]);
      }
      Serial.print(" ");
    }
    Serial.print("\n");
  }
}

void fill_data(){
  int sensor_num = 0;
  sensor_num = str_to_parse.charAt(4) - '0';
  // Serial.println(sensor_num);

  // remove sensor number infomation
  str_to_parse.remove(0,6);

  for(int j=0;j<3;j++){ //j for x,y,z index
    int data_len = str_to_parse.indexOf(',');
    for(int i=0;i<data_len;i++){ // i for cahr index
      // fill x, y, z data
      sensor_data[sensor_num-1][j][i] = str_to_parse.charAt(i);
    }
    // add null to the end
    sensor_data[sensor_num-1][j][data_len] == '\0';
    //remove data in string for next parsing
    str_to_parse.remove(0, data_len + 1);
  }
}

void get_one_line(){
  while(true){
    // get char and append to receive_buf 
    while (Serial1.available()){
      char inChar = (char)Serial1.read();
      receive_buf += inChar;
     
      // remove \r\n
      if (inChar == '\r'){
        receive_buf.remove(receive_buf.indexOf('\r'));
      }
      else if(inChar == '\n'){
        stringComplete = true;
        receive_buf.remove(receive_buf.indexOf('\n'));
        break;
      }
    }
    // if complete string received, copy to str_to_parse
    if(stringComplete){
      str_to_parse = "";
      str_to_parse = receive_buf;
      receive_buf = "";
      stringComplete = false;
      break;
    }
  }
}
