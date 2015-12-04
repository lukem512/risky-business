// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNIT_H
#define __FETCHUNIT_H

#include <vector>

#include "Register.h"
#include "MemoryLocation.h"

class FetchUnit {
private:
	int delta;
	void init();

public:
	Register pc;
	Register ir;

	bool debug;		// Debugging output
	bool fetched;	// Holding fetched instruction
	bool ready;		// Ready to fetch input
	bool stalled;
	
	FetchUnit();
	std::string toString();
	bool tick(std::vector<MemoryLocation>* m);
	bool tick(std::vector<MemoryLocation>* m, bool pipelining);
};

#endif
