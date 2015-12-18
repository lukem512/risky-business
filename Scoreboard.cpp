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

	if (halted) {
		return false;
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
		if (debug) {
			std::cout << "[SCORE] no." << std::endl;
		}
		return false;
	}

	// Does this modify the PC contents? Is that free?
	if (hasFpc(opcode) && ResultPc != NULL) {
		if (debug) {
			std::cout << "[SCORE] no." << std::endl;
		}
		return false;
	}

	// Does this modify memory? Is that location free?
	// TODO - this locks entire memory.
	if (hasMi(opcode) && ResultM != NULL) {
		if (debug) {
			std::cout << "[SCORE] no." << std::endl;
		}
		return false;
	}

	// Does this read memory? If so, stall (TODO)
	if (hasMj(opcode) && ResultM != NULL) {
		if (debug) {
			std::cout << "[SCORE] no." << std::endl;
		}
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

		case OP_PRNT:
			Fj[eu] = r1;
			Qj[eu] = Result[r1];
			Rj[eu] = Qj[eu] == NULL;
			break;

		case OP_ST:
			Mi[eu] = true; // = im1
			ResultM = eu;

			Fj[eu] = r1;
			Qj[eu] = Result[r1];
			Rj[eu] = Qj[eu] == NULL;
			break;

		case OP_STR:
			Mi[eu] = true; // = r2
			ResultM = eu;

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
			Mj[eu] = true; // im1;
			// RMj[eu] = QMj[eu] == NULL

			Fi[eu] = r1;
			Result[r1] = eu;
			break;

		case OP_LDR:
			Mj[eu] = true; // r2;
			// QMj[eu] = ResultM[Mj[eu]];
			// RMj[eu] = QMj[eu] == NULL;
			
			Fi[eu] = r1;
			Result[r1] = eu;
			break;

		case OP_LDC:
			Fi[eu] = r1;
			Result[r1] = eu;
			break;

		case OP_UNKNOWN:
			std::cerr << "Unknown operation issued." << std::endl;
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
			if (debug) {
				std::cout << "Waiting for Fj (" << rtos(Fj[eu]) << ")." << std::endl;
			}
			if (!Rj[eu]) {
				return false;
			}
			Rj[eu] = false;
			Rk[eu] = true;
			break;

		case OP_NOP:
		case OP_HLT:
		case OP_LDC:
		case OP_B:
			Rj[eu] = true;
			Rk[eu] = true;
			break;

		case OP_LD:
		case OP_LDR:
			if (ResultM) {
				return false;
			}
			// if (!RMj[eu]) {
			//     RMj[eu] = false;
			// }
			break;

		case OP_STR:
		default:
			if (debug) {
				std::cout << "Waiting for Fj (" << rtos(Fj[eu]) << ") and Fk (";
				std::cout << rtos(Fk[eu]) << ")." << std::endl;
			}
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
    	if (debug) {
			std::cout << "[SCORE] Writing Fi (" << rtos(Fi[eu]) << ")." << std::endl;
		}
		for (int i = 0; i < eum->eus.size(); i++) {
			ExecutionUnit* peu = &eum->eus[i];
			if (peu == eu)
				continue;
			if (hasFj(peu) && !FjValid(eu, peu))
				return false;
			if (hasFk(peu) && !FkValid(eu, peu))
				return false;
		}
	}

	// Release any held register dependencies
	releaseRegisterAllocation(eu);

	// Reset the stored instruction
	Op[eu] = OP_UNKNOWN;

    if (debug) {
		std::cout << "[SCORE] Written successfully!" << std::endl;
	}

	// Relinquish the FU
	eu->ready = true;

	// Have we finished with memory?
	if (Mi[eu]) {
		ResultM = NULL;
	}

	// TODO
	if (Mj[eu]) {
		ResultM = NULL;
	}

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

	bool empty = true;

	for (int i = 0; i < eum->eus.size(); i++) {
		ExecutionUnit* peu = &eum->eus[i];
		if (S[peu] == WRITE) {
			if (write(peu)) {
				S[peu] = ISSUE;
			} else {
				empty = false;
			}
		}
	}

	for (int i = 0; i < eum->eus.size(); i++) {
		ExecutionUnit* peu = &eum->eus[i];
		if (S[peu] == EXECUTE) {
			if (execute(peu, r, m, bpt, bht)) {
				S[peu] = WRITE;

				// Invalid pipeline!
				if (peu->invalid) {
					return false;
				}

				// Return, we're halted
				if (peu->halted) {
					halted = true;
				}
			}
			empty = false;
		}
	}

	for (int i = 0; i < eum->eus.size(); i++) {
		ExecutionUnit* peu = &eum->eus[i];
		if (S[peu] == READ) {
			if (read(peu)) {
				S[peu] = EXECUTE;
			}
			empty = false;
		}
	}

	if (halted && empty) {
		return true;
	}

	// We're not halted!
	return false;
}

bool Scoreboard::hasFpc(ExecutionUnit* eu) {
	return hasFpc(Op[eu]);
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
		case OP_UNKNOWN:
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
		case OP_UNKNOWN:
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
		case OP_UNKNOWN:
			return false;

		default:
			return true;
	}
}

bool Scoreboard::FjValid(ExecutionUnit* eu, ExecutionUnit* other) {
	if (debug) {
		std::cout << "[FjValid] Check " << optos(Op[eu]) << " vs. " << optos(Op[other]);
		std::cout << ": Fj is " << rtos(Fj[other]) << " and Rj is " << Rj[other] << std::endl;
	}
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

bool Scoreboard::hasMi(ExecutionUnit* eu) {
	return hasMi(Op[eu]);
}

bool Scoreboard::hasMi(uint8_t opcode) {
	switch(opcode) {
		case OP_ST:
		case OP_STR:
			return true;

		default:
			return true;
	}
}

bool Scoreboard::hasMj(ExecutionUnit* eu) {
	return hasMj(Op[eu]);
}

bool Scoreboard::hasMj(uint8_t opcode) {
	switch(opcode) {
		case OP_LD:
		case OP_LDR:
			return true;

		default:
			return true;
	}
}

void Scoreboard::clear(ExecutionUnit* eu) {
	S[eu] = ISSUE;

	Rj[eu] = true;
	Rk[eu] = true;

	Mj[eu] = false;

	Op[eu] = OP_UNKNOWN;
    Type[eu] = EU_ISSUE_UNKNOWN;

	if (Fpc[eu]) {
		ResultPc = NULL;
		Fpc[eu] = false;
	}

	if (Mi[eu]) {
		ResultM = NULL;
		Mi[eu] = false;
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
	    		Fj[peu] = 0;
	    		Rj[peu] = true;
	    	}
    	}
    	if (hasFk(peu)) {
	    	if (Qk[peu] == eu) {
	    		Fk[peu] = 0;
	    		Rk[peu] = true;
	    	} 
    	}
    }
}
