// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "ExecutionUnit.h"
#include "opcodes.h"
#include "common.h"

using namespace std;

ExecutionUnit::ExecutionUnit() {
	// Initialise arguments to 0
	r1 = 0;
	r2 = 0;
	r3 = 0;
	im1 = 0;

	// No debugging by default
	debug = false;

	// Reset instruction counter
	n = 0;
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

void ExecutionUnit::issue(uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3) {
	type = EU_ISSUE_ORRR;
	this->opcode = opcode;
	this->r1 = r1;
	this->r2 = r2;
	this->r3 = r3;
}

void ExecutionUnit::issue(uint8_t opcode, uint8_t r1, uint8_t r2, int16_t im1) {
	type = EU_ISSUE_ORRI;
	this->opcode = opcode;
	this->r1 = r1;
	this->r2 = r2;
	this->im1 = im1;
}

void ExecutionUnit::issue(uint8_t opcode, uint8_t r1, uint8_t r2) {
	type = EU_ISSUE_ORR;
	this->opcode = opcode;
	this->r1 = r1;
	this->r2 = r2;
}

void ExecutionUnit::issue(uint8_t opcode, uint8_t r1, int16_t im1) {
	type = EU_ISSUE_ORI;
	this->opcode = opcode;
	this->r1 = r1;
	this->im1 = im1;
}

void ExecutionUnit::issue(uint8_t opcode, uint8_t r1) {
	type = EU_ISSUE_OR;
	this->opcode = opcode;
	this->r1 = r1;
}

void ExecutionUnit::issue(uint8_t opcode, int16_t im1) {
	type = EU_ISSUE_OI;
	this->opcode = opcode;
	this->im1 = im1;
}

void ExecutionUnit::issue(uint8_t opcode) {
	type = EU_ISSUE_O;
	this->opcode = opcode;
}

// Returns true when state should be halted
bool ExecutionUnit::tick(Register* pc, std::vector<Register>* r, std::vector<MemoryLocation>* m) {
	bool halted = false;

	if (debug) {
		std::cout << "Executing instruction " << optos(opcode) << std::endl;
	}

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
				std::cout << "Subtracting " << r2val_s << " and " << r3val_s << std::endl;
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
				std::cout << "OP_LD: test me!" << std::endl;
				std::cout << rtos(r1) << " = " << "M[" << std::to_string((long long unsigned int)r->at(r2).contents) << "] (" << std::to_string((long long unsigned int)m->at(im1).contents) << ")" << std::endl;
			}
			r->at(r1).contents = m->at(im1).contents;
		break;

		case OP_ST:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			if (debug) {
				std::cout << "OP_ST: test me!" << std::endl;
				std::cout << "M[" << std::to_string((long long unsigned int)im1) << "] = " << std::to_string((long long unsigned int)r->at(r1).contents) << std::endl;
			}
			m->at(im1).contents = r->at(r1).contents;
		break;

		case OP_LDR:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			if (debug) {
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
			pc->contents = pc->contents + im1;
		break;

		case OP_BZ:
			if (debug) {
				std::cout << "BZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			if (r->at(r1).contents == 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				pc->contents = pc->contents + im1;
			}
		break;

		case OP_BNZ:
			if (debug) {
				std::cout << "BNZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			if (r->at(r1).contents != 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				pc->contents = pc->contents + im1;
			}
		break;
		
		case OP_BLTZ:
			if (debug) {
				std::cout << "BLTZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			if (r1val_s < 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				pc->contents = pc->contents + im1;
			}
		break;

		case OP_BLTEZ:
			if (debug) {
				std::cout << "BLTEZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			if (r1val_s <= 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				pc->contents = pc->contents + im1;
			}
		break;

		case OP_BGTZ:
			if (debug) {
				std::cout << "BGTZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			if (r1val_s > 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				pc->contents = pc->contents + im1;
			}
		break;

		case OP_BGTEZ:
			if (debug) {
				std::cout << "BGTEZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			}
			if (r1val_s >= 0) {
				if (debug) {
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				}
				pc->contents = pc->contents + im1;
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

	n++;
	return halted;
}
