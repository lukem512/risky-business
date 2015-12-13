// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include <map>

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

	ExecutionUnitManager* eum;

	bool hasFi(ExecutionUnit* eu);
	bool hasFi(uint8_t opcode);
	bool hasFj(ExecutionUnit* eu);
	bool hasFk(ExecutionUnit* eu);
	bool FjValid(ExecutionUnit* eu, ExecutionUnit* other);
	bool FkValid(ExecutionUnit* eu, ExecutionUnit* other);

public:
	bool debug;

	Scoreboard(ExecutionUnitManager* eum) {
		this->eum = eum;

		// Set debug to false by default
		debug = false;
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
};

#endif
