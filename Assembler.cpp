
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <regex>

#include "Assembler.h"
#include "DecodeUnit.h"
#include "ExecutionUnit.h"
#include "option.h"
#include "common.h"

void printUnhandledTypeError(int type) {
	std::cerr << "Unhandled instruction type encountered: " << type << std::endl;
}

void printInvalidArgumentCountError(std::string instr, int got, int expected, int type) {
	std::cerr << "Incorrect number of arguments specified for opcode " << instr << ", decoded as type " << type << ". Expected " << expected << " and got " << got << "." << std::endl;
}

Assembler::Assembler() {
	std::string base = "[a-zA-Z][a-zA-Z0-9]*";
	lineLabelRegex = std::regex(base + ":", std::regex_constants::basic);
	operandLabelRegex = std::regex(base, std::regex_constants::basic);
	debug = false;
}

bool Assembler::setDebug(bool set) {
	debug = set;
	return debug;
}

bool Assembler::getDebug() {
	return debug;
}

bool Assembler::isLineLabel(std::string line) {
	return std::regex_match(line, lineLabelRegex);
}

bool Assembler::isOperandLabel(std::string operand) {
	return std::regex_match(operand, operandLabelRegex);
}

bool Assembler::isLineComment(std::string line) {
	return (line.at(0) == '%');
}

int Assembler::determineArguments(instruction_t instr) {
	if (instr.opcode == OP_HLT) {
		return EU_ISSUE_O;
	} else
	if (IS_ORR(instr.opcode)) { // All 2-register operand instruction are 11xx
		return EU_ISSUE_ORR;
	} else
	if (IS_ORRR(instr.opcode)) { // All 3-register operand instructions are 10xx
		return EU_ISSUE_ORRR;
	} else
	if (IS_ORI(instr.opcode)) { // All register, immediate operand instructions are 01xx
		return EU_ISSUE_ORI;
	} else
	if (instr.opcode == OP_NOP) { // NOP is 0000
		return EU_ISSUE_O;
	} else
	if (instr.opcode == OP_PRNT) { // PRNT is 0001
		return EU_ISSUE_OR;
	} else // B is 0011
	{
		return EU_ISSUE_OI;
	}
}

int Assembler::determineBranchAmount(unsigned int src, unsigned int dst) {
	return dst - src - 1;
}

void Assembler::extractLabels(std::string program) {
	std::istringstream f(program);
	unsigned int lineNumber = 1;

	std::string line;
	while (std::getline(f, line)) {
		if (isLineComment(line)) {
			continue;
		}

		if (isLineLabel(line)) {
			// Remove ':'
			std::string label = line.substr(0, line.size() - 1);
			labels[label] = lineNumber;

			if (debug) {
				std::cout << "Found a label (" << label << ") at line " << lineNumber << std::endl;
			}

			continue;
		}

		lineNumber++;
	}
}

