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
State::State(uint32_t memorySize, uint8_t registerCount, uint32_t pipelineWidth) {
	init(memorySize, registerCount, pipelineWidth);
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

void State::setPipelineWidth(unsigned int width) {
	// Set up pipeline
	fu = FetchUnit(width);
	du.assign(width, DecodeUnit());
	eu.assign(width, ExecutionUnit());
}

unsigned int State::getPipelineWidth() {
	return du.size();
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
	float n = 0;

	for (int i = 0; i < eu.size(); i++) {
		n += eu[i].n;
	}

	return (n / (float) (getTicks() + 1));
}

bool State::tickNoPipeline() {
	bool halted = false;
	switch (state) {
		case STATE_FETCH:
			// Grab the next instruction from memory
			fu.tick(&memory, false);
			state = STATE_DECODE;
		break;

		case STATE_DECODE:
			// Decode the instruction from the PC
			// and feed it into the EU
			for (int i = 0; i < du.size(); i++) {
				// The IR buffer is filled from L to R;
				// as soon as one is empty we can exit.
				if (!fu.ready[i]) {
					du[i].ready = false;
					break;
				}

				du[i].tick(&fu.irs[i], &fu.pcs[i], &eu[i]);

				// Update flags
				du[i].ready = true;
				fu.ready[i] = false;
			}
			state = STATE_EXECUTE;
		break;

		case STATE_EXECUTE:
			// Poke the EU into life
			for (int i = 0; i < eu.size(); i++) {
				// Ensure the decode unit feeding in
				// has decoded its instruction.
				if (!du[i].ready) {
					break;
				}

				bool halts = false;

				// Store the old PC
				Register current;
				current.contents = eu[i].pc.contents;

				halts = eu[i].tick(&registerFile, &memory);

				// Does the PC need updating?
				if (current.contents != eu[i].pc.contents) {
					fu.pc.contents = eu[i].pc.contents;
				}

				// Clear decode ready flag
				du[i].ready = false;

				// Did the instruction halt?
				if (halts) {
					halted = true;
				}
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

	if (getPipeline()) {
		// Execute
		if (!waitForExecute) {
			halted = eum.tick(&registerFile, &memory);
		} else {
			waitForExecute--;
		}

		if (!waitForDecode) {
			dum.tick();
		} else {
			waitForDecode--;
		}

		if (!waitForFetch) {
			if (stalled) {
				// TODO: which PC value do we use?
				// Also, we now need to wait until EU executes
				// a specific PC value, not just wait a certain amount of ticks
			}
			stalled = fum.tick(&memory, true);
		} else {
			waitForFetch--;
		}
	}

	return halted;
};

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
				// Ensure the decode unit feeding in
				// has decoded its instruction.
				if (!du[i].ready) {
					break;
				}

				// If the PC value is changed, update global PC
				Register current;
				current.contents = eu[i].pc.contents;
				halted = eu[i].tick(&registerFile, &memory);

				// NOTE: This gets complicated with branch-prediction
				if (current.contents != eu[i].pc.contents) {
					pc.contents = eu[i].pc.contents;
				}

				du[i].ready = false;
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
				if (!fu.ready[i]) {
					du[i].ready = false;
					break;
				}
				// TODO: this should pass ALL execution units
				// and the DU should select one.
				// This would allow for heterogenous EUs.
				du[i].tick(&fu.irs[i], &fu.pcs[i], &eu[i]);
				du[i].ready = true;
				fu.ready[i] = false;
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
				fu.pc.contents = pc.contents;	
			}

			stalled = fu.tick(&memory);

			// Newly fetched instruction caused a stall
			// Wait for two ticks
			if (stalled) {
				waitForFetch = 2;
			}

			// Update registers
			pc = fu.pc;
		} else {
			waitForFetch--;
		}
	} else {
		halted = tickNoPipeline();
	}

	if (getDebug()) {
		cout << endl;
	}

	ticks++;
	return halted;
}
