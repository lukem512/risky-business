// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __OPCODES_H
#define __OPCODES_H

//
// O instructions
// 00000 - 00010
//
#define IS_O(x) (x <= 2)

// No operation
// NOP
#define OP_NOP 0

// Halt execution
// HLT
#define OP_HLT 1


//
// OI instructions
// 00011 - 00100
//
#define IS_OI(x) (3 <= x && x <= 4)

// Branch to relative address
// B im1
#define OP_B 3


//
// ORI instructions
// 00101 - 01111
//
#define IS_ORI(x) (5 <= x && x <= 15)

// Load from memory location
// LD r1 im1
#define OP_LD 5

// Store in memory location
// ST r1 im1
#define OP_ST 6

// Load constant
// LDC r1 im1
#define OP_LDC 7

// Branch when register is zero
// BZ r1 im1
#define OP_BZ 8

// Branch when register is not zero
// BNZ r1 im1
#define OP_BNZ 9

// Branch when register is less-than zero
// BLTZ r1 im1
#define OP_BLTZ 10

// Branch when register is less-than or equal-to zero
// BLTEZ r1 im1
#define OP_BLTEZ 11

// Branch when register is greater-than zero
// BGTZ r1 im1
#define OP_BGTZ 12

// Branch when register is greater-than or equal-to zero
// BGTEZ r1 im1
#define OP_BGTEZ 13


//
// ORR instructions
// 10000 - 10011
//
#define IS_ORR(x) (16 <= x && x <= 19)

// Move a value between registers
// MOV r1 r2
#define OP_MOV 16

// Load from memory location
// LDR r1 r2
#define OP_LDR 17

// Store in memory location
// STR r1 r2
#define OP_STR 18


//
// ORRR instructions
// 10100 - 11100
//
#define IS_ORRR(x) (20 <= x && x <= 28)

// Addition
// ADD r1 r2 r3
#define OP_ADD 20

// Subtraction
// SUB  r1 r2 r3
#define OP_SUB 21

// Multiplication
// MUL r1 r2 r3
#define OP_MUL 22

// Division
// DIV r1 r2 r3
#define OP_DIV 23

// Compare two values
// CMP r1 r2 r3
#define OP_CMP 24


//
// Psuedo-instructions
//

// Print psuedo-operation
#define OP_PRNT 29


//
// Reserved instruction
//

// Unknown operation!
#define OP_UNKNOWN 31


#endif
