#include "Arduino.h"
#include "calculator.h"

Calculator::Calculator() {

}

void Calculator::init() {
    memset(this->input_buffer, 0, 19);
    memset(this->c1, 0, 19);
    memset(this->c2, 0, 19);
    memset(this->result, 0, 19);
    this->ib_idx = 0;
}

/*
 * Handle Keyboard input
 */ 
void Calculator::input(char key) {

    // Valid chars to add to input_buffer: [0-9], '.'
    if(isDigit(key) || key == '.') {            // Regular Input
        this->input_buffer[ib_idx] = key;
        this->input_buffer[ib_idx+1] = '\0';
        this->ib_idx++;
    } else if(strchr(this->operators, key)) {   // Operator Input
        // Store Operator
        this->oper = key;

        // Check to see if this is first value. Store it if it is.
        // If not store in second value.
        if(strlen(this->c1) == 0 && strlen(this->c2) == 0) {
            strncpy(this->input_buffer, this->c1, sizeof(this->input_buffer));
            memset(this->input_buffer, 0, 19);
            this->ib_idx = 0;
        } else if(strlen(this->c2) == 0) {  
            strncpy(this->input_buffer, this->c2, sizeof(this->input_buffer));
            this->calculate();
        }
    } else if(key == '=') {                     // Equals. 

    }
}

void Calculator::calculate() {

}