void Assembler::assemble(std::string program, std::vector<uint32_t>* out) {
	// Create stream object
	std::istringstream f(program);
	unsigned int lineNumber = 1;

	// First pass, extract label locations
	extractLabels(program);

	// Read the program, line by line
	std::string line;
	while (std::getline(f, line)) {
		// Store args, temporarily, as strings
		std::string opcode, arg1, arg2, arg3;

		// If the line begins with a '%', discard it as a comment
		if (isLineComment(line) || isLineLabel(line)) {
			continue;
		}

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
							case EU_ISSUE_ORR:
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
							case EU_ISSUE_ORR:
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
					printInvalidArgumentCountError(opcode, numArgs, 3, EU_ISSUE_ORRR);
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
					printInvalidArgumentCountError(opcode, numArgs, 2, EU_ISSUE_ORI);
					break;
				}
				instruction_ori_t instr_ori;
				memset(&instr_ori, 0, sizeof(instruction_ori_t));
				instr_ori.opcode = stoop(opcode);
				instr_ori.r1 = stor(arg1);
				if (isOperandLabel(arg2)) {
					int16_t offset = determineBranchAmount(lineNumber, getLabelLocation(arg2));
					if (debug) {
						std::cout << "Conditionally branching by " << std::to_string((long long int)offset) << " to label " << arg2 << std::endl;
					}
					instr_ori.im1 = offset;
				} else {
					instr_ori.im1 = strtoi(arg2);
				}
				memcpy(&packedInstr, &instr_ori, sizeof(packedInstr));
			break;

			case EU_ISSUE_ORR:
				if (numArgs != 2) {
					printInvalidArgumentCountError(opcode, numArgs, 2, EU_ISSUE_ORR);
					break;
				}
				instruction_orr_t instr_orr;
				memset(&instr_orr, 0, sizeof(instruction_orr_t));
				instr_orr.opcode = stoop(opcode);
				instr_orr.r1 = stor(arg1);
				instr_orr.r2 = stor(arg2);
				memcpy(&packedInstr, &instr_orr, sizeof(packedInstr));
			break;

			case EU_ISSUE_OR:
				if (numArgs != 1) {
					printInvalidArgumentCountError(opcode, numArgs, 1, EU_ISSUE_OR);
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
					printInvalidArgumentCountError(opcode, numArgs, 1, EU_ISSUE_OI);
					break;
				}
				instruction_oi_t instr_oi;
				memset(&instr_oi, 0, sizeof(instruction_oi_t));
				instr_oi.opcode = stoop(opcode);
				if (isOperandLabel(arg1)) {
					int offset = determineBranchAmount(lineNumber, getLabelLocation(arg1));
					if (debug) {
						std::cout << "Branching by " << std::to_string((long long int)offset) << " to label " << arg1 << std::endl;
					}
					instr_oi.im1 = offset;
				} else {
					instr_oi.im1 = strtoi(arg1);
				}
				memcpy(&packedInstr, &instr_oi, sizeof(packedInstr));
			break;

			case EU_ISSUE_O:
				if (numArgs != 0) {
					printInvalidArgumentCountError(opcode, numArgs, 0, EU_ISSUE_O);
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
		lineNumber++;
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

unsigned int Assembler::getLabelLocation(std::string label) {
	if (labels.find(label) != labels.end()) {
		return labels[label];
	} else {
		std::cerr << "Label " << label << " was not found" << std::endl;
	}

	return UINT16_MAX;
}

int main (int argc, char** argv) {
	std::string input, output;

	// Was an input name specified?
	if (option_exists(argv, argv+argc, "-f")) {
		char* filename = get_option(argv, argv+argc, "-f");
		if (filename) {
			input = load_from_file(filename);
		} else {
			std::cerr << "No input filename was specified." << std::endl;
			return 1;
		}
	} else {
		std::cerr << "No input filename was specified." << std::endl;
		return 1;
	}

	// Was an output filename specified?
	if (option_exists(argv, argv+argc, "-o")) {
		char* filename = get_option(argv, argv+argc, "-o");
		if (filename) {
			output = filename;
		} else {
			std::cerr << "No output filename was specified." << std::endl;
			return 1;
		}
	} else {
		// Default behaviour
		output = "a.out";
	}

	// Use debug?
	bool debug = false;
	if (option_exists(argv, argv+argc, "-d")) {
		debug = true;
	}

    // Assemble the program
	Assembler a;
	std::vector<uint32_t> program;
	a.setDebug(debug);
	a.assemble(input, &program);
	std::cout << "Program source was successfully assembled. The program is " << 
	  program.size() << " bytes." << std::endl;

	// Write to output file
	std::ofstream os(output, std::ios::out | std::ofstream::binary);

    if(!os)
    {
        std::cerr << "Cannot open output file" << std::endl;
        return 1;
    }

    os.write(reinterpret_cast<const char *>(&program[0]), program.size()*sizeof(uint32_t));
    os.close();

  	std::cout << "Successfully written to " << output << "." << std::endl;

	return 0;
}
