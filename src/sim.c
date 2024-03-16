#include "shell.h"
#include "./instructions/adds_imm.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCIONES PARA SABER SI UN OPCODE ES TAL INSTRUCCION O NO
bool is_adds_imm(uint32_t opcode) { 
    printf("El opcode es %x\n", (opcode & (0xFF << 24)) >> 24);
    return (((opcode & (0xFF << 24)) >> 24) == 0xb1); 
}
bool is_adds_ext(uint32_t opcode) { 
    printf("El opcode es %x\n", (opcode & (0xFF << 24)) >> 24);
    return (((opcode & (0xFF << 24)) >> 24) == 0xab); 
}



// SE RETORNA EL OPCODE
uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction
  uint32_t opcode = 0;
  if (is_adds_imm(instruction)) {
    opcode = 0xb1;
  } else if (is_adds_ext(instruction)) {
    opcode = 0xab;
  }
  return opcode;
}

// VIENDO EL OPCODE RETORNADO DECIDO QUE ACCION TOMAR
void execute(uint32_t opcode, uint32_t instruction) {
  // Execute the instruction
  if (opcode == 0xb1) {
    printf("Entra al imm");
    // Adds immediate
    adds_imm(instruction);
  } else if (opcode == 0xab){
    // Adds extended register
    //adds_ext(instruction);
    printf("Tengo que sumar adds ext");
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