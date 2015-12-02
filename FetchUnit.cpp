// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "Dependence.h"
#include "opcodes.h"
#include "common.h"
	
FetchUnit::FetchUnit() {
	// Initialise the local program counter
	pc.contents = 0;

	// Initilise the ir vector
	// This is one by default
	const int irSize = 2;
	ir.resize(irSize);
	fetched.resize(irSize);

	// Not stalled!
	stalled = false;

	// No debugging by default
	debug = false;
}

std::string FetchUnit::toString() {
	std::ostringstream ss;

	ss << "fu.pc = " << pc.toString() << std::endl;

	for (int i = 0; i < ir.size(); i++) {
		ss << "fu.ir[" << i << "] = " << ir[i].toString() << std::endl;
	}

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
bool FetchUnit::tick(std::vector<Register>* irs, Register* pc, std::vector<MemoryLocation>* m) {
	return tick(irs, pc, m, false);
}

// General-purpose
bool FetchUnit::tick(std::vector<Register>* irs, Register* pc, std::vector<MemoryLocation>* m, bool pipeline) {

	// Set all IR contents to not fetched
	fetched.assign(irs->size(), false);

	// For all instruction registers...
	// Fill from left to right
	// Stop if a stall is reached or a dependency encountered
	for (int i = 0; i < irs->size(); i++) {

		// Check for dependencies
		// Check ir[i] with ir[j] where j in {0 to i}
		for (int j = 0; j < i; j++) {
			if (Dependence::depends(irs->at(i), irs->at(j), pc)) {
				// If a dependency is reached, return
				return false;
			}
		}

		// Update flags
		fetched[i] = true;

		// Fill the instruction registers
		irs->at(i).contents = m->at(pc->contents).contents;

		// [Pre-] decode the instruction and check for a branch
		// if the branch is conditional, stall
		if (pipeline) {
			instruction_t instr = *(instruction_t*) &irs->at(i).contents;
			instruction_oi_t data = *(instruction_oi_t *) &irs->at(i).contents; // B
			instruction_ori_t dataCond = *(instruction_ori_t *) &irs->at(i).contents; // Conditional
			
			if (debug) {
				std::cout << "[FU #" << i << "] Fetched instruction " << optos(instr.opcode) << std::endl;
			}

			switch (instr.opcode) {
				case OP_B:
					if (debug) {
						std::cout << "[FU #" << i << "] Found a branch at location " << std::to_string(pc->contents);
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
						std::cout << "[FU #" << i << "] Found a conditional branch at location " << std::to_string(pc->contents) << std::endl;
						std::cout << "Decide whether to jump by " << dataCond.im1 << " or not." << std::endl;
					}
					return true;
				break;

				case OP_HLT:
					if (debug) {
						std::cout << "[FU #" << i << "] Found a halt at location " << std::to_string(pc->contents) << std::endl;
					}
					return true;
				break;
			}
		}

		// Always increment, unless stalled
		// Even if we've already added an offset using a branch.
		// This is because the offset operand assumes an incremented PC
		pc->contents++;
	}

	// Not stalled!
	return false;
}
