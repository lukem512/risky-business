// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __OPCODES_H
#define __OPCODES_H

// Operand type masks
#define OP_MASK_ORRR 	8
#define OP_MASK_ORI		4

// Macros for determining operand types
#define IS_ORRR(x) (x & OP_MASK_ORRR)
#define IS_ORI(x) (x & OP_MASK_ORI)
#define IS_ORR(x) (x & OP_MASK_ORRR && x & OP_MASK_ORI)

// No operation
#define OP_NOP	0

// Print a register
// Psuedo-operation
#define OP_PRNT 1

// Arithmetic
// r1, r2, r3
#define OP_ADD	8
#define OP_SUB	9
#define OP_MUL	10

// Comparison
// r1, r2, r3
#define OP_CMP	11

// Memory load
// im1, r1
#define OP_LD	4

// Memory store
// r1, r2
#define OP_STR	12

// Constant load
// Dst, im1
#define OP_LDC	5

// Branch
// Supplied with immediate offset
#define OP_B	3

// Comparitive branches
// Supplied with register to test and immediate offset
#define OP_BZ	6
#define OP_BLTZ 7

// BNZ = !BZ
// BLTEZ = BZ || BLTZ
// BGTZ = !(BZ || BLTZ)
// BGTEZ = !BLTZ

// Unknown operation
#define OP_UNKNOWN 15

#endif
