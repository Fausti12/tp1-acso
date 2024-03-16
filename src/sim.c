#include "shell.h"
#include "decoder.h"
//#include "./instructions/adds_imm.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void adds_imm(uint32_t instruction){   //adds immediate
    uint32_t immediate = (instruction & (0xFFF << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;

    printf("d_reg = %x ", dest_register);
    printf("n_reg = %x ", n_register);
    printf("imm = %x ", immediate);
    printf("shift = %x\n", shift);

    if (shift == 0b01){
        immediate = immediate << 12;

    }
    

    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + immediate;

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
}




void adds_ext_register(uint32_t instruction){   //adds extended register
    uint32_t immediate = (instruction & (0x7 << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t option = (instruction & (0x7 << 13)) >> 13;
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;

    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("option = %d ", option);
    printf("m_reg = %d\n", m_register);

  
    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + NEXT_STATE.REGS[m_register];

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
}




void subs_imm(uint32_t instruction){   //adds immediate
    uint32_t immediate = (instruction & (0xFFF << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;
    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("shift = %d\n", shift);

    if (shift == 0b01){
        immediate = immediate << 12;

    }
    

    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] - immediate;

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
}


void subs_ext_register(uint32_t instruction){   //adds extended register
    uint32_t immediate = (instruction & (0x7 << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t option = (instruction & (0x7 << 13)) >> 13;
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;

    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("option = %d ", option);
    printf("m_reg = %d\n", m_register);

  
    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] - NEXT_STATE.REGS[m_register];

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
}



// VIENDO EL OPCODE RETORNADO DECIDO QUE ACCION TOMAR
void execute(uint32_t opcode, uint32_t instruction) {
  // Execute the instruction

  // Extraxt the opcode from the instruction
  opcode = decode(instruction);

  if (opcode == 0xb1) {
    printf("Entra al imm\n");
    // Adds immediate
    adds_imm(instruction);
  } 
  else if (opcode == 0xab){
    // Adds extended register
    //adds_ext(instruction);
    printf("Tengo que sumar adds ext");
    adds_ext_register(instruction);
    

  } 
  
  else if (opcode == 0xf1){
    // Substracts immediate
    printf("Tengo que restar subs imm");
    subs_imm(instruction);
    
    }
  else if (opcode == 0b11101011001){
    // Substracts extended register
    printf("Tengo que restar subs ext");
    subs_ext_register(instruction);
  }
  else if (opcode == 0b11010100010){
    printf("Halt\n");
    RUN_BIT= 0;
  }

  else if (opcode == 0b01010100){  //B.CONDITIONS
    if (decode_b_cond(instruction) == 0){
      printf("B.eq\n");
      //B.eq();
    }
    else if (decode_b_cond(instruction) == 1){
      printf("B.ne\n");
      //B.ne();
    }
    else if (decode_b_cond(instruction) == 0b1100){
      printf("B.gt\n");
      //B.gt();
    }
    else if (decode_b_cond(instruction) == 0b1011){
      printf("B.lt\n");
      //B.lt();
    }
    else if (decode_b_cond(instruction) == 0b1010){
      printf("B.ge\n");
      //B.ge();
    }
    else if (decode_b_cond(instruction) == 0b1101){
      printf("B.le\n");
      //B.le();
    }
  }
  
  else{
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

  //printf("array opcodes: %x\n", array_opcodes[0]);
  uint32_t instruction = mem_read_32(CURRENT_STATE.PC); // Here we have the 32 bits instruction
  printf("Instruction: %x\n", instruction);
  // Decode the instruction
  uint32_t opcode = decode(instruction);

  // Execute the instruction
  execute(opcode, instruction);

  // Update the PC
  NEXT_STATE.PC += 4;
  
  return;
}

