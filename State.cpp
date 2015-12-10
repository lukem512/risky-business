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

void State::checkPipelineValid() {
	bool speculative = false;
	bool clearSpeculative = false;

	for (auto entry : bt.entries) {
		if (entry->actual == UNKNOWN) {
			if (debug) {
				std::cout << "Unknown branch at " << entry->location << std::endl;
				std::cout << "Predicted " << entry->predicted << std::endl;
			}
			speculative = true;
			continue;
		}
		if (entry->actual != entry->predicted) {
			if (debug) {
				std::cout << "Incorrect prediction at " << entry->location << std::endl;
				std::cout << "Predicted " << entry->predicted << " vs. " << entry->actual << std::endl;
			}
			eum->clearPipeline();
			dum->clearPipeline();
			fum->clearPipeline(entry->pc);
			// bt.update(entry);
			entry->predicted = entry->actual;
			speculative = false;
			clearSpeculative = true;
			break;
		}
	}

	// for (auto it : bt.predicted) {
	// 	if (bt.actual[it.first] == UNKNOWN) {
	// 		if (debug) {
	// 			std::cout << "Unknown branch at " << it.first << std::endl;
	// 			std::cout << "Predicted " << bt.predicted[it.first] << std::endl;
	// 		}
	// 		speculative = true;
	// 		continue;
	// 	}
	// 	if (bt.predicted[it.first] != bt.actual[it.first]) {
	// 		if (debug) {
	// 			std::cout << "Incorrect prediction at " << it.first << std::endl;
	// 			std::cout << "Predicted " << bt.predicted[it.first] << " vs. " << bt.actual[it.first] << std::endl;
	// 		}
	// 		eum->clearPipeline();
	// 		dum->clearPipeline();
	// 		fum->clearPipeline(bt.pc[it.first]);
	// 		bt.predicted[it.first] = bt.actual[it.first];
	// 		speculative = false;
	// 		clearSpeculative = true;
	// 		break;
	// 	}
	// }

	// Hoisted from foreach due to iterator
	if (clearSpeculative) {
		bt.clearSpeculative();
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
		eum->tick(&registerFile, &memory, &bt);
		halted = eum->halted;

		if (!halted) {
			// Check pipeline is valid
			bool branchPrediction = true; // TODO: parameterize
			if (branchPrediction) {
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
