// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include <map>

#include "ExecutionUnitManager.h"
#include "ExecutionUnit.h"

class Scoreboard {
private:
	std::map<ExecutionUnit*, uint8_t> Op;
	std::map<ExecutionUnit*, uint8_t> Type;
	std::map<ExecutionUnit*, uint8_t> Fi, Fj, Fk;
	std::map<ExecutionUnit*, ExecutionUnit*> Qj, Qk;
	std::map<ExecutionUnit*, bool> Rj, Rk;
	std::map<uint8_t, ExecutionUnit*> Result;

	ExecutionUnitManager* eum;

public:

	bool issue(uint8_t type, uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3,
		int16_t im1, Register* pc, bool speculative);
	bool read(ExecutionUnit* eu);
	bool execute(ExecutionUnit* eu, std::vector<Register>* r,
		std::vector<MemoryLocation>* m, BranchPredictionTable* bpt,
		BranchHistoryTable* bht);
	bool write(ExecutionUnit* eu);
};

#endif
