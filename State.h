// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __STATE_H
#define __STATE_H

#include <cstdint>
#include <vector>

#include "DecodeUnit.h"
#include "ExecutionUnit.h"
#include "MemoryLocation.h"
#include "Register.h"

using namespace std;

#define DEFAULT_MEMORY_SIZE 32
#define DEFAULT_REGISTER_COUNT 16

#define STATE_FETCH 1
#define STATE_DECODE 2
#define STATE_EXECUTE 3

class State {
private:
	int state;
	DecodeUnit du;
	ExecutionUnit eu;
	
	// Initialise state
	void init(uint32_t memorySize = DEFAULT_MEMORY_SIZE,
	  uint8_t registerCount = DEFAULT_REGISTER_COUNT) {
	  	// Initialise memory and registers
		memory.assign(memorySize, MemoryLocation());
		registerFile.assign(registerCount, Register());

		// Begin in the fetch part of the cycle
		state = STATE_FETCH;

		// Initial value of program counter is set to be 0
		pc.contents = 0;
	};

public:
	// Memory
	vector<MemoryLocation> memory;

	// Registers
	vector<Register> registerFile;
	Register pc;
	Register ir;

	State();
	State(uint32_t memorySize, uint8_t registerCount);
	void print();
	bool tick();
};

#endif
