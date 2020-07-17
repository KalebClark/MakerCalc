#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include "display.h"

Display::Display(uint16_t addr, uint16_t cols, uint16_t rows) {
    _lcd = new LiquidCrystal_I2C(addr, cols, rows);
    num_units = NUM_UNITS;
    um_rows = UM_ROWS;
    um_cols = UM_COLS;
}

void Display::init() {
    _lcd->init();
    _lcd->backlight();
    _lcd->clear();
    this->metric_unit_idx = 0;
    memset(this->input_buffer, 0, 19);
    memset(this->result_buffer, 0, 19);
    // memset(this->calc_input, 0, 19);
    // this->calc_input[0] = 0;
}

void Display::draw() {
    int unit_from_idx   = unit_matrix[this->enc_val][0];
    int unit_to_idx     = unit_matrix[this->enc_val][1];
    int flip_bit        = unit_matrix[this->enc_val][2];

    // If we are in calculator mode, draw calculator.
    if(unit_from_idx == 0 || unit_to_idx == 0) { this->drawCalc(); return; }

    // Clear and draw statics
    _lcd->clear();

    _lcd->setCursor(0, 2); _lcd->print(">");
    _lcd->setCursor(0, 3); _lcd->print("Res: ");

    // Draw to/From
    _lcd->setCursor(0, 0);
    _lcd->print(units[unit_from_idx].name); _lcd->print(" -> "); _lcd->print(units[unit_to_idx].name);

    // Draw variable data

    _lcd->setCursor(2, 2); _lcd->print(this->input_buffer);
    //_lcd->setCursor(18, 2); _lcd->print(metric_units[this->metric_unit_idx].name);
    _lcd->setCursor(5, 3); _lcd->print(this->result_buffer);
}

void Display::drawCalc() {
    _lcd->clear();
    _lcd->setCursor(0, 0); _lcd->print("Basic Calculator");
}

bool Display::flipUnits() {
    // Store in temp var to not overwrite.
    int tmp_to      = unit_matrix[this->enc_val][0];
    int tmp_from    = unit_matrix[this->enc_val][1];
    
    // Swap indexes in unit matrix.
    unit_matrix[this->enc_val][0] = tmp_from;
    unit_matrix[this->enc_val][1] = tmp_to;

    // Flip the flip flipper bit
    if(unit_matrix[this->enc_val][2] == 0) { unit_matrix[this->enc_val][2] = 1; } else
    if(unit_matrix[this->enc_val][2] == 1) { unit_matrix[this->enc_val][2] = 0; }

    return true;
    
}