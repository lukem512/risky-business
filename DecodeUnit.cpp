// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include "common.h"

#include "DecodeUnit.h"

void DecodeUnit::tick(Register *ir, ExecutionUnit* eu) {
	// Cast to struct to decode bits
	instruction_t instr = *(instruction_t*) &ir->contents;

	// Decode operands
	if (IS_O(instr.opcode)) {
		if (debug) {
			std::cout << optos(instr.opcode) << std::endl;
		}
		eu->issue(instr.opcode);
		return;
	}

	if (IS_OI(instr.opcode)) {
		instruction_oi_t data = *(instruction_oi_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << hexify(data.im1) << std::endl;
		}
		eu->issue(data.opcode, (int16_t) data.im1);
		return;
	}

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << hexify(data.im1) << std::endl;
		}
		eu->issue(data.opcode, data.r1, data.im1);
		return;
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << std::endl;
		}
		eu->issue(data.opcode, data.r1, data.r2);
		return;
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << " " << rtos(data.r3) << std::endl;
		}
		eu->issue(data.opcode, data.r1, data.r2, data.r3);
		return;
	}

	// Special-case for PRNT
	if (instr.opcode == OP_PRNT) {
		instruction_or_t data = *(instruction_or_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << std::endl;
		}
		eu->issue(data.opcode, (uint8_t) data.r1);
		return;
	}

	// Issue with the instruction anyway!
	if (debug) {
		std::cout << "Unknown instruction encountered: " << optos(data.opcode) << std::endl;
	}
	eu->issue(data.opcode);
}
