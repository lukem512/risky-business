
#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "DecodeUnit.h"

class Assembler {
private:
	unsigned int currentPosition;
	std::map<std::string, unsigned int> labels;
public:
	void assemble(std::string program, std::vector<uint32_t>* out);
};

#endif
