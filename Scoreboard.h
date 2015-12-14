// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include <map>

#include <iostream>

#include "ExecutionUnitManager.h"
#include "ExecutionUnit.h"

class Scoreboard {
private:
	enum State {
		ISSUE,
		READ,
		EXECUTE,
		WRITE
	};

	std::map<ExecutionUnit*, State> S;
	std::map<ExecutionUnit*, uint8_t> Op;
	std::map<ExecutionUnit*, uint8_t> Type;
	std::map<ExecutionUnit*, uint8_t> Fi, Fj, Fk;
	std::map<ExecutionUnit*, ExecutionUnit*> Qj, Qk;
	std::map<ExecutionUnit*, bool> Rj, Rk;
	std::map<uint8_t, ExecutionUnit*> Result;

	// std::map<ExecutionUnit*, uint32_t> Mj;
	std::map<ExecutionUnit*, bool> Mj;
	// std::map<uint32_t, ExecutionUnit*> ResultM;
	std::map<ExecutionUnit*, bool> Mi;
	ExecutionUnit* ResultM;

	std::map<ExecutionUnit*, bool> Fpc;
	ExecutionUnit* ResultPc;

	ExecutionUnitManager* eum;

	bool hasFpc(uint8_t opcode);
	bool hasFpc(ExecutionUnit* eu);
	bool hasFi(uint8_t opcode);
	bool hasFi(ExecutionUnit* eu);
	bool hasFj(ExecutionUnit* eu);
	bool hasFk(ExecutionUnit* eu);
	bool FjValid(ExecutionUnit* eu, ExecutionUnit* other);
	bool FkValid(ExecutionUnit* eu, ExecutionUnit* other);
	bool hasMi(ExecutionUnit* eu);
	bool hasMi(uint8_t opcode);
	bool hasMj(ExecutionUnit* eu);
	void clear(ExecutionUnit* eu);
	void releaseRegisterAllocation(ExecutionUnit* eu);

public:
	bool debug;
	bool halted;

	Scoreboard(ExecutionUnitManager* eum) {
		this->eum = eum;

		// Important! This must be NULL.
		ResultPc = NULL;
		ResultM = NULL;

		// Set debug to false by default
		debug = false;

		// Set halted to false
		halted = false;
	}

	bool issue(uint8_t type, uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3,
		int16_t im1, Register* pc, bool speculative);
	bool read(ExecutionUnit* eu);
	bool execute(ExecutionUnit* eu, std::vector<Register>* r,
		std::vector<MemoryLocation>* m, BranchPredictionTable* bpt,
		BranchHistoryTable* bht);
	bool write(ExecutionUnit* eu);
	bool tick(std::vector<Register>* r,
		std::vector<MemoryLocation>* m, BranchPredictionTable* bpt,
		BranchHistoryTable* bht);
	void clearPipeline();
};

#endif
