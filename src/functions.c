#include "functions.h"

void add_imm(uint32_t instruction){
  uint32_t immediate = (instruction & (0xFFF << 10)) >> 10;
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t shift = (instruction & (0x3 << 22)) >> 22;
  uint64_t extend_immediate = 0;
  if (shift == 0b01){
    extend_immediate = immediate;
    extend_immediate = extend_immediate << 12;
  }

  else if (shift == 0b00){ extend_immediate = immediate; }
  NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + extend_immediate;
}

void add_ext_register(uint32_t instruction){
  uint32_t immediate = (instruction & (0x7 << 10)) >> 10;
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t option = (instruction & (0x7 << 13)) >> 13;
  uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
  NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + NEXT_STATE.REGS[m_register];
}

void mul(uint32_t instruction){
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
  NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] * NEXT_STATE.REGS[m_register];
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
      extend_immediate = immediate;  
    }

    NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] + extend_immediate;

    if (NEXT_STATE.REGS[dest_register] < 0){
        NEXT_STATE.FLAG_N = 1;
        NEXT_STATE.FLAG_Z = 0;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
        NEXT_STATE.FLAG_N = 0;
    } else {
        NEXT_STATE.FLAG_N = 0;
        NEXT_STATE.FLAG_Z = 0;
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
        NEXT_STATE.FLAG_Z = 0;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
        NEXT_STATE.FLAG_Z = 1;
        NEXT_STATE.FLAG_N = 0;
    } else {
        NEXT_STATE.FLAG_N = 0;
        NEXT_STATE.FLAG_Z = 0;
    }
}



// se usa también para cmp imm
void subs_imm(uint32_t instruction){   
  uint32_t immediate = (instruction & (0xFFF << 10)) >> 10;
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t shift = (instruction & (0x3 << 22)) >> 22;
  int32_t number = 0;

  //solo se hace shift = 01
  if (shift == 0b01){immediate = immediate << 12;}
  
  if (dest_register != 0b11111) {
    NEXT_STATE.REGS[dest_register] = CURRENT_STATE.REGS[n_register] - immediate;

    if (NEXT_STATE.REGS[dest_register] < 0){
      NEXT_STATE.FLAG_N = 1;
      NEXT_STATE.FLAG_Z = 0;
    } else if (NEXT_STATE.REGS[dest_register] == 0){
      NEXT_STATE.FLAG_Z = 1;
      NEXT_STATE.FLAG_N = 0;
    } else {
      NEXT_STATE.FLAG_N = 0;
      NEXT_STATE.FLAG_Z = 0;
    }
  }
  
  // en caso de cmp
  else if (dest_register == 0b11111){
    number = CURRENT_STATE.REGS[n_register] - immediate;
    if (number < 0){
      NEXT_STATE.FLAG_N = 1;
      NEXT_STATE.FLAG_Z = 0;
    } else if (number == 0){
      NEXT_STATE.FLAG_Z = 1;
      NEXT_STATE.FLAG_N = 0;
    } else {
      NEXT_STATE.FLAG_N = 0;
      NEXT_STATE.FLAG_Z = 0;
    }
  }
}

void subs_ext_register(uint32_t instruction){   
  uint32_t immediate = (instruction & (0x7 << 10)) >> 10;
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t option = (instruction & (0x7 << 13)) >> 13;
  uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
  int32_t number = 0;
  
  if (dest_register != 0b11111) {
    NEXT_STATE.REGS[dest_register] = CURRENT_STATE.REGS[n_register] - CURRENT_STATE.REGS[m_register];

    if (NEXT_STATE.REGS[dest_register] < 0){
      NEXT_STATE.FLAG_N = 1;
      NEXT_STATE.FLAG_Z = 0;
    } else if (NEXT_STATE.REGS[dest_register] ==0){
      NEXT_STATE.FLAG_Z = 1;
      NEXT_STATE.FLAG_N = 0;
    } else {
      NEXT_STATE.FLAG_N = 0;
      NEXT_STATE.FLAG_Z = 0;
    }
  }

  // en caso de cmp
  else if (dest_register == 0b11111){
    number = CURRENT_STATE.REGS[n_register] - CURRENT_STATE.REGS[m_register];
    if (number < 0){
      NEXT_STATE.FLAG_N = 1;
      NEXT_STATE.FLAG_Z = 0;
    } else if (number == 0){
      NEXT_STATE.FLAG_Z = 1;
      NEXT_STATE.FLAG_N = 0;
    } else {
      NEXT_STATE.FLAG_N = 0;
      NEXT_STATE.FLAG_Z = 0;
    }
  }
}


