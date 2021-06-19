#ifndef UnitConv_h
#define UnitConv_h

#include "Arduino.h"

class UnitConvertor {
public:
  char input_buffer[19];
  int ib_idx;
  int matrix_idx;
  bool flip;
  char result[19];

  UnitConvertor();

  void init();
  void input(char key);
  void convert();
  void metricFrac();
  void metricTenths();
  void fractTenths();
  void lumenCandela();
  void mphKph();
  void mphIns();
  void mphMms();
  void kphIns();
  void kphMms();
  void insMms();
  long getMultiplier(char *input);
  char *subStr(char *input_string, char separator, int segment_number);
  long *splitFraction();

private:
  int gcd(int a, int b);
  void dectofrac(float input);

  // char* subStr(char* input_string, char separator, int segment_number);
};

#endif