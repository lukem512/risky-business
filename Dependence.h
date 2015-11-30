// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __DEPENDENCE_H
#define __DEPENDENCE_H

#include <vector>
#include <set>

#include "Register.h"
#include "MemoryLocation.h"
#include "DecodeUnit.h"

class Dependence {
private:
	static std::set<uint32_t> IM(instruction_t instr, uint32_t pc, std::vector<Register>* r);
	static std::set<uint8_t> IR(instruction_t instr);
	static std::set<uint32_t> OM(instruction_t instr, std::vector<Register>* r);
	static std::set<uint8_t> OR(instruction_t instr);
	static bool __depends(uint32_t s1, uint32_t s2, uint32_t pc, std::vector<Register>* r);

public:
	static bool depends(uint32_t s1, uint32_t s2, uint32_t pc, std::vector<Register>* r);
	static bool depends(uint32_t s1, uint32_t s2, Register* pc, std::vector<Register>* r);
	static bool depends(Register s1, Register s2, Register* pc, std::vector<Register>* r);
};

#endif