// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __LOCATION_H
#define __LOCATION_H

#include <cstdint>
#include <string>

#include <time.h>
#include <stdlib.h>

class Location {
public:
	uint32_t contents;
	
	Location();
	Location(const Location& copy);
	std::string toString(); 
};

#endif
