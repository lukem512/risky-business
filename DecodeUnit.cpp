// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include "common.h"

#include "DecodeUnit.h"

void DecodeUnit::tick(Register *ir, ExecutionUnit* eu) {
	// Cast to struct to decode bits
	instruction_t instr = *(instruction_t*) &ir->contents;

	// Decode operands
	if (instr.opcode == OP_HLT) { // HLT is 1110
		if (debug) {
			std::cout << optos(instr.opcode) << std::endl;
		}
		eu->issue(instr.opcode);
	} else
	if (IS_ORR(instr.opcode)) { // All 2-register operand instruction are 11xx
		instruction_orr_t data = *(instruction_orr_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << std::endl;
		}
		eu->issue(data.opcode, data.r1, data.r2);
	} else 
	if (IS_ORRR(instr.opcode)) { // All 3-register operand instructions are 10xx
		instruction_orrr_t data = *(instruction_orrr_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << " " << rtos(data.r3) << std::endl;
		}
		eu->issue(data.opcode, data.r1, data.r2, data.r3);
	} else
	if (IS_ORI(instr.opcode)) { // All register, immediate operand instructions are 01xx
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << hexify(data.im1) << std::endl;
		}
		eu->issue(data.opcode, data.r1, data.im1);
	} else
	if (instr.opcode == OP_PRNT) { // PRNT is 0001
		instruction_or_t data = *(instruction_or_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << std::endl;
		}
		eu->issue(data.opcode, (uint8_t) data.r1);
	} else
	if (instr.opcode == OP_NOP) { // NOP is 0000
		if (debug) {
			std::cout << optos(instr.opcode) << std::endl;
		}
		eu->issue(instr.opcode);
	} else
	{ // B is 0011
		instruction_oi_t data = *(instruction_oi_t *) &ir->contents;
		if (debug) {
			std::cout << optos(data.opcode) << " " << hexify(data.im1) << std::endl;
		}
		eu->issue(data.opcode, (int16_t) data.im1);
	}
}
