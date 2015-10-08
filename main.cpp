// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

#include "State.h"
#include "Assembler.h"

std::string source = 
	"LDC r1 14\n" \
	"LDC r2 6\n" \
	"ADD r1 r1 r2";

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

	while (1) {
		s.tick();
	}

	return 0;
}
