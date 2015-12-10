// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNIT_H
#define __FETCHUNIT_H

#include <vector>

#include "Register.h"
#include "MemoryLocation.h"
#include "DecodeUnitManager.h"
#include "BranchTable.h"

class FetchUnit {
private:
	void setState(bool ready);

public:
	Register _pc;
	Register _ir;

	bool debug;		// Debugging output
	bool fetched;	// Holding fetched instruction
	bool ready;		// Ready to fetch input

	bool branchPrediction;

	bool stalled;
	bool halted;
	bool dependent;

	bool speculative;

	DecodeUnitManager* dum;
	int delta;
	
	FetchUnit(const FetchUnit &copy);
	FetchUnit(DecodeUnitManager* dum);
	std::string toString();
	bool checkForStallResolution(BranchTable* bt, Register* pc);
	void tick(std::vector<MemoryLocation>* m, std::vector<FetchUnit>* fus,
		bool pipelining, BranchTable* bt, Register* pc);
	bool passToDecodeUnit();
	void clear();
};

#endif
