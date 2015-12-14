// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

// Scoreboarding Out-of-Order Algorithm
// Ref: https://en.wikipedia.org/wiki/Scoreboarding

#include <iostream>

#include "Scoreboard.h"
#include "common.h"
#include "opcodes.h"

bool Scoreboard::issue(uint8_t type, uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3,
		int16_t im, Register* pc, bool speculative) {

	if (debug) {
		std::cout << "[SCORE] Trying to issue " << optos(opcode) << std::endl;
	}

	// Get an available execution unit
	ExecutionUnit* eu = eum->getAvailableExecutionUnit();

	// Is the execution unit free?
	if (eu == NULL) {
		return false;
	}

	if (debug) {
		std::cout << "[SCORE] Is the destination register free?" << std::endl;
	}

	// Is the destination register free?
	if (hasFi(opcode) && Result[r1] != NULL) {
		return false;
	}

	if (hasFpc(opcode) && ResultPc != NULL) {
		return false;
	}

	if (debug) {
		std::cout << "[SCORE] yes." << std::endl;
	}

	// Issue the instruction to the EU
	eu->issue(type, opcode, r1, r2, r3, im, pc, speculative);

	Op[eu] = opcode;
    Type[eu] = type;

    Rj[eu] = true;
    Rk[eu] = true;

    Fpc[eu] = false;

	switch(opcode) {
		case OP_BZ:
		case OP_BNZ:
		case OP_BLTZ:
		case OP_BLTEZ:
		case OP_BGTZ:
		case OP_BGTEZ:
			Fpc[eu] = true;
			ResultPc = eu;

			Fj[eu] = r1;
			Qj[eu] = Result[r1];
			Rj[eu] = Qj[eu] == NULL;
			break;

		case OP_B:
			Fpc[eu] = true;
			ResultPc = eu;
			break;

		case OP_ST:
		case OP_PRNT:
			Fj[eu] = r1;
			Qj[eu] = Result[r1];
			Rj[eu] = Qj[eu] == NULL;
			break;

		case OP_STR:
			Fj[eu] = r1;
			Fk[eu] = r2;
			Qj[eu] = Result[r1];
    		Qk[eu] = Result[r2];
    		Rj[eu] = Qj[eu] == NULL;
    		Rk[eu] = Qk[eu] == NULL;
			break;

		case OP_NOP:
		case OP_HLT:
			break;

		case OP_LD:
		case OP_LDC:
			Fi[eu] = r1;
			Result[r1] = eu;
			break;

		default:
			// Destination
			Fi[eu] = r1;

			// Operands
			Fj[eu] = r2;
			Fk[eu] = r3;

			// FU that will provide result of operands
			Qj[eu] = Result[r2];
    		Qk[eu] = Result[r3];

    		// Operands ready?
    		Rj[eu] = Qj[eu] == NULL;
    		Rk[eu] = Qk[eu] == NULL;

    		// This will provide result of destination
			Result[r1] = eu;
			break;
	}

	// Update state
	S[eu] = READ;

	return true;
}

bool Scoreboard::read(ExecutionUnit* eu) {
	if (debug) {
		std::cout << "[SCORE] Read operands for " << optos(Op[eu]) 	 << std::endl;
		std::cout << "[SCORE] Waiting for input registers to become available." << std::endl;
	}

	switch(Op[eu]) {
		case OP_ST:
		case OP_PRNT:
		case OP_BZ:
		case OP_BNZ:
		case OP_BLTZ:
		case OP_BLTEZ:
		case OP_BGTZ:
		case OP_BGTEZ:
			if (!Rj[eu]) {
				return false;
			}
			Rj[eu] = false;
			Rk[eu] = true;
			break;

		case OP_NOP:
		case OP_HLT:
		case OP_LD:
		case OP_LDC:
		case OP_B:
			Rj[eu] = true;
			Rk[eu] = true;
			break;

		case OP_STR:
		default:
			if (!Rj[eu] || !Rk[eu]) {
				return false;
			}
			Rj[eu] = false;
			Rk[eu] = false;
			break;
	}

    if (debug) {
		std::cout << "[SCORE] They\'re ready!" << std::endl;
	}

    return true;
}

bool Scoreboard::execute(ExecutionUnit* eu, std::vector<Register>* r,
	std::vector<MemoryLocation>* m, BranchPredictionTable* bpt,
	BranchHistoryTable* bht) {

	if (debug) {
		std::cout << "[SCORE] Execution Unit is whirring." << std::endl;
	}

	eu->tick(r, m, bpt, bht);

	if (debug) {
		std::cout << "[SCORE] Ready flag is " << eu->ready;
		std::cout << " and working flag is " << eu->working << std::endl;
	}

	bool finished = (eu->ready && !eu->working);

	// We still want to reserve the FU!
	if (finished) {
		eu->ready = false;
	}

	return finished;
}

