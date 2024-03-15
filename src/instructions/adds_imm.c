#include "../shell.h"
#include "adds_imm.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void adds_imm(uint32_t instruction){
    uint32_t immediate = (instruction & (0x11111111111 << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;

    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);

    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + immediate;

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
}