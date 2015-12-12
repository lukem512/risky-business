// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>
#include <random>

#include "ExecutionUnit.h"
#include "opcodes.h"
#include "common.h"

using namespace std;

ExecutionUnit::ExecutionUnit() {

	// Generate unique ID
	id = rand() % INT_MAX;

	// No debugging by default
	debug = false;

	clear();

	// Reset instruction counter
	n = 0;
}

void ExecutionUnit::setState(bool ready) {
	this->ready = ready;
	working = !ready;
}

void ExecutionUnit::clear() {
	// Set halted flag to false
	halted = false;

	// Set speculative flag to false
	speculative = false;
	invalid = false;

	// Initialise arguments to 0
	r1 = 0;
	r2 = 0;
	r3 = 0;
	im1 = 0;
	pc.contents = 0;

	// Reset current instruction counter
	count = 1;

	// Set state to ready
	setState(true);
}

std::string ExecutionUnit::toString() {

	ostringstream ss;

	// add opcode to string
	ss << optos(opcode);

	// Add the operands
	switch (type) {
		case EU_ISSUE_ORRR:
			ss << " " << rtos(r1) << ", " << rtos(r2) << ", " << rtos(r3);
		break;

		case EU_ISSUE_ORI:
			ss << " " << rtos(r1) << ", " << hexify(im1);
		break;

		case EU_ISSUE_OI:
			ss << " " << hexify(im1);
		break;

		case EU_ISSUE_O:
		break;

		default:
			// Unknown issue type
		break;
	}

	return ss.str();
}

// Issue the instruction to the EU
// This is a general-purpose EU
// Operands not needed are ignored, as determined by type
void ExecutionUnit::issue(uint8_t type, uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3,
		int16_t im1, Register* pc, bool speculative) {
	if (debug) {
		std::cout << "Being issued with " << (speculative ? "speculative " : "") << "instruction " << optos(opcode) << std::endl;
	}

	// Instruction type
	this->type = type;
	
	// Instruction and operands
	this->opcode = opcode;
	this->r1 = r1;
	this->r2 = r2;
	this->r3 = r3;
	this->im1 = im1;

	// Speculative execution
	this->speculative = speculative;

	// Value of PC
	this->pc.contents = pc->contents;

	// Set up counter
	// How many ticks to execute this instruction?
	// TODO: vary this for different instructions!
	count = 1;

	// Set the state to ready
	setState(false);
}

bool ExecutionUnit::willCompleteThisTick() {
	return (count <= 0);
}

