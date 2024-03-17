#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t is_opcode_length_11(instruction, array_opcodes_11);
uint32_t is_opcode_length_9(instruction, array_opcodes_9);
uint32_t is_opcode_length_8(instruction, array_opcodes_8);

uint32_t decode(instruction);
uint8_t decode_b_cond(uint32_t instruction);


