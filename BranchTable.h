// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __BRANCHTABLE_H
#define __BRANCHTABLE_H

#include <map>

enum Prediction {
	TAKEN,
	NOT_TAKEN,
	STALLED,
	HALTED,
	UNKNOWN
};

// This class encapsulates a translation table
// between a branch at PC and the direction.
class BranchTable {
public:
	std::map<uint32_t, Prediction> predicted;
	std::map<uint32_t, Prediction> actual;
};

#endif
