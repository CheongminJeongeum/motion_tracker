
void setup(){
	Serial.begin(230400); // local port for debug
  Serial1.begin(230400); // port income from gyro sensors
  Serial2.begin(9600); // BT for phone 

}

void loop(){

	Serial.print("A0: ");
	if(analogRead(A0) < 500){
		Serial.println("fold!");
	} else{
		Serial.println("===");
	}
	Serial.print("A1: ");
	if(analogRead(A1) < 500){
		Serial.println("fold!");
	} else{
		Serial.println("===");
	}
	Serial.print("A2: ");
	if(analogRead(A2) < 500){
		Serial.println("fold!");
	} else{
		Serial.println("===");
	}
	Serial.print("A3: ");
	if(analogRead(A3) < 500){
		Serial.println("fold!");
	} else{
		Serial.println("===");
	}
	

	delay(100);


}

