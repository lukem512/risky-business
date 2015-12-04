// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __EXECUTIONUNIT_H
#define __EXECUTIONUNIT_H

#include <vector>

#include "Register.h"
#include "MemoryLocation.h"

#define EU_ISSUE_ORRR 1
#define EU_ISSUE_ORRI 2
#define EU_ISSUE_ORR  3
#define EU_ISSUE_ORI  4
#define EU_ISSUE_OR   5
#define EU_ISSUE_OI   6
#define EU_ISSUE_O    0
#define EU_ISSUE_UNKNOWN 15

class ExecutionUnit {
private:
	uint8_t opcode; 	// Instruction opcode
	uint8_t r1, r2, r3; // Register operands
	int16_t im1; 		// Immediate operand
	uint8_t type;

public:
	bool debug; 		// Debugging output
	bool working; 		// Executing an instruction
	bool ready;			// Ready to receive decoded input

	unsigned int n; 	// Instruction count

	Register pc;
	
	ExecutionUnit();
	std::string toString();
	void issue(uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3);
	void issue(uint8_t opcode, uint8_t r1, uint8_t r2, int16_t im1);
	void issue(uint8_t opcode, uint8_t r1, uint8_t r2);
	void issue(uint8_t opcode, uint8_t r1, int16_t im1);
	void issue(uint8_t opcode, uint8_t r1);
	void issue(uint8_t opcode, int16_t im1);
	void issue(uint8_t opcode);
	bool tick(std::vector<Register>* r,
	  std::vector<MemoryLocation>* m);
};

#endif
