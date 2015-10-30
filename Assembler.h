
#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#include <cstdint>
#include <string>
#include <vector>
#include <regex>
#include <map>

#include "DecodeUnit.h"

class Assembler {
private:
	std::regex lineLabelRegex;
	std::regex operandLabelRegex;
	std::map<std::string, unsigned int> labels;

	bool isLineLabel(std::string line);
	bool isOperandLabel(std::string operand);
	int determineArguments(instruction_t instr);
public:
	void assemble(std::string program, std::vector<uint32_t>* out);
	unsigned int getLabelLocation(std::string label);
	Assembler();
};

#endif