void ands_shifted_register(uint32_t instruction){   
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t immediate = (instruction & (0x7F << 10)) >> 10;  
  uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
  uint32_t shift = (instruction & (0x3 << 22)) >> 22;    
  
  // left logical shift
  if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] << immediate);}
  // right logical shift
  else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}

  if (NEXT_STATE.REGS[dest_register] < 0){
    NEXT_STATE.FLAG_N = 1;
    NEXT_STATE.FLAG_Z = 0;
  } 
  else if (NEXT_STATE.REGS[dest_register] == 0){
    NEXT_STATE.FLAG_Z = 1;
    NEXT_STATE.FLAG_N = 0;
  }
  else{
    NEXT_STATE.FLAG_Z = 0;
    NEXT_STATE.FLAG_N = 0;
  }
}


// exclusive or
void eor_shifted_register(uint32_t instruction){  
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t immediate = (instruction & (0x7F << 10)) >> 10; 
  uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
  uint32_t shift = (instruction & (0x3 << 22)) >> 22;   
  
  //shift logical left
  if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] ^ (NEXT_STATE.REGS[m_register] << immediate);}
  // shift logical right
  else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}

}



// or
void orr_shifted_register(uint32_t instruction){   
  uint32_t dest_register = instruction & 0x1F;
  uint32_t n_register = (instruction & (0x1F << 5)) >> 5;
  uint32_t immediate = (instruction & (0x7F << 10)) >> 10;    
  uint32_t m_register = (instruction & (0x1F << 16)) >> 16;
  uint32_t shift = (instruction & (0x3 << 22)) >> 22;     
  
  //shift logical left
  if (shift == 0b00){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] | (NEXT_STATE.REGS[m_register] << immediate);}
  // shift logical right
  else if (shift == 0b01){NEXT_STATE.REGS[dest_register] = NEXT_STATE.REGS[n_register] & (NEXT_STATE.REGS[m_register] >> immediate);}

}


void b(uint32_t instruction){
  int32_t imm = instruction & (0xFFFFFFF >>2) ;   
  int64_t offset = imm << 2;
  NEXT_STATE.PC = CURRENT_STATE.PC + offset;
}


void br(uint32_t instruction){
  uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
  NEXT_STATE.PC = CURRENT_STATE.REGS[n_register];
}


