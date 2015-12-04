// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include "FetchUnitManager.h"
	
FetchUnitManager::FetchUnitManager() {
	init();
}

FetchUnitManager::FetchUnitManager(unsigned int n) {
	init(n);
}

void FetchUnitManager::setDebug(bool debug) {
	this->debug = debug;

	for (int i = 0; i < eus.size(); i++) {
		fus[i].debug = debug;
	}
}

bool FetchUnitManager::getDebug() {
	return debug;
}

std::string FetchUnitManager::toString() {
	std::ostringstream ss;

	ss << "FetchUnitManager with " << fus.size() << " FUs." << std::endl;

	return ss.str();
}

void FetchUnitManager::tick(std::vector<MemoryLocation>* m, bool pipeline) {
	for (int i = 0; i < fus.size(); i++) {
		// Create dependents vector. This is all
		// other FUs.
		std::vector<FetchUnit> dependents(fus);
		dependents.erase(dependents.begin() + i);

		// Call tick!
		fus[i].tick(m, dependents, pipeline):
	}
}

// Retrieve an available FU
// Returns the FU or NULL;
FetchUnit* FetchUnitManager::getAvailableFetchUnit() {
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].ready && !fus[i].fetched) {
			return &fus[i];
		}
	}
	return NULL;
}
