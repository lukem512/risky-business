// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "FetchUnitManager.h"

FetchUnitManager::FetchUnitManager(unsigned int width, DecodeUnitManager* dum) {
	// Set debugging to false, by default
	debug = false;

	// Store a local reference to the DU manager
	this->dum = dum;

	// Set up PC
	pc.contents = 0;

	// Set up the Fetch Units
	fus.assign(width, FetchUnit(dum));
}

void FetchUnitManager::setDebug(bool debug) {
	this->debug = debug;

	for (int i = 0; i < fus.size(); i++) {
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

void FetchUnitManager::tick(std::vector<MemoryLocation>* m, bool pipeline, BranchTable* bt) {
	int s = 0;

	// If any FU is stalled, start from there
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].stalled) {
			s = i;
			break;
		}
	}

	// TODO: when a HLT is fetched, all Fetch Units
	// should stop reading new instructions

	for (int i = s; i < fus.size(); i++) {

		if (debug) {
			std::cout << "[FU #" << i << "] calling tick()." << std::endl;
		}

		// Create dependents vector. This is all
		// other FUs.
		std::vector<FetchUnit> dependents(fus);
		dependents.erase(dependents.begin() + i);

		// Call tick!
		fus[i].tick(m, &dependents, pipeline, bt, &pc);

		if (fus[i].stalled) {
			// Don't fill the other FUs
			break;
		}

		if (fus[i].halted) {
			// Don't fill the other FUs
			break;
		}
	}
}

// Retrieve an available FU
// Returns the FU or NULL;
FetchUnit* FetchUnitManager::getAvailableFetchUnit() {
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].ready && !fus[i].fetched) {
			if (debug) {
				std::cout << "FU #" << i << " is available." << std::endl;
			}
			return &fus[i];
		}
	}
	return NULL;
}
