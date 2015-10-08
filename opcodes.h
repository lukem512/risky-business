// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __OPCODES_H
#define __OPCODES_H

// Operand type masks
#define OP_MASK_ORRR 	8
#define OP_MASK_ORI		4

// No operation
#define OP_NOP	0

// Print a register
// Psuedo-operation
#define OP_PRNT 1

// Arithmetic
// Dst, r1, r2
#define OP_ADD	8
#define OP_SUB	9
#define OP_MUL	10

// Comparison
// Dst, r1, r2
#define OP_CMP	11

// Memory load
// Dst, im1
#define OP_LD	4

// Memory store
// im1, r1
#define OP_STR	5

// Constant load
// Dst, im1
#define OP_LDC	6

// Branch
// Supplied with immediate offset
#define OP_B	3

// Comparitive branch
// Supplied with register to test and immediate offset
#define OP_BZ	7

// Unknown operation
#define OP_UNKNOWN 15

#endif
