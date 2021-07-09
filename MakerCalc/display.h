#ifndef Display_h
#define Display_h

#include "Arduino.h"

typedef struct {
  char name[10];
  int metric;
} Unit;

typedef struct {
  char name[2];
  long mult;
} Metric_Units;

#define NUM_UNITS 10
#define UM_ROWS 11
#define UM_COLS 3

class Display {
public:
  int enc_val;
  int um_rows;
  int um_cols;
  int num_units;
  int metric_unit_idx;
  char input_buffer[19];
  char result_buffer[19];
  char calc_input[19];

  Display(uint16_t addr, uint16_t cols, uint16_t rows);

  Unit units[NUM_UNITS] = {
      {"Calculator", 0}, // 0
      {"Metric", 1},     // 1
      {"Frac", 0},       // 2
      {"Tenths", 0},     // 3
      {"Lumen", 0},      // 4
      {"Candela", 0},    // 5
      {"MPH", 0},        // 6
      {"KPH", 0},        // 7
      {"in/s", 0},       // 8
      {"mm/s", 0},       // 9
  };

  Metric_Units metric_units[4] = {
      {"mm", 1},       // 0
      {"cm", 10},      // 1
      {"m", 1000},     // 2
      {"km", 1000000}, // 3
  };

  int unit_matrix[UM_ROWS][UM_COLS] = {
      // | IDX |   Conversion   |    Flipped     |
      {0, 0, 0}, // |  0  |       ---- Calculator ----      |
      {1, 2, 0}, // |  1  | Metric->Frac   | Frac->Metric   |
      {1, 3, 0}, // |  2  | Metric->Tenths | Tenths->Metric |
      {2, 3, 0}, // |  3  | Frac->Tenths   | Tenths->Frac   |
      {4, 5, 0}, // |  4  | Lumen->Candela | Candela->Lumen |
      {6, 7, 0}, // |  5  | MPH->KPH       | KPH->MPH       |
      {6, 8, 0}, // |  6  | MPH->in/s      | in/s->MPH      |
      {6, 9, 0}, // |  7  | MPH->mm/s      | mm/s->MPH      |
      {7, 8, 0}, // |  8  | KPH->in/s      | in/s->KPH      |
      {7, 9, 0}, // |  9  | KPH->mm/s      | mm/s->KPH      |
      {8, 9, 0}  // | 10  | in/s->mm/s     | mm/s->in/s     |
  };
  void init();
  void draw();
  void drawCalc();
  bool flipUnits();

private:
  LiquidCrystal_I2C *_lcd;
};

#endif