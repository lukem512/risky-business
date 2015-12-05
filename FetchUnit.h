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
	DecodeUnitManager* dum;
	int delta;

	void setState(bool ready);
	bool passToDecodeUnit();

public:
	Register _pc;
	Register _ir;

	bool debug;		// Debugging output
	bool fetched;	// Holding fetched instruction
	bool ready;		// Ready to fetch input

	bool stalled;
	bool halted;
	
	FetchUnit(DecodeUnitManager* dum);
	std::string toString();
	void tick(std::vector<MemoryLocation>* m, std::vector<FetchUnit>* fus,
		bool pipelining, BranchTable* bt, Register* pc);
};

#endif
