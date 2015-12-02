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
State::State(uint32_t memorySize, uint8_t registerCount) {
	init(memorySize, registerCount);
}

void State::setDebug(bool debug) {
	this->debug = debug;
	fu.debug = debug;

	for (int i = 0; i < du.size(); i++) {
		du[i].debug = debug;
	}

	for (int i = 0; i < eu.size(); i++) {
		eu[i].debug = debug;
	}
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
	cout << "ir = " << ir.toString() << endl;

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
	float n = 0;

	for (int i = 0; i < eu.size(); i++) {
		n += eu[i].n;
	}

	return (n / (float) getTicks());
}

bool State::tickNoPipeline() {
	bool halted = false;
	std::vector<Register> irs = {ir};

	switch (state) {
		case STATE_FETCH:
			// Grab the next instruction from memory
			fu.tick(&irs, &pc, &memory, false);
			ir = irs[0];
			state = STATE_DECODE;
		break;

		case STATE_DECODE:
			// Decode the instruction from the PC
			// and feed it into the EU
			for (int i = 0; i < du.size(); i++) {
				du[i].tick(&ir, &eu[i]);
			}
			state = STATE_EXECUTE;
		break;

		case STATE_EXECUTE:
			// Poke the EU into life
			for (int i = 0; i < eu.size(); i++) {
				halted &= eu[i].tick(&pc, &registerFile, &memory);
			}
			state = STATE_FETCH;
		break;

		default:
			// How has this happened?
			cerr << "Something strange has happened. State has reached an unknown location." << endl;
			halted = true;
		break;
	}

	return halted;
}

bool State::tick() {
	bool halted = false;

	if (getDebug()) {
		cout << getTicks() << endl;
	}

	// TODO: branch prediction
	// TODO: multiple-cycle instruction execution
	// TODO: out-of-order execution
	if (getPipeline()) {
		// Execute
		if (!waitForExecute) {
			for (int i = 0; i < eu.size(); i++) {
				halted = eu[i].tick(&pc, &registerFile, &memory);
				if (halted) {
					return halted;
				}
			}
		} else {
			waitForExecute--;
		}

		// Decode
		if (!waitForDecode) {
			for (int i = 0; i < du.size(); i++) {
				// The IR buffer is filled from L to R;
				// as soon as one is empty we can exit.
				if (!fu.fetched[i]) {
					break;
				}
				du[i].tick(&fu.ir[i], &eu[i]);
			}

			// Wait for one tick
			if (stalled) {
				waitForDecode = 2;
			}
		} else {
			waitForDecode--;
		}

		// Fetch
		if (!waitForFetch) {
			// Previous instruction caused a stall?
			// If so, use EU-updated PC
			if (stalled) {
				// Increment program counter - we want to fetch the next entry!
				fu.pc.contents = pc.contents + 1;
				stalled = fu.tick(&memory);
			} else {
				stalled = fu.tick(&memory);
			}

			// Newly fetched instruction caused a stall
			// Wait for two ticks
			if (stalled) {
				waitForFetch = 2;
			}
		} else {
			waitForFetch--;
		}

		// Update registers
		pc = fu.pc;
		ir = fu.ir[0];
	} else {
		halted = tickNoPipeline();
	}

	if (getDebug()) {
		cout << endl;
	}

	ticks++;
	return halted;
}
