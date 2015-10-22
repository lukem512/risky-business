// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "ExecutionUnit.h"
#include "opcodes.h"
#include "common.h"

//#define DEBUG

using namespace std;

ExecutionUnit::ExecutionUnit() {
	// Initialise arguments to 0
	r1 = 0;
	r2 = 0;
	r3 = 0;
	im1 = 0;
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

	// Signed value representations
	int32_t r1val_s, r2val_s, r3val_s;
	r1val_s = r->at(r1).contents;
	r2val_s = r->at(r2).contents;
	r3val_s = r->at(r3).contents;

	switch (opcode) {
		case OP_ADD:
		#ifdef DEBUG
			std::cout << "Adding " << r2val_s << " and " << r3val_s << std::endl;
		#endif
			r->at(r1).contents = (uint32_t) (r2val_s + r3val_s);
		break;

		case OP_SUB:
		#ifdef DEBUG
			std::cout << "Subtracting " << r2val_s << " and " << r3val_s << std::endl;
		#endif
			r->at(r1).contents = (uint32_t) (r2val_s - r3val_s);
		break;

		case OP_MUL:
		#ifdef DEBUG
			std::cout << "Multiplying " << r2val_s << " and " << r3val_s << std::endl;
		#endif
			r->at(r1).contents = (uint32_t) (r2val_s * r3val_s);
		break;

		case OP_CMP:
			r->at(r1).contents = 
			  ((r->at(r2).contents < r->at(r3).contents) ? -1 :
			  (r->at(r2).contents > r->at(r3).contents));
		break;

		case OP_LD:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			r->at(r1).contents = m->at(im1).contents;
		break;

		case OP_STR:
			// TODO: use a LoadStoreUnit
			// For now, the memory access is instant
			m->at(r->at(r1).contents).contents = r->at(r2).contents;
		break;

		case OP_LDC:
		#ifdef DEBUG
			std::cout << "Perfoming load of constant " << hexify(im1) << " into register " << rtos(r1) << std::endl;
		#endif
			r->at(r1).contents = im1;
		break;

		case OP_B:
		#ifdef DEBUG
			std::cout << "B with a jump of " << std::to_string((long long int)im1) << std::endl;
		#endif
			pc->contents = pc->contents + im1;
		break;

		case OP_BZ:
			if (r->at(r1).contents == 0) {
				pc->contents = pc->contents + im1;
			}
		break;
		
		case OP_BLTZ:
			#ifdef DEBUG
				std::cout << "BLTZ " << std::to_string((long long int)r1val_s) << " " << std::to_string((long long int)im1) << std::endl;
			#endif
			if (r1val_s < 0) {
				#ifdef DEBUG
					std::cout << "Performing jump of " << std::to_string((long long int)im1) << std::endl;
				#endif
				pc->contents = pc->contents + im1;
			}
		break;

		case OP_PRNT:
		#ifdef DEBUG
			std::cout << "Printing register " << rtos(r1) << " (unsigned: " <<
				std::to_string((long long unsigned int) r->at(r1).contents) <<
				", signed: " << std::to_string((long long int) r1val_s) << ")" << std::endl;
		#endif
			std::cout << hexify(r->at(r1).contents) << std::endl;
		break;

		case OP_HLT:
		#ifdef DEBUG
			std::cout << "Halting execution" << std::endl;
		#endif
			halted = true;
		break;

		case OP_NOP:
		#ifdef DEBUG
			std::cout << "No operation" << std::endl;
		#endif
		break;

		default:
		#ifdef DEBUG
			std::cerr << "Unknown opcode encountered (" << hexify(opcode) << ")" << std::endl;
		#endif
		break;
	}

	return halted;
}
