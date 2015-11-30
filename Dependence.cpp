// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <algorithm>

#include "Dependence.h"
#include "opcodes.h"

// Get the set of memory locations read by instr
std::vector<uint32_t> Dependence::IM(instruction_t instr, uint32_t pc, std::vector<Register>* r) {
	std::vector<uint32_t> vim;

	if (IS_OI(instr.opcode)) {
		instruction_oi_t data = *(instruction_oi_t *) &instr;

		switch (instr.opcode) {
			case OP_B:
				vim.push_back(pc + data.im1);
			break;
		}
	}

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;

		switch (instr.opcode) {
			case OP_LD:
				vim.push_back(data.im1);
			break;

			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				vim.push_back(pc + data.im1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_LDR:
				vim.push_back(r->at(data.r2).contents);
			break;
		}
	}

	return vim;
}

// Get the set of registers read by instr
std::vector<uint8_t> Dependence::IR(instruction_t instr) {
	std::vector<uint8_t> vir;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;

		switch (instr.opcode) {
			case OP_ST:
			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				vir.push_back(data.r1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_STR:
				vir.push_back(data.r1);
			break;
		}

		vir.push_back(data.r2);
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &instr;
		vir.push_back(data.r2);
		vir.push_back(data.r3);
	}

	if (instr.opcode == OP_PRNT) {
		instruction_or_t data = *(instruction_or_t *) &instr;
		vir.push_back(data.r1);
	}

	return vir;
}

// Get the set of memory locations written by instr
std::vector<uint32_t> Dependence::OM(instruction_t instr, std::vector<Register>* r) {
	std::vector<uint32_t> vom;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;

		switch (instr.opcode) {
			case OP_ST:
				vom.push_back(data.im1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_STR:
				vom.push_back(r->at(data.r1).contents);
			break;
		}
	}

	return vom;
}

// Get the set of registers written by instr
std::vector<uint8_t> Dependence::OR(instruction_t instr) {
	std::vector<uint8_t> vor;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;
		switch (instr.opcode) {
			case OP_LD:
				vor.push_back(data.r1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_MOV:
			case OP_LDR:
				vor.push_back(data.r1);
			break;
		}
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &instr;
		vor.push_back(data.r1);
	}

	return vor;
}

bool Dependence::depends(uint32_t s1, uint32_t s2, uint32_t pc, std::vector<Register>* r) {
	__depends(s1, s2, pc, r);
}

bool Dependence::depends(uint32_t s1, uint32_t s2, Register* pc, std::vector<Register>* r) {
	__depends(s1, s2, pc->contents, r);
}

bool Dependence::depends(Register s1, Register s2, Register* pc, std::vector<Register>* r) {
	__depends(s1.contents, s2.contents, pc->contents, r);
}

// Bernstein condition
// s2 depends on s1 if the following is true:
// [I(s1) ∩ O(s2)] ∪ [O(s1) ∩ I(s2)] ∪ [O(s1) ∩ O(s2)] ≠ Ø
bool Dependence::__depends(uint32_t s1, uint32_t s2, uint32_t pc, std::vector<Register>* r) {
	// Get instructions
	instruction_t s1_instr = *(instruction_t*) &s1;
	instruction_t s2_instr = *(instruction_t*) &s2;

	// NOP never causes a dep.
	if (s1_instr.opcode == OP_NOP) {
		return false;
	}

	// Compute the sets
	std::vector<uint32_t> ims1, oms1, ims2, oms2;
	std::vector<uint8_t> irs1, ors1, irs2, ors2;

	ims1 = IM(s1_instr, pc, r);
	ims2 = IM(s2_instr, pc, r);

	oms1 = OM(s1_instr, r);
	oms2 = OM(s2_instr, r);

	irs1 = IR(s1_instr);
	irs2 = IR(s2_instr);

	ors1 = OR(s1_instr);
	ors2 = OR(s2_instr);

	// Check IM(s1) ∩ OM(s2)
	if (std::set_intersection(ims1, oms2)) return true;

	// Check IM(s2) ∩ OM(s1)
	if (std::set_intersection(oms1, ims2)) return true;

	// Check IR(s1) ∩ OR(s2)
	if (std::set_intersection(irs1, ors2)) return true;

	// Check IR(s2) ∩ OR(s1)
	if (std::set_intersection(irs2, ors1)) return true;

	// Check OM(s1) ∩ OM(s2)
	if (std::set_intersection(oms1, oms2)) return true;

	// Check OR(s1) ∩ OR(s2)
	if (std::set_intersection(ors1, ors2)) return true;

	return false;
}