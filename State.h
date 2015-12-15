// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __STATE_H
#define __STATE_H

#include <cstdint>
#include <vector>

#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "ExecutionUnit.h"
#include "MemoryLocation.h"
#include "Register.h"

#include "FetchUnitManager.h"
#include "DecodeUnitManager.h"
#include "ExecutionUnitManager.h"
#include "BranchPredictionTable.h"

using namespace std;

#define DEFAULT_MEMORY_SIZE 60000
#define DEFAULT_REGISTER_COUNT 16

#define DEFAULT_PIPELINE_WIDTH 4
#define DEFAULT_EU_WIDTH 4
#define DEFAULT_DU_WIDTH 4
#define DEFAULT_FU_WIDTH 4

#define STATE_FETCH 1
#define STATE_DECODE 2
#define STATE_EXECUTE 3

class State {
private:
	unsigned int ticks;
	unsigned int waitForFetch, waitForDecode, waitForExecute;
	int state;
	bool debug;
	bool branchPrediction;
	bool dynamicBranchPrediction;
	bool pipeline;
	bool stalled;

	BranchPredictionTable bpt;
	BranchHistoryTable bht;

	FetchUnitManager* fum;
	DecodeUnitManager* dum;
	ExecutionUnitManager* eum;
	
	// Initialise state
	void init(uint32_t memorySize = DEFAULT_MEMORY_SIZE,
	  uint8_t registerCount = DEFAULT_REGISTER_COUNT,
	  uint32_t eus = DEFAULT_EU_WIDTH,
	  uint32_t dus = DEFAULT_DU_WIDTH,
	  uint32_t fus = DEFAULT_FU_WIDTH) {
	  	// Initialise memory and registers
		memory.assign(memorySize, MemoryLocation());
		registerFile.assign(registerCount, Register());

		// Set up variable-width pipeline
		setEus(eus);
		setDus(dus);
		setFus(fus);

		// Begin in the fetch part of the cycle
		state = STATE_FETCH;

		// Initial value of program counter is set to be 0
		pc.contents = 0;

		// Clear ticks
		ticks = 0;

		// Not stalled
		stalled = false;
		waitForFetch = 0;
		waitForDecode = 1;
		waitForExecute = 2;

		// Set debug to false
		setDebug(false);

		// Set branch prediction to true by default
		setDynamicBranchPrediction(true);

		// Set pipeline to true
		setPipeline(true);
	};

	bool tickNoPipeline();
	void checkPipelineValid();

public:
	// Memory
	vector<MemoryLocation> memory;

	// Registers
	vector<Register> registerFile;
	Register pc;

	// Setter functions for pipeline width
	void setEus(uint32_t eus) {
		eum = new ExecutionUnitManager(eus);
	};

	void setDus(uint32_t dus) {
		dum = new DecodeUnitManager(dus, eum);
	};

	void setFus(uint32_t fus) {
		fum = new FetchUnitManager(fus, dum);
	};

	State();
	State(uint32_t memorySize, uint8_t registerCount,
		uint32_t eus, uint32_t dus, uint32_t fus);
	void setDebug(bool debug);
	bool getDebug();
	void setBranchPrediction(bool branchPrediction);
	bool getBranchPrediction();
	void setDynamicBranchPrediction(bool dynamicBranchPrediction);
	bool getDynamicBranchPrediction();
	void setPipeline(bool pipeline);
	bool getPipeline();
	void print();
	float getInstructionsPerTick();
	unsigned int getTicks();
	bool tick();
};

#endif
