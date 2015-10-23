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

bool State::tick() {
	bool halted = false;
	
	switch (state) {
		case STATE_FETCH:
			ir.contents = memory.at(pc.contents).contents;
			pc.contents++;
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
			if (eu.tick(&pc, &registerFile, &memory)) {
				// Execution is halted
				halted = true;
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
