// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "opcodes.h"
#include "common.h"
	
FetchUnit::FetchUnit() {
	// Initialise the local program counter
	pc.contents = 0;

	// Not stalled!
	stalled = false;

	// No debugging by default
	debug = false;
}

std::string FetchUnit::toString() {
	std::ostringstream ss;
	ss << "fu.ir = " << ir.toString() << std::endl;
	ss << "fu.pc = " << pc.toString() << std::endl;
	if (stalled) {
		ss << "Currently stalled." << std::endl;
	}
	return ss.str();
}

// Use local registers, for pipelining
bool FetchUnit::tick(std::vector<MemoryLocation>* m) {
	return tick(&ir, &pc, m, true);
}

// Use specific registers 
bool FetchUnit::tick(Register* ir, Register* pc, std::vector<MemoryLocation>* m) {
	return tick(ir, pc, m, false);
}

// General-purpose
bool FetchUnit::tick(Register* ir, Register* pc, std::vector<MemoryLocation>* m, bool pipeline) {
	// Fill the instruction register
	ir->contents = m->at(pc->contents).contents;

	if (pipeline) {
		// Decode the instruction and check for a branch
		// if the branch is conditional, stall
		instruction_t instr = *(instruction_t*) &ir->contents;
		instruction_oi_t data = *(instruction_oi_t *) &ir->contents; // B
		instruction_ori_t dataCond = *(instruction_ori_t *) &ir->contents; // Conditional
		
		if (debug) {
			std::cout << "Fetched instruction " << optos(instr.opcode) << std::endl;
		}

		switch (instr.opcode) {
			case OP_B:
				if (debug) {
					std::cout << "Found a branch at location " << std::to_string(pc->contents);
					std::cout <<  ". Jumping by " << data.im1 << "." << std::endl;
				}
				pc->contents += data.im1;
			break;

			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				if (debug) {
					std::cout << "Found a conditional branch at location " << std::to_string(pc->contents) << std::endl;
					std::cout << "Decide whether to jump by " << dataCond.im1 << " or not." << std::endl;
				}
				return true;
			break;

			case OP_HLT:
				if (debug) {
					std::cout << "Found a halt at location " << std::to_string(pc->contents) << std::endl;
				}
				return true;
			break;
		}
	}

	// Always increment, unless stalled
	// Even if we've already added an offset using a branch.
	// This is because the offset operand assumes an incremented PC
	pc->contents++;

	// Not stalled!
	return false;
}
