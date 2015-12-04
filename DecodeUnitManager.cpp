// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include "DecodeUnitManager.h"
	
DecodeUnitManager::DecodeUnitManager() {
	init();
}

DecodeUnitManager::DecodeUnitManager(unsigned int n) {
	init(n);
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
