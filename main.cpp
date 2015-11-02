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

// iain at StackOverflow.com
char* get_option(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

// iain at StackOverflow.com
bool option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char** argv){
	// Seed the random function
	srand(time(NULL));

	// Initialise the machine state
	State s;
    s = State();

    // Load the source into a string
    std::string source;
	if (option_exists(argv, argv+argc, "-f")) {
		char* filename = get_option(argv, argv+argc, "-f");
		if (filename) {
			std::cout << "Loading file " << filename << std::endl;
			source = load_from_file(filename);
		} else {
			std::cerr << "No filename was specified." << std::endl;
			return 1;
		}
	} else {
		// Default behaviour, just halt
		source = "HLT";
	}
	
	// Use debug?
	if (option_exists(argv, argv+argc, "-d")) {
		// TODO: turn on debugging
		// this currently uses a #define
	}

    // Assemble the program
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
