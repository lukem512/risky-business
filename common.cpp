// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "opcodes.h"
#include "common.h"

int strtoi(std::string str) {
	return atoi(str.c_str());
}

std::string removeWhitespace(std::string str) {
	std::string tmp = str;
	tmp = removeCharacter(tmp, ' ');
	tmp = removeCharacter(tmp, '\n');
	tmp = removeCharacter(tmp, '\r');
	tmp = removeCharacter(tmp, '\t');
	return tmp;
}

std::string removeCharacter(std::string str, char chr) {
	str.erase(std::remove(str.begin(), str.end(), chr),
      str.end());
	return str;
}

std::string hexify(uint32_t data) {
	std::ostringstream out;  
  	out << std::hex << std::setw(sizeof(data)*2) << std::setfill('0') << data;
  	return out.str();
}

std::string optos(uint8_t opcode) {
	// TODO: there must be a pre-processor macro that I can use for this...
	switch (opcode) {
		case OP_NOP:
		return "NOP";

		case OP_ADD:
		return "ADD";

		case OP_SUB:
		return "SUB";

		case OP_MUL:
		return "MUL";

		case OP_CMP:
		return "CMP";

		case OP_LD:
		return "LD";

		case OP_STR:
		return "STR";

		case OP_LDC:
		return "LDC";

		case OP_B:
		return "B";

		case OP_BZ:
		return "BZ";

		case OP_UNKNOWN:
		return "???";

		default:
		break;
	}

	return hexify(opcode);
}

uint8_t stoop(std::string opcode) {
	// TODO: there must be a pre-processor macro that I can use for this...
	if (opcode == "NOP") return OP_NOP;
	if (opcode == "ADD") return OP_ADD;
	if (opcode == "SUB") return OP_SUB;
	if (opcode == "MUL") return OP_MUL;
	if (opcode == "CMP") return OP_CMP;
	if (opcode == "LD")  return OP_LD;
	if (opcode == "STR") return OP_STR;
	if (opcode == "LDC") return OP_LDC;
	if (opcode == "B")   return OP_B;
	if (opcode == "BZ")  return OP_BZ;
	return OP_UNKNOWN;
}

std::string rtos(uint8_t reg) {
	std::ostringstream out;
	out << "r" << std::to_string(reg);
	return out.str();
}

uint8_t stor(std::string reg) {
	reg.erase(reg.begin());
	return (uint8_t) atoi(reg.c_str());
}

