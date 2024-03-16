#include "shell.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// la función add tiene de opcode 10010001 entonces hay que hacer una máscara
// para obtener los 8 bits de opcode
bool is_add(uint32_t opcode) { return ((opcode & (0xFF << 24)) >> 24) == 0x91; }


uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction
  uint32_t opcode = 0;
  if (is_add(instruction)) {
    opcode = 0x91;
  }
  return opcode;
}

void execute(uint32_t opcode, uint32_t instruction) {
  // Execute the instruction
  if (opcode == 0x91) {
    // Add
    printf("Ejecuto Add\n");
  } else{
    printf("No se ejecuto nada\n");
  }
  return;
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

  // Update the PC
  NEXT_STATE.PC += 4;
  

  return;
}


int main() {
  process_instruction();
  return 0;
}