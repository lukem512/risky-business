// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <iostream>

#include "../../Assembler.h"
#include "../../Dependence.h"

int main (int argc, char** argv) {

	// Make the test program
	Assembler ass;
	std::vector<uint32_t> program;

	std::string source =
		"NOP\n"
		"LDC r1 10\n"
		"LDC r2 15\n"
		"ADD r3 r1 r2\n"
		"PRNT r3";

	ass.assemble(source, &program);

	// Simulate the program
	Register pc;
	pc.contents = 0;

	std::vector<Register> r;
	r.assign(16, Register());

	int failed = 0;

	// Test 1
	// NOP should not depend upon anything
	// Expected result: false
	std::cout << "Test 1...";
	if (Dependence::depends(program[0], program[1], &pc, &r) == false) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	// Test 2
	// Two loads to different registers should not depend
	// Expected result: false
	std::cout << "Test 2...";
	if (Dependence::depends(program[1], program[2], &pc, &r) == false) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	// Test 3
	// A load and an add, taking the LDC destination as an operand, depend
	// Expected result: true
	std::cout << "Test 3...";
	if (Dependence::depends(program[1], program[3], &pc, &r) == true) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	// Test 4
	// See Test 3.
	// Expected result: true
	std::cout << "Test 4...";
	if (Dependence::depends(program[2], program[3], &pc, &r) == true) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	return 0;
}