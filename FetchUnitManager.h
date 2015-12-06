// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNITMANAGER_H
#define __FETCHUNITMANAGER_H

#include <vector>
#include <string>

#include "FetchUnit.h"
#include "MemoryLocation.h"
#include "DecodeUnitManager.h"
#include "BranchTable.h"

class FetchUnitManager {
private:
	bool debug;
	DecodeUnitManager* dum;
	std::vector<FetchUnit> fus;
	int lastIssued;

public:
	Register pc;

	FetchUnitManager(unsigned int width, DecodeUnitManager* dum);
	void setDebug(bool debug);
	bool getDebug();
	std::string toString();
	FetchUnit* getAvailableFetchUnit();
	void tick(std::vector<MemoryLocation>* m, bool pipeline, BranchTable* bt);
};

#endif
