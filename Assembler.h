// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

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
	bool debug;

	bool isLineLabel(std::string line);
	bool isOperandLabel(std::string operand);
	bool isLineComment(std::string line);
	int determineArguments(instruction_t instr);
	int determineBranchAmount(unsigned int src, unsigned int dst);
	void extractLabels(std::string program);
public:
	void assemble(std::string program, std::vector<uint32_t>* out);
	unsigned int getLabelLocation(std::string label);
	bool setDebug(bool set);
	bool getDebug();
	Assembler();
};

#endif
