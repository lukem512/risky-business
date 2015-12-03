// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <sstream>
#include <string>
#include <iostream>

#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "Dependence.h"
#include "opcodes.h"
#include "common.h"
	
void FetchUnit::init(uint32_t pipelineWidth) {
	// Initialise the pc
	pc.contents = 0;

	// Initilise the ir/pc vectors
	pcs.resize(pipelineWidth);
	irs.resize(pipelineWidth);
	ready.resize(pipelineWidth);

	// Not stalled!
	stalled = false;

	// No debugging by default
	debug = false;
}

FetchUnit::FetchUnit(uint32_t pipelineWidth) {
	init(pipelineWidth);
}


FetchUnit::FetchUnit() {
	init(1);
}

std::string FetchUnit::toString() {
	std::ostringstream ss;

	ss << "fu.pc = " << pc.toString() << std::endl;

	for (int i = 0; i < pcs.size(); i++) {
		ss << "fu.pcs[" << i << "] = " << pcs[i].toString() << std::endl;
	}

	for (int i = 0; i < irs.size(); i++) {
		ss << "fu.irs[" << i << "] = " << irs[i].toString() << std::endl;
	}

	if (stalled) {
		ss << "Currently stalled." << std::endl;
	}

	return ss.str();
}

// Use local registers, for pipelining
bool FetchUnit::tick(std::vector<MemoryLocation>* m) {
	return tick(&irs, &pcs, m, true);
}

bool FetchUnit::tick(std::vector<MemoryLocation>* m, bool pipelining) {
	return tick(&irs, &pcs, m, pipelining);
}

// Use specific registers 
bool FetchUnit::tick(std::vector<Register>* irs, std::vector<Register>* pcs, std::vector<MemoryLocation>* m) {
	return tick(irs, pcs, m, false);
}

// General-purpose
bool FetchUnit::tick(std::vector<Register>* irs, std::vector<Register>* pcs, std::vector<MemoryLocation>* m, bool pipeline) {

	// Set all IR contents to not fetched
	ready.assign(irs->size(), false);

	bool stalled = false;

	// For all instruction registers...
	// Fill from left to right
	// Stop if a stall is reached or a dependency encountered
	for (int i = 0; i < irs->size(); i++) {

		// Grab next instruction
		uint32_t next =  m->at(pc.contents).contents;

		if (debug) {
			std::cout << "[FU #" << i << "] Fetched instruction " << optos(next) << std::endl;
		}

		// Check for dependencies
		// Check ir[i] with ir[j] where j in {0 to i}
		for (int j = 0; j < i; j++) {
			if (Dependence::depends(next, irs->at(j).contents, pc.contents)) {
				// If a dependency is reached, return
				if (debug) {
					std::cout << "Dependency between FU #" << i << " and FU #" << j << " found." << std::endl;
					std::cout << optos(next) << " and " << optos(irs->at(j).contents) << std::endl;
				}
				return false;
			}
		}
		
		// Fill the instruction registers
		irs->at(i).contents = next;

		// Update flags
		ready[i] = true;

		// Change to make to PC for next fetch
		int delta = 0;

		// [Pre-] decode the instruction and check for a branch
		// if the branch is conditional, stall
		if (pipeline) {
			instruction_t instr = *(instruction_t*) &irs->at(i).contents;
			instruction_oi_t data = *(instruction_oi_t *) &irs->at(i).contents; // B
			instruction_ori_t dataCond = *(instruction_ori_t *) &irs->at(i).contents; // Conditional

			switch (instr.opcode) {
				case OP_B:
					if (debug) {
						std::cout << "[FU #" << i << "] Found a branch at location " << std::to_string(pc.contents);
						std::cout <<  ". Jumping by " << data.im1 << "." << std::endl;
					}
					delta = data.im1;
				break;

				case OP_BZ:
				case OP_BNZ:
				case OP_BLTZ:
				case OP_BLTEZ:
				case OP_BGTZ:
				case OP_BGTEZ:
					if (debug) {
						std::cout << "[FU #" << i << "] Found a conditional branch at location " << std::to_string(pc.contents) << std::endl;
						std::cout << "Decide whether to jump by " << dataCond.im1 << " or not." << std::endl;
					}
					stalled = true;
				break;

				case OP_HLT:
					if (debug) {
						std::cout << "[FU #" << i << "] Found a halt at location " << std::to_string(pc.contents) << std::endl;
					}
					stalled = true;
				break;
			}
		}

		// Always increment
		// Even if we've already added an offset using a branch.
		// This is because the offset operand assumes an incremented PC
		pc.contents++;

		// Add to PC file
		pcs->at(i).contents = pc.contents;

		// And jump ahead for the next fetch
		pc.contents += delta;

		if (stalled) {
			break;
		}
	}

	return stalled;
}
