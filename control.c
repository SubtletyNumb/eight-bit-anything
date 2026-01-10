#include "./memory.h"

int get_instruct_code(int d7, int d6, int d5, int d4, int d3, int d2, int d1,
                      int d0) {
  int idx = eight_to_256_idx(d7, d6, d5, d4, d3, d2, d1, d0);

  return idx;
}

void wrt_ins_latch(bool i1, bool i0,
    bool d7, bool d6, bool d5, bool d4,
                        bool d3, bool d2, bool d1, bool d0,
     edge_ff *ins_latch) {
  static bool clk = false;
  if (!i1 && !i0) {
    clk = true;
  }

  wrt_8_edge_trgd_ff(d7, d6, d5, d4,
                     d3, d2, d1, d0,
                     clk, ins_latch);
}