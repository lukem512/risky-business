// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "State.h"
#include "Assembler.h"

// Load a file into a std::string
std::string load_from_file(std::string filename) {
    std::ifstream fs(filename);
	std::stringstream ss;
	ss << fs.rdbuf();
	return ss.str();
}

int main(int argc, char** argv){
	// Seed the random function
	srand(time(NULL));

	// Initialise the machine state
	State s;
    s = State();

    // Load the source into a string
    // TODO: pass as parameter
    std::string source = load_from_file("benchmarks/fib.s");

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
		if (s.tick()) {
			// Execution halted
			break;
		}
	}

	std::cout << std::endl;
	s.print();

	return 0;
}
