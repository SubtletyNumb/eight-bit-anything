#include "./headers/arithmetic.h"
#include "./headers/control.h"
#include "./headers/memory.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {

  bool clk = 1;
  long long clk_iterations = 0;

  // PROGRAM COUNTER
  edge_ff ffA00 = {0, 1, 0};
  edge_ff ffA01 = {0, 1, 1};
  edge_ff ffA02 = {0, 1, 1};
  edge_ff ffA03 = {0, 1, 1};
  edge_ff ffA04 = {0, 1, 1};
  edge_ff ffA05 = {0, 1, 1};
  edge_ff ffA06 = {0, 1, 1};
  edge_ff ffA07 = {0, 1, 1};
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
  edge_ff acc_ff_0 = {0, 0, 0};
  edge_ff acc_ff_1 = {0, 0, 0};
  edge_ff acc_ff_2 = {0, 0, 0};
  edge_ff acc_ff_3 = {0, 0, 0};
  edge_ff acc_ff_4 = {0, 0, 0};
  edge_ff acc_ff_5 = {0, 0, 0};
  edge_ff acc_ff_6 = {0, 0, 0};
  edge_ff acc_ff_7 = {0, 0, 0};
  edge_ff *acc[8] = {&acc_ff_0, &acc_ff_1, &acc_ff_2, &acc_ff_3,
                     &acc_ff_4, &acc_ff_5, &acc_ff_6, &acc_ff_7};

  // INSTRUCTION LATCH
  eight_bit_d acc_latch_in;
  edge_ff ins_ff_0 = {0, 0, 0};
  edge_ff ins_ff_1 = {0, 0, 0};
  edge_ff ins_ff_2 = {0, 0, 0};
  edge_ff ins_ff_3 = {0, 0, 0};
  edge_ff ins_ff_4 = {0, 0, 0};
  edge_ff ins_ff_5 = {0, 0, 0};
  edge_ff ins_ff_6 = {0, 0, 0};
  edge_ff ins_ff_7 = {0, 0, 0};

  edge_ff *ins[8] = {&ins_ff_0, &ins_ff_1, &ins_ff_2, &ins_ff_3,
                     &ins_ff_4, &ins_ff_5, &ins_ff_6, &ins_ff_7};

  // ADR OUT FOR RAM LATCH
  edge_ff adr_ff_0 = {0, 0, 0};
  edge_ff adr_ff_1 = {0, 0, 0};
  edge_ff adr_ff_2 = {0, 0, 0};
  edge_ff adr_ff_3 = {0, 0, 0};
  edge_ff adr_ff_4 = {0, 0, 0};
  edge_ff adr_ff_5 = {0, 0, 0};
  edge_ff adr_ff_6 = {0, 0, 0};
  edge_ff adr_ff_7 = {0, 0, 0};

  edge_ff *adr[8] = {&adr_ff_0, &adr_ff_1, &adr_ff_2, &adr_ff_3,
                     &adr_ff_4, &adr_ff_5, &adr_ff_6, &adr_ff_7};

  bool adr_carry_in = 0;
  bool adr_carry_out = 0;

  edge_ff insc_ff_0 = {0, 1, 0};
  edge_ff insc_ff_1 = {0, 1, 1};

  // CONTROL SIGNALS
  bool ctrl_sig_read_from_adr_latch = 0;
  bool ctrl_sig_write_ram = 0;
  bool ctrl_sig_add = 0;
  bool ctrl_sig_adc = 0;
  bool ctrl_sig_su = 0;
  bool ctrl_sig_sub = 0;
  bool ctrl_sig_simple_jump = 0;

  int clk_time_in_ms = 200;

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
  write_256x8_ram(0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ram);

  // DATA
  write_256x8_ram(1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, ram);
  write_256x8_ram(1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, ram);

  while (true) {
    clk = !clk;

#ifdef __linux__
    usleep(clk_time_in_ms * 1000);
#elif defined(_WIN32) || defined(_WIN64)
    Sleep(1000);
#endif
    system("clear");
    if (clk) {
      clk_iterations++;
    }

    printf("CLOCK: %d\tCLK EDG RISINGS:%lld\n", clk, clk_iterations);
    // UPDATE COUNTER
    edg_ff_calc_upt(&ffA00, clk, ffA00.qn);
    edg_ff_calc_upt(&ffA01, ffA00.qn, ffA01.qn);
    edg_ff_calc_upt(&ffA02, ffA01.qn, ffA02.qn);
    edg_ff_calc_upt(&ffA03, ffA02.qn, ffA03.qn);
    edg_ff_calc_upt(&ffA04, ffA03.qn, ffA04.qn);
    edg_ff_calc_upt(&ffA05, ffA04.qn, ffA05.qn);
    edg_ff_calc_upt(&ffA06, ffA05.qn, ffA06.qn);
    edg_ff_calc_upt(&ffA07, ffA06.qn, ffA07.qn);

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

    edg_ff_calc_upt(&insc_ff_0, clk, insc_ff_0.qn);
    edg_ff_calc_upt(&insc_ff_1, insc_ff_0.qn, insc_ff_1.qn);
    printf("INSTRUCTION COUNTER OUT:\n");
    printf("%d%d\n\n", insc_ff_1.q, insc_ff_0.q);

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
    ctrl_sig_adc = ins_code == ADC && (insc_ff_1.q && insc_ff_0.q && clk);
    ctrl_sig_su = ins_code == SU && (insc_ff_1.q && insc_ff_0.q && clk);
    ctrl_sig_sub = ins_code == SUB && (insc_ff_1.q && insc_ff_0.q && clk);

    // make inverter for subtracting
    // make sums between numbers greater than eight bits
    eight_bit_adder(ctrl_sig_add || ctrl_sig_adc || ctrl_sig_su || ctrl_sig_sub,
                    (adr_carry_in && ctrl_sig_adc), &adr_carry_out, &adder_out,
                    acc[7]->q, acc[6]->q, acc[5]->q, acc[4]->q, acc[3]->q,
                    acc[2]->q, acc[1]->q, acc[0]->q, ram_out.d[7], ram_out.d[6],
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

    if (ctrl_sig_simple_jump) {
      set_eight_bit_ff_from_ff(ff_pc, adr, clk);
      printf("NEW COUNTER OUT (JMP INSTRUCTION EXECUTED) :\n");
      printf("%d%d%d%d%d%d%d%d\n\n", ffA07.q, ffA06.q, ffA05.q, ffA04.q,
             ffA03.q, ffA02.q, ffA01.q, ffA00.q);
      clk_time_in_ms = 1500;

      insc_ff_0.q = 0;
      insc_ff_0.qn = 1;
      insc_ff_0.c = 0;

      insc_ff_1.q = 0;
      insc_ff_1.qn = 1;
      insc_ff_1.c = 1;
    }

    printf("ACCUMULATOR OUT:\n");
    printf("%d%d%d%d%d%d%d%d\n", acc[7]->q, acc[6]->q, acc[5]->q, acc[4]->q,
           acc[3]->q, acc[2]->q, acc[1]->q, acc[0]->q);

    // RESET INSTRUCTION CLOCK COUNTER
    if (insc_ff_1.q && insc_ff_0.q && !clk) {
      insc_ff_0.q = 0;
      insc_ff_0.qn = 1;
      insc_ff_0.c = 0;

      insc_ff_1.q = 0;
      insc_ff_1.qn = 1;
      insc_ff_1.c = 1;
    }
  }
  return 0;
}
