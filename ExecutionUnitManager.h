// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __EXECUTIONUNITMANAGER_H
#define __EXECUTIONUNITMANAGER_H

#include <vector>
#include <string>

#include "ExecutionUnit.h"

#define DEFAULT_EU_COUNT 1

class ExecutionUnitManager {
private:
	bool debug;
	std::vector<ExecutionUnit> eus;

	void init(unsigned int n = DEFAULT_EU_COUNT) {
		debug = false;
		eus.assign(width, ExecutionUnit());
	};

public:
	ExecutionUnitManager();
	ExecutionUnitManager(unsigned int n);
	void setDebug(bool debug);
	bool getDebug();
	std::string toString();
	ExecutionUnit* getAvailableExecutionUnit();
};

#endif
