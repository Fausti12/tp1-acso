#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

#define Opcode_mask 0x7F000000    //para 0001011 (adds entre 2 registros)


int get_opcode(uint32_t instruction){
    return (unsigned int)(instruction & Opcode_mask);
}

void decode(uint32_t instruction){
    uint32_t adds_immediate_opcode = 0b10110001;
    uint32_t adds_ext_register_opcode = 0b1010101100;  //son 10 bits el opcode
    if(get_opcode(instruction) == adds_immediate_opcode){
        printf("adds\n");
        NEXT_STATE.REGS[0] = 200; 
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
    printf("instruction: %x\n", instruction);
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




*/