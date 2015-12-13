// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "DecodeUnitManager.h"

DecodeUnitManager::DecodeUnitManager(unsigned int width, ExecutionUnitManager* eum) {
	// Store a local reference to the EU manager
	this->eum = eum;

	// Nothing has been issued yet...
	lastIssued = 0;

	// Set out-of-order flag to true
	setOutOfOrder(true);

	// Set up the Decode Units
	dus.assign(width, DecodeUnit(eum));

	// Set debugging to false, by default
	setDebug(false);
}

DecodeUnitManager::DecodeUnitManager(unsigned int width, Scoreboard* score) {
	// Store a local reference to the Scoreboard
	this->score = score;

	// Nothing has been issued yet...
	lastIssued = 0;

	// Set out-of-order flag to true
	setOutOfOrder(true);

	// Set up the Decode Units
	dus.assign(width, DecodeUnit(score));

	// Set debugging to false, by default
	setDebug(false);
}

void DecodeUnitManager::setOutOfOrder(bool outOfOrder) {
	this->outOfOrder = outOfOrder;
}

bool DecodeUnitManager::getOutOfOrder() {
	return outOfOrder;
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
	// Instructions should be issued to EU oldest-first;
	// as with the Fetch Units.
	// Use a queue for simulation
	while (!waiting.empty()){
		auto i = waiting.front();
		if (debug) {
			std::cout << "[DU #" << i << "] trying to issue decoded instruction." << std::endl;
		}
		if (outOfOrder) {
			if (!dus[i].passToScoreboard()) {
				break;
			}
		} else {
			if (!dus[i].passToExecutionUnit()) {
				break;
			}
		}
		waiting.pop();
	}
	
	for (int i = 0; i < dus.size(); i++) {
		
		// Skip a waiting DU
		if (dus[i].decoded) {
			continue;
		}

		if (debug) {
			std::cout << "[DU #" << i << "] calling tick()." << std::endl;
		}

		// Call tick!
		dus[i].tick();

		// Add to waiting queue?
		if (dus[i].decoded) {
			waiting.push(i);
		}
	}
}

// Retrieve an available DU
// Returns the DU or NULL;
DecodeUnit* DecodeUnitManager::getAvailableDecodeUnit() {
	for (int i = 0; i < dus.size(); i++) {
		if (dus[i].ready && !dus[i].decoded) {
			if (debug) {
				std::cout << "[DU #" << i << "] I am available." << std::endl;
			}
			return &dus[i];
		}
	}
	return NULL;
}

// Removes speculative values after incorrect branch prediction
void DecodeUnitManager::clearPipeline() {
	// Clear the DUs
	for (int i = 0; i < dus.size(); i++) {
		if (dus[i].speculative) {
			dus[i].clear();
		}
	}

	// Empty the waiting queue
	std::queue<int>().swap(waiting);
}
