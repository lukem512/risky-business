// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <algorithm>
#include <vector>
#include <iostream>

#include "Dependence.h"
#include "opcodes.h"

// Remove duplicates from vector
template <class T> inline std::vector<T> removeDuplicates (std::vector<T> v) {
	auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end()); 
    return v;
}

// Get the vector of memory locations read by instr
std::vector<uint32_t> Dependence::IM(instruction_t instr, uint32_t pc, std::vector<Register>* r) {
	std::vector<uint32_t> sim;

	if (IS_OI(instr.opcode)) {
		instruction_oi_t data = *(instruction_oi_t *) &instr;

		switch (instr.opcode) {
			case OP_B:
				sim.push_back(pc + data.im1);
			break;
		}
	}

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;

		switch (instr.opcode) {
			case OP_LD:
				sim.push_back(data.im1);
			break;

			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				sim.push_back(pc + data.im1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_LDR:
				sim.push_back(r->at(data.r2).contents);
			break;
		}
	}

	return removeDuplicates(sim);
}

// Get the vector of registers read by instr
std::vector<uint8_t> Dependence::IR(instruction_t instr) {
	std::vector<uint8_t> sir;

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
				sir.push_back(data.r1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_STR:
				sir.push_back(data.r1);
			break;
		}

		sir.push_back(data.r2);
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &instr;
		sir.push_back(data.r2);
		sir.push_back(data.r3);
	}

	if (instr.opcode == OP_PRNT) {
		instruction_or_t data = *(instruction_or_t *) &instr;
		sir.push_back(data.r1);
	}

	return removeDuplicates(sir);
}

// Get the vector of memory locations written by instr
std::vector<uint32_t> Dependence::OM(instruction_t instr, std::vector<Register>* r) {
	std::vector<uint32_t> som;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;

		switch (instr.opcode) {
			case OP_ST:
				som.push_back(data.im1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_STR:
				som.push_back(r->at(data.r1).contents);
			break;
		}
	}

	return removeDuplicates(som);
}

// Get the vector of registers written by instr
std::vector<uint8_t> Dependence::OR(instruction_t instr) {
	std::vector<uint8_t> sor;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &instr;
		switch (instr.opcode) {
			case OP_LD:
			case OP_LDC:
				sor.push_back(data.r1);
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &instr;

		switch (instr.opcode) {
			case OP_MOV:
			case OP_LDR:
				sor.push_back(data.r1);
			break;
		}
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &instr;
		sor.push_back(data.r1);
	}

	return removeDuplicates(sor);
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

	// Compute the vectors
	std::vector<uint32_t> ims1, oms1, ims2, oms2;
	std::vector<uint8_t> irs1, ors1, irs2, ors2;

	ims1 = IM(s1_instr, pc, r);
	ims2 = IM(s2_instr, pc, r);

	std::sort(ims1.begin(), ims1.end());
	std::sort(ims2.begin(), ims2.end());

	oms1 = OM(s1_instr, r);
	oms2 = OM(s2_instr, r);

	std::sort(oms1.begin(), oms1.end());
	std::sort(oms2.begin(), oms2.end());

	irs1 = IR(s1_instr);
	irs2 = IR(s2_instr);

	std::sort(irs1.begin(), irs1.end());
	std::sort(irs2.begin(), irs2.end());

	ors1 = OR(s1_instr);
	ors2 = OR(s2_instr);

	std::sort(ors1.begin(), ors1.end());
	std::sort(ors2.begin(), ors2.end());

	// Output vectors
	std::vector<uint32_t> smem;
	std::vector<uint8_t> sreg;

	// Check IM(s1) ∩ OM(s2)
	std::set_intersection(ims1.begin(), ims1.end(), oms2.begin(), oms2.end(), std::back_inserter(smem));

	if (smem.size() > 0) {
		return true;
	}

	// Check IM(s2) ∩ OM(s1)
	std::set_intersection(oms1.begin(), oms1.end(), ims2.begin(), ims2.end(), std::back_inserter(smem));

	if (smem.size() > 0) {
		return true;
	}

	// // Check IR(s1) ∩ OR(s2)
	std::set_intersection(irs1.begin(), irs1.end(), ors2.begin(), ors2.end(), std::back_inserter(sreg));

	if (sreg.size() > 0) {
		return true;
	}

	// // Check IR(s2) ∩ OR(s1)
	std::set_intersection(irs2.begin(), irs2.end(), ors1.begin(), ors1.end(), std::back_inserter(sreg));

	if (sreg.size() > 0) {
		return true;
	}

	// // Check OM(s1) ∩ OM(s2)
	std::set_intersection(oms1.begin(), oms1.end(), oms2.begin(), oms2.end(), std::back_inserter(smem));

	if (smem.size() > 0) {
		return true;
	}

	// // Check OR(s1) ∩ OR(s2)
	std::set_intersection(ors1.begin(), ors1.end(), ors2.begin(), ors2.end(), std::back_inserter(sreg));

	if (sreg.size() > 0) {
		return true;
	}

	return false;
}