// Returns true when state should be halted
void ExecutionUnit::tick(std::vector<Register>* r, std::vector<MemoryLocation>* m,
 BranchPredictionTable* bpt, BranchHistoryTable* bht) {

	// Waiting for input?
	if (ready) {
		if (debug) {
			std::cout << "Returning due to lack of input." << std::endl;
		}
		return;
	}

	// Decrement execution counter
	count--;

	if (debug) {
		std::cout << "Executing instruction " << optos(opcode);
		if (!willCompleteThisTick()) {
			std::cout << " (" << count << " cycles remaining)";
		}
		std::cout << "." << std::endl;
	}

	// When the instruction is not complete, just return.
	// NOTE: a more complete simulation could execute
	// different parts of an instruction per-cycle.
	if (!willCompleteThisTick()) {
		return;
	}

	// Halted flag
	halted = false;

	// Flags for branch prediction
	bool branched = false;
	bool taken = false;
	int32_t delta = 0;

	// Signed value representations
	int32_t r1val_s, r2val_s, r3val_s;
	r1val_s = r->at(r1).contents;
	r2val_s = r->at(r2).contents;
	r3val_s = r->at(r3).contents;

	switch (opcode) {
		case OP_MOV:
			if (debug) {
				std::cout << "MOV " << rtos(r2) << " to " << rtos(r1) << std::endl;
			}
			r->at(r1).contents = r->at(r2).contents;
		break;

		case OP_ADD:
			if (debug) {
				std::cout << "Adding " << r2val_s << " and " << r3val_s << std::endl;
			}
			r->at(r1).contents = (uint32_t) (r2val_s + r3val_s);
		break;

		case OP_SUB:
			if (debug) {
				std::cout << "Subptracting " << r2val_s << " and " << r3val_s << std::endl;
			}
			r->at(r1).contents = (uint32_t) (r2val_s - r3val_s);
		break;

		case OP_MUL:
			if (debug) {
				std::cout << "Multiplying " << r2val_s << " and " << r3val_s << std::endl;
			}
			r->at(r1).contents = (uint32_t) (r2val_s * r3val_s);
		break;

		case OP_DIV:
			if (debug) {
				std::cout << "Dividing " << r2val_s << " and " << r3val_s << std::endl;
			}
			r->at(r1).contents = (uint32_t) (r2val_s / r3val_s);
		break;

		case OP_CMP:
			r->at(r1).contents = 
			  ((r->at(r2).contents < r->at(r3).contents) ? -1 :
			  (r->at(r2).contents > r->at(r3).contents));
		break;

		case OP_LD:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			if (debug) {
				std::cout << rtos(r1) << " = " << "M[" << std::to_string((long long unsigned int)r->at(r2).contents) << "] (" << std::to_string((long long unsigned int)m->at(im1).contents) << ")" << std::endl;
			}
			r->at(r1).contents = m->at(im1).contents;
		break;

		case OP_ST:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			if (debug) {
				std::cout << "M[" << std::to_string((long long unsigned int)im1) << "] = " << std::to_string((long long unsigned int)r->at(r1).contents) << std::endl;
			}
			m->at(im1).contents = r->at(r1).contents;
		break;

		case OP_LDR:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			if (debug) {
				std::cout << "r1 = " << rtos(r1) << std::endl;
				std::cout << "r2 = " << rtos(r2) << std::endl;
				std::cout << std::to_string((long long unsigned int)r->at(r2).contents) << std::endl;
				std::cout << std::to_string((long long unsigned int)m->at(r->at(r2).contents).contents) << std::endl;
				std::cout << rtos(r1) << " = " << "M[" << std::to_string((long long unsigned int)r->at(r2).contents) << "] (" << std::to_string((long long unsigned int)m->at(r->at(r2).contents).contents) << ")" << std::endl;
			}	
			r->at(r1).contents = m->at(r->at(r2).contents).contents;
		break;

		case OP_STR:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			if (debug) {
				std::cout << "M[" << std::to_string((long long unsigned int)r->at(r1).contents) << "] = " << std::to_string((long long unsigned int)r->at(r2).contents) << std::endl;
			}
			m->at(r->at(r1).contents).contents = r->at(r2).contents;
		break;

		case OP_LDC:
			if (debug) {
				std::cout << "Perfoming load of constant " << hexify(im1) << " into register " << rtos(r1) << std::endl;
			}
			r->at(r1).contents = im1;
		break;

		case OP_B:
			if (debug) {
				std::cout << "B " << " " << std::to_string((long long int)im1) << std::endl;
			}
			delta = im1;
		break;

		case OP_BZ:
			if (debug) {
				std::cout << "BZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			branched = true;
			if (r->at(r1).contents == 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				delta = im1;
				taken = true;
			} else {
				taken = false;
			}
		break;

		case OP_BNZ:
			if (debug) {
				std::cout << "BNZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			branched = true;
			if (r->at(r1).contents != 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				delta = im1;
				taken = true;
			} else {
				taken = false;
			}
		break;
		
		case OP_BLTZ:
			if (debug) {
				std::cout << "BLTZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			branched = true;
			if (r1val_s < 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				delta = im1;
				taken = true;
			} else {
				taken = false;
			}
		break;

		case OP_BLTEZ:
			if (debug) {
				std::cout << "BLTEZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			branched = true;
			if (r1val_s <= 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				delta = im1;
				taken = true;
			} else {
				taken = false;
			}
		break;

		case OP_BGTZ:
			if (debug) {
				std::cout << "BGTZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			branched = true;
			if (r1val_s > 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				delta = im1;
				taken = true;
			} else {
				taken = false;
			}
		break;

		case OP_BGTEZ:
			if (debug) {
				std::cout << "BGTEZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			branched = true;
			if (r1val_s >= 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				delta = im1;
				taken = true;
			} else {
				taken = false;
			}
		break;

		case OP_PRNT:
			if (debug) {
				std::cout << "Printing register " << rtos(r1) << " (unsigned: " <<
					std::to_string((long long unsigned int) r->at(r1).contents) <<
					", signed: " << std::to_string((long long int) r1val_s) << ")" << std::endl;
			}
			std::cout << hexify(r->at(r1).contents) << std::endl;
		break;

		case OP_HLT:
			if (debug) {
				std::cout << "Halting execution" << std::endl;
			}
			halted = true;

			for (auto it = bpt->entries.begin(); it != bpt->entries.end(); ++it) {
			    if ((*it)->location == pc.contents - 1) {
			    	if ((*it)->actual == UNKNOWN) {
			    		(*it)->actual = HALTED;
			    		(*it)->pc = pc.contents;
			    	}	
			    }
			}
		break;

		case OP_NOP:
			if (debug) {
				std::cout << "No operation" << std::endl;
			}
		break;

		default:
			if (debug) {
				std::cerr << "Unknown opcode encountered (" << hexify(opcode) << ")" << std::endl;
			}
		break;
	}

	if (branched) {
		for (auto it = bpt->entries.begin(); it != bpt->entries.end(); ++it) {
		    if ((*it)->location == pc.contents - 1) {
		    	if ((*it)->actual == UNKNOWN) {
		    		(*it)->actual = (taken ? TAKEN : NOT_TAKEN);
		    		(*it)->pc = pc.contents + delta;
		    		if ((*it)->actual != (*it)->predicted && (*it)->predicted != STALLED) {
		    			invalid = true;
		    		}
		    	}
		    }
		}
		if (debug) {
			std::cout << "Updating BHT at location " << pc.contents << " with " << taken << std::endl;
		}
		bht->update(pc.contents, taken);
		pc.contents += delta;
	}

	// Set state to ready
	setState(true);

	// Update instruction counter
	n++;
}
