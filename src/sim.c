#include "shell.h"
#//include "decoder.h"
//#include "./instructions/adds_imm.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//uint32_t array_opcodes[3] = [0xb1, 0xab, 0xf0];  // 10110001, 10101011, 11110000


typedef struct Node {
  uint32_t opcode;
  void (*function)(uint32_t);
} Node_t;

uint32_t is_opcode_length_8(uint32_t instruction, uint32_t* array_opcodes) {
  printf("instruction = %x\n", instruction);
  printf("array op [0] = %x\n", array_opcodes[0]);
  uint32_t result =  (instruction & (0xFF << 24)) >> 24;
  printf("result = %x\n", result);
  for (int i = 0; i < 8; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      printf("El valor de i es %d\n", i);
      return array_opcodes[i];
    }
  }
  return -1;

}

uint32_t is_opcode_length_9(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b111111111 << 23)) >> 23;
  for (int i = 0; i < 3; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return array_opcodes[i];
    }
  }
  return -1;

}

uint32_t is_opcode_length_11(uint32_t instruction, uint32_t* array_opcodes) {
  uint32_t result =  (instruction & (0b11111111111 << 21)) >> 21;
  for (int i = 0; i < 9; i++) {    //i llega hasta la cantidad de opcodes de ese largo
    //printf("El valor array es %x\n", array_opcodes[i]);
    if (result == array_opcodes[i]) {
      return array_opcodes[i];
    }
  }
  return -1;

}







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
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] | (NEXT_STATE.REGS[m_register] << immediate);}
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



uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction
  uint32_t array_opcodes_6 = 0b000101; // B  
  //uint32_t array_opcodes_8[8] = {0xb1, 0xab, 0xf1, 0xea, 0xaa, 0b11101010, 0b1001010, 0xaa, 0b01010100, };  
// 10110001, 10101011, 11110000    // adds imm, adds ext, subs imm, cmp imm, ands_shit, eor_shift, orr_shift, b.cond,
  uint32_t array_opcodes_9[3] = {0b110100110, 0b110100110, 0b110100101}; // lsl_imm, lsr_imm, movz
  uint32_t array_opcodes_11[9] = {0b11101011001, 0b11010100010, 0b11101011001, 0b11111000000, 0b00111000000, 0b01111000000, 0b11111000010, 0b01111000010, 0b00111000010};  
// subs ext, hlt, cmp_ext, stur, sturb, sturh, lduzr, ldurh, ldurb
  uint32_t array_opcodes_22 = 0b1101011000011111000000;   // BR

  uint8_t array_b_cond[6] = {0b0, 0b1, 0b1100, 0b1011, 0b1010, 0b1101};   
//B.:eq, ne, gt, lt, ge, le



  uint32_t array_opcodes_8[8] = {0xb1, 0xab, 0xf1, 0xea ,0xca, 0xaa  ,0b11101011001, 0b11010100010};
  // índice 0-> adds imm, 1-> adds ext, 2-> subs imm, 3-> ands shifted, 4-> eor shifted,
  // 5-> orr shifted, 6-> subs ext (ver pq al final va 1), 7-> hlt


  // Verify if it is an 6 bit opcode
  uint32_t opcode = (instruction & (0b111111 << 26)) >> 26;
  if (opcode == array_opcodes_6) {return opcode;}

  // Verify if it is an 8 bit opcode
  opcode = is_opcode_length_8(instruction, array_opcodes_8);
  printf("El opcode es %d\n", opcode);
  if (opcode != -1) {return opcode;}

  // Verify if it is an 9 bit opcode
  opcode = is_opcode_length_9(instruction, array_opcodes_9);
  if (opcode != -1) { return opcode; }

  // Verify if it is an 11 bit opcode
  opcode = is_opcode_length_11(instruction, array_opcodes_11);
  if (opcode != -1) {return opcode;}

  if (is_subs_ext(instruction)) {return 0b11101011001;}

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




// VIENDO EL OPCODE RETORNADO DECIDO QUE ACCION TOMAR
void execute(uint32_t opcode, uint32_t instruction) {
  Node_t array_opcodes[8] = {
    {0xb1, adds_imm},
    {0xab, adds_ext_register},
    {0xf1, subs_imm},
    {0b11101011001, subs_ext_register},
    {0xea, ands_shifted_register},
    {0xca, eor_shifted_register},
    {0xaa, orr_shifted_register},
    {0b01010100, 0}
  };

  for (int i = 0; i < 8; i++) {
    if (opcode == array_opcodes[i].opcode) {
      printf("Entra al if\n");
      array_opcodes[i].function(instruction);
      return;
    }
  }  
  
  if (opcode == 0b01010100){  //B.CONDITIONS
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

