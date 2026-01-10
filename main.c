#include "./arithmetic.h"
#include "./control.h"
#include "./memory.h"
#include "unistd.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int exit = 0;

  bool clk = 0;

  // PROGRAM COUNTER
  edge_ff ffA00 = {0, 0, 0};
  edge_ff ffA01 = {0, 0, 0};
  edge_ff ffA02 = {0, 0, 0};
  edge_ff ffA03 = {0, 0, 0};
  edge_ff ffA04 = {0, 0, 0};
  edge_ff ffA05 = {0, 0, 0};
  edge_ff ffA06 = {0, 0, 0};
  edge_ff ffA07 = {0, 0, 0};

  // RAM
  eight_bit_d ram[256];
  eight_bit_d ram_out = {{0, 0, 0, 0, 0, 0, 0, 0}};

  // ADDER OUT
  eight_bit_adder_out adder_out = {{0, 0, 0, 0, 0, 0, 0, 0, 0}};

  // ACCUMULATOR
  edge_ff acc_ff_0 = {0, 0, 0};
  edge_ff acc_ff_1 = {0, 0, 0};
  edge_ff acc_ff_2 = {0, 0, 0};
  edge_ff acc_ff_3 = {0, 0, 0};
  edge_ff acc_ff_4 = {0, 0, 0};
  edge_ff acc_ff_5 = {0, 0, 0};
  edge_ff acc_ff_6 = {0, 0, 0};
  edge_ff acc_ff_7 = {0, 0, 0};
  edge_ff acc[8] = {acc_ff_0, acc_ff_1, acc_ff_2, acc_ff_3,
                    acc_ff_4, acc_ff_5, acc_ff_6, acc_ff_7};

  // INSTRUCTION LATCH
  edge_ff ins_ff_0 = {0, 0, 0};
  edge_ff ins_ff_1 = {0, 0, 0};
  edge_ff ins_ff_2 = {0, 0, 0};
  edge_ff ins_ff_3 = {0, 0, 0};
  edge_ff ins_ff_4 = {0, 0, 0};
  edge_ff ins_ff_5 = {0, 0, 0};
  edge_ff ins_ff_6 = {0, 0, 0};
  edge_ff ins_ff_7 = {0, 0, 0};

  edge_ff ins[8] = {ins_ff_0, ins_ff_1, ins_ff_2, ins_ff_3,
                    ins_ff_4, ins_ff_5, ins_ff_6, ins_ff_7};

  edge_ff insc_ff_0 = {0, 0, 0};
  edge_ff insc_ff_1 = {0, 0, 0};
  edge_ff insc[2] = {insc_ff_0, insc_ff_1};

  // INITIAL INSTRUCTIONS
  write_256x8_ram(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, ram);

  write_256x8_ram(0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, ram);

  write_256x8_ram(0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, ram);

  while (true) {
    clk = !clk;
    sleep(1);
    system("clear");

    // UPDATE COUNTER
    edg_ff_upt(&ffA00, clk, ffA00.qn);
    edg_ff_upt(&ffA01, ffA00.qn, ffA01.qn);
    edg_ff_upt(&ffA02, ffA01.qn, ffA02.qn);
    edg_ff_upt(&ffA03, ffA02.qn, ffA03.qn);
    edg_ff_upt(&ffA04, ffA03.qn, ffA04.qn);
    edg_ff_upt(&ffA05, ffA04.qn, ffA05.qn);
    edg_ff_upt(&ffA06, ffA05.qn, ffA06.qn);
    edg_ff_upt(&ffA07, ffA06.qn, ffA07.qn);

    printf("COUNTER OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ffA07.q, ffA06.q, ffA05.q, ffA04.q, ffA03.q,
           ffA02.q, ffA01.q, ffA00.q);

    read_256x8_ram(ffA07.q, ffA06.q, ffA05.q, ffA04.q, ffA03.q, ffA02.q,
                   ffA01.q, ffA00.q, ram, &ram_out);

    printf("RAM OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ram_out.d[7], ram_out.d[6], ram_out.d[5],
           ram_out.d[4], ram_out.d[3], ram_out.d[2], ram_out.d[1],
           ram_out.d[0]);

    edg_ff_upt(&insc_ff_0, clk, insc_ff_0.qn);
    edg_ff_upt(&insc_ff_1, insc_ff_0.qn, insc_ff_1.qn);
    printf("INSTRUCTION COUNTER OUT:\n");
    printf("%d%d\n\n", insc_ff_1.q, insc_ff_0.q);

    wrt_ins_latch(insc_ff_1.q, insc_ff_0.q, ram_out.d[7], ram_out.d[6],
                  ram_out.d[5], ram_out.d[4], ram_out.d[3], ram_out.d[2],
                  ram_out.d[1], ram_out.d[0], ins);

    /* PROCESS TO ADD ACCUMULATOR WITH RAM OUT */

    /*

    eight_bit_adder(acc[7].q, acc[6].q, acc[5].q, acc[4].q, acc[3].q, acc[2].q,
                    acc[1].q, acc[0].q, ram_out.d[7], ram_out.d[6],
                    ram_out.d[5], ram_out.d[4], ram_out.d[3], ram_out.d[2],
                    ram_out.d[1], ram_out.d[0], 0, &adder_out);

    wrt_8_edge_trgd_ff(adder_out.d[7], adder_out.d[6], adder_out.d[5],
                       adder_out.d[4], adder_out.d[3], adder_out.d[2],
                       adder_out.d[1], adder_out.d[0], clk, acc);
                       */

    // printf("TOTAL OUT:\n");
    // printf("%d%d%d%d%d%d%d%d\n\n", acc[7].q, acc[6].q, acc[5].q, acc[4].q,
    //        acc[3].q, acc[2].q, acc[1].q, acc[0].q);
  }
  return 0;
}
