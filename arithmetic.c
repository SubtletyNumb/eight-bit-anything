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
    two_bit_d res_0 = { {0, 0} };

    half_adder(i1, i0, &res_0);

    two_bit_d res_1 = { {0, 0} };

    half_adder(res_0.d[0], cin, &res_1);

    out->d[1] = res_0.d[1] || res_1.d[1]; //carry
    out->d[0] = res_1.d[0]; //sum
}

void eight_bit_adder(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0,
                     bool b7, bool b6, bool b5, bool b4, bool b3, bool b2, bool b1, bool b0,
                     bool cin,
                     eight_bit_adder_out* out)
{
    two_bit_d res_0 = { {0, 0} };
    two_bit_d res_1 = { {0, 0} };
    two_bit_d res_2 = { {0, 0} };
    two_bit_d res_3 = { {0, 0} };
    two_bit_d res_4 = { {0, 0} };
    two_bit_d res_5 = { {0, 0} };
    two_bit_d res_6 = { {0, 0} };
    two_bit_d res_7 = { {0, 0} };

    full_adder(a0, b0, cin, &res_0);
    full_adder(a1, b1, res_0.d[1], &res_1);
    full_adder(a2, b2, res_1.d[1], &res_2);
    full_adder(a3, b3, res_2.d[1], &res_3);
    full_adder(a4, b4, res_3.d[1], &res_4);
    full_adder(a5, b5, res_4.d[1], &res_5);
    full_adder(a6, b6, res_5.d[1], &res_6);
    full_adder(a7, b7, res_6.d[1], &res_7);

    out->d[0] = res_0.d[0];
    out->d[1] = res_1.d[0];
    out->d[2] = res_2.d[0];
    out->d[3] = res_3.d[0];
    out->d[4] = res_4.d[0];
    out->d[5] = res_5.d[0];
    out->d[6] = res_6.d[0];
    out->d[7] = res_7.d[0];
    out->d[8] = res_7.d[1];
}