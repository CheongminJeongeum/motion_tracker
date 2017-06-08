#define FLEX_NUM 4

int flex_sensor[FLEX_NUM] = {};
int threshhold[FLEX_NUM] = {330,330,330,330};
char fold[FLEX_NUM] = {};

void setup(){
  Serial.begin(230400); // local port and BT
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  // pinMode(A4, INPUT);

  // Serial1.println("start sending data");
  Serial.println("start sending data");
}

void loop(){

  for(int i=0; i<FLEX_NUM; i++){
    flex_sensor[i] = analogRead(i);
    if(flex_sensor[i] > threshhold[i]){
      fold[i] = '0'; // unfold
    }
    else
      fold[i] = '1'; // fold
  }


  for(int i=0;i<FLEX_NUM;i++){
//    Serial.print(flex_sensor[i]);
    Serial.write(fold[i]);
    Serial.write(",");
  }
  Serial.write("\n");
  

  delay(300);


}


