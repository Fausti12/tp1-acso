#include "shell.h"
//include "decoder.h"
//#include "./instructions/adds_imm.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    //shift logical izquierda
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << immediate);}
    // shift logical derecha
    else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}
    //else if (shift == 0b10){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << NEXT_STATE.REGS[immediate]);}
    //else if (shift == 0b11){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> NEXT_STATE.REGS[immediate]);}
    /*
    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
 

}
*/
}


// exclusive or
void eor_shifted_register(uint32_t instruction){   //adds immediate
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t immediate = (instruction & (0x7F << 10)) >> 10;    //te dice cuánto shiftear en registro M
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;     //te dice qué tipo de shift (Rm)
  //shift logical izquierda
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] ^ (NEXT_STATE.REGS[m_register] << immediate);}
    // shift logical derecha
    else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}
    //else if (shift == 0b10){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << NEXT_STATE.REGS[immediate]);}
    //else if (shift == 0b11){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> NEXT_STATE.REGS[immediate]);}
   /*
    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;}
   */



}



// or
void orr_shifted_register(uint32_t instruction){   //adds immediate
    uint32_t dest_register = instruction & 0x1F;
    uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
    uint32_t immediate = (instruction & (0x7F << 10)) >> 10;    //te dice cuánto shiftear en registro M
    uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
    uint32_t shift = (instruction & (0x3 << 22)) >> 22;     //te dice qué tipo de shift (Rm)
    //shift logical izquierda
    if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] | (NEXT_STATE.REGS[m_register] << immediate);}
    // shift logical derecha
    else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}
    //else if (shift == 0b10){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << NEXT_STATE.REGS[immediate]);}
    //else if (shift == 0b11){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> NEXT_STATE.REGS[immediate]);}
/*
    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
 

}
*/

}


void b(uint32_t instruction){
  int32_t imm = instruction & (0xFFFFFFF >>2) ;   //1ros 26 bits
  int64_t offset = imm << 2;
  NEXT_STATE.PC = CURRENT_STATE.PC + offset;
}


//ver
void br(uint32_t instruction){
  uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
  NEXT_STATE.PC = CURRENT_STATE.REGS[n_register];
}


void bcond(uint32_t instruction){
  uint32_t condition = instruction & 0xF;
  int32_t imm = (instruction & (0b1111111111111111111 << 5)) >> 5;  //ver pq da mal en beq.x aunque de bien el imm y condition

  int64_t offset = imm << 2;  //ver si es correcto
  bool negative = false;
  // Verifica si el bit 21 es 1 (indicando un valor negativo)
  printf("El offset es %x\n", offset);
  if ((offset & 0x100000) == 0x100000) {
    // Convierte el offset a un valor negativo
    offset = ((~offset) + 1);
    printf("offset == %x\n", offset);
    offset = offset & 0x1FFFFF;
    printf("offset == %x\n", offset);
    negative = true;
    
    } 
  printf("El inmediato es %x\n", imm);
  printf("El offset es %x\n", offset);
  printf("current pc es %x\n", CURRENT_STATE.PC);
  printf("pc + offset es %x\n", CURRENT_STATE.PC + offset);
  if (condition == 0b0000){   //equal
    if (CURRENT_STATE.FLAG_Z == 1){
      if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
      else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
    }
  }
  else if (condition == 0b0001){  //not equal
    if (CURRENT_STATE.FLAG_Z == 0){
      if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
      else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
    }
  }
  else if (condition == 0b1100){  //greater than
    if (CURRENT_STATE.FLAG_N == 0){
      if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
      else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
    }
  }
  else if (condition == 0b1011){   //less than
    if (CURRENT_STATE.FLAG_N == 1){
     if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
      else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
    }
  }

  //Ver si es correcto
  else if (condition == 0b1010){
    if (CURRENT_STATE.FLAG_N == 0 && CURRENT_STATE.FLAG_Z == 0){
      NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
  }
  else if (condition == 0b1101){
    if (CURRENT_STATE.FLAG_N == 1 || CURRENT_STATE.FLAG_Z == 1){
      NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
  }
}

// LSL immediate
void lsl_lsr_imm(uint32_t instruction){   
    uint32_t dest_register = instruction & 0b11111;
    uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
    uint32_t immediate = (instruction & (0b111111 << 16)) >> 16;

    uint32_t mask = 0b111111 << 10;
    // Si la máscara da 0b111111 es LSR, sino es LSL
    if (((instruction & mask) >> 10) == 0b111111){
        NEXT_STATE.REGS[dest_register] = CURRENT_STATE.REGS[n_register] >> (64 - immediate);
    } else {
        printf("LSL %d\n", CURRENT_STATE.REGS[n_register]);
        NEXT_STATE.REGS[dest_register] = CURRENT_STATE.REGS[n_register] << (64 - immediate);
    }

  /*  
    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
    }
    */
}


// STUR --> stur X1, [X2, #0x10] (descripción: M[X2 + 0x10] = X1)
// QUEDA EN DUDA SI CON LOS PRIMEROS BITS SE REFIERE A LOS MÁS SIGNIFICATIVOS O MENOS SIGNIFICATIVOS
void stur_b_h(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111;
    uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
    int64_t immediate = (instruction & (0b111111111 << 12)) >> 12;
    // Si es 11 --> stur, si es 01 --> sturh, si es 00 --> sturb
    uint32_t mask = 0b11 << 30;
    uint32_t type = (instruction & mask) >> 30;

    if (type == 0b11){          // STUR
        mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register]);
    } else if (type == 0b01){   // STURH
        //mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register] & (0xFFFF << 16));
        mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register] & (0xFFFF ));
    } else {                    // STURB
        //mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register] & (0xFF << 24));
        mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register] & (0xFF ));
    }

    // Y también supongo que no hay que actualizar los flags

}


