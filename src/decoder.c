#include "decoder.h"

bool is_subs_ext(uint32_t instruction) { 
    printf("El opcode subs ext es %d\n", ((instruction & (0b11111111111 << 21)) >> 21) + 1);
    return (((instruction& (0b11111111111 << 21)) >> 21) + 1 == 0b11101011001);    //11101011001
} // sin el + 1 no anda

uint32_t is_opcode_length_8(uint32_t instruction, uint32_t* array_opcodes) {
  printf("instruction = %x\n", instruction);
  uint32_t result =  (instruction & (0xFF << 24)) >> 24;
  for (int i = 0; i < 8; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return array_opcodes[i];
    }
  }
  return -1;

}

uint32_t is_opcode_length_10(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b111111111 << 22)) >> 22;
  for (int i = 0; i < 2; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return array_opcodes[i];
    }
  }
  return -1;

}

uint32_t is_opcode_length_11(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b11111111111 << 21)) >> 21;
  for (int i = 0; i < 11; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return array_opcodes[i];
    }
  }
  return -1;

}

uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction
  uint32_t array_opcodes_6 = 0b000101; // B  

  uint32_t array_opcodes_8[8] = {0xb1, 0xab, 0xf1, 0xea, 0xaa, 0xca, 0x54, 0x91};  
  // adds_imm, adds_ext, subs_imm, cmp_imm, ands_shift, eor_shift, orr_shift, b.cond, add_imm,
  
  uint32_t array_opcodes_9 = 0b110100101; // movz
  
  uint32_t array_opcodes_10 = 0b1101001101; // lsl_imm, lsr_imm SON IGUALES
  
  uint32_t array_opcodes_11[11] = {0b11101011001, 0b11010100010, 0b11101011001, 0b11111000000, 0b00111000000, 
                                  0b01111000000, 0b11111000010, 0b01111000010, 0b00111000010, 0b10001011000, 0b10011011000};  
  // subs ext, hlt, cmp_ext, stur, sturb, sturh, ldur, ldurh, ldurb, add_ext, mul
  
  uint32_t array_opcodes_22 = 0b1101011000011111000000;   // BR


  uint8_t array_b_cond[6] = {0b0, 0b1, 0b1100, 0b1011, 0b1010, 0b1101};   
  //B.:eq, ne, gt, lt, ge, le

  // Verify if it is an 6 bit opcode
  uint32_t opcode = (instruction & (0b111111 << 26)) >> 26;
  if (opcode == array_opcodes_6) {return opcode;}

  // Verify if it is an 8 bit opcode
  opcode = is_opcode_length_8(instruction, array_opcodes_8);
  if (opcode != -1) {
    printf("El opcode de 8 bits es %x\n", opcode);
    return opcode;}

  // Verify if it is an 9 bit opcode
  opcode = (instruction & (0b111111111 << 23)) >> 23;
  //printf("El opcode de 9 bits es %x\n", opcode);
  if (opcode == array_opcodes_9) {return opcode;}

  // Verify if it is an 10 bit opcode
  opcode = (instruction & (0b1111111111 << 22)) >> 22;
  if (opcode == array_opcodes_10) { 
    printf("El opcode de 10 bits es %x\n", opcode);
    return opcode; }

  // Verify if it is an 11 bit opcode
  opcode = is_opcode_length_11(instruction, array_opcodes_11);
  if (opcode != -1) {return opcode;}

  if (is_subs_ext(instruction)) {return 0b11101011001;}   // ????????

  // Verify if it is an 22 bit opcode
  opcode = (instruction & (0b1111111111111111111111 << 10)) >> 10;
  if (opcode == array_opcodes_22) {return opcode;}

  return opcode;
}

uint8_t decode_b_cond(uint32_t instruction) {
  uint8_t array_b_cond[6] = {0b0, 0b1, 0b1100, 0b1011, 0b1010, 0b1101};  
  uint8_t result = instruction & 0b1111;
  for (int i = 0; i < 6; i++) {
    if (result == array_b_cond[i]) {
      return i;
    }
  }
  return -1;
}