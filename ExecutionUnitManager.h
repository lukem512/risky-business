// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __EXECUTIONUNITMANAGER_H
#define __EXECUTIONUNITMANAGER_H

#include <vector>
#include <string>

#include "ExecutionUnit.h"
#include "BranchPredictionTable.h"
#include "BranchHistoryTable.h"

#define DEFAULT_EU_COUNT 1

class ExecutionUnitManager {
private:
	bool debug;

	void init(unsigned int width = DEFAULT_EU_COUNT) {
		debug = false;
		halted = false;
		eus.assign(width, ExecutionUnit());
	};

public:
	std::vector<ExecutionUnit> eus;
	bool halted;

	ExecutionUnitManager();
	ExecutionUnitManager(unsigned int width);
	void setDebug(bool debug);
	bool getDebug();
	void setSpeculative(bool speculative);
	std::string toString();
	float getTotalInstructionsExecuted();
	void tick(std::vector<Register>* r, std::vector<MemoryLocation>* m,
		BranchPredictionTable* bpt, BranchHistoryTable* bht);
	ExecutionUnit* getAvailableExecutionUnit();
	void clearPipeline();
};

#endif