// LDUR --> ldur X1, [X2, #0x10] (descripción: X1 = M[X2 + 0x10])
// QUEDA EN DUDA SI CON LOS PRIMEROS BITS SE REFIERE A LOS MÁS SIGNIFICATIVOS O MENOS SIGNIFICATIVOS
void ldur(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111;
    uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
    int64_t immediate = (instruction & (0b111111111 << 12)) >> 12;

    printf("d_reg = %d ", t_register);
    printf("n_reg = %d ", n_register);
    printf("imm = %d\n", immediate);

    // Si es 11 --> ldur, si es 01 --> ldurh, si es 00 --> ldurb
    uint32_t mask = 0b11 << 30;
    uint32_t type = (instruction & mask) >> 30;

    if (type == 0b11) { // LDUR
        uint64_t lower_half = mem_read_32(NEXT_STATE.REGS[n_register] + immediate);
        uint64_t upper_half = mem_read_32(NEXT_STATE.REGS[n_register] + immediate + 4);
        int64_t result = ((uint64_t)upper_half << 32) | (uint64_t) lower_half;
        printf("lower_half = %x\n", lower_half);
        printf("upper_half = %x\n", upper_half);
        printf("El resultado es %x\n", result);
        NEXT_STATE.REGS[t_register] = result;
        //NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate);
    }
    else if (type == 0b01){   // LDURH
        //NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate) & (0xFFFF << 16);
        NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate) & (0xFFFF );
    } else {                    // LDURB
        //NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate) & (0xFF << 24);
        NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate) & (0xFF );
    }

    //supongo que no hay que actualizar los flags
}


// MOVZ --> movz X1, #0x10 (descripción: X1 = 0x10) 
// hw = 0
void movz(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111;
    uint32_t immediate = (instruction & (0b1111111111111111 << 5)) >> 5;
    NEXT_STATE.REGS[t_register] = immediate;

    //supongo que no hay que actualizar los flags
}

void hlt(uint32_t instruction){
  RUN_BIT = 0;
}


bool is_subs_ext(uint32_t instruction) { 
    printf("El opcode subs ext es %d\n", ((instruction & (0b11111111111 << 21)) >> 21) + 1);
    return (((instruction& (0b11111111111 << 21)) >> 21) + 1 == 0b11101011001);    //11101011001
} // sin el + 1 no anda



uint32_t decode(uint32_t instruction) {
  // Extract the opcode from the instruction
  uint32_t array_opcodes_6 = 0b000101; // B  

  uint32_t array_opcodes_8[7] = {0xb1, 0xab, 0xf1, 0xea, 0xaa, 0xca, 0x54};  
  // adds_imm, adds_ext, subs_imm, cmp_imm, ands_shift, eor_shift, orr_shift, b.cond
  
  uint32_t array_opcodes_9 = 0b110100101; // movz
  
  uint32_t array_opcodes_10 = 0b1101001101; // lsl_imm, lsr_imm SON IGUALES
  
  uint32_t array_opcodes_11[9] = {0b11101011001, 0b11010100010, 0b11101011001, 0b11111000000, 0b00111000000, 
                                  0b01111000000, 0b11111000010, 0b01111000010, 0b00111000010};  
  // subs ext, hlt, cmp_ext, stur, sturb, sturh, ldur, ldurh, ldurb
  
  uint32_t array_opcodes_22 = 0b1101011000011111000000;   // BR


  uint8_t array_b_cond[6] = {0b0, 0b1, 0b1100, 0b1011, 0b1010, 0b1101};   
  //B.:eq, ne, gt, lt, ge, le

  // Verify if it is an 6 bit opcode
  uint32_t opcode = (instruction & (0b111111 << 26)) >> 26;
  if (opcode == array_opcodes_6) {return opcode;}

  // Verify if it is an 8 bit opcode
  opcode = is_opcode_length_8(instruction, array_opcodes_8);
  if (opcode != -1) {return opcode;}

  // Verify if it is an 9 bit opcode
  opcode = (instruction & (0b111111111 << 23)) >> 23;
  printf("El opcode de 9 bits es %x\n", opcode);
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






// VIENDO EL OPCODE RETORNADO DECIDO QUE ACCION TOMAR
bool execute(uint32_t opcode, uint32_t instruction) {
  Node_t array_opcodes[19] = {
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
    {0b11111000010, ldur},
    {0b11111000000, stur_b_h},
    {0b00111000000, stur_b_h},
    {0b01111000000, stur_b_h},
    {0b11111000010, ldur},
    {0b01111000010, ldur},
    {0b00111000010, ldur}
    
  };

  for (int i = 0; i < sizeof(array_opcodes)/sizeof(array_opcodes[0]); i++) {
    if (opcode == array_opcodes[i].opcode) {
      printf("Entra al if\n");
      printf("OPcode es %x\n", opcode);
      array_opcodes[i].function(instruction);
      return;
      //if (opcode == 0x54){ return false;}     // si es b.cond no cambia el pc en función process_instruction
      //else {return true;}   //estaría mal pq si era bcond pero no cumple condición, no hago salto
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

  //printf("array opcodes: %x\n", array_opcodes[0]);
  uint32_t instruction = mem_read_32(CURRENT_STATE.PC); // Here we have the 32 bits instruction
  printf("Instruction: %x\n", instruction);
  // Decode the instruction
  uint32_t opcode = decode(instruction);
  //uint32_t aux_pc = CURRENT_STATE.PC;
  // Execute the instruction
  execute(opcode, instruction);

  // si no cambio en b o bcond
  if (CURRENT_STATE.PC == NEXT_STATE.PC) {NEXT_STATE.PC += 4;}
  
  return;
}
// 400014 en inst 2  pero debería darme 400010
