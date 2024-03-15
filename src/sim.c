#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "shell.h"


#define Opcode_mask 0x7F000000    //para 0001011 (adds entre 2 registros)



bool is_add_immediate(uint32_t opcode) { return ((opcode & (0xFF << 24)) >> 24) == 0x91; }
bool is_add_ext_register(uint32_t opcode) { return ((opcode & (0xFFF << 21)) >> 21) == 0b10001011001; }   //opcode= 10001011001
bool is_adds_immediate(uint32_t opcode) { return ((opcode & (0xFF << 24)) >> 24) == 0b10110001; }


bool mask_opcode(uint32_t instruction, int len_opcode) { 
    printf("0xfff: %d\n", 0xfff);
    return ((instruction & (0xFFF << (20 - len_opcode))) >> (20 - len_opcode)); }  //0xfff pq supongo que opcode tiene menos de 12 bits
// muevo 20 para que me queden los 12 bits de la mask


bool mask_opcode_2(uint32_t instruction, int len_opcode) { 
    printf("0xfff: %d\n", 0xfff);
    printf("0x1f: %d\n", 0xF);
    printf("0x1f <<1: %d\n", 0xF<<28);   //reconoce con signo   si hago <<32 da 0
    printf(" instruction & (0xF << 1): %d\n", (instruction & (0xFF << 24))>>24);
    printf("0x91 %d\n", 0x91);
    return ((instruction & (0xF)) ) ; } 



int array_opcodes[] = {0x91, 0b10001011001, 0b10110001}; 
// 0x91 -> add immediate  0b10001011001 -> add extended register  0b10110001 -> adds immediate
int get_opcode(uint32_t instruction){
    
}

void decode(uint32_t instruction){
    printf("Instruction: %d\n", instruction);
    for (int i=5; i < 11; i++){
        uint32_t mask_result = mask_opcode_2(0x00000091, i);
        printf("Mask result: %d\n", mask_result);
        for (int j=0; j < sizeof(array_opcodes); j++){
            if (mask_result == array_opcodes[j]){
                printf("Opcode: %d\n", mask_result);
                printf("Opcode: %d\n", array_opcodes[j]);
            }
        }
            
    }   
}


void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * 
     * Sugerencia: hagan una funcion para decode()
     *             y otra para execute()
     * 
     * */
    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    decode(instruction);
    
    CURRENT_STATE.PC = CURRENT_STATE.PC + 4;
    //ecode()
    

}




/*
cd inputs
cat *.s  -> imprime cualquier archivo que tiene .s
cat sturb.s -> imprime el archivo sturb.s
./asm2hex sturb.s -> lo pasa a hexadecimal
cat sturb.x
el .x es lo que usamos para simular
armar otro test de subs sin immediate
Hacer variantes de 64 bits-> sf=1 en varios casos
pag 531
tenemos que hacer operaciones del sudo code en c.
nos va llegar el valor de un registro o immediate en hexa
int a = 0b000000001100 -> 12
int b = 0xc; printf("%d", b); -> 12

me llega instrucción en binario
Pc es la dirección de la instrucción. como las instrucciones están guardadas en memoria, uso la función mem_read 


./asm2hex adds.s
./ref_sim inputs/adds.x
run 1  -> ejecuta una instrucción
los corchetes indican memoria
lsl: shift left. lsl x1, x1,16 -> x1 = x1 << 16


input 10 -1  le resto 1 al registro 10

ver qué pasa si una instrucción tiene bits después del opcode de forma que se pueda confundir con otro opcode
no pasaría nada porque no puede haber opcode de algo dentro de otro
*/