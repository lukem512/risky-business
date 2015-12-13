// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __DECODEUNIT_H
#define __DECODEUNIT_H

#include <cstdint>

#include "Register.h"
#include "ExecutionUnit.h"
#include "ExecutionUnitManager.h"
#include "Scoreboard.h"
#include "opcodes.h"

// Instruction and unknown operands
typedef struct {
	uint8_t opcode : 5;
	uint32_t data  : 28;
} instruction_t;

// Three register operands
typedef struct {
	uint8_t opcode : 5;
	uint8_t r1     : 4;
	uint8_t r2     : 4;
	uint8_t r3     : 4;
	uint16_t       : 15;
} instruction_orrr_t;

// Two register and an immediate
typedef struct {
	uint8_t opcode : 5;
	uint8_t r1     : 4;
	uint8_t r2     : 4;
	int16_t im1   : 16;
	uint8_t        : 3;
} instruction_orri_t;


// Two register operands
typedef struct {
	uint8_t opcode : 5;
	uint8_t r1     : 4;
	uint8_t r2     : 4;
	uint32_t       : 19;
} instruction_orr_t;

// A register and an immediate operand
typedef struct {
	uint8_t opcode : 5;
	uint8_t r1     : 4;
	int16_t im1   : 16;
	uint8_t        : 7;
} instruction_ori_t;

// A single register operand
typedef struct {
	uint8_t opcode : 5;
	uint8_t r1     : 4;
	uint32_t       : 23;
} instruction_or_t;

// A single immediate operand
typedef struct {
	uint8_t opcode : 5;
	int16_t im1   : 16;
	uint16_t       : 11;
} instruction_oi_t;

typedef struct {
	uint8_t opcode : 5;
	uint32_t       : 28;
} instruction_o_t;

class DecodeUnit {
private:
	ExecutionUnitManager* eum;
	Scoreboard* score;

	Register ir;		// Instruction register, passed from FU
	Register pc;		// Program counter, passed from FU

	uint8_t opcode; 	// Instruction opcode
	uint8_t r1, r2, r3; // Register operands
	int16_t im1; 		// Immediate operand
	uint8_t type;		// Type of instruction

	void setState(bool ready);

public:
	bool debug;		// Debugging output
	bool decoded; 	// Holding decoded instruction
	bool ready;		// Ready to receive fetched input 

	bool speculative;

	DecodeUnit(ExecutionUnitManager* eum);
	DecodeUnit(Scoreboard* score);
	void issue(Register* ir, Register* pc, bool speculative);
	void tick();
	bool passToExecutionUnit();
	bool passToScoreboard();
	void clear();
};

#endif
