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

	// Nothing has been issued yet...
	lastIssued = 0;

	// Not speculative yet...
	speculative = false;

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

void FetchUnitManager::setSpeculative(bool speculative) {
	this->speculative = speculative;

	for (int i = 0; i < fus.size(); i++) {
		fus[i].speculative = speculative;
	}
}

bool FetchUnitManager::getSpeculative() {
	return speculative;
}

std::string FetchUnitManager::toString() {
	std::ostringstream ss;

	ss << "FetchUnitManager with " << fus.size() << " FUs." << std::endl;

	for (int i = 0; i < fus.size(); i++) {
		ss << "[FU #" << i << "]" <<	 std::endl;
		ss << fus[i].toString();
		ss << std::endl;
	}

	return ss.str();
}

void FetchUnitManager::tick(std::vector<MemoryLocation>* m, bool pipeline, BranchTable* bt) {
	
	// Instructions should be issued to DU oldest-first;
	// this is to prevent stagnation in the pipeline.
	// According to Neil Burgess (a guest lecturer from ARM), this is
	// fairly easy to do in hardware.
	// Use a simple round-robin scheduler
	for (int i = 0; i < fus.size(); i++) {
		if (fus[lastIssued].fetched) {
			if (debug) {
				std::cout << "[FU #" << lastIssued << "] trying to issue stored instruction." << std::endl;
			}
			if (!fus[lastIssued].passToDecodeUnit()) {
				break;
			}
		}
		lastIssued = (lastIssued + 1) % fus.size();
	}

	// Resolve any stalls in pipeline
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].stalled) {
			if (fus[i].checkForStallResolution(bt, &pc)) {
				if (debug) {
					std::cout << "[FU #" << i << "] stall resolved!" << std::endl;
				}
			}
			break;
		}
	}

	// Don't fetch if we're halted
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].halted) {
			return;
		}
	}

	// Tick all FUs
	for (int i = 0; i < fus.size(); i++) {

		if (debug) {
			std::cout << "[FU #" << i << "] calling tick()." << std::endl;
		}

		// Create dependents vector. This is all
		// other FUs.
		std::vector<FetchUnit> dependents(fus);
		dependents.erase(dependents.begin() + i);

		// Call tick!
		fus[i].speculative = getSpeculative();
		fus[i].tick(m, &dependents, pipeline, bt, &pc);

		if (fus[i].speculative) {
			// TODO: mark all future fetches as such
			speculative = true;
		}

		if (fus[i].dependent) {
			// Don't fill up the other FUs
			break;
		}

		if (fus[i].stalled) {
			// Don't fill the other FUs
			break;
		}

		if (fus[i].halted) {
			// Don't fill the other FUs
			// TODO: this should persist
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
				std::cout << "[FU #" << i << "] I am available!" << std::endl;
			}
			return &fus[i];
		}
	}
	return NULL;
}

// Removes speculative values after incorrect branch prediction
void FetchUnitManager::clearPipeline(uint32_t pc) {
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].speculative) {
			fus[i].clear();
		}
	}
	this->pc.contents = pc;
	if (debug) {
		std::cout << "Setting PC to " << this->pc.toString() << " after clearing pipeline." << std::endl;
	}
	speculative = false;
}
