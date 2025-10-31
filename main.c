#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <stdlib.h>
#include "./memory.h"
#include "./arithmetic.h"

int main(int argc, char* argv[])
{
    int exit = 0;
    bool clock = 0;

    edge_ff ffA00 = { 0, 0, 0 };
    edge_ff ffA01 = { 0, 0, 0 };
    edge_ff ffA02 = { 0, 0, 0 };
    edge_ff ffA03 = { 0, 0, 0 };
    edge_ff ffA04 = { 0, 0, 0 };
    edge_ff ffA05 = { 0, 0, 0 };
    edge_ff ffA06 = { 0, 0, 0 };
    edge_ff ffA07 = { 0, 0, 0 };

    eight_bit_d my_ram[256];

    write_256x8_ram(
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
        0, 0, 0, 0, 0, 0, 0, 1,
        my_ram
    );

    write_256x8_ram(
        0, 0, 0, 0, 0, 0, 0, 1,
        1,
        0, 0, 0, 0, 0, 0, 1, 0,
        my_ram
    );
	
    write_256x8_ram(
        0, 0, 0, 0, 0, 0, 1, 0,
        1,
        0, 0, 0, 0, 0, 0, 1, 1,
        my_ram
    );

    eight_bit_d data_read_out =
    {
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

	two_bit_d add_out = { {0, 0} };
	
	eight_bit_adder_out adder_out = { {0, 0, 0, 0, 0, 0, 0, 0, 0} };
	
	eight_bit_adder(0, 0, 0, 1, 0, 0, 0, 1,
					0, 0, 1, 0, 0, 1, 0, 1,
					0,
					&adder_out);
					
	printf("%d\t%d%d%d%d%d%d%d%d", adder_out.d[8],
			adder_out.d[7], adder_out.d[6], adder_out.d[5],
			adder_out.d[4], adder_out.d[3], adder_out.d[2],
			adder_out.d[1], adder_out.d[0]);
			
	scanf("%d", &exit);
    // while (true)
    // {
        // clock = !clock;
        // Sleep(1000);
        // system("cls");

        // edg_ff_upt(&ffA00, clock, ffA00.qn);
        // edg_ff_upt(&ffA01, ffA00.qn, ffA01.qn);
        // edg_ff_upt(&ffA02, ffA01.qn, ffA02.qn);
        // edg_ff_upt(&ffA03, ffA02.qn, ffA03.qn);
        // edg_ff_upt(&ffA04, ffA03.qn, ffA04.qn);
        // edg_ff_upt(&ffA05, ffA04.qn, ffA05.qn);
        // edg_ff_upt(&ffA06, ffA05.qn, ffA06.qn);
        // edg_ff_upt(&ffA07, ffA06.qn, ffA07.qn);


        // printf("COUNTER OUT:\n");
        // printf("%d%d%d%d%d%d%d%d\n\n",
               // ffA07.q, ffA06.q,
               // ffA05.q, ffA04.q, ffA03.q,
               // ffA02.q, ffA01.q, ffA00.q
              // );


        // read_256x8_ram(
            // ffA07.q, ffA06.q,
            // ffA05.q, ffA04.q, ffA03.q,
            // ffA02.q, ffA01.q, ffA00.q,
            // my_ram,
            // &data_read_out
        // );


        // printf("RAM OUT:\n");
        // printf("%d%d%d%d%d%d%d%d\n",
               // data_read_out.d[7], data_read_out.d[6],
               // data_read_out.d[5], data_read_out.d[4], data_read_out.d[3],
               // data_read_out.d[2], data_read_out.d[1], data_read_out.d[0]
              // );

    
    return 0;
}
