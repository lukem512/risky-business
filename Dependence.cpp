// Get the set of memory locations read by instr
std::vector<MemoryLocation> Dependence::IM(instruction_t instr, Register* pc, std::vector<Register>* r) {
	std::vector<MemoryLocation> vim;

	if (IS_OI(instr.opcode)) {
		instruction_oi_t data = *(instruction_oi_t *) &ir->contents;

		switch (instr.opcode) {
			case OP_B:
				vim.push_back(new MemoryLocation(pc->contents + data.im1));
			break;
		}
	}

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;

		switch (instr.opcode) {
			case OP_LD:
				vim.push_back(new MemoryLocation(data.im1));
			break;

			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				vim.push_back(new MemoryLocation(pc->contents + data.im1));
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &ir->contents;

		switch (instr.opcode) {
			case OP_LDR:
				vim.push_back(new MemoryLocation(r[data.r2.contents].contents));
			break;
		}
	}

	return vim;
}

// Get the set of registers read by instr
std::vector<Register> Dependence::IR(instruction_t instr) {
	std::vector<Register> vir;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;

		switch (instr.opcode) {
			case OP_ST:
			case OP_BZ:
			case OP_BNZ:
			case OP_BLTZ:
			case OP_BLTEZ:
			case OP_BGTZ:
			case OP_BGTEZ:
				vir.push_back(new Register(data.r1));
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &ir->contents;

		switch (instr.opcode) {
			case: OP_STR:
				vir.push_back(new Register(data.r1));
			break;
		}

		vir.push_back(new Register(data.r2));
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &ir->contents;
		vir.push_back(new Register(data.r2));
		vir.push_back(new Register(data.r3));
	}

	if (instr.opcode == OP_PRNT) {
		vir.push_back(new Register(data.r1));
	}

	return vir;
}

// Get the set of memory locations written by instr
std::vector<MemoryLocation> Dependence::OM(instruction_t instr) {
	std::vector<MemoryLocation> vom;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;

		switch (instr.opcode) {
			case OP_ST:
				vim.push_back(new MemoryLocation(data.im1));
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &ir->contents;

		switch (instr.opcode) {
			case OP_STR:
				vim.push_back(new MemoryLocation(r[data.r1.contents].contents));
			break;
		}
	}

	return vom;
}

// Get the set of registers written by instr
std::vector<Register> Dependence::OR(instruction_t instr) {
	std::vector<Register> vor;

	if (IS_ORI(instr.opcode)) {
		instruction_ori_t data = *(instruction_ori_t *) &ir->contents;
		switch (instr.opcode) {
			case OP_LD:
				vor.push_back(new Register(data.r1));
			break;
		}
	}

	if (IS_ORR(instr.opcode)) {
		instruction_orr_t data = *(instruction_orr_t *) &ir->contents;

		switch (instr.opcode) {
			case: OP_MOV:
			case: OP_LDR:
				vor.push_back(new Register(data.r1));
			break;
		}
	}

	if (IS_ORRR(instr.opcode)) {
		instruction_orrr_t data = *(instruction_orrr_t *) &ir->contents;
		vor.push_back(new Register(data.r1));
	}

	return vor;
}

bool Dependence::depends(Register s1, Register s2, Register* pc, std::vector<Register>* r) {
	// Bernstein condition
	// s2 depends on s1 if the following is true:
	// [I(s1) ∩ O(s2)] ∪ [O(s1) ∩ I(s2)] ∪ [O(s1) ∩ O(s2)] ≠ Ø
	instruction_t s1_instr = *(instruction_t*) &s1->contents;
	instruction_t s2_instr = *(instruction_t*) &s2->contents;

	// Print, NOP never causes a dep.
	if (s1_instr.opcode == OP_NOP || s1_instr.opcode == OP_PRNT) {
		return false;
	}

	// Compute the sets
	std::vector<MemoryLocation> ims1, oms1, ims2, oms2;
	std::vector<Register> irs1, ors1, irs2, ors2;

	ims1 = IM(s1_instr);
	ims2 = IM(s2_instr);

	oms1 = OM(s1_instr);
	oms2 = OM(s2_instr);

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