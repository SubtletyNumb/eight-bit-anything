#include "./memory.h"
#include <stdbool.h>

int eight_to_one_idx(bool a2, bool a1, bool a0) {
  return (a2 << 2) | (a1 << 1) | a0;
}

bool eight_to_one_selector(bool a2, bool a1, bool a0, eight_bit_d *data) {
  int idx = eight_to_one_idx(a2, a1, a0);
  return data->d[idx];
}

void three_to_eight_decoder(bool a2, bool a1, bool a0, eight_bit_d *out) {
  int idx = eight_to_one_idx(a2, a1, a0);
  for (int i = 0; i < 8; i++) {
    out->d[i] = false;
  }
  out->d[idx] = true;
  return;
}

void write_eight_bit_latch(bool w, bool d7, bool d6, bool d5, bool d4, bool d3,
                           bool d2, bool d1, bool d0, eight_bit_d *db0) {
  if (!w)
    return;
  db0->d[7] = d7;
  db0->d[6] = d6;
  db0->d[5] = d5;
  db0->d[4] = d4;
  db0->d[3] = d3;
  db0->d[2] = d2;
  db0->d[1] = d1;
  db0->d[0] = d0;
}

void read_eight_bit_latch(eight_bit_d *db0, eight_bit_d *out) {
  out->d[0] = db0->d[0];
  out->d[1] = db0->d[1];
  out->d[2] = db0->d[2];
  out->d[3] = db0->d[3];
  out->d[4] = db0->d[4];
  out->d[5] = db0->d[5];
  out->d[6] = db0->d[6];
  out->d[7] = db0->d[7];
}

void write_eight_bit_ff(eight_bit_d *db0, edge_ff *out, bool clk) {
  edg_ff_calc(&out[0], clk, db0->d[0]);
  edg_ff_upt(&out[0], clk);
  edg_ff_calc(&out[1], clk, db0->d[1]);
  edg_ff_upt(&out[1], clk);
  edg_ff_calc(&out[2], clk, db0->d[2]);
  edg_ff_upt(&out[2], clk);
  edg_ff_calc(&out[3], clk, db0->d[3]);
  edg_ff_upt(&out[3], clk);
  edg_ff_calc(&out[4], clk, db0->d[4]);
  edg_ff_upt(&out[4], clk);
  edg_ff_calc(&out[5], clk, db0->d[5]);
  edg_ff_upt(&out[5], clk);
  edg_ff_calc(&out[6], clk, db0->d[6]);
  edg_ff_upt(&out[6], clk);
  edg_ff_calc(&out[7], clk, db0->d[7]);
  edg_ff_upt(&out[7], clk);
}

void write_eight_bit_ff_from_latch(eight_bit_d *db0, edge_ff *out[], bool clk) {
  edg_ff_calc(out[0], clk, db0->d[0]);
  edg_ff_upt(out[0], clk);
  edg_ff_calc(out[1], clk, db0->d[1]);
  edg_ff_upt(out[1], clk);
  edg_ff_calc(out[2], clk, db0->d[2]);
  edg_ff_upt(out[2], clk);
  edg_ff_calc(out[3], clk, db0->d[3]);
  edg_ff_upt(out[3], clk);
  edg_ff_calc(out[4], clk, db0->d[4]);
  edg_ff_upt(out[4], clk);
  edg_ff_calc(out[5], clk, db0->d[5]);
  edg_ff_upt(out[5], clk);
  edg_ff_calc(out[6], clk, db0->d[6]);
  edg_ff_upt(out[6], clk);
  edg_ff_calc(out[7], clk, db0->d[7]);
  edg_ff_upt(out[7], clk);
}

void write_eight_bit_ff_from_ff(edge_ff *db0[], edge_ff *out[], bool clk) {
  edg_ff_calc(out[0], clk, db0[0]->q);
  edg_ff_upt(out[0], clk);
  edg_ff_calc(out[1], clk, db0[1]->q);
  edg_ff_upt(out[1], clk);
  edg_ff_calc(out[2], clk, db0[2]->q);
  edg_ff_upt(out[2], clk);
  edg_ff_calc(out[3], clk, db0[3]->q);
  edg_ff_upt(out[3], clk);
  edg_ff_calc(out[4], clk, db0[4]->q);
  edg_ff_upt(out[4], clk);
  edg_ff_calc(out[5], clk, db0[5]->q);
  edg_ff_upt(out[5], clk);
  edg_ff_calc(out[6], clk, db0[6]->q);
  edg_ff_upt(out[6], clk);
  edg_ff_calc(out[7], clk, db0[7]->q);
  edg_ff_upt(out[7], clk);
}

void read_8x1_edge_trgd_ff(edge_ff *ffs, eight_bit_d *out) {
  for (int i = 7; i >= 0; i--) {
    out->d[i] = ffs[i].q;
  }
}

void edg_ff_calc(edge_ff *st0, bool c, bool d) {
  if (c && !st0->c) {
    st0->q_next = d;
    st0->qn_next = !d;
  } 
}

void edg_ff_upt(edge_ff *st0, bool c) {
  st0->q = st0->q_next;
  st0->qn = st0->qn_next;
  st0->c = c;
}

void edg_ff_init(edge_ff *st0) {
  st0->c = 0;
  st0->q = 0;
  st0->qn = 1;
}

int eight_to_256_idx(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2,
                     bool a1, bool a0) {
  return (a7 << 7) | (a6 << 6) | (a5 << 5) | (a4 << 4) | (a3 << 3) | (a2 << 2) |
         (a1 << 1) | a0;
}

void write_256x8_ram(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2,
                     bool a1, bool a0, bool w, bool d7, bool d6, bool d5,
                     bool d4, bool d3, bool d2, bool d1, bool d0,
                     eight_bit_d *ram_array) {
  if (!w)
    return;

  int idx = eight_to_256_idx(a7, a6, a5, a4, a3, a2, a1, a0);

  write_eight_bit_latch(w, d7, d6, d5, d4, d3, d2, d1, d0, &ram_array[idx]);
}

void read_256x8_ram(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2,
                    bool a1, bool a0, eight_bit_d *ram_array, eight_bit_d *out
                    ) {

  int idx = eight_to_256_idx(a7, a6, a5, a4, a3, a2, a1, a0);

  read_eight_bit_latch(&ram_array[idx], out);
}