// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "Dependence.h"
#include "opcodes.h"
#include "common.h"

FetchUnit::FetchUnit(const FetchUnit &copy) {
	_pc.contents = copy._pc.contents;
	_ir.contents = copy._ir.contents;

	debug = copy.debug;
	fetched = copy.fetched;
	ready = copy.ready;

	stalled = copy.stalled;
	halted = copy.halted;
	dependent = copy.dependent;

	dum = copy.dum;

	delta = copy.delta;
}

FetchUnit::FetchUnit(DecodeUnitManager* dum) {
	// Initialise the _pc, _ir
	_pc.contents = 0;
	_ir.contents = 0;
	delta = 0;

	// Add local reference to DU manager
	this->dum = dum;

	// Set up flags
	setState(true);

	stalled = false;
	halted = false;
	dependent = false;

	// No debugging by default
	debug = false;
}

std::string FetchUnit::toString() {
	std::ostringstream ss;

	ss << "_pc = " << _pc.toString() << std::endl;
	ss << "_ir = " << _ir.toString() << std::endl;
	ss << "_ir decodes to " << optos(_ir.contents) << std::endl;

	return ss.str();
}

// Sets the FU state.
// When ready is true, the FU
// is ready to accept input.
void FetchUnit::setState(bool ready) {
	this->ready = ready;
	this->fetched = !ready;
}

bool FetchUnit::passToDecodeUnit() {
	DecodeUnit* du = dum->getAvailableDecodeUnit();
	if (du != NULL) {
		du->issue(&_ir, &_pc);
		setState(true);
		return true;
	}
	return false;
}

bool FetchUnit::checkForStallResolution(BranchTable* bt, Register* pc) {
	if (stalled) {
		uint32_t previous = _pc.contents - 1;
		if (bt->predicted[previous] == STALLED) {
			if (bt->actual[previous] == UNKNOWN) {
				// Waiting...
				if (debug) {
					std::cout << "Waiting for branch information for location " << hexify(previous) << "." << std::endl;
				}
				return false;
			}

			if (bt->actual[previous] == TAKEN) {
				instruction_ori_t dataCond = *(instruction_ori_t *) &_ir.contents;
				pc->contents += dataCond.im1;
				if (debug) {
					std::cout << "Branch at location " << previous << " was taken. Updating PC to " << pc->contents << "." << std::endl;
				}
			} else {
				if (debug) {
					std::cout << "Branch at location " << previous << " was not taken." << std::endl;
				}
			}

			// Clear flag
			stalled = false;

			// Clear table entry
			bt->predicted.erase(previous);
			bt->actual.erase(previous);
		} else {
			if (debug) {
				std::cerr << "fu.stalled is true but bt is not stalled." << std::endl;
			}
			return false;
		}
	}
	return true;
}

// General-purpose
void FetchUnit::tick(std::vector<MemoryLocation>* m, std::vector<FetchUnit>* fus,
	bool pipeline, BranchTable* bt, Register* pc) {

	if (stalled) {
		if (debug) {
			std::cout << "Stalled." << std::endl;
		}
		return;
	}

	if (halted) {
		if (debug) {
			std::cout << "Halted." << std::endl;
		}
		return;
	}

	if (fetched) {
		if (debug) {
			std::cout << "Waiting for a DU to become available." << std::endl;
		}
		return;
	}

	if (debug) {
		std::cout << "Looking for next instruction at " << pc->contents << "." << std::endl;
	}

	// Grab next instruction
	uint32_t next =  m->at(pc->contents).contents;

	if (debug) {
		std::cout << "Fetched instruction " << optos(next) << std::endl;
	}

	// Check for dependencies
	dependent = false;
	for (int i = 0; i < fus->size(); i++) {
		if (fus->at(i).ready || !fus->at(i).fetched) {
			// Don't check, FU is empty
			continue;
		}

		if (Dependence::depends(next, fus->at(i)._ir.contents, pc->contents)) {
			// Dependency!
			if (debug) {
				std::cout << "Dependency found!" << std::endl;
			}
			dependent = true;
			return;
		}
	}
	
	// Fill the instruction register
	if (debug) {
		std::cout << "Filling _ir with " << optos(next) << std::endl;
	}
	_ir.contents = next;

	// Update flags
	setState(false);

	// [Pre-] decode the instruction and check for a branch
	// if the branch is conditional, stall
	if (pipeline) {
		instruction_t instr = *(instruction_t*) &_ir.contents;			  	// O_pcode
		instruction_oi_t data = *(instruction_oi_t *) &_ir.contents; 	  	// B
		instruction_ori_t dataCond = *(instruction_ori_t *) &_ir.contents;	// Conditional

		switch (instr.opcode) {
			case OP_B:
				if (debug) {
					std::cout << "Found a branch at location " << std::to_string(pc->contents);
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
					std::cout << "Found a conditional branch at location " << std::to_string(pc->contents) << std::endl;
					std::cout << "Decide whether to jump by " << dataCond.im1 << " or not." << std::endl;
				}
				bt->predicted[pc->contents] = STALLED;
				bt->actual[pc->contents] = UNKNOWN;
				stalled = true;
			break;

			case OP_HLT:
				if (debug) {
					std::cout << "Found a halt at location " << std::to_string(pc->contents) << std::endl;
				}
				bt->predicted[pc->contents] = HALTED;
				bt->actual[pc->contents] = UNKNOWN;
				halted = true;
			break;
		}
	}

	// Always increment
	// Even if we've already added an offset using a branch.
	// This is because the offset operand assumes an incremented PC
	pc->contents++;
	_pc.contents = pc->contents;

	if (debug) {
		std::cout << "Setting _pc to " << hexify(_pc.contents) << std::endl;
	}

	// ...and increment the copy for the FUM
	pc->contents = pc->contents + delta;
	delta = 0;

	if (debug) {
		std::cout << "Setting _pc to " << hexify(_pc.contents) << std::endl;
	}
}
