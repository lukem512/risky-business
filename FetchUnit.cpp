// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "opcodes.h"
	
FetchUnit::FetchUnit() {
	// Initialise the local program counter
	pc.contents = 0;

	// No debugging by default
	debug = false;
}

void FetchUnit::setPc(uint32_t contents) {
	pc.contents = contents;
}

Register FetchUnit::getPc() {
	return pc;
}

Register FetchUnit::getIr() { 
	return ir;
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

	if (debug) {
		std::cout << "fu.ir = " << ir->toString() << std::endl;
	}

	if (pipeline) {
		// Decode the instruction and check for a branch
		// if the branch is conditional, stall
		instruction_t instr = *(instruction_t*) &ir->contents;
		instruction_oi_t data = *(instruction_oi_t *) &ir->contents; // B
		switch (instr.opcode) {
			case OP_B:
				// Jump!
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
				}
				// TODO: Stall until the rest of the pipeline catches up
			break;

			default:
				// Increment the PC
				pc->contents++;
			break;
		}
	} else {
		// Increment the PC
		pc->contents++;
	}

	if (debug) {
		std::cout << "fu.pc = " << pc->toString() << std::endl;
	}
}
