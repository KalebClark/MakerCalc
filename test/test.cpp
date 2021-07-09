#include <iostream>
#include <string>

#include "calculator.h"

bool driveCalc(std::string inputStr, std::string result);

int main(void) {
    std::cout << "add test " << (driveCalc("2+2=","4") ? "pass" : "fail") << std::endl;
    std::cout << "multiadd test " << (driveCalc("2+3+4=","9") ? "pass" : "fail") << std::endl;
    std::cout << "mult test " << (driveCalc("2*3=","6") ? "pass" : "fail") << std::endl;
    std::cout << "multmult test " << (driveCalc("2*3*4=","24") ? "pass" : "fail") << std::endl;
    std::cout << "div test " << (driveCalc("6/3=","2") ? "pass" : "fail") << std::endl;
    std::cout << "multidiv test " << (driveCalc("12/2/3=","2") ? "pass" : "fail") << std::endl;
    std::cout << "precedence test 1 " << (driveCalc("2*3+4=","10") ? "pass" : "fail") << std::endl;
    std::cout << "precedence test 2 " << (driveCalc("2+3*4=","14") ? "pass" : "fail") << std::endl;
    std::cout << "precedence test 2 " << (driveCalc("2+3/4=","2.75") ? "pass" : "fail") << std::endl;
}

bool driveCalc(std::string inputStr, std::string result) {
    Calculator calculator;
    for (char c : inputStr) {
        calculator.input(c);
    }
    std::cout << "result for " << inputStr << ": " << calculator.result << " expected " << result << std::endl;
    return calculator.result == result;
}