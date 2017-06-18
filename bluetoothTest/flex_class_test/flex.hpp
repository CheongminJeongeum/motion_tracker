#ifndef FLEX_h
#define FLEX_h

#include "Arduino.h" 
#define FLEX_NUM 4

class Flex
{
public:
  Flex();
  static void attachInterrupt_MAX(int INT_PIN_MAX, int mode);
  static void attachInterrupt_MIN(uint8_t INT_PIN_MIN, int mode);
  void readEEPROM();
  void updateFlexData();
  void printThreashold(Stream &serial);
  void printFoldInfo(Stream &serial);
  void printADC(Stream &serial);
  static void INT_set_max_threshold();
  static void INT_set_min_threshold();
 
private:
  int _FLEX_NUM = FLEX_NUM;
  static volatile int flex_sensor[FLEX_NUM] = {};
  static volatile int threshold[FLEX_NUM] = {330,330,330,330};
  static volatile int MAX_threshold[FLEX_NUM] = {700,700,700,700};
  static volatile int MIN_threshold[FLEX_NUM] = {};
  static volatile int update_flag;
  char fold[FLEX_NUM] = {};
};

#endif
