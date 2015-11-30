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
		"NOP\n" 			// 0
		"LDC r1 10\n"		// 1
		"LDC r2 15\n"		// 2
		"ADD r3 r1 r2\n"	// 3
		"PRNT r3\n"			// 4
		"ADD r4 r3 r1\n"	// 5
		"ADD r4 r1 r1\n";	// 6

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
	if (Dependence::depends(program[3], program[4], &pc, &r) == true) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	// Test 5
	// Fails due to both instructions writing to the same register
	// Expected result: true
	std::cout << "Test 5...";
	if (Dependence::depends(program[5], program[6], &pc, &r) == true) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	// Test 6
	// See Test 3, reversed
	// Expected result: true
	std::cout << "Test 6...";
	if (Dependence::depends(program[4], program[3], &pc, &r) == true) {
		std::cout << " passed!" << std::endl;
	} else {
		std::cout << " failed!" << std::endl;
		failed++;
	}

	// TODO: fail at IM(s1) ∩ OM(s2)

	// TODO: fail at IM(s2) ∩ OM(s1)

	// TODO: fail at OM(s1) ∩ OM(s2)

	return failed;
}