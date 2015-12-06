// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "ExecutionUnitManager.h"
	
ExecutionUnitManager::ExecutionUnitManager() {
	init();
}

ExecutionUnitManager::ExecutionUnitManager(unsigned int width) {
	init(width);
}

void ExecutionUnitManager::setDebug(bool debug) {
	this->debug = debug;

	for (int i = 0; i < eus.size(); i++) {
		eus[i].debug = debug;
	}
}

bool ExecutionUnitManager::getDebug() {
	return debug;
}

float ExecutionUnitManager::getTotalInstructionsExecuted() {
	float n = 0;

	for (int i = 0; i < eus.size(); i++) {
		n += eus[i].n;
	}

	return n;
}

std::string ExecutionUnitManager::toString() {
	std::ostringstream ss;

	ss << "ExecutionUnitManager with " << eus.size() << " EUs." << std::endl;

	return ss.str();
}

void ExecutionUnitManager::tick(std::vector<Register>* r, std::vector<MemoryLocation>* m,
	BranchTable* bt) {
	for (int i = 0; i < eus.size(); i++) {
		if (debug) {
			std::cout << "[EU #" << i << "] calling tick()." << std::endl;
		}

		// Tick!
		eus[i].tick(r, m, bt);

		// Encountered a halt?
		if (eus[i].halted) {
			halted = true;
		}
	}
}

// Retrieve an available EU
// Optionally, this can be extended to retrieve an EU with a specific
// capability, for example an ALU.
// Returns the EU or NULL;
ExecutionUnit* ExecutionUnitManager::getAvailableExecutionUnit() {
	for (int i = 0; i < eus.size(); i++) {
		if (eus[i].ready && !eus[i].working) {
			if (debug) {
				std::cout << "EU #" << i << " is available." << std::endl;
			}
			return &eus[i];
		}
	}
	return NULL;
}
