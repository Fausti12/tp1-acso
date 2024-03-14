#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


#define Opcode_mask_add_immediate 0xFF    // Máscara para los últimos 7 bits
#define Opcode_mask_add_ext_register 0xFFF    // Máscara para los últimos 11 bits

int get_opcode(uint32_t instruction){
    printf("instruction: %d\n", instruction);
    unsigned int a = instruction & Opcode_mask_add_immediate;
    printf("a: %d\n", a);
    return a;
}

void decode(uint32_t instruction){
    uint32_t adds_immediate_opcode = 0b10110001;  // aunque agregue mas bits da igual el &
    uint32_t adds_ext_register_opcode = 0b10101011001;
    printf("adds_immediate_opcode: {%d}\n", adds_immediate_opcode);
    uint32_t adds_ext_register_opcode = 0b1010101100;  //son 10 bits el opcode
    if(get_opcode(instruction) == adds_immediate_opcode){
        printf("adds\n");
    }
}

int main(){
    uint32_t instruction = 0b10110001;
    decode(instruction);
    printf("instruction: %d\n", instruction);
    return 0;
}

// gcc -o pruba pruba.c

