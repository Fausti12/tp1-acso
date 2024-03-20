.text
adds x0, x10, -10
// Cargar una dirección base en X1 y multiplicarla por 2^32
mov X1, 0x1
lsl X1, X1, 28

// Sumar un valor inmediato a X1
adds X1, X1, 0x2

// Cargar una dirección base en X10 y multiplicarla por 2^20
mov X10, 0x0
lsl X10, X10, 20

// Sumar un valor inmediato a X10
adds X10, X10, 0x7ff

// Multiplicar X10 por 2^20
lsl X10, X10, 20

// Sumar un valor inmediato a X10
adds X10, X10, 0x88

// Multiplicar X10 por 2^8
lsl X10, X10, 8

// Sumar un valor inmediato a X10
adds X10, X10, 0xfff

// Almacenar el valor de X10 en la dirección apuntada por X1 (64 bits)
stur X10, [X1, 0x0]

// Mover un valor inmediato a X5
mov X5, 0xfff

// Almacenar los bits menos significativos de X5 en la dirección apuntada por X1 desplazada en 1 (16 bits)
sturh W5, [X1, 0x3]

// Cargar un valor de 64 bits desde la dirección apuntada por X1 (desplazada en 0)
ldur X13, [X1, 0x0]

// Cargar un valor de 64 bits desde la dirección apuntada por X1 (desplazada en 4)
ldur X14, [X1, 0x4]

// Cargar un valor de 8 bits desde la dirección apuntada por X1 (desplazada en 6)
ldurh W15, [X1, 0x6]

// Terminar el programa
HLT 0
