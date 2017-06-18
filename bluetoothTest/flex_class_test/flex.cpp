#include "Arduino.h" 
#include "flex.hpp"
#include <EEPROM.h> 

#define FLEX_NUM 4

Flex::Flex()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

static void Flex::attachInterrupt_MAX(int INT_PIN_MAX, int mode)
{
  pinMode(INT_PIN_MAX,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN_MAX), INT_set_max_threshold, mode);
}

static void Flex::attachInterrupt_MIN(uint8_t INT_PIN_MIN, int mode)
{
  pinMode(INT_PIN_MIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN_MIN), INT_set_min_threshold, mode);
}

void Flex::readEEPROM()
{
  for(int i=0;i<_FLEX_NUM;i++){
    threshold[i] = EEPROM.read(i)*4;
  }
}

void Flex::updateFlexData()
{
  for(int i=0; i<FLEX_NUM; i++){
    flex_sensor[i] = analogRead(i);
    if(flex_sensor[i] > threshold[i]){
      fold[i] = '0'; // unfold
    }
    else
      fold[i] = '1'; // fold
  }
}

void Flex::printThreashold(Stream &serial)
{
  serial.print("threshold: ");
  for(int i=0;i<FLEX_NUM;i++){
    serial.print(this->threshold[i]); serial.print(" ");
  }
  serial.print("\n");
}

void Flex::printFoldInfo(Stream &serial)
{
  for(int i=0;i<FLEX_NUM;i++){
    serial.print(fold[i]);
    if(i != FLEX_NUM-1)
      serial.print(",");
  }
  serial.print("\n");
}

void Flex::printADC(Stream &serial)
{
  for(int i=0;i<FLEX_NUM;i++){
    serial.print(flex_sensor[i]);
    if(i != FLEX_NUM-1)
      serial.print(",");
  }
  serial.print("\n");
}

static void Flex::INT_set_max_threshold()
{
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

static void Flex::INT_set_min_threshold()
{
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

