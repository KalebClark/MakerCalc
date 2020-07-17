#include <Arduino.h>
#include "unitconv.h"


UnitConvertor::UnitConvertor() {

}

void UnitConvertor::init() {
    memset(this->input_buffer, 0, 19);
    memset(this->result, 0, 19);
    this->ib_idx = 0;
    this->flip = 0;
}

void UnitConvertor::input(char key) {
    // Check if Equals or input.
    if(key == '=') {                // Do Conversion
        this->convert();
    } else if(key == 'C') {         // Clear
        this->init();
    } else {                        // Read values into buffer
        this->input_buffer[ib_idx] = key;
        this->input_buffer[ib_idx+1] = '\0';
        this->ib_idx++;
    }
}

void UnitConvertor::convert() {
    switch(this->matrix_idx) {
        case 1:
            // TODO: START
            this->metricFrac();
            break;
        case 2:
            this->metricTenths();
            break;
        case 3:
            this->fractTenths();
            break;
        case 4:
            // TODO: FINISH
            this->lumenCandela();
            break;
        case 5:
            this->mphKph();
            break;
        case 6:
            this->mphIns();
            break;
        case 7:
            this->mphMms();
            break;
        case 8:
            this->kphIns();
            break;
        case 9:
            this->kphMms();
            break;
        case 10:
            this->insMms();
            break;
    }
}

void UnitConvertor::metricFrac() {
    float input = atof(this->input_buffer);

    if(this->flip) {            // Fractional -> Metric
        char tmp_input[19];
        strcpy(tmp_input, this->input_buffer);
        char* token = strtok(tmp_input, " "); long whole_number = atol(token);
        token = strtok(NULL, "/"); long numerator = atol(token);
        token = strtok(NULL, "/"); long denominator = atol(token);

                // Do the math
        float dec = (float)numerator / (float)denominator;
        float res = (float)whole_number + dec;
        res = (res * 25.4);

        // TODO: Add metric unit designator

        dtostrf(res, 0, 7, this->result);


    } else {                    // Metric -> Fractional
        
        // Convert to mm
        float metric_input = (input / 25.4);
        
        this->dectofrac(metric_input);
    }
}

void UnitConvertor::metricTenths() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // Tenths -> Metric
        res = (input * 25.4);
    } else {                    // Metric -> Tenths
        res = (input / 25.4);
    }
    
    dtostrf(res, 0, 4, this->result);    
}

void UnitConvertor::fractTenths() {
    float input = atof(this->input_buffer);

    if(this->flip) {
        dectofrac(input);
    } else {
        // Split fraction into whole, numerator, denominator.
        char tmp_input[19];
        strcpy(tmp_input, this->input_buffer);
        char* token = strtok(tmp_input, " "); long whole_number = atol(token);
        token = strtok(NULL, "/"); long numerator = atol(token);
        token = strtok(NULL, "/"); long denominator = atol(token);

        // Serial.print("WN: "); Serial.println(whole_number);
        // Serial.print("NU: "); Serial.println(numerator);
        // Serial.print("DE: "); Serial.println(denominator);

        // Do the math
        float dec = (float)numerator / (float)denominator;
        float res = (float)whole_number + dec;


        dtostrf(res, 0, 7, this->result);

    }
}

void UnitConvertor::lumenCandela() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // Candela -> Lumens
        res = input / ((2*PI) - cos(0/2));
    } else {                    // Lumens -> Candela

    }

    dtostrf(res, 0, 4, this->result);
}

void UnitConvertor::mphKph() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // KPH -> MPH
        res = (input / 1.60934);
    } else {                    // MPH -> KPH
        res = (input * 1.60934);
    }

    dtostrf(res, 0, 4, this->result);
}

void UnitConvertor::mphIns() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // inch/sec -> MPH
        res = (input / 17.6);
    } else {                    // MPH -> inch/sec
        res = (input * 17.6);
    }

    dtostrf(res, 0, 4, this->result);
}

