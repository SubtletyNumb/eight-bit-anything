#include <stdbool.h>
#include "./memory.h"

void half_adder(bool i1, bool i0, bool cin, two_bit_d* out);
void full_adder(bool i1, bool i0, bool cin, two_bit_d* out);
void eight_bit_adder(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0,
                     bool b7, bool b6, bool b5, bool b4, bool b3, bool b2, bool b1, bool b0,
                     bool cin,
                     eight_bit_adder_out* out);