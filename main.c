#include "./arithmetic.h"
#include "./control.h"
#include "./memory.h"
#include "unistd.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  bool clk = 1;
  long long clk_iterations = 0;

  // PROGRAM COUNTER
  edge_ff ffA00 = {0, 1, 0, 0, 1};
  edge_ff ffA01 = {0, 1, 1, 0, 1};
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
  eight_bit_d adder_out = {{0, 0, 0, 0, 0, 0, 0, 0}};

  // ACCUMULATOR
  eight_bit_d acc_in = {{0, 0, 0, 0, 0, 0, 0, 0}};
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
  eight_bit_d acc_latch_in;
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
  // 0x7fffffffcd9b index0 ins_ff_0

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

  // CONTROL SIGNALS
  bool ctrl_sig_read_from_adr_latch = 0;
  bool ctrl_sig_write_ram = 0;
  bool ctrl_sig_add = 0;
  bool ctrl_sig_simple_jump = 0;

  bool sum_cin = 0;
  bool sum_cout = 0;

  // INITIAL INSTRUCTIONS
  write_256x8_ram(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, ram);

  // LDA
  write_256x8_ram(0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, ram);
  write_256x8_ram(0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, ram);

  // ADD
  write_256x8_ram(0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, ram);
  write_256x8_ram(0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, ram);

  // STA
  write_256x8_ram(0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, ram);
  write_256x8_ram(0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, ram);

  // JUMP
  write_256x8_ram(0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, ram);
  write_256x8_ram(0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, ram);

  // DATA
  write_256x8_ram(1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, ram);
  write_256x8_ram(1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, ram);

  while (true) {
    clk = !clk;
    sleep(1);
    system("clear");
    if (clk) {
      clk_iterations++;
    }

    printf("CLOCK: %d\tCLK EDG RISINGS:%lld\n", clk, clk_iterations);
    // UPDATE COUNTER
    edg_ff_calc(&ffA00, clk, ffA00.qn);
    edg_ff_upt(&ffA00, clk);
    edg_ff_calc(&ffA01, ffA00.qn, ffA01.qn);
    edg_ff_upt(&ffA01, ffA00.qn);
    // edg_ff_calc(&ffA02, ffA01.qn && clk, ffA02.qn_next);
    // edg_ff_upt(&ffA02, ffA01.qn_next);
    // edg_ff_calc(&ffA03, ffA02.qn_next && clk, ffA03.qn_next);
    // edg_ff_calc(&ffA04, ffA03.qn_next && clk, ffA04.qn_next);
    // edg_ff_upt(&ffA03, ffA02.qn_next);
    // edg_ff_calc(&ffA05, ffA04.qn_next && clk, ffA05.qn_next);
    // edg_ff_upt(&ffA04, ffA03.qn_next);
    // edg_ff_calc(&ffA06, ffA05.qn_next && clk, ffA06.qn_next);
    // edg_ff_upt(&ffA05, ffA04.qn_next);
    // edg_ff_calc(&ffA07, ffA06.qn_next && clk, ffA07.qn_next);
    // edg_ff_upt(&ffA06, ffA05.qn_next);
    // edg_ff_upt(&ffA07, ffA06.qn_next);

    printf("COUNTER OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ffA07.q, ffA06.q, ffA05.q, ffA04.q, ffA03.q,
           ffA02.q, ffA01.q, ffA00.q);

    int ins_code = get_inst_code_from_ffs(ins);
    ctrl_sig_read_from_adr_latch = (ins_code == LDA || ins_code == STA ||
                                    ins_code == ADD || ins_code == JMP) &&
                                   (insc_ff_1.q && !insc_ff_0.q && clk);

    // MAKE SELECTION OF ADDR IN FOR RAM
    eight_bit_edg_ff_in_sel2_to_1(ff_pc, adr, &ram_adr_in,
                                  ctrl_sig_read_from_adr_latch);

    read_256x8_ram(ram_adr_in.d[7], ram_adr_in.d[6], ram_adr_in.d[5],
                   ram_adr_in.d[4], ram_adr_in.d[3], ram_adr_in.d[2],
                   ram_adr_in.d[1], ram_adr_in.d[0], ram, &ram_out);

    printf("RAM OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", ram_out.d[7], ram_out.d[6], ram_out.d[5],
           ram_out.d[4], ram_out.d[3], ram_out.d[2], ram_out.d[1],
           ram_out.d[0]);

    edg_ff_calc(&insc_ff_0, clk, insc_ff_0.qn);
    edg_ff_calc(&insc_ff_1, insc_ff_0.qn_next && clk, insc_ff_1.qn_next);
    edg_ff_upt(&insc_ff_0, clk);
    edg_ff_upt(&insc_ff_1, insc_ff_0.qn_next);
    printf("INSTRUCTION COUNTER OUT:\n");
    printf("%d%d%d\n\n", insc_ff_2.q, insc_ff_1.q, insc_ff_0.q);

    // WRITE INSTRUCTION LATCH
    wrt_ins_latch(insc_ff_1.q, insc_ff_0.q, clk, &ram_out, ins);

    printf("INSTRUCTION CODE:\n");
    printf("%d%d%d%d%d%d%d%d\n", ins[7]->q, ins[6]->q, ins[5]->q, ins[4]->q,
           ins[3]->q, ins[2]->q, ins[1]->q, ins[0]->q);

    // WRITE ADDRESS LATCH
    write_eight_bit_ff_from_latch(&ram_out, adr,
                                  (insc_ff_1.q && !insc_ff_0.q && clk));

    printf("\nADDRESS LATCH OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n\n", adr[7]->q, adr[6]->q, adr[5]->q, adr[4]->q,
           adr[3]->q, adr[2]->q, adr[1]->q, adr[0]->q);

    ctrl_sig_write_ram =
        ins_code == STA && (insc_ff_1.q && !insc_ff_0.q && clk);

    write_256x8_ram(adr[7]->q, adr[6]->q, adr[5]->q, adr[4]->q, adr[3]->q,
                    adr[2]->q, adr[1]->q, adr[0]->q, ctrl_sig_write_ram,
                    acc[7]->q, acc[6]->q, acc[5]->q, acc[4]->q, acc[3]->q,
                    acc[2]->q, acc[1]->q, acc[0]->q, ram);

    ctrl_sig_add = ins_code == ADD && (insc_ff_1.q && insc_ff_0.q && clk);

    eight_bit_adder(ctrl_sig_add, sum_cin, &sum_cout, &adder_out, acc[7]->q,
                    acc[6]->q, acc[5]->q, acc[4]->q, acc[3]->q, acc[2]->q,
                    acc[1]->q, acc[0]->q, ram_out.d[7], ram_out.d[6],
                    ram_out.d[5], ram_out.d[4], ram_out.d[3], ram_out.d[2],
                    ram_out.d[1], ram_out.d[0]);

    eight_bit_d_in_sel2_to_1(&adder_out, &ram_out, &acc_in,
                             get_inst_code_from_ffs(ins) == LDA &&
                                 (insc_ff_1.q && insc_ff_0.q && clk));
    // WRITE ACCUMULATOR LATCH
    write_eight_bit_ff_from_latch(&acc_in, acc,
                                  (insc_ff_1.q && insc_ff_0.q && clk &&
                                   (get_inst_code_from_ffs(ins) == LDA ||
                                    get_inst_code_from_ffs(ins) == ADD)));

    // WRITE JUMP
    ctrl_sig_simple_jump =
        ins_code == JMP && (insc_ff_1.q && !insc_ff_0.q && clk);

    write_eight_bit_ff_from_ff(adr, ff_pc, ctrl_sig_simple_jump);

    printf("ACCUMULATOR OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n", acc[7]->q, acc[6]->q, acc[5]->q, acc[4]->q,
           acc[3]->q, acc[2]->q, acc[1]->q, acc[0]->q);

    // RESET INSTRUCTION CLOCK COUNTER
    // if (insc_ff_1.q && insc_ff_0.q && !clk) {
    //   edg_ff_init(&insc_ff_0);
    //   edg_ff_init(&insc_ff_1);
    // }
  }
  return 0;
}