// Delay until earlier instructions have completed their 
// read_operands stage
bool Scoreboard::write(ExecutionUnit* eu) {

	if (debug) {
		std::cout << "[SCORE] Attempting to write back results for " << optos(Op[eu]) << std::endl;
	}

	// OK, do it!
    if (hasFi(eu)) {
		for (int i = 0; i < eum->eus.size(); i++) {
			ExecutionUnit* peu = &eum->eus[i];
			if (hasFj(peu) && !FjValid(eu, peu))
				return false;
			if (hasFk(peu) && !FkValid(eu, peu))
				return false;
		}
	}

	// Release any held register dependencies
	releaseRegisterAllocation(eu);

    if (debug) {
		std::cout << "[SCORE] Written successfully!" << std::endl;
	}

	// Relinquish the FU
	eu->ready = true;

	// Have we finished with PC?
	if (Fpc[eu]) {
		ResultPc = NULL;
	}

    // NULL means no FU generates the register's result
    Result[Fi[eu]] = NULL;
    return true;
}

bool Scoreboard::tick(std::vector<Register>* r,
	std::vector<MemoryLocation>* m, BranchPredictionTable* bpt,
	BranchHistoryTable* bht) {
	for (int i = 0; i < eum->eus.size(); i++) {
		ExecutionUnit* peu = &eum->eus[i];
		switch (S[peu]) {
			case ISSUE:
			break;

			case READ:
				if (read(peu)) {
					S[peu] = EXECUTE;
				}
				break;

			case EXECUTE:
				if (execute(peu, r, m, bpt, bht)) {
					S[peu] = WRITE;

					// Invalid pipeline!
					if (peu->invalid) {
						return false;
					}

					// Return, we're halted
					if (peu->halted) {
						// TODO: HLT needs to be delayed if we're OoO
						return false;
					}
				}
				break;

			case WRITE:
				if (write(peu)) {
					S[peu] = ISSUE;
				}
				break;

			default:
				std::cerr << "Unknown Scoreboard state reached." << std::endl;
			break;
		}
	}

	// We're not halted!
	return false;
}

bool Scoreboard::hasFpc(ExecutionUnit* eu) {
	// return hasFpc(Op[eu]);
	return Fpc[eu];
}

bool Scoreboard::hasFpc(uint8_t opcode) {
	switch(opcode) {
		case OP_BZ:
		case OP_BNZ:
		case OP_BLTZ:
		case OP_BLTEZ:
		case OP_BGTZ:	
		case OP_BGTEZ:
		case OP_B:
			return true;

		default:
			return false;
	}
}

bool Scoreboard::hasFi(ExecutionUnit* eu) {
	return hasFi(Op[eu]);
}

bool Scoreboard::hasFi(uint8_t opcode) {
	switch(opcode) {
		case OP_BZ:
		case OP_BNZ:
		case OP_BLTZ:
		case OP_BLTEZ:
		case OP_BGTZ:
		case OP_BGTEZ:
		case OP_B:
		case OP_ST:
		case OP_PRNT:
		case OP_STR:
		case OP_NOP:
		case OP_HLT:
			return false;

		default:
			return true;
	}
}

bool Scoreboard::hasFj(ExecutionUnit* eu) {
	switch(Op[eu]) {
		case OP_NOP:
		case OP_HLT:
		case OP_B:
		case OP_LD:
		case OP_LDC:
			return false;

		default:
			return true;
	}
}

bool Scoreboard::hasFk(ExecutionUnit* eu) {
	switch(Op[eu]) {
		case OP_BZ:
		case OP_BNZ:
		case OP_BLTZ:
		case OP_BLTEZ:
		case OP_BGTZ:
		case OP_BGTEZ:
		case OP_ST:
		case OP_PRNT:
		case OP_NOP:
		case OP_HLT:
		case OP_B:
		case OP_LD:
		case OP_LDC:
			return false;

		default:
			return true;
	}
}

bool Scoreboard::FjValid(ExecutionUnit* eu, ExecutionUnit* other) {
	if (Fj[other] != Fi[eu] || !Rj[other]) {
		return true;
	}
	return false;
}

bool Scoreboard::FkValid(ExecutionUnit* eu, ExecutionUnit* other) {
	if (Fk[other] != Fi[eu] || !Rk[other]) {
		return true;
	}
	return false;
}

void Scoreboard::clear(ExecutionUnit* eu) {
	S[eu] = ISSUE;

	Rj[eu] = true;
	Rk[eu] = true;

	Fpc[eu] = false;

	Op[eu] = OP_UNKNOWN;
    Type[eu] = EU_ISSUE_UNKNOWN;

	if (Fpc[eu]) {
		ResultPc = NULL;
	}

	Result[Fi[eu]] = NULL;
}

void Scoreboard::clearPipeline() {
	for (int i = 0; i < eum->eus.size(); i++) {
		ExecutionUnit* peu = &eum->eus[i];
		if (peu->speculative) {
			clear(peu);
			releaseRegisterAllocation(peu);
		}
	}
}

// Releases the register allocation for Execution Unit
void Scoreboard::releaseRegisterAllocation(ExecutionUnit* eu) {
	for (int i = 0; i < eum->eus.size(); i++) {
    	ExecutionUnit* peu = &eum->eus[i];
    	if (hasFj(peu)) {
	    	if (Qj[peu] == eu) {
	    		Rj[peu] = true;
	    	}
    	}
    	if (hasFk(peu)) {
	    	if (Qk[peu] == eu) {
	    		Rk[peu] = true;
	    	} 
    	}
    }
}
