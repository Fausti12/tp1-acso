#include "shell.h"
//#include "./instructions/adds_imm.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// execute functions * pongo funciones para cada instrucción


//uint32_t array_opcodes[3] = [0xb1, 0xab, 0xf0];  // 10110001, 10101011, 11110000



void adds_imm(uint32_t instruction){   //adds immediate
    uint32_t immediate = (instruction & (0xFFF << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;
    printf("d_reg = %x ", dest_register);
    printf("n_reg = %x ", n_register);
    printf("imm = %x ", immediate);
    printf("shift = %x\n", shift);


    uint64_t extend_immediate = 0;
    if (shift == 0b01){
        extend_immediate = immediate;
        extend_immediate = extend_immediate << 12;
    }

    else if (shift == 0b00){
      printf("shift = 0\n");
      extend_immediate = immediate;   //ver si está bien
    }

    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + extend_immediate;

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



// se usa también para cmp imm
void subs_imm(uint32_t instruction){   //adds immediate
    uint32_t immediate = (instruction & (0xFFF << 10)) >> 10;
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;
    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("shift = %d\n", shift);
 
    //solo se hace shift = 01
    if (shift == 0b01){
        immediate = immediate << 12;

    }
    
    // para cmp
    if (dest_register != 0b11111) {NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] - immediate;}

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

    // para cmp
    if (dest_register != 0b11111) {NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] - NEXT_STATE.REGS[m_register];}

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
}



void ands_shifted_register(uint32_t instruction){   //adds immediate
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t immediate = (instruction & (0x7F << 10)) >> 10;    //te dice cuánto shiftear en registro M
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;     //te dice qué tipo de shift (Rm)
    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("shift = %d\n", shift);
    printf("m_reg = %d\n", m_register);



    //shift logical izquierda
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << immediate);}
    // shift logical derecha
    else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}
    //else if (shift == 0b10){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << NEXT_STATE.REGS[immediate]);}
    //else if (shift == 0b11){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> NEXT_STATE.REGS[immediate]);}

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
 

}

}


// exclusive or
void eor_shifted_register(uint32_t instruction){   //adds immediate
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t immediate = (instruction & (0x7F << 10)) >> 10;    //te dice cuánto shiftear en registro M
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;     //te dice qué tipo de shift (Rm)
    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("shift = %d\n", shift);
    printf("m_reg = %d\n", m_register);



    //shift logical izquierda
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] ^ (NEXT_STATE.REGS[m_register] << immediate);}
    // shift logical derecha
    else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}
    //else if (shift == 0b10){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << NEXT_STATE.REGS[immediate]);}
    //else if (shift == 0b11){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> NEXT_STATE.REGS[immediate]);}

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
 

}

}



// or
void orr_shifted_register(uint32_t instruction){   //adds immediate
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t immediate = (instruction & (0x7F << 10)) >> 10;    //te dice cuánto shiftear en registro M
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;     //te dice qué tipo de shift (Rm)
    printf("d_reg = %d ", dest_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d ", immediate);
    printf("shift = %d\n", shift);
    printf("m_reg = %d\n", m_register);



    //shift logical izquierda
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] ^ (NEXT_STATE.REGS[m_register] << immediate);}
    // shift logical derecha
    else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}
    //else if (shift == 0b10){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << NEXT_STATE.REGS[immediate]);}
    //else if (shift == 0b11){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> NEXT_STATE.REGS[immediate]);}

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
 

}

}



bool is_subs_ext(uint32_t instruction) { 
    printf("El opcode subs ext es %d\n", ((instruction & (0b11111111111 << 21)) >> 21) + 1);
    return (((instruction& (0b11111111111 << 21)) >> 21) + 1 == 0b11101011001);    //11101011001
} // sin el + 1 no anda


uint32_t is_opcode_length_8(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0xFF << 24)) >> 24;
  for (int i = 0; i < 5; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return i;
    }
  }
  return -1;

}

uint32_t is_opcode_length_11(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b11111111111 << 21)) >> 21;
  for (int i = 6; i < 8; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return i;
    }
  }
  return -1;

}



// SE RETORNA EL OPCODE
uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction
  uint32_t array_opcodes[8] = {0xb1, 0xab, 0xf1, 0xea ,0xca, 0xaa  ,0b11101011001, 0b11010100010};
  // índice 0-> adds imm, 1-> adds ext, 2-> subs imm, 3-> ands shifted, 4-> eor shifted,
  // 5-> orr shifted, 6-> subs ext (ver pq al final va 1), 5-> hlt


  uint32_t opcode = 0;
  int index = is_opcode_length_8(instruction, array_opcodes);
  printf("El index es %d\n", index);
  if (index != -1) {return array_opcodes[index];}
  int index2 = is_opcode_length_11(instruction, array_opcodes);
  printf("El index2 es %d\n", index2);
  if (index2 != -1) {opcode = array_opcodes[index2];}
  else if (is_subs_ext(instruction)) {
    printf("Entra al subs ext\n");
    opcode = 0b11101011001;
  }
  return opcode;
}

// VIENDO EL OPCODE RETORNADO DECIDO QUE ACCION TOMAR
void execute(uint32_t opcode, uint32_t instruction) {
  // Execute the instruction
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
  else if (opcode == 0xea){
    printf("Tengo que hacer ands ext");
    ands_shifted_register(instruction);
  }
  else if (opcode == 0xca){
    printf("Tengo que hacer eor ext");
    eor_shifted_register(instruction);
  }
  else if (opcode == 0xaa){
    printf("Tengo que hacer orr ext");
    orr_shifted_register(instruction);
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

