#ifndef calculator_h
#define calculator_h

#define INPUT_BUFFER_SIZE 32
#define RESULT_SIZE 32
#define NUM_STACK_LENGTH 20
#define OP_STACK_LENGTH 20

class Calculator {
public:
  char result[RESULT_SIZE + 1];
  char input_buffer[INPUT_BUFFER_SIZE + 1];
  uint16_t input_pos;

  Calculator() { init(); }

  void init();
  void input(char key);
  void calculate();

private:
  static char const *operators;
  static uint8_t operator_precedence[];
  bool saw_decimal;
  double num_stack[NUM_STACK_LENGTH];
  uint8_t num_pos;
  char oper_stack[OP_STACK_LENGTH];
  uint8_t oper_pos;

  void collapse_stack();
  int precedence(char op);
};
#endif
