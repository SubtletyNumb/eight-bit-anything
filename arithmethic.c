#include <stdbool.h>
#include "./memory.h"


void half_adder(bool i1, bool i0, two_bit_d* out)
{
    out->d[1] = i1 && i0; //carry
    out->d[0] = i1 ^ i0; //sum
    return;
}

void full_adder(bool i1, bool i0, bool cin, two_bit_d* out)
{
    struct two_bit_d res_0 = { {0, 0} };

    half_adder(i1, i0, &res_0);

    struct two_bit_d res_1 = { {0, 0} };

    half_adder(res_0.d[0], cin, &res_1);

    out->d[1] = res_0.d[1] || res_1.d[1]; //carry
    out->d[0] = res_0.d[0] && res_1.d[0]; //sum
}