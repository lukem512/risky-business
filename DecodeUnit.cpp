// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "DecodeUnit.h"
#include "common.h"

DecodeUnit::DecodeUnit(ExecutionUnitManager* eum) {
	// No debugging by default
	debug = false;

	// Add local reference to EU manager
	this->eum = eum;

	// Set up local register values
	r1 = 0;
	r2 = 0;
	r3 = 0;
	im1 = 0;

	// Set state to ready
	setState(true);
}

void DecodeUnit::issue(Register* ir, Register* pc) {
	if (debug) {
		std::cout << "Being issued with " << optos(ir->contents) << std::endl;
	}
	this->ir.contents = ir->contents;
	this->pc.contents = pc->contents;
	ready = false;
}

void DecodeUnit::setState(bool ready) {
	this->ready = ready;
	this->decoded = !ready;
}

bool DecodeUnit::passToExecutionUnit() {
	ExecutionUnit* eu = eum->getAvailableExecutionUnit();
	if (eu != NULL) {
		if (debug) {
			std::cout << "Success! Calling eu->issue" << std::endl;
		}
		eu->issue(type, opcode, r1, r2, r3, im1, &pc);
		setState(true);
	}
	return false;
}

void DecodeUnit::tick() {

	// Waiting for input?
	if (ready) {
		if (debug) {
			std::cout << "Returning due to lack of input." << std::endl;
		}
		return;
	}

	// Is there an instruction waiting?
	if (decoded) {
		if (debug) {
			std::cout << "Trying to pass to EU" << std::endl;
		}
		if (!passToExecutionUnit()) {
			if (debug) {
				std::cout << "No EU available" << std::endl;
			}
			return;
		}
	}

	// Cast to struct to decode bits
	instruction_t instr = *(instruction_t*) &ir.contents;

	if (debug) {
		std::cout << "Decoding instruction " << optos(instr.opcode) << std::endl;
	}

	// Set type to unknown as default
	type = EU_ISSUE_UNKNOWN;

	// Decode operands
	if (IS_O(instr.opcode)) {
		if (debug) {
			std::cout << optos(instr.opcode) << std::endl;
		}
		type = EU_ISSUE_O;
		opcode = instr.opcode;
	} else if (IS_OI(instr.opcode)) {
		instruction_oi_t data = *(instruction_oi_t *) &ir.contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << hexify(data.im1) << std::endl;
		}
		type = EU_ISSUE_OI;
		opcode = data.opcode;
		im1 =  data.im1;
	} else if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &ir.contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << hexify(data.im1) << std::endl;
		}
		type = EU_ISSUE_ORI;
		opcode = data.opcode;
		r1 = data.r1;
		im1 = data.im1;
	} else if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &ir.contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << std::endl;
		}
		type = EU_ISSUE_ORR;
		opcode = data.opcode;
		r1 = data.r1;
		r2 = data.r2;
	} else if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &ir.contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << " " << rtos(data.r3) << std::endl;
		}
		type = EU_ISSUE_ORR;
		opcode = data.opcode;
		r1 = data.r1;
		r2 = data.r2;
		r3 = data.r3;
	} else if (instr.opcode == OP_PRNT) {
		instruction_or_t data = *(instruction_or_t *) &ir.contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << std::endl;
		}
		type = EU_ISSUE_OR;
		opcode = data.opcode;
		r1 =  data.r1;
	}

	if (type == EU_ISSUE_UNKNOWN) {
		// Unknown instruction, discard
		setState(true);

		if (debug) {
			std::cout << "Unknown instruction encountered: " << optos(instr.opcode) << std::endl;
		}
	} else {
		// Update flags
		setState(false);

		// ...and try to pass to EU
		passToExecutionUnit();
	}
}
