#include "./memory.h"
#include <stdbool.h>

void half_adder(bool i1, bool i0, bool cin, two_bit_d *out);
void full_adder(bool i1, bool i0, bool cin, two_bit_d *out);
void eight_bit_adder(bool sum_enable, bool cin, bool *cout, eight_bit_d *out,
                     bool a7, bool a6, bool a5, bool a4, bool a3, bool a2,
                     bool a1, bool a0, bool b7, bool b6, bool b5, bool b4,
                     bool b3, bool b2, bool b1, bool b0);


void invert_8bits_latch(eight_bit_d * out, bool invert);