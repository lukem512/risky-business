// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNITMANAGER_H
#define __FETCHUNITMANAGER_H

#include <vector>
#include <string>
#include <queue>

#include "FetchUnit.h"
#include "MemoryLocation.h"
#include "DecodeUnitManager.h"
#include "BranchPredictionTable.h"
#include "BranchHistoryTable.h"

class FetchUnitManager {
private:
	bool debug;
	bool branchPrediction;
	bool speculative;
	DecodeUnitManager* dum;
	std::vector<FetchUnit> fus;
	int lastIssued;
	std::queue<int> waiting;

public:
	Register pc;

	FetchUnitManager(unsigned int width, DecodeUnitManager* dum);
	void setDebug(bool debug);
	bool getDebug();
	void setBranchPrediction(bool branchPrediction);
	bool getBranchPrediction();
	void setSpeculative(bool speculative);
	bool getSpeculative();
	std::string toString();
	FetchUnit* getAvailableFetchUnit();
	void tick(std::vector<MemoryLocation>* m, bool pipeline,
		BranchPredictionTable* bpt, BranchHistoryTable* bht);
	void clearPipeline(uint32_t pc);
};

#endif
