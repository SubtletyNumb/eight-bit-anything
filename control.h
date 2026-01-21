#include "./memory.h"

typedef enum {
  LDA = 10,
  STA = 20,
  ADD = 30,
  HLT = 255,
} INSTRUCTIONS;

int get_instruct_code(int d7, int d6, int d5, int d4, int d3, int d2, int d1,
                      int d0);

void wrt_ins_latch(bool i1, bool i0, bool clk_in, eight_bit_d *d_in,
                   edge_ff *ins_latch[]);

void eight_bit_edg_ff_in_sel2_to_1(edge_ff *d1[], edge_ff *d0[],
                                   eight_bit_d *out, bool switcher);

void eight_bit_d_in_sel2_to_1(eight_bit_d *d1, eight_bit_d *d0,
                              eight_bit_d *out, bool switcher);

int get_inst_code_from_ffs(edge_ff *ffs[]);