// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "FetchUnitManager.h"

FetchUnitManager::FetchUnitManager(unsigned int width, DecodeUnitManager* dum) {
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

	// Set debugging to false, by default
	setDebug(false);

	// Set branch prediction to true, by default
	setDynamicBranchPrediction(true);
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


void FetchUnitManager::setBranchPrediction(bool branchPrediction) {
	this->branchPrediction = branchPrediction;

	for (int i = 0; i < fus.size(); i++) {
		fus[i].branchPrediction = branchPrediction;
	}
}

bool FetchUnitManager::getBranchPrediction() {
	return branchPrediction;
}

void FetchUnitManager::setDynamicBranchPrediction(bool dynamicBranchPrediction) {
	// Set branch prediction to be the same
	// We cannot be dynamically predicting if we're not predicting!
	setBranchPrediction(dynamicBranchPrediction);

	// Now set dynamic branch prediction flag
	this->dynamicBranchPrediction = dynamicBranchPrediction;

	// and propagate to Fetch Units
	for (int i = 0; i < fus.size(); i++) {
		fus[i].dynamicBranchPrediction = dynamicBranchPrediction;
	}
}

bool FetchUnitManager::getDynamicBranchPrediction() {
	return dynamicBranchPrediction;
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

void FetchUnitManager::tick(std::vector<MemoryLocation>* m, bool pipeline,
	BranchPredictionTable* bpt, BranchHistoryTable* bht) {
	
	// Instructions should be issued to DU oldest-first;
	// this is to prevent stagnation in the pipeline.
	// According to Neil Burgess (a guest lecturer from ARM), this is
	// fairly easy to do in hardware.
	// Use a queue for simulation
	while (!waiting.empty()){
		auto i = waiting.front();
		if (debug) {
			std::cout << "[FU #" << i << "] trying to issue stored instruction." << std::endl;
		}
		if (!fus[i].passToDecodeUnit()) {
			break;
		}
		waiting.pop();
	}

	// Resolve any stalls in pipeline
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].stalled) {
			if (fus[i].checkForStallResolution(bpt, &pc)) {
				if (debug) {
					std::cout << "[FU #" << i << "] stall resolved!" << std::endl;
				}
			} else {
				// We have stalled...
				return;
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

		// Skip a waiting FU
		if (fus[i].fetched) {
			continue;
		}

		if (debug) {
			std::cout << "[FU #" << i << "] calling tick()." << std::endl;
		}

		// Create dependents vector. This is all
		// other FUs.
		std::vector<FetchUnit> dependents(fus);
		dependents.erase(dependents.begin() + i);

		// Call tick!
		fus[i].speculative = getSpeculative();
		fus[i].tick(m, &dependents, pipeline, bpt, bht, &pc);

		// Add to waiting queue
		if (fus[i].fetched) {
			waiting.push(i);
		}

		if (fus[i].speculative) {
			// All future instructions will be marked
			// as speculative
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
	// Clear the FUs
	for (int i = 0; i < fus.size(); i++) {
		if (fus[i].speculative) {
			fus[i].clear();
		}
	}

	// Empty the waiting queue
	std::queue<int>().swap(waiting);

	// Reset the PC
	this->pc.contents = pc;

	if (debug) {
		std::cout << "Setting PC to " << this->pc.toString() << " after clearing pipeline." << std::endl;
	}

	// Reset the speculative flags
	setSpeculative(false);
}

unsigned int FetchUnitManager::getNumberOfBranches() {
	unsigned int n = 0;
	for (int i = 0; i < fus.size(); i++) {
		n += fus[i].getNumberOfBranches();
	}
	return n;
}
