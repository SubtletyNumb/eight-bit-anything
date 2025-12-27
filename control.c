#include "./memory.h"

int get_instruct_code(int d7, int d6, int d5, int d4, int d3, int d2, int d1,
                       int d0) {
                        int idx = eight_to_256_idx(d7, d6, d5, d4, d3, d2, d1, d0);
                        
                        return idx;
                       }

