// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __DECODEUNIT_H
#define __DECODEUNIT_H

#include <cstdint>

#include "Register.h"
#include "ExecutionUnit.h"
#include "opcodes.h"

// Instruction and unknown operands
typedef struct {
	uint8_t opcode : 4;
	uint32_t data  : 28;
} instruction_t;

// Three register operands
typedef struct {
	uint8_t opcode : 4;
	uint8_t r1     : 4;
	uint8_t r2     : 4;
	uint8_t r3     : 4;
	uint16_t       : 16;
} instruction_orrr_t;

// Two register and an immediate
typedef struct {
	uint8_t opcode : 4;
	uint8_t r1     : 4;
	uint8_t r2     : 4;
	int16_t im1   : 16;
	uint8_t        : 4;
} instruction_orri_t;

// A register and an immediate operand
typedef struct {
	uint8_t opcode : 4;
	uint8_t r1     : 4;
	int16_t im1   : 16;
	uint8_t        : 8;
} instruction_ori_t;

// A single register operand
typedef struct {
	uint8_t opcode : 4;
	uint8_t r1     : 4;
	uint32_t       : 24;
} instruction_or_t;

// A single immediate operand
typedef struct {
	uint8_t opcode : 4;
	int16_t im1   : 16;
	uint16_t       : 12;
} instruction_oi_t;

typedef struct {
	uint8_t opcode : 4;
	uint32_t       : 28;
} instruction_o_t;

class DecodeUnit {
public:
	void decode(Register* ir, ExecutionUnit* eu);
};

#endif
