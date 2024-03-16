#include "shell.h"


uint32_t array_opcodes_6 = 0b000101; // B  
uint32_t array_opcodes_8[8] = {0xb1, 0xab, 0xf0, 0b11110001, 0b11101010, 0b1001010, 0b10101010, 0b01010100, };  
// 10110001, 10101011, 11110000    // adds imm, adds ext, subs imm, cmp imm, ands_shit, eor_shift, orr_shift, b.cond,
uint32_t array_opcodes_9[3] = {0b110100110, 0b110100110, 0b110100101}; // lsl_imm, lsr_imm, movz
uint32_t array_opcodes_11[9] = {0b11101011001, 0b11010100010, 0b11101011001, 0b11111000000, 0b00111000000, 0b01111000000, 0b11111000010, 0b01111000010, 0b00111000010};  
// subs ext, hlt, cmp_ext, stur, sturb, sturh, lduzr, ldurh, ldurb
uint32_t array_opcodes_22 = 0b1101011000011111000000;   // BR

uint8_t array_b_cond[6] = {0b0, 0b1, 0b1100, 0b1011, 0b1010, 0b1101};   
//B.:eq, ne, gt, lt, ge, le



uint32_t is_opcode_length_8(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0xFF << 24)) >> 24;
  for (int i = 0; i < 8; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes_8[i]) {
      return i;
    }
  }
  return -1;

}

uint32_t is_opcode_length_9(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b111111111 << 23)) >> 23;
  for (int i = 0; i < 3; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes_9[i]) {
      return i;
    }
  }
  return -1;

}

uint32_t is_opcode_length_11(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b11111111111 << 21)) >> 21;
  for (int i = 0; i < 9; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes_11[i]) {
      return i;
    }
  }
  return -1;

}


uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction

  // Verify if it is an 6 bit opcode
  uint32_t opcode = (instruction & (0b111111 << 26)) >> 26;
  if (opcode == array_opcodes_6) {
    return opcode;
  }

  // Verify if it is an 8 bit opcode
  uint32_t opcode = is_opcode_length_8(instruction, array_opcodes_8);
  if (opcode != -1) {
    return opcode;
  }

  // Verify if it is an 9 bit opcode
  opcode = is_opcode_length_9(instruction, array_opcodes_9);
  if (opcode != -1) {
    return opcode;
  }

  // Verify if it is an 11 bit opcode
  opcode = is_opcode_length_11(instruction, array_opcodes_11);
  if (opcode != -1) {
    return opcode;
  }

  // Verify if it is an 22 bit opcode
  opcode = (instruction & (0b1111111111111111111111 << 10)) >> 10;
  if (opcode == array_opcodes_22) {
    return opcode;
  }

  return opcode;
}

uint8_t decode_b_cond(uint32_t instruction) {
  uint8_t result = instruction & 0b1111;
  for (int i = 0; i < 6; i++) {
    if (result == array_b_cond[i]) {
      return i;
    }
  }
  return -1;
}


/*
// FUNCIONES PARA SABER SI UN OPCODE ES TAL INSTRUCCION O NO
bool is_adds_imm(uint32_t instruction) { 
    printf("El opcode imm es %x\n", (instruction & (0xFF << 24)) >> 24);
    return (((instruction & (0xFF << 24)) >> 24) == 0xb1);   //10110001
}
bool is_adds_ext(uint32_t instruction) { 
    printf("El opcode ext es %x\n", (instruction & (0xFF << 24)) >> 24);
    return (((instruction & (0xFF << 24)) >> 24) == 0xab);    //10101011
}

bool is_subs_imm(uint32_t instruction) {   //VER
    printf("El opcode subs imm es %x\n", (instruction & (0xFF << 24)) >> 24);
    return (((instruction & (0xFF << 24)) >> 24) == 0xf1);   //11110000  (el ult es 0?)
}

bool is_subs_ext(uint32_t instruction) { 
    printf("El opcode subs ext es %d\n", ((instruction & (0b11111111111 << 21)) >> 21) + 1);
    return (((instruction& (0b11111111111 << 21)) >> 21) + 1 == 0b11101011001);    //11101011001
}
*/