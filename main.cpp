// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

#include "State.h"
#include "Assembler.h"

// std::string source = 
// 	"LDC r1 14\n" \
// 	"PRNT r1\n" \
// 	"LDC r2 6\n" \
// 	"PRNT r2\n" \
// 	"ADD r1 r1 r2\n" \
// 	"PRNT r1\n" \
// 	"HLT";

std::string source =
	"\%\n" \
	"\% Store array in memory\n" \
	"\%\n" \
	"\% Location\n" \
	"LDC r14 100\n" \
	"\% Length\n" \
	"LDC r15 30\n" \
	"\% Iterator\n" \
	"LDC r13 100\n" \
	"LDC r12 1\n" \
	"\% Contents\n" \
	"STR r13 5\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 2\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 6\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 4\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 15\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 8\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 20\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 30\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 21\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 1\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 16\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 29\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 11\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 14\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 18\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 9\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 28\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 22\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 7\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 23\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 19\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 27\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 10\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 24\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 13\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 17\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 25\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 12\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 26\n" \
	"ADD r13 r13 r12\n" \
	"STR r13 3\n" \
	"PRNT r13\n" \
	"LD r12 r13\n" \
	"PRNT r12\n" \
	"HLT";

int main(int argc, char** argv){
	// Seed the random function
	srand(time(NULL));

	// Initialise the machine state
	State s;
    s = State();

    // Assemble the simple program
	Assembler a;
	std::vector<uint32_t> program;
	a.assemble(source, &program);
	std::cout << "Program source was successfully assembled. The program is " << 
	  program.size() << " bytes." << std::endl << std::endl;

	// Load the program into memory
    for (int i = 0; i < program.size(); i++) {
    	s.memory.at(i).contents = program.at(i);
    }

    s.print();

	while (1) {
		if (s.tick()) {
			// Execution halted
			break;
		}
	}

	s.print();

	return 0;
}
