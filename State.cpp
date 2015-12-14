// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>

#include "State.h"
#include "common.h"

// Empty c'tor, use default sizes
State::State() {
	init();
}

// Specific c'tor
State::State(uint32_t memorySize, uint8_t registerCount,
	uint32_t eus, uint32_t dus, uint32_t fus) {
	init(memorySize, registerCount, eus, dus, fus);
}

void State::setBranchPrediction(bool branchPrediction) {
	this->branchPrediction = branchPrediction;
	fum->setBranchPrediction(branchPrediction);
}

bool State::getBranchPrediction() {
	return branchPrediction;
}

void State::setDynamicBranchPrediction(bool dynamicBranchPrediction) {
	// If enabled, enable branch prediction
	// and visa versa
	setBranchPrediction(dynamicBranchPrediction);
	this->dynamicBranchPrediction = dynamicBranchPrediction;
	fum->setDynamicBranchPrediction(dynamicBranchPrediction);
}

bool State::getDynamicBranchPrediction() {
	return dynamicBranchPrediction;
}

void State::setDebug(bool debug) {
	this->debug = debug;
	score->debug = debug;
	fum->setDebug(debug);
	dum->setDebug(debug);
	eum->setDebug(debug);
}

bool State::getDebug() {
	return debug;
}

void State::setOutOfOrder(bool outOfOrder) {
	this->outOfOrder = outOfOrder;
	dum->setOutOfOrder(outOfOrder);
}

bool State::getOutOfOrder() {
	return outOfOrder;
}

void State::setPipeline(bool pipeline) {
	this->pipeline = pipeline;
}

bool State::getPipeline() {
	return pipeline;
}

void State::print() {
	// Print registers
	for (int i = 0; i < registerFile.size(); i++) {
		cout << "r" << i << " = " << registerFile.at(i).toString() << endl;
	}

	cout << endl;

	cout << "pc = " << pc.toString() << endl;

	cout << endl;

	// Dump memory
	// TODO: display in ASCII at the end of the line
	for (int i = 0; i < memory.size(); i++) {
		cout << memory.at(i).toString();
		if (((i+1) % 4) == 0) {
			cout << endl;
		} else {
			cout << " ";
		}
	}
	cout << endl;
}

unsigned int State::getTicks() {
	return ticks;
}

float State::getInstructionsPerTick() {
	return (eum->getTotalInstructionsExecuted() / (float) getTicks());
}

void State::checkPipelineValid() {
	bool speculative = false;
	bool clearSpeculative = false;

	for (auto entry : bpt.entries) {
		if (entry->actual == UNKNOWN) {
			if (debug) {
				std::cout << "Unknown branch at " << entry->location << std::endl;
			}
			speculative = true;
			continue;
		}
		if (entry->actual != entry->predicted) {
			if (debug) {
				std::cout << "Incorrect prediction at " << entry->location << std::endl;
			}
			eum->clearPipeline();
			score->clearPipeline();
			dum->clearPipeline();
			fum->clearPipeline(entry->pc);
			entry->predicted = entry->actual;
			speculative = false;
			clearSpeculative = true;
			break;
		}
	}

	// Hoisted from foreach due to iterator
	if (clearSpeculative) {
		bpt.clearSpeculative();
	}

	// Update speculative flag
	fum->setSpeculative(speculative);
}

bool State::tick() {

	bool halted = false;

	if (getDebug()) {
		cout << getTicks() << endl;
	}

	if (getPipeline()) {
		// Execute
		if (getOutOfOrder()) {
			halted = score->tick(&registerFile, &memory, &bpt, &bht);
		} else {
			eum->tick(&registerFile, &memory, &bpt, &bht);
			halted = eum->halted;
		}

		if (!halted) {
			// Check pipeline is valid
			if (getBranchPrediction()) {
				checkPipelineValid();
			}

			if (getDebug()) {
				std::cout << std::endl;
			}

			// Decode
			dum->tick();

			if (getDebug()) {
				std::cout << std::endl;
			}

			// Fetch
			fum->tick(&memory, true, &bpt, &bht);
		}
	} else {
		switch(state) {
			case STATE_FETCH:
				fum->tick(&memory, true, &bpt, &bht);
				state = STATE_DECODE;
			break;

			case STATE_DECODE:
				dum->tick();
				state = STATE_EXECUTE;
			break;

			case STATE_EXECUTE:
				eum->tick(&registerFile, &memory, &bpt, &bht);
				halted = eum->halted;
				state = STATE_FETCH;
			break;
		}
	}

	if (getDebug()) {
		cout << endl;
	}

	ticks++;
	return halted;
};
