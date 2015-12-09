// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>

#include "DecodeUnitManager.h"

DecodeUnitManager::DecodeUnitManager(unsigned int width, ExecutionUnitManager* eum) {
	// Set debugging to false, by default
	debug = false;

	// Store a local reference to the EU manager
	this->eum = eum;

	// Nothing has been issued yet...
	lastIssued = 0;

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
	// Instructions should be issued to EU oldest-first;
	// as with the Fetch Units.

	// Round-robin.
	for (int i = 0; i < dus.size(); i++) {
		if (dus[lastIssued].decoded) {
			if (debug) {
				std::cout << "[DU #" << lastIssued << "] trying to issue decoded instruction." << std::endl;
			}
			if (!dus[lastIssued].passToExecutionUnit()) {
				break;
			}
		}
		lastIssued = (lastIssued + 1) % dus.size();
	}

	// Use a queue for simulation
	while (!waiting.empty()){
		auto i = waiting.front();
		if (debug) {
			std::cout << "[DU #" << i << "] trying to issue decoded instruction." << std::endl;
		}
		if (!dus[i].passToExecutionUnit()) {
			break;
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
