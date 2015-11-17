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
	du.debug = debug;
	eu.debug = debug;
}

bool State::getDebug() {
	return debug;
}

void State::setPipeline(bool pipeline) {
	this->pipeline = pipeline;;
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

bool State::tickNoPipeline() {
	bool halted = false;

	switch (state) {
		case STATE_FETCH:
			// Grab the next instruction from memory
			fu.tick(&ir, &pc, &memory, false);
			state = STATE_DECODE;
		break;

		case STATE_DECODE:
			// Decode the instruction from the PC
			// and feed it into the EU
			du.tick(&ir, &eu);
			state = STATE_EXECUTE;
		break;

		case STATE_EXECUTE:
			// Poke the EU into life
			halted = eu.tick(&pc, &registerFile, &memory);
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
	if (getPipeline()) {
		// Execute
		if (!waitForExecute) {
			halted = eu.tick(&pc, &registerFile, &memory);
			if (halted) {
				return halted;
			}
		} else {
			waitForExecute--;
		}

		// Decode
		if (!waitForDecode) {
			du.tick(&ir, &eu);

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
		ir = fu.ir;
	} else {
		halted = tickNoPipeline();
	}

	if (getDebug()) {
		cout << endl;
	}

	ticks++;
	return halted;
}
