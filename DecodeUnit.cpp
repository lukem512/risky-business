// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include "common.h"

#include "DecodeUnit.h"

void DecodeUnit::decode(Register *ir, ExecutionUnit* eu) {
	// Cast to struct to decode bits
	instruction_t instr = *(instruction_t*) &ir->contents;

	// Decode operands
	if (instr.opcode & OP_MASK_ORRR) { // All 3-register operand instructions are 10xx
		instruction_orrr_t data = *(instruction_orrr_t *) &ir->contents;
		#ifdef DEBUG
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << rtos(data.r2) << " " << rtos(data.r3) << std::endl;
		#endif
		eu->issue(instr.opcode, data.r1, data.r2, data.r3);
	} else
	if (instr.opcode & OP_MASK_ORI) { // All register, immediate operand instructions are 01xx
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;
		#ifdef DEBUG
			std::cout << optos(data.opcode) << " " << rtos(data.r1) << " " << hexify(data.im1) << std::endl;
		#endif
		eu->issue(instr.opcode, data.r1, data.im1);
	} else
	if (instr.opcode == OP_NOP) { // NOP is 0000
		#ifdef DEBUG
			std::cout << optos(instr.opcode) << std::endl;
		#endif
		eu->issue(instr.opcode);
	} else // B is 0011
	{
		instruction_oi_t data = *(instruction_oi_t *) &ir->contents;
		#ifdef DEBUG
			std::cout << optos(data.opcode) << " " << hexify(data.im1) << std::endl;
		#endif
		eu->issue(instr.opcode, data.im1);
	}
}
