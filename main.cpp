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

	// Use pipeline?
	if (option_exists(argv, argv+argc, "-no-pipeline")) {
		std::cout << "Setting pipeline flag to false." << std::endl;
		s.setPipeline(false);
		// s.setPipelineWidth(1);
	}

	// Pipeline width
	// unsigned int width = 0;
	// if (option_exists(argv, argv+argc, "-width")) {
	// 	if (!s.getPipeline()) {
	// 		std::cerr << "You must enable the pipeline to set its width." << std::endl;
	// 		return 1;
	// 	}
	// 	char* w = get_option(argv, argv+argc, "-width");
	// 	if (w) {
	// 		width = atoi(w);
	// 		s.setPipelineWidth(width);
	// 		std::cout << "Setting pipeline width to " << width << "." << std::endl;
	// 	} else {
	// 		std::cerr << "No pipeline width specified." << std::endl;
	// 		return 1;
	// 	}
	// }

	// Use debug?
	if (option_exists(argv, argv+argc, "-d")) {
		std::cout << "Setting debug flag to true." << std::endl;
		s.setDebug(true);
	}

	// Cap execution time?
	unsigned int maxTicks = 0;
	if (option_exists(argv, argv+argc, "-max-ticks")) {
		char* t = get_option(argv, argv+argc, "-max-ticks");
		if (t) {
			maxTicks = atoi(t);
			std::cout << "Setting max-ticks to " << maxTicks << "." << std::endl;
		} else {
			std::cerr << "No maximum number of ticks specified." << std::endl;
			return 1;
		}
	}

	// Load program from file into vector
	std::vector<uint32_t> program;
	int size;

	std::cout << "Loading file " << source << std::endl;

	std::ifstream is(source, std::ios::in | std::ifstream::binary);

    if(!is)
    {
        std::cerr << "Cannot open input file" << std::endl;
        return 1;
    }

    uint32_t f;
    while(is.read(reinterpret_cast<char *>(&f), sizeof(f))) {
        program.push_back(f);
    }
    is.close();

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
		if (maxTicks != 0 && s.getTicks() + 1 >= maxTicks) {
			// Max ticks exceeded
			break;
		}
	}

	std::cout << std::endl;
	std::cout << "Finished execution successfully." << std::endl;
	std::cout << "Average instructions per cycle: " << s.getInstructionsPerTick();
	std::cout << " (" << s.getTicks() + 1 << " cycles)" << std::endl;
	std::cout << std::endl;

	if (s.getDebug()) {
		s.print();
	}

	return 0;
}
