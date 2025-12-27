#include "./memory.h"

typedef enum {
  LDA = 10,
  STA = 20,
  ADD = 30,
  HLT = 255,
} INSTRUCTIONS;

int get_instruct_code(int d7, int d6, int d5, int d4, int d3, int d2, int d1,
                      int d0);
