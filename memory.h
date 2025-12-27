#pragma once
#include <stdbool.h>

typedef struct
{
    bool d[8];
} eight_bit_d;

typedef struct
{
	bool d[9];
} eight_bit_adder_out;

typedef struct
{
    bool q;
    bool qn;
    bool c;
} edge_ff;

typedef struct
{
    bool d[2];
} two_bit_d;


int eight_to_one_idx(bool a2, bool a1, bool a0);
int eight_to_256_idx(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0);


bool eight_to_one_selector(bool a2, bool a1, bool a0, eight_bit_d* data);

void three_to_eight_decoder(bool a2, bool a1, bool a0, eight_bit_d* out);

void read_eight_bit_latch(eight_bit_d* db0, eight_bit_d* out);

void read_three_times_eight_ram(bool a2, bool a1, bool a0,
                                eight_bit_d* out,
                                eight_bit_d* db0, eight_bit_d* db1,
                                eight_bit_d* db2, eight_bit_d* db3,
                                eight_bit_d* db4, eight_bit_d* db5,
                                eight_bit_d* db6, eight_bit_d* db7
                               );
							   
void read_8x1_edge_trgd_ff(edge_ff* ffs, eight_bit_d* out);

void wrt_8_edge_trgd_ff(bool d7, bool d6, bool d5, bool d4,
						bool d3, bool d2, bool d1, bool d0,
                        bool clk, edge_ff* ffs);							   
							   

void write_eight_bit_latch(bool w,
                           bool d7, bool d6, bool d5, bool d4, bool d3, bool d2, bool d1, bool d0,
                           eight_bit_d* db0);

void write_three_times_eight_ram(bool a2, bool a1, bool a0,
                                 bool w,
                                 bool d7, bool d6, bool d5, bool d4, bool d3, bool d2, bool d1, bool d0,
                                 eight_bit_d* db0, eight_bit_d* db1,
                                 eight_bit_d* db2, eight_bit_d* db3,
                                 eight_bit_d* db4, eight_bit_d* db5,
                                 eight_bit_d* db6, eight_bit_d* db7
                                );

void edg_ff_upt(edge_ff* st0, bool c, bool d);

void read_256x8_ram(
    bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0,
    eight_bit_d* ram_array,
    eight_bit_d* out
);

void write_256x8_ram(
    bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0,
    bool w,
    bool d7, bool d6, bool d5, bool d4, bool d3, bool d2, bool d1, bool d0,
    eight_bit_d* ram_array
);