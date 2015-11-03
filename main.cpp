// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "State.h"
#include "option.h"

int main(int argc, char** argv){
	// Seed the random function
	srand(time(NULL));

	// Initialise the machine state
	State s;
    s = State();

    // Load the program filename
    std::string source;
	if (option_exists(argv, argv+argc, "-f")) {
		char* filename = get_option(argv, argv+argc, "-f");
		if (filename) {
			source = filename;
		} else {
			std::cerr << "No input filename was specified." << std::endl;
			return 1;
		}
	} else {
		// Default behaviour
		std::cerr << "No input filename was specified." << std::endl;
		return 1;
	}
	
	// Use debug?
	if (option_exists(argv, argv+argc, "-d")) {
		// TODO: turn on debugging
		// this currently uses a #define
	}

	// TODO: load program from file into vector
	std::vector<uint32_t> program;
	int size;

	std::cout << "Loading file " << source << std::endl;

	ifstream inputFile(source, std::ios::binary);
    inputFile.read((char*)&size, sizeof(int));

    std::cout << "File size is " << std::to_string(size) << std::endl;

    program.resize(size);
    inputFile.read((char*)&program[0], size * sizeof(uint32_t));

	std::cout << "Program was successfully loaded. The program is " << 
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
