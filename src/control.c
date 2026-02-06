#include "./headers/memory.h"
#include "stdio.h"

int get_instruct_code(int d7, int d6, int d5, int d4, int d3, int d2, int d1,
                      int d0) {
  int idx = eight_to_256_idx(d7, d6, d5, d4, d3, d2, d1, d0);

  return idx;
}

int get_inst_code_from_ffs(edge_ff *ffs[]) {
  int idx = eight_to_256_idx(ffs[7]->q, ffs[6]->q, ffs[5]->q, ffs[4]->q,
                             ffs[3]->q, ffs[2]->q, ffs[1]->q, ffs[0]->q);
  return idx;
}
/*
  CHECK ABOUT STATIC
   WHY IT WAS CAUSING THE PROBLEM
   keep going by putting the adr latch to be the addr in then load it to te
  accumulator we will got the lod completed.
  */

void wrt_ins_latch(bool i1, bool i0, bool clk_in, eight_bit_d *d_in,
                   edge_ff *ins_latch[]) {
  // printf("\nINS ARR POINTER IN FUNCTION: %p\n\n", (void*)ins_latch[1]);
  bool update_signal = false;
  if(!i1 && i0 && clk_in) {
    update_signal = true;
  }
  write_eight_bit_ff_from_latch(d_in, ins_latch, update_signal);
}

void eight_bit_edg_ff_in_sel2_to_1(edge_ff *d1[], edge_ff *d0[],
                                   eight_bit_d *out, bool switcher) {
                                    if(switcher == true || switcher == 1) {
                                      printf("\nShould read from adr latch \n");
                                    }
  out->d[7] = switcher > 0 ? d0[7]->q : d1[7]->q;
  out->d[6] = switcher > 0 ? d0[6]->q : d1[6]->q;
  out->d[5] = switcher > 0 ? d0[5]->q : d1[5]->q;
  out->d[4] = switcher > 0 ? d0[4]->q : d1[4]->q;
  out->d[3] = switcher > 0 ? d0[3]->q : d1[3]->q;
  out->d[2] = switcher > 0 ? d0[2]->q : d1[2]->q;
  out->d[1] = switcher > 0 ? d0[1]->q : d1[1]->q;
  out->d[0] = switcher > 0 ? d0[0]->q : d1[0]->q;
}

void eight_bit_d_in_sel2_to_1(eight_bit_d *d1, eight_bit_d *d0,
                                   eight_bit_d *out, bool switcher) {
  out->d[7] = switcher > 0 ? d0->d[7] : d1->d[7];
  out->d[6] = switcher > 0 ? d0->d[6] : d1->d[6];
  out->d[5] = switcher > 0 ? d0->d[5] : d1->d[5];
  out->d[4] = switcher > 0 ? d0->d[4] : d1->d[4];
  out->d[3] = switcher > 0 ? d0->d[3] : d1->d[3];
  out->d[2] = switcher > 0 ? d0->d[2] : d1->d[2];
  out->d[1] = switcher > 0 ? d0->d[1] : d1->d[1];
  out->d[0] = switcher > 0 ? d0->d[0] : d1->d[0];
}

void edg_ff_calc_upt(edge_ff *st0, bool c, bool d) {
  if(c && !st0->c) {
  st0->q = d;
  st0->qn = !d;
  }
  st0->c = c;
}