void UnitConvertor::mphMms() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // mm/s -> MPH
        res = (input / 447.04);
    } else {                    // MPH -> mm/s
        res = (input * 447.04);
    }

    dtostrf(res, 0, 4, this->result);
}

void UnitConvertor::kphIns() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // Inch/Sec -> KPH
        res = (input / 10.9361);
    } else {                    // KPH -> Inch/Sec
        res = (input * 10.9361);
    }

    dtostrf(res, 0, 4, this->result);
}

void UnitConvertor::kphMms() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // mm/s -> KPH
        res = (input / 277.778);
    } else {                    // KPH -> mm/s
        res = (input * 277.778);
    }

    dtostrf(res, 0, 4, this->result);
}

void UnitConvertor::insMms() {
    float input = atof(this->input_buffer);
    float res;
    if(this->flip) {            // mm/s -> Inch/Sec
        res = (input / 25.4);
    } else {                    // Inch/Sec -> mm/s
        res = (input * 25.4);
    }

    dtostrf(res, 0, 4, this->result);
}


// Utility Functions

/*
 * getMultiplier()
 * This function separates the number to the right
 * of the decimal place, counts the digits and 
 * returns the multiplier factor for converting
 * decimal to fractions.
 */
long UnitConvertor::getMultiplier(char *input) {
    char* charDec = strtok(input, ".");
    charDec = strtok(NULL, ".");
    int i = strlen(charDec);
    long multiplier = pow(10, (i + 0.000005));

    return multiplier;
}

/*
 * gcd()
 * Returns the greatest common denominator from the 
 * provided numberator and denominator.
 */
int UnitConvertor::gcd(int a, int b) {
  int R;
  while((a % b) > 0) {
    R = a % b;
    a = b;
    b = R;
  }
  return b;    
}

void UnitConvertor::dectofrac(float input) {
    static char res[19];

    // Split whole number and decimal
    int whole_number = (int)input;
    float decimal = input - whole_number;

    long multiplier = this->getMultiplier(this->input_buffer);

    long numerator = ((decimal + 0.000001) * multiplier);
    long denominator = (1 * multiplier);

    int gcd_val = gcd(numerator, denominator);

    // Serial.println("DECTOFRAC");
    // Serial.print("Whole Number: "); Serial.println(whole_number);
    // Serial.print("Decimal: "); Serial.println(decimal, 7);
    // Serial.print("Multiplier: "); Serial.println(multiplier);
    // Serial.print("GCD: "); Serial.println(gcd_val);
    // Serial.print("Numerator: "); Serial.println(numerator);
    // Serial.print("Denominator: "); Serial.println(denominator);

    // Reduce!
    numerator = numerator / gcd_val;
    denominator = denominator / gcd_val;

    // Serial.print("\tNumerator: "); Serial.println(numerator);
    // Serial.print("\tDenominator: "); Serial.println(denominator);

    sprintf(res, "%d %ld/%ld", whole_number, numerator, denominator);
    Serial.print("result: "); Serial.println(res);

    strcpy(this->result, res);
}

long* UnitConvertor::splitFraction() {
    char input[19];
    long whole_number;
    long numerator;
    long denominator;
    long ret[2];
    strcpy(input, this->input_buffer);
    char* token = strtok(input, " ");      // WHOLE Number
    whole_number = atol(token);
    token = strtok(NULL, "/");
    numerator = atol(token);
    token = strtok(NULL, "/");
    denominator = atol(token);

    // Serial.print("FIRST: "); Serial.println(whole_number);
    // Serial.print("SECOND: "); Serial.println(numerator);
    // Serial.print("THIRD: "); Serial.println(denominator);

    ret[0] = whole_number;
    ret[1] = numerator;
    ret[2] = denominator;

    return ret;
}

// This works and its magic. No touchy
char* UnitConvertor::subStr(char* input_string, char separator, int segment_number) {
  char *act, *sub, *ptr;
  static char copy[100];
  int i;

  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}
