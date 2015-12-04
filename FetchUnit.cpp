// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "FetchUnit2.h"
#include "DecodeUnit.h"
#include "Dependence.h"
#include "opcodes.h"
#include "common.h"
	
void FetchUnit::init(uint32_t pipelineWidth) {
	// Initialise the pc
	pc.contents = 0;
	delta = 0;

	// Set up flags
	setState(true);

	// Not stalled!
	stalled = false;

	// No debugging by default
	debug = false;
}

FetchUnit::FetchUnit() {
	init();
}

std::string FetchUnit::toString() {
	std::ostringstream ss;

	ss << "pc = " << pc.toString() << std::endl;
	ss << "ir = " << ir.toString() << std::endl;

	if (stalled) {
		ss << "Currently stalled." << std::endl;
	}

	return ss.str();
}

// Toggles the FU state
void FetchUnit::toggleState() {
	fetched = !fetched;
	ready = !ready;
}

// Sets the FU state.
// When ready is true, the FU
// is ready to accept input.
void FetchUnit::setState(bool ready) {
	this->ready = ready;
	this->fetched = !ready;
}

bool FetchUnit::passToDecodeUnit() {
	DecodeUnit du = DecodeUnitManager.getAvailableDecodeUnit();
	if (du) {
		du.issue(ir, pc);
		setState(true);
	}
	return false;
}

// Use local registers, for pipelining
bool FetchUnit::tick(std::vector<MemoryLocation>* m, std::vector<FetchUnit>* fus) {
	return tick(m, fus, true);
}

// General-purpose
bool FetchUnit::tick(std::vector<MemoryLocation>* m, std::vector<FetchUnit>* fus, bool pipeline) {

	bool stalled = false;

	// Is there an instruction waiting?
	if (fetched) {
		if (!passToDecodeUnit()) {
			continue;
		}
	}

	// Update the PC with any jumps
	// performed at the last fetch step.
	// This is only done now as we want a pristine PC value
	// to be passed to the DU (see above step).
	pc.contents += delta;
	delta = 0;

	// Grab next instruction
	uint32_t next =  m->at(pc.contents).contents;

	if (debug) {
		std::cout << "Fetched instruction " << optos(next) << std::endl;
	}

	// Check for dependencies
	for (int i = 0; i < fus.size(); i++) {
		if (fus->at(i).ready || !fus->at(i).fetched) {
			// Don't check, FU is empty
			continue;
		}

		if (Dependence::depends(next, fus->at(i).ir.contents, pc.contents)) {
			// Dependency!
			if (debug) {
				std::cout << "Dependency found between ";
				std::cout << optos(next) << " and " << optos(irs->at(j).contents) << std::endl;
			}
			return false;
		}
	}
	
	// Fill the instruction registers
	ir.contents = next;

	// Update flags
	setState(false);

	// [Pre-] decode the instruction and check for a branch
	// if the branch is conditional, stall
	if (pipeline) {
		instruction_t instr = *(instruction_t*) &ir.contents;			  // Opcode
		instruction_oi_t data = *(instruction_oi_t *) &ir.contents; 	  // B
		instruction_ori_t dataCond = *(instruction_ori_t *) &ir.contents; // Conditional

		switch (instr.opcode) {
			case OP_B:
				if (debug) {
					std::cout << "Found a branch at location " << std::to_string(pc.contents);
					std::cout <<  ". Jumping by " << data.im1 << "." << std::endl;
				}
				delta = data.im1;
			break;

			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				if (debug) {
					std::cout << "Found a conditional branch at location " << std::to_string(pc.contents) << std::endl;
					std::cout << "Decide whether to jump by " << dataCond.im1 << " or not." << std::endl;
				}
				stalled = true;
			break;

			case OP_HLT:
				if (debug) {
					std::cout << "Found a halt at location " << std::to_string(pc.contents) << std::endl;
				}
				stalled = true;
			break;
		}
	}

	// Always increment
	// Even if we've already added an offset using a branch.
	// This is because the offset operand assumes an incremented PC
	pc.contents++;

	// Try to pass this to an available Decode Unit
	passToDecodeUnit();

	// Return stall state
	return stalled;
}
