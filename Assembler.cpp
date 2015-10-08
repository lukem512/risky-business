
#include <iostream>
#include <sstream>
#include <cstring>

#include "Assembler.h"
#include "DecodeUnit.h"
#include "ExecutionUnit.h"
#include "common.h"

void printUnhandledTypeError(int type) {
	std::cerr << "Unhandled instruction type encountered: " << type << std::endl;
}

void printInvalidArgumentCountError(std::string instr, int got, int expected) {
	std::cerr << "Incorrect number of arguments specified for opcode " << instr << ". Expected " << expected << " and got " << got << "." << std::endl;
}

int determineArguments(instruction_t instr) {
	if (instr.opcode & OP_MASK_ORRR) { // All 3-register operand instructions are 10xx
		return EU_ISSUE_ORRR;
	} else
	if (instr.opcode & OP_MASK_ORI) { // All register, immediate operand instructions are 01xx
		return EU_ISSUE_ORI;
	} else
	if (instr.opcode == OP_NOP) { // NOP is 0000
		return EU_ISSUE_O;
	} else
	if (instr.opcode == OP_PRNT) { // PRNT is 0001
		return EU_ISSUE_OR;
	} else // B is 0011
	{
		EU_ISSUE_OI;
	}
}

void Assembler::assemble(std::string program, std::vector<uint32_t>* out) {
	// Create stream object
	std::istringstream f(program);

	// Read the program, line by line
	std::string line;
	while (std::getline(f, line)) {
		// Store args, temporarily, as strings
		std::string opcode, arg1, arg2, arg3;

		// Extract the opcode and count operands
		std::string operand;
		int numArgs = 0;
		bool gotType = false;
		int type;

		// Create another stream object
		std::istringstream l(line);

		while (std::getline(l, operand, ' ')) {
			operand = removeWhitespace(operand);

			if (!gotType) {

				instruction_t instr;
				instr.opcode = stoop(operand);
				type = determineArguments(instr);
				opcode = operand;

				gotType = true;
			} else {
				switch(numArgs) {
					case 1:
						switch (type) {
							case EU_ISSUE_ORRR:
							case EU_ISSUE_ORI:
							case EU_ISSUE_OR:
								arg1 = operand;
							break;

							case EU_ISSUE_OI:
								arg1 = operand;
							break;

							default:
								printUnhandledTypeError(type);
							break;
						}
					break;

					case 2:
						switch (type) {
							case EU_ISSUE_ORRR:
							case EU_ISSUE_ORI:
								arg2 = operand;
							break;

							default:
								printUnhandledTypeError(type);
							break;
						}
					break;

					case 3:
						switch (type) {
							case EU_ISSUE_ORRR:
								arg3 = operand;
							break;

							default:
								printUnhandledTypeError(type);
							break;
						}
					break;

					default:
					break;
				}
			}
			numArgs++;	
		}
		numArgs--;

		uint32_t packedInstr;
		switch(type) {
			case EU_ISSUE_ORRR:
				if (numArgs != 3) {
					printInvalidArgumentCountError(opcode, numArgs, 3);
					break;
				}
				instruction_orrr_t instr_orrr;
				memset(&instr_orrr, 0, sizeof(instruction_orrr_t));
				instr_orrr.opcode = stoop(opcode);
				instr_orrr.r1 = stor(arg1);
				instr_orrr.r2 = stor(arg2);
				instr_orrr.r3 = stor(arg3);
				memcpy(&packedInstr, &instr_orrr, sizeof(packedInstr));
			break;

			case EU_ISSUE_ORI:
				if (numArgs != 2) {
					printInvalidArgumentCountError(opcode, numArgs, 2);
					break;
				}
				instruction_ori_t instr_ori;
				memset(&instr_ori, 0, sizeof(instruction_ori_t));
				instr_ori.opcode = stoop(opcode);
				instr_ori.r1 = stor(arg1);
				instr_ori.im1 = strtoi(arg2);
				memcpy(&packedInstr, &instr_ori, sizeof(packedInstr));
			break;

			case EU_ISSUE_OR:
				if (numArgs != 1) {
					printInvalidArgumentCountError(opcode, numArgs, 1);
					break;
				}
				instruction_or_t instr_or;
				memset(&instr_or, 0, sizeof(instruction_or_t));
				instr_or.opcode = stoop(opcode);
				instr_or.r1 = stor(arg1);
				memcpy(&packedInstr, &instr_or, sizeof(packedInstr));
			break;

			case EU_ISSUE_OI:
				if (numArgs != 1) {
					printInvalidArgumentCountError(opcode, numArgs, 1);
					break;
				}
				instruction_oi_t instr_oi;
				memset(&instr_oi, 0, sizeof(instruction_oi_t));
				instr_oi.opcode = stoop(opcode);
				instr_oi.im1 = stor(arg1);
				memcpy(&packedInstr, &instr_oi, sizeof(packedInstr));
			break;

			case EU_ISSUE_O:
				if (numArgs != 0) {
					printInvalidArgumentCountError(opcode, numArgs, 0);
					break;
				}
				instruction_o_t instr_o;
				memset(&instr_o, 0, sizeof(instruction_o_t));
				instr_o.opcode = stoop(opcode);
				memcpy(&packedInstr, &instr_o, sizeof(packedInstr));
			break;

			default:
				std::cerr << "Unhandled number of operands: " << numArgs << std::endl;
			break;
		}
		out->push_back(packedInstr);
	}

	// Add a marker so we can easily identify the finish in memory
	// This is at least 4 blank instructions (NOPs) on a 4-word
	// alignment, so they are easily visible.
	while ((out->size() % 4) != 0) {
		out->push_back(0);
	}
	for (int i = 0; i < 4; i++) {
		out->push_back(0);
	}
}
