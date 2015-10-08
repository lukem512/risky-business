
#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#include <cstdint>
#include <string>
#include <vector>

#include "DecodeUnit.h"

class Assembler {
public:
	void assemble(std::string program, std::vector<uint32_t>* out);
};

#endif