void bcond(uint32_t instruction){
  uint32_t condition = instruction & 0xF;
  int32_t imm = (instruction & (0b1111111111111111111 << 5)) >> 5;  

  int64_t offset = imm << 2;  
  bool negative = false;

  if ((offset & 0x100000) == 0x100000) {
    // Convierte el offset a un valor negativo
    offset = ((~offset) + 1);
    offset = offset & 0x1FFFFF;
    negative = true;
    
    } 

  if (condition == 0b0000){       //equal
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
    if (CURRENT_STATE.FLAG_N == 0 && CURRENT_STATE.FLAG_Z == 0){
      if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
      else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
    }
  }
  
  else if (condition == 0b1011){   //less than
    if (CURRENT_STATE.FLAG_N == 1 && CURRENT_STATE.FLAG_Z == 0){
     if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
      else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
    }
  }

  else if (condition == 0b1010){   // greater or equal
    if (CURRENT_STATE.FLAG_N == 0){
      NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
  }

  else if (condition == 0b1101){   // less or equal
    if (CURRENT_STATE.FLAG_N == 1 || CURRENT_STATE.FLAG_Z == 1){
      NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
  }
}


void lsl_lsr_imm(uint32_t instruction){   // lsl and lsr immediate
  uint32_t dest_register = instruction & 0b11111;
  uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
  uint32_t immediate = (instruction & (0b111111 << 16)) >> 16;

  uint32_t mask = 0b111111 << 10;

  if (((instruction & mask) >> 10) == 0b111111){ // LSR
    NEXT_STATE.REGS[dest_register] = CURRENT_STATE.REGS[n_register] >> (64 - immediate);
  } else {                                       // LSL
    NEXT_STATE.REGS[dest_register] = CURRENT_STATE.REGS[n_register] << (64 - immediate);
  }
}

void stur_b_h(uint32_t instruction){   
  uint32_t t_register = instruction & 0b11111;
  uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
  int64_t immediate = (instruction & (0b111111111 << 12)) >> 12;

  uint32_t mask = 0b11 << 30;
  uint32_t type = (instruction & mask) >> 30;

  if (type == 0b11){          // STUR
      mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register]);
  } else if (type == 0b01){   // STURH
      mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register] & (0xFFFF ));
  } else {                    // STURB
      mem_write_32(NEXT_STATE.REGS[n_register] + immediate, NEXT_STATE.REGS[t_register] & (0xFF ));
  }
}



void ldur(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111;
    uint32_t n_register = (instruction & (0b11111 << 5)) >> 5;
    int64_t immediate = (instruction & (0b111111111 << 12)) >> 12;


    // if it is 11 --> ldur, 01 --> ldurh, 00 --> ldurb
    uint32_t mask = 0b11 << 30;
    uint32_t type = (instruction & mask) >> 30;

    if (type == 0b11) {       // LDUR
        uint64_t lower_half = mem_read_32(NEXT_STATE.REGS[n_register] + immediate);
        uint64_t upper_half = mem_read_32(NEXT_STATE.REGS[n_register] + immediate + 4);
        // unir ambos valores en un solo uint64_t y que lower quede en los primeros 32 bits y upper en los siguientes 32
        uint64_t result = lower_half | (upper_half << 32);
        NEXT_STATE.REGS[t_register] = result;
    }
    else if (type == 0b01){   // LDURH
        NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate) & (0xFFFF );
    } else {                  // LDURB
        NEXT_STATE.REGS[t_register] = mem_read_32(NEXT_STATE.REGS[n_register] + immediate) & (0xFF );
    }
}


void movz(uint32_t instruction){   
  uint32_t t_register = instruction & 0b11111;
  uint32_t immediate = (instruction & (0b1111111111111111 << 5)) >> 5;
  NEXT_STATE.REGS[t_register] = immediate;
}

void hlt(uint32_t instruction){
  RUN_BIT = 0;
}

void cbz(uint32_t instruction){
  uint32_t t_register = instruction & 0b11111;
  int32_t imm = (instruction & (0b1111111111111111111 << 5)) >> 5;  
  int64_t offset = imm << 2;  
  bool negative = false;

  if ((offset & 0x100000) == 0x100000) {
    // Convierte el offset a un valor negativo
    offset = ((~offset) + 1);
    offset = offset & 0x1FFFFF;
    negative = true;  
  } 

  if (CURRENT_STATE.REGS[t_register] == 0){
    if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
    else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
  }
}

void cbnz(uint32_t instruction){
  uint32_t t_register = instruction & 0b11111;
  int32_t imm = (instruction & (0b1111111111111111111 << 5)) >> 5;  
  int64_t offset = imm << 2;  
  bool negative = false;

  if ((offset & 0x100000) == 0x100000) {
    // Convierte el offset a un valor negativo
    offset = ((~offset) + 1);
    offset = offset & 0x1FFFFF;
    negative = true;  
  } 

  if (CURRENT_STATE.REGS[t_register] != 0){
    if (negative==true) {NEXT_STATE.PC = CURRENT_STATE.PC - offset;}
    else {NEXT_STATE.PC = CURRENT_STATE.PC + offset;}
  }
}