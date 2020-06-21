#ifndef calculator_h
#define calculator_h

#include "Arduino.h"

#endif

class Calculator {
    public:
        char c1[19];
        char c2[19];
        char oper;
        char result[19];
        char input_buffer[19];
        int ib_idx;
        
        Calculator();

        void init();
        void input(char key);
        void calculate();

    private:
        char operators[5] = "+-*/";


};
