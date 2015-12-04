// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include "ExecutionUnitManager.h"
	
ExecutionUnitManager::ExecutionUnitManager() {
	init();
}

ExecutionUnitManager::ExecutionUnitManager(unsigned int n) {
	init(n);
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

std::string ExecutionUnitManager::toString() {
	std::ostringstream ss;

	ss << "ExecutionUnitManager with " << eus.size() << " EUs." << std::endl;

	return ss.str();
}

// Retrieve an available EU
// Optionally, this can be extended to retrieve an EU with a specific
// capability, for example an ALU.
// Returns the EU or NULL;
ExecutionUnit* ExecutionUnitManager::getAvailableExecutionUnit() {
	for (int i = 0; i < eus.size(); i++) {
		if (eus[i].ready && !eus[i].working) {
			return &eus[i];
		}
	}
	return NULL;
}

bool ExecutionUnitManager::tick(std::vector<Register>* r, std::vector<MemoryLocation>* m) {
	for (int i = 0; i < eus.size(); i++) {
		if (eus[i].tick(r, m)) {
			return true;
		}
	}
}
