#include "shell.h"
#include "decoder.h"
#include "functions.h"

typedef struct Node {
  uint32_t opcode;
  void (*function)(uint32_t);
} Node_t;

// VIENDO EL OPCODE RETORNADO DECIDO QUE ACCION TOMAR
void execute(uint32_t opcode, uint32_t instruction) {
  Node_t array_opcodes[24] = {
    {0x91, add_imm},
    {0b10001011000, add_ext_register},
    {0b10011011000, mul},
    {0xb1, adds_imm},
    {0xab, adds_ext_register},
    {0xf1, subs_imm},
    {0b11101011001, subs_ext_register},
    {0xea, ands_shifted_register},
    {0xca, eor_shifted_register},
    {0xaa, orr_shifted_register},
    {0x54, bcond},
    {0b000101, b},
    {0b1101011000011111000000, br},
    {0b110100101, movz},
    {0b1101001101, lsl_lsr_imm},
    {0b11010100010, hlt},
    {0b11111000000, stur_b_h},
    {0b00111000000, stur_b_h},
    {0b01111000000, stur_b_h},
    {0b11111000010, ldur},
    {0b01111000010, ldur},
    {0b00111000010, ldur},
    {0b10110100, cbz},
    {0b10110101, cbnz}
   
  };

  for (int i = 0; i < sizeof(array_opcodes)/sizeof(array_opcodes[0]); i++) {
    if (opcode == array_opcodes[i].opcode) {
      array_opcodes[i].function(instruction);
      return;
    }
  }  
}

void process_instruction() {
  /* execute one instruction here. You should use CURRENT_STATE and modify
   * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
   * access memory.
   *
   * Sugerencia: hagan una funcion para decode()
   *             y otra para execute()
   *
   * */

  uint32_t instruction = mem_read_32(CURRENT_STATE.PC); // Here we have the 32 bits instruction
  // Decode the instruction
  uint32_t opcode = decode(instruction);
  // Execute the instruction
  execute(opcode, instruction);

  // si no cambio en b o bcond
  if (CURRENT_STATE.PC == NEXT_STATE.PC) {NEXT_STATE.PC += 4;}
  
  return;
}