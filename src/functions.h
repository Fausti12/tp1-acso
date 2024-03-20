#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "shell.h"

void adds_imm(uint32_t instruction);

void adds_ext_register(uint32_t instruction);

void subs_imm(uint32_t instruction);

void subs_ext_register(uint32_t instruction);

void ands_shifted_register(uint32_t instruction);

void eor_shifted_register(uint32_t instruction);

void orr_shifted_register(uint32_t instruction);

void b(uint32_t instruction);

void br(uint32_t instruction);

void bcond(uint32_t instruction);  

void lsl_lsr_imm(uint32_t instruction);

void stur_b_h(uint32_t instruction);

void ldur(uint32_t instruction);

void movz(uint32_t instruction);

void hlt(uint32_t instruction);

void cbz(uint32_t instruction);

void cbnz(uint32_t instruction);