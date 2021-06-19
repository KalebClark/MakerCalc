#include "calculator.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const *Calculator::operators = "+-/*";
uint8_t Calculator::operator_precedence[] = {0, 0, 1, 1};

void Calculator::init() {
  this->input_buffer[0] = '\0';
  this->result[0] = '\0';
  this->saw_decimal = false;
  input_pos = 0;
  num_pos = 0;
  oper_pos = 0;
}

/*
 * Handle Keyboard input
 */
void Calculator::input(char key) {
  // Valid chars to add to input_buffer: [0-9], '.'
  if (isdigit(key) || key == '.') { // Regular Input
    if (key == '.') {
      if (saw_decimal)
        return;
      saw_decimal = true;
    }
    if (input_pos < INPUT_BUFFER_SIZE) {
      this->input_buffer[input_pos++] = key;
      this->input_buffer[input_pos] = '\0';
    }
  } else {
    // Store Operand
    double value = strtod(input_buffer, nullptr);
    if (num_pos < NUM_STACK_LENGTH) {
      num_stack[num_pos++] = value;
    }
    saw_decimal = false;
    input_pos = 0;

    if (index(operators, key) != nullptr) { // Operator Input
      int keyPrecedence = precedence(key);
      if ((oper_pos > 0 &&
           precedence(oper_stack[oper_pos - 1]) >= keyPrecedence)) {
        collapse_stack();
      }
      oper_stack[oper_pos++] = key;
    } else if (key == '=') { // Equals.
      collapse_stack();
    }
  }
}

void Calculator::calculate() {}

void Calculator::collapse_stack() {
  double calc_result = 0;
  while (oper_pos > 0) {
    char key = oper_stack[--oper_pos];
    if (num_pos < 2) {
      num_pos = 0;
      oper_pos = 0;
      break;
    }
    double op2 = num_stack[--num_pos];
    double op1 = num_stack[--num_pos];
    switch (key) {
    case '+':
      calc_result = op1 + op2;
      break;
    case '-':
      calc_result = op1 - op2;
      break;
    case '*':
      calc_result = op1 * op2;
      break;
    case '/':
      calc_result = op1 / op2;
      break;
    }
    if (num_pos < NUM_STACK_LENGTH) {
      num_stack[num_pos++] = calc_result;
    }
  }
  snprintf(result, RESULT_SIZE, "%g", calc_result);
}

int Calculator::precedence(char op) {
  char *idx = index((char *)operators, op);
  if (idx == nullptr) {
    return -1;
  }
  return operator_precedence[idx - operators];
}