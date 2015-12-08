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

void State::setDebug(bool debug) {
	this->debug = debug;
	fum->setDebug(debug);
	dum->setDebug(debug);
	eum->setDebug(debug);
}

bool State::getDebug() {
	return debug;
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

bool State::tick() {

	bool halted = false;

	if (getDebug()) {
		cout << getTicks() << endl;
	}

	if (getPipeline()) {
		// Execute
		eum->tick(&registerFile, &memory, &bt);
		halted = eum->halted;

		if (!halted) {
			bool branchPrediction = true;
			bool speculative = false;
			if (branchPrediction) {
				for (auto it : bt.predicted) {
					if (bt.actual[it.first] == UNKNOWN) {
						speculative = true;
						continue;
					}
					if (bt.predicted[it.first] != bt.actual[it.first]) {
						if (debug) {
							std::cout << "Incorrect prediction at " << it.first << std::endl;
						}
						eum->clearPipeline();
						dum->clearPipeline();
						fum->clearPipeline(bt.pc[it.first]);
						bt.predicted[it.first] = bt.actual[it.first];
						break;
					}
				}
			}

			// Update speculative wire
			fum->setSpeculative(speculative);

			if (getDebug()) {
				std::cout << std::endl;
			}

			// Decode
			dum->tick();

			if (getDebug()) {
				std::cout << std::endl;
			}

			// Fetch
			fum->tick(&memory, true, &bt);
		}
	} else {
		switch(state) {
			case STATE_FETCH:
				fum->tick(&memory, true, &bt);
				state = STATE_DECODE;
			break;

			case STATE_DECODE:
				dum->tick();
				state = STATE_EXECUTE;
			break;

			case STATE_EXECUTE:
				eum->tick(&registerFile, &memory, &bt);
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
