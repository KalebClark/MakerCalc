#include "Arduino.h"
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <ClickEncoder.h>
#include <Adafruit_Keypad.h>
#include "display.h"
#include "calculator.h"
#include "unitconv.h"

/*
 * Instantiate Display
 */
//LiquidCrystal_I2C lcd(0x27, 20, 4);
Display *display;

Calculator *calc;
UnitConvertor *unit;

/*
 * Instantiate Rotary Encoder
 */
ClickEncoder *encoder;
int16_t enc_val = 1;        // Setting default to idx1 to bypass basic calc.
                            // Set back to zero if calc is ever completed.
int16_t enc_last = 2;

/*
 * Instantiate Keypad
 */
const byte KP_ROWS = 5;
const byte KP_COLS = 4;
// char keys[KP_ROWS][KP_COLS] = {
//     {'7', '8', '9', 'AC', 'C'},
//     {'4', '5', '6', 'x', '÷'},
//     {'1', '2', '3', '+', '-'},
//     {'0', '.', '/', '=', ' '}
// };
char keys[KP_ROWS][KP_COLS] = {
    {'C', '/', '-', ' '},
    {'A', '*', '+', '='},
    {'9', '6', '3', '/'},
    {'8', '5', '2', '.'},
    {'7', '4', '1', '0'}
};
byte kp_row_pins[KP_ROWS] = {5, 6, 7, 8, 9};
byte kp_col_pins[KP_COLS] = {10, 11, 12, 13};
Adafruit_Keypad kpd = Adafruit_Keypad(makeKeymap(keys), kp_row_pins, kp_col_pins, KP_ROWS, KP_COLS);

/*
 * Misc global vars
 */
char operators[] = "+-*/";
char calc_operator;
char calc_num1[19];
char calc_num2[19];
float calc_res = NULL;
const int btn_up_pin = 3;
const int btn_down_pin = 2;
unsigned long last_int;

/* SETUP                                                               SETUP
 * ========================================================================= */
void setup()
{
    // Serial setup
    Serial.begin(115200);

    display = new Display(0x27, 20, 4);
    display->init();

    calc = new Calculator();
    calc->init();

    unit = new UnitConvertor();
    unit->init();

    // Encoder setup
    encoder = new ClickEncoder(A1, A0, A2, 4);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr);

    // up/down buttons
    pinMode(btn_up_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(btn_up_pin), btn_up, FALLING);
    pinMode(btn_down_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(btn_down_pin), btn_down, FALLING);

    kpd.begin();
}

char input_buffer[19];
int ib_idx = 0;

/* LOOP                                                                LOOP
 * ========================================================================= */
void loop()
{
    // Handle Encoder turns
    enc_val += encoder->getValue();
    if(enc_val != enc_last) {
        enc_last = enc_val;
 

        // Set Limits TODO: Bonus Feature - Loop through menu
        if(enc_val >= (display->um_rows -1)) { enc_val = (display->um_rows -1); }   // Upper Limit
        if(enc_val < 0) { enc_val = 0; }                                            // Lower Limit
        display->enc_val = enc_val;

        Serial.println(enc_val);
        display->draw(); 
    } // END Encoder IF

    // Handle Keypad
    kpd.tick();
    while(kpd.available()) {
        keypadEvent e = kpd.read();
        if(e.bit.EVENT == KEY_JUST_PRESSED) {
            Serial.print("KP Input: "); Serial.println((char)e.bit.KEY);

            // Determine Mode
            if(display->enc_val == 0) {                     // Calculator Mode
                calc->input((char)e.bit.KEY);
                display->draw();
            } else {                                        // Unit Conv Mode
                unit->matrix_idx = display->enc_val;
                unit->flip = display->unit_matrix[display->enc_val][2];
                unit->input((char)e.bit.KEY);
                strcpy(display->input_buffer, unit->input_buffer);
                strcpy(display->result_buffer, unit->result);
                display->draw();
                //unitDebug();
            }
        }
    }

    // Handle Encoder Button
    ClickEncoder::Button enc_btn = encoder->getButton();
    if(enc_btn != ClickEncoder::Open) {
        bool flip = display->flipUnits();
        if(flip) { display->draw(); menuDebug(); }
    }
}
/* ========================== CUSTOM FUNCTIONS ============================= */
/* ========================================================================= */

// Timer Interrupt
void timerIsr() { encoder->service(); }

void calcInput(char key) {
    // Check to see if the key is a operator key. 
    char * p = strchr(operators, key);
    
    if(p) {
        calc_operator = key;
        if(calc_num1 != NULL) { 
            // calc_num1 = display->input_buffer;
            memcpy(calc_num1, display->input_buffer, 19);
            strcat(display->calc_input, calc_num1);
            strcat(display->calc_input, key);
            strcat(display->calc_input, '\0');
            ib_idx = 0;
            memset(display->input_buffer, 0, 19);
        }
    }

    Serial.print("Input1: "); Serial.println(calc_num1);
    Serial.print("Operator: "); Serial.println(calc_operator);
    Serial.print("Input2: "); Serial.println(calc_num2);
    Serial.print("Calc_input: "); Serial.println(display->calc_input);
}

void btn_up() {
    if(millis() - last_int > 10) {
        Serial.println("BUTTON UP");
        display->metric_unit_idx++;
        display->draw();
        last_int = millis();
    }
}

void btn_down() {
    if(millis() - last_int > 10) {
        Serial.println("BUTTON DOWN");
        display->metric_unit_idx--;
        display->draw();
        last_int = millis();
    }
}

void menuDebug() {
    int unit_from   = display->unit_matrix[display->enc_val][0];
    int unit_to     = display->unit_matrix[display->enc_val][1];
    int flip_bit    = display->unit_matrix[display->enc_val][2];
    Serial.println("Menu Debug============================================");
    Serial.print("Loop enc_val: "); Serial.print(enc_val);
    Serial.print(" Display enc_val: "); Serial.println(display->enc_val);

    Serial.print("\tFrom: "); Serial.print(display->units[unit_from].name);
    Serial.print(" To: "); Serial.println(display->units[unit_to].name);
}

void calcDebug() {
    Serial.println("Calculator Debug======================================");
    Serial.print("IB("); Serial.print("): "); Serial.println(calc->input_buffer);
    Serial.print("RES: "); Serial.println(calc->result);

}

void unitDebug() {
    Serial.println("Unit Convertor Debug ================================");
    Serial.print("\tIB("); Serial.print(unit->ib_idx); Serial.print("): "); Serial.println(unit->input_buffer);
    Serial.print("\tResult: "); Serial.println(unit->result);

}
