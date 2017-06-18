
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define FLEX_NUM 4
#define INT_PIN_MAX 3
#define INT_PIN_MIN 2

volatile int flex_sensor[FLEX_NUM] = {};
volatile int threshold[FLEX_NUM] = {330,330,330,330};
volatile int MAX_threshold[FLEX_NUM] = {700,700,700,700};
volatile int MIN_threshold[FLEX_NUM] = {};
volatile int update_flag = 0;
char fold[FLEX_NUM] = {};
SoftwareSerial mySerial(10, 12); // RX, TX

void setup(){
	pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(230400); // local port for debug
  mySerial.begin(9600); // BT port
 

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

  mySerial.println("start sending data");
  Serial.println("start sending data"); 
}

void loop(){

  for(int i=0; i<FLEX_NUM; i++){
    flex_sensor[i] = analogRead(i);
    if(flex_sensor[i] > threshold[i]){
      fold[i] = '0'; // unfold
    }
    else
      fold[i] = '1'; // fold
  }

  // print threashold
//  Serial.print("threshold: ");
//  for(int i=0;i<FLEX_NUM;i++){
//  	Serial.print(threshold[i]); Serial.print(" ");
//  }
//  Serial.print("\n");

  // print ADC value & fold info
  for(int i=0;i<FLEX_NUM;i++){
    Serial.print(flex_sensor[i]);
    Serial.print(":");
    Serial.print(fold[i]);
    Serial.print(",");

    // print fold info to BT
    mySerial.print(fold[i]);
    mySerial.print(",");
  }
  Serial.print("\n");
  mySerial.print("\n");
  
  delay(100);

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
