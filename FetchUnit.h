// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNIT_H
#define __FETCHUNIT_H

#include <vector>

#include "Register.h"
#include "MemoryLocation.h"

class FetchUnit {
private:
	void init(uint32_t pipelineWidth);

public:
	Register pc;
	
	std::vector<Register> pcs;
	std::vector<Register> irs;
	std::vector<bool> ready;

	bool debug;
	bool stalled;
	
	FetchUnit();
	FetchUnit(uint32_t pipelineWidth);
	std::string toString();
	bool tick(std::vector<MemoryLocation>* m);
	bool tick(std::vector<MemoryLocation>* m, bool pipelining);
	bool tick(std::vector<Register>* irs, std::vector<Register>* pcs, std::vector<MemoryLocation>* m);
	bool tick(std::vector<Register>* irs, std::vector<Register>* pcs, std::vector<MemoryLocation>* m,
		bool pipeline);
};

#endif
