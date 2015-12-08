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
	}

	// Pipeline width
	uint32_t eus = DEFAULT_EU_WIDTH;
	if (option_exists(argv, argv+argc, "-eus")) {
		char* w = get_option(argv, argv+argc, "-eus");
		if (w) {
			eus = atoi(w);
		} else {
			std::cerr << "No EU width specified." << std::endl;
			return 1;
		}
	}

	uint32_t dus = DEFAULT_DU_WIDTH;
	if (option_exists(argv, argv+argc, "-dus")) {
		char* w = get_option(argv, argv+argc, "-dus");
		if (w) {
			dus = atoi(w);
		} else {
			std::cerr << "No EU width specified." << std::endl;
			return 1;
		}
	}

	uint32_t fus = DEFAULT_FU_WIDTH;
	if (option_exists(argv, argv+argc, "-fus")) {
		char* w = get_option(argv, argv+argc, "-fus");
		if (w) {
			fus = atoi(w);
		} else {
			std::cerr << "No FU width specified." << std::endl;
			return 1;
		}
	}
	
	// Set up configuration
	s.setEus(eus);
	s.setDus(dus);
	s.setFus(fus);


	// Use debug?
	if (option_exists(argv, argv+argc, "-d")) {
		std::cout << "Setting debug flag to true." << std::endl;
		s.setDebug(true);
	}

	// Print configuration?
	if (s.getDebug()) {
		std::cout << "Setting number of EUs to " << eus << "." << std::endl;
		std::cout << "Setting number of DUs to " << dus << "." << std::endl;
		std::cout << "Setting number of FUs to " << fus << "." << std::endl;
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
