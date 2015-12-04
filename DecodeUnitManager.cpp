// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include "DecodeUnitManager.h"

DecodeUnitManager::DecodeUnitManager(unsigned int width, ExecutionUnitManager* eum) {
	// Set debugging to false, by default
	debug = false;

	// Store a local reference to the EU manager
	this->eum = eum;

	// Set up the Decode Units
	dus.assign(width, DecodeUnit(eum));
}

void DecodeUnitManager::setDebug(bool debug) {
	this->debug = debug;

	for (int i = 0; i < dus.size(); i++) {
		dus[i].debug = debug;
	}
}

bool DecodeUnitManager::getDebug() {
	return debug;
}

std::string DecodeUnitManager::toString() {
	std::ostringstream ss;

	ss << "DecodeUnitManager with " << dus.size() << " DUs." << std::endl;

	return ss.str();
}

void DecodeUnitManager::tick() {
	for (int i = 0; i < dus.size(); i++) {
		// Call tick!
		dus[i].tick();
	}
}

// Retrieve an available DU
// Returns the DU or NULL;
DecodeUnit* DecodeUnitManager::getAvailableDecodeUnit() {
	for (int i = 0; i < dus.size(); i++) {
		if (dus[i].ready && !dus[i].decoded) {
			return &dus[i];
		}
	}
	return NULL;
}
