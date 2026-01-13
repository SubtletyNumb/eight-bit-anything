#include "./arithmetic.h"
#include "./control.h"
#include "./memory.h"
#include "unistd.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  bool clk = 1;

  // PROGRAM COUNTER
  edge_ff ffA00 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA00);
  edge_ff ffA01 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA01);
  edge_ff ffA02 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA02);
  edge_ff ffA03 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA03);
  edge_ff ffA04 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA04);
  edge_ff ffA05 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA05);
  edge_ff ffA06 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA06);
  edge_ff ffA07 = {0, 0, 0, 0, 0};
  edg_ff_init(&ffA07);
  edge_ff *ff_pc[8] = {&ffA00, &ffA01, &ffA02, &ffA03,
                       &ffA04, &ffA05, &ffA06, &ffA07};

  // RAM
  eight_bit_d ram[256];
  eight_bit_d ram_adr_in;

  eight_bit_d ram_out = {{0, 0, 0, 0, 0, 0, 0, 0}};

  // ADDER OUT
  eight_bit_adder_out adder_out = {{0, 0, 0, 0, 0, 0, 0, 0, 0}};

  // ACCUMULATOR
  edge_ff acc_ff_0 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_0);
  edge_ff acc_ff_1 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_1);
  edge_ff acc_ff_2 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_2);
  edge_ff acc_ff_3 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_3);
  edge_ff acc_ff_4 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_4);
  edge_ff acc_ff_5 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_5);
  edge_ff acc_ff_6 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_6);
  edge_ff acc_ff_7 = {0, 0, 0, 0, 0};
  edg_ff_init(&acc_ff_7);
  edge_ff *acc[8] = {&acc_ff_0, &acc_ff_1, &acc_ff_2, &acc_ff_3,
                     &acc_ff_4, &acc_ff_5, &acc_ff_6, &acc_ff_7};

  // INSTRUCTION LATCH
  edge_ff ins_ff_0 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_0);
  edge_ff ins_ff_1 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_1);
  edge_ff ins_ff_2 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_2);
  edge_ff ins_ff_3 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_3);
  edge_ff ins_ff_4 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_4);
  edge_ff ins_ff_5 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_5);
  edge_ff ins_ff_6 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_6);
  edge_ff ins_ff_7 = {0, 0, 0, 0, 0};
  edg_ff_init(&ins_ff_7);

  edge_ff *ins[8] = {&ins_ff_0, &ins_ff_1, &ins_ff_2, &ins_ff_3,
                     &ins_ff_4, &ins_ff_5, &ins_ff_6, &ins_ff_7};

  // ADR OUT FOR RAM LATCH
  edge_ff adr_ff_0 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_0);
  edge_ff adr_ff_1 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_1);
  edge_ff adr_ff_2 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_2);
  edge_ff adr_ff_3 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_3);
  edge_ff adr_ff_4 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_4);
  edge_ff adr_ff_5 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_5);
  edge_ff adr_ff_6 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_6);
  edge_ff adr_ff_7 = {0, 0, 0, 0, 0};
  edg_ff_init(&adr_ff_7);

  edge_ff *adr[8] = {&adr_ff_0, &adr_ff_1, &adr_ff_2, &adr_ff_3,
                     &adr_ff_4, &adr_ff_5, &adr_ff_6, &adr_ff_7};

  edge_ff insc_ff_0 = {0, 0, 0, 0, 0};
  edg_ff_init(&insc_ff_0);
  edge_ff insc_ff_1 = {0, 0, 0, 0, 0};
  edg_ff_init(&insc_ff_1);
  edge_ff insc_ff_2 = {0, 0, 0, 0, 0};
  edg_ff_init(&insc_ff_2);

  // INITIAL INSTRUCTIONS

  // LOD

  write_256x8_ram(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, ram);
  write_256x8_ram(0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, ram);
  write_256x8_ram(0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, ram);

  // DATA
  write_256x8_ram(1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, ram);

  while (true) {
    clk = !clk;
    sleep(1);
    system("clear");

    // UPDATE COUNTER
    edg_ff_calc(&ffA00, clk, ffA00.qn);
    edg_ff_calc(&ffA01, ffA00.qn_next, ffA01.qn_next);
    edg_ff_upt(&ffA00, clk);
    edg_ff_calc(&ffA02, ffA01.qn_next, ffA02.qn_next);
    edg_ff_upt(&ffA01, ffA00.qn);
    edg_ff_calc(&ffA03, ffA02.qn_next, ffA03.qn_next);
    edg_ff_upt(&ffA02, ffA01.qn);
    edg_ff_calc(&ffA04, ffA03.qn_next, ffA04.qn_next);
    edg_ff_upt(&ffA03, ffA02.qn);
    edg_ff_calc(&ffA05, ffA04.qn_next, ffA05.qn_next);
    edg_ff_upt(&ffA04, ffA03.qn);
    edg_ff_calc(&ffA06, ffA05.qn_next, ffA06.qn_next);
    edg_ff_upt(&ffA05, ffA04.qn);
    edg_ff_calc(&ffA07, ffA06.qn_next, ffA07.qn_next);
    edg_ff_upt(&ffA06, ffA05.qn);
    edg_ff_upt(&ffA07, ffA06.qn);

    printf("COUNTER OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ffA07.q, ffA06.q, ffA05.q, ffA04.q, ffA03.q,
           ffA02.q, ffA01.q, ffA00.q);


    //UPDATE ADDR IN FOR RAM
    eight_bit_edg_ff_in_sel2_to_1(ff_pc, adr, &ram_adr_in,
                        get_inst_code_from_ffs(ins) == LDA && (!insc_ff_0.q && insc_ff_1.q));

    read_256x8_ram(ram_adr_in.d[7], ram_adr_in.d[6], ram_adr_in.d[5],
                   ram_adr_in.d[4], ram_adr_in.d[3], ram_adr_in.d[2],
                   ram_adr_in.d[1], ram_adr_in.d[0], ram, &ram_out);

    printf("RAM OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ram_out.d[7], ram_out.d[6], ram_out.d[5],
           ram_out.d[4], ram_out.d[3], ram_out.d[2], ram_out.d[1],
           ram_out.d[0]);

    edg_ff_calc(&insc_ff_0, clk, insc_ff_0.qn);
    edg_ff_calc(&insc_ff_1, insc_ff_0.qn_next, insc_ff_1.qn_next);
    edg_ff_upt(&insc_ff_0, clk);
    edg_ff_upt(&insc_ff_1, insc_ff_0.qn_next);
    printf("INSTRUCTION COUNTER OUT:\n");
    printf("%d%d%d\n\n", insc_ff_2.q, insc_ff_1.q, insc_ff_0.q);


    wrt_ins_latch(insc_ff_1.q, insc_ff_0.q, clk, &ram_out, ins);

    printf("INSTRUCTION CODE:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ins[7]->q, ins[6]->q, ins[5]->q, ins[4]->q,
           ins[3]->q, ins[2]->q, ins[1]->q, ins[0]->q);

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
