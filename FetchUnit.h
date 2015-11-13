// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNIT_H
#define __FETCHUNIT_H

#include <vector>

#include "Register.h"
#include "MemoryLocation.h"

class FetchUnit {
private:
	Register pc;
	Register ir;

public:
	bool debug;
	
	FetchUnit();
	void setPc(uint32_t contents);
	Register getPc();
	Register getIr();
	bool tick(std::vector<MemoryLocation>* m);
	bool tick(Register* ir, Register* pc, std::vector<MemoryLocation>* m);
	bool tick(Register* ir, Register* pc, std::vector<MemoryLocation>* m,
		bool pipeline);
};

#endif
