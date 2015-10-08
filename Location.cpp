// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include "common.h"
#include "Location.h"

// Generate a random 32-bit value
uint32_t randomise() {
	return (uint32_t) (rand() % UINT32_MAX);
}

// Empty c'tor
Location::Location() {
	// Initialise contents to be random
	// This emulates the behaviour of real hardware
	contents = randomise();
}

// Copy c'tor
// This is the same as the traditional behaviour, for now
// as we use vectors of this class.
Location::Location(const Location& copy) {
	contents = randomise();
}

// Create a string of the hex-encoded contents
std::string Location::toString() {
	return hexify(contents);
}
