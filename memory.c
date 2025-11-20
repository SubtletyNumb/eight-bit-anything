#include <stdbool.h>
#include "./memory.h"

int eight_to_one_idx(bool a2, bool a1, bool a0)
{
    return (a2 << 2) | (a1 << 1) | a0;
}

bool eight_to_one_selector(bool a2, bool a1, bool a0, eight_bit_d* data)
{
    int idx = eight_to_one_idx(a2, a1, a0);
    return data->d[idx];
}

void three_to_eight_decoder(bool a2, bool a1, bool a0, eight_bit_d* out)
{
    int idx = eight_to_one_idx(a2, a1, a0);
    for (int i = 0; i < 8; i++)
    {
        out->d[i] = false;
    }
    out->d[idx] = true;
    return;
}

void write_eight_bit_latch(bool w,
                           bool d7, bool d6, bool d5, bool d4, bool d3,
                           bool d2, bool d1, bool d0,
                           eight_bit_d* db0)
{
    if (!w) return;
    db0->d[7] = d7;
    db0->d[6] = d6;
    db0->d[5] = d5;
    db0->d[4] = d4;
    db0->d[3] = d3;
    db0->d[2] = d2;
    db0->d[1] = d1;
    db0->d[0] = d0;
}


void read_eight_bit_latch(eight_bit_d* db0, eight_bit_d* out)
{
    out->d[0] = db0->d[0];
    out->d[1] = db0->d[1];
    out->d[2] = db0->d[2];
    out->d[3] = db0->d[3];
    out->d[4] = db0->d[4];
    out->d[5] = db0->d[5];
    out->d[6] = db0->d[6];
    out->d[7] = db0->d[7];
}

void w_eight_edg_trg_ff(bool d7, bool d6, bool5, bool d4,
                        bool d3, bool d2, bool d1, bool d0,
                        bool clock, edge_ff* ffs)
{

    if(clock && !ffs->d[0].c)
    {
        ffs->[7].q = d7;
        ffs->[7].qn = !d7;

        ffs->[6].q = d6;
        ffs->[6].qn = !d6;

        ffs->[5].q = d5;
        ffs->[5].qn = !d5;

        ffs->[4].q = d4;
        ffs->[4].qn = !d4;

        ffs->[3].q = d3;
        ffs->[3].qn = !d3;

        ffs->[2].q = d2;
        ffs->[2].qn = !d2;

        ffs->[1].q = d1;
        ffs->[1].qn = !d1;

        ffs->[0].q = d0;
        ffs->[0].qn = !d0;
    }
    for(int i = 7; i >= 0; i--)
    {
        ffs->d[i].c = clock;
    }
}


void write_three_times_eight_ram(bool a2, bool a1, bool a0,
                                 bool w,
                                 bool d7, bool d6, bool d5, bool d4, bool d3,
                                 bool d2, bool d1, bool d0,
                                 eight_bit_d* db0, eight_bit_d* db1,
                                 eight_bit_d* db2, eight_bit_d* db3,
                                 eight_bit_d* db4, eight_bit_d* db5,
                                 eight_bit_d* db6, eight_bit_d* db7
                                )
{
    if (!w) return;
    eight_bit_d adr = { {0, 0, 0, 0, 0, 0, 0, 0} };
    three_to_eight_decoder(a2, a1, a0, &adr);
    write_eight_bit_latch(adr.d[7], d7, d6, d5, d4, d3, d2, d1, d0, db0);
    write_eight_bit_latch(adr.d[6], d7, d6, d5, d4, d3, d2, d1, d0, db1);
    write_eight_bit_latch(adr.d[5], d7, d6, d5, d4, d3, d2, d1, d0, db2);
    write_eight_bit_latch(adr.d[4], d7, d6, d5, d4, d3, d2, d1, d0, db3);
    write_eight_bit_latch(adr.d[3], d7, d6, d5, d4, d3, d2, d1, d0, db4);
    write_eight_bit_latch(adr.d[2], d7, d6, d5, d4, d3, d2, d1, d0, db5);
    write_eight_bit_latch(adr.d[1], d7, d6, d5, d4, d3, d2, d1, d0, db6);
    write_eight_bit_latch(adr.d[0], d7, d6, d5, d4, d3, d2, d1, d0, db7);
}


void read_three_times_eight_ram(bool a2, bool a1, bool a0,
                                eight_bit_d* out,
                                eight_bit_d* db0, eight_bit_d* db1,
                                eight_bit_d* db2, eight_bit_d* db3,
                                eight_bit_d* db4, eight_bit_d* db5,
                                eight_bit_d* db6, eight_bit_d* db7
                               )
{
    eight_bit_d adr = { {0, 0, 0, 0, 0, 0, 0, 0} };
    three_to_eight_decoder(a2, a1, a0, &adr);
    if (adr.d[7])
    {
        read_eight_bit_latch(out, db0);
    }
    else if (adr.d[6])
    {
        read_eight_bit_latch(out, db1);
    }
    else if (adr.d[5])
    {
        read_eight_bit_latch(out, db2);
    }
    else if (adr.d[4])
    {
        read_eight_bit_latch(out, db3);
    }
    else if (adr.d[3])
    {
        read_eight_bit_latch(out, db4);
    }
    else if (adr.d[2])
    {
        read_eight_bit_latch(out, db5);
    }
    else if (adr.d[1])
    {
        read_eight_bit_latch(out, db6);
    }
    else if (adr.d[0])
    {
        read_eight_bit_latch(out, db7);
    }
}

void edg_ff_upt(edge_ff* st0, bool c, bool d)
{
    if (c && !st0->c)
    {
        st0->q = d;
        st0->qn = !st0->q;
    }
    st0->c = c;
}

int eight_to_256_idx(bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0)
{
    return (a7 << 7) | (a6 << 6) | (a5 << 5) | (a4 << 4) |
           (a3 << 3) | (a2 << 2) | (a1 << 1) | a0;
}

void write_256x8_ram(
    bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0,
    bool w,
    bool d7, bool d6, bool d5, bool d4, bool d3, bool d2, bool d1, bool d0,
    eight_bit_d* ram_array
)
{
    if (!w) return;

    int idx = eight_to_256_idx(a7, a6, a5, a4, a3, a2, a1, a0);

    write_eight_bit_latch(w, d7, d6, d5, d4, d3, d2, d1, d0, &ram_array[idx]);
}

void read_256x8_ram(
    bool a7, bool a6, bool a5, bool a4, bool a3, bool a2, bool a1, bool a0,
    eight_bit_d* ram_array,
    eight_bit_d* out
)
{

    int idx = eight_to_256_idx(a7, a6, a5, a4, a3, a2, a1, a0);

    read_eight_bit_latch(&ram_array[idx], out);
}