#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>


int main(){
    int8_t numero = 0b11000101;
    numero = numero >> 3;
    printf("instruction: %d\n", numero);
    return 0;
}

// gcc -o pruba pruba.c
