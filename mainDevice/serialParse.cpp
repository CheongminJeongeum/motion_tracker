#include "WProgram.h"
#include "serialParse.hpp"

boolean is_stop(int frame_num, int tolerance){

	if(is_diff(tolerance) == false)
		stop_count_f += 1;
	else
		stop_count_f = 0;

	if(stop_count_f >= frame_num)
		return true;
	else
		return false;
}

boolean is_diff(int tolerance){
	double tolerance_sum = 0;
	double diff = 0;

	for(int i=0;i<6;i++){
    for(int j=0;j<3;j++){
    	diff = abs(sensor_data_prev[i][j] - sensor_data[i][j]);
    	if(diff > 180)
    		tolerance_sum += 360 - diff;
    	else
    		tolerance_sum += diff;
    }
  }

  if(tolerance_sum > tolerance)
  	return true;
  else
  	return false;
}

void update_data_prev(){
	for(int i=0;i<6;i++){
    for(int j=0;j<3;j++){
      sensor_data_prev[i][j] = sensor_data[i][j];
    }
  }
}

void print_float(){
  // double foo = atof(c_sensor_data[0][0]);
  // Serial.println(foo);
  for(int i=0;i<6;i++){
    Serial.print(i+1);
    Serial.print(": ");
    for(int j=0;j<3;j++){
      Serial.print(sensor_data[i][j]);
      Serial.print(" ");
    }
    Serial.print("\n");
  }
}

void char2float_sensor_data(){
  for(int i=0;i<6;i++){
    for(int j=0;j<3;j++){
      sensor_data[i][j] = atof(c_sensor_data[i][j]);
    }
  }
}

void print_c_sensor_data(){
  for(int i=0;i<6;i++){
    Serial.print(i+1);
    Serial.print(": ");
    for(int j=0;j<3;j++){
      for(int k=0;;k++){
        if(c_sensor_data[i][j][k] == '\0')
          break;
        Serial.print(c_sensor_data[i][j][k]);
      }
      Serial.print(" ");
    }
    Serial.print("\n");
  }
}

void fill_c_sensor_data(){
  int sensor_num = 0;
  sensor_num = str_to_parse.charAt(4) - '0';
  // Serial.println(sensor_num);

  // remove sensor number infomation
  str_to_parse.remove(0,6);

  for(int j=0;j<3;j++){ //j for x,y,z index
    int data_len = str_to_parse.indexOf(',');
    for(int i=0;i<data_len;i++){ // i for cahr index
      // fill x, y, z data
      c_sensor_data[sensor_num-1][j][i] = str_to_parse.charAt(i);
    }
    // add null to the end
    c_sensor_data[sensor_num-1][j][data_len] == '\0';
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
    	// exception handling for sliced string (by wrong ON timing) 
    	if(receive_buf.substring(0,4) != "100-"){
    		receive_buf = "";
    	}
      str_to_parse = "";
      str_to_parse = receive_buf;
      receive_buf = "";
      stringComplete = false;
      break;
    }
  }
}

void blink(){
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
