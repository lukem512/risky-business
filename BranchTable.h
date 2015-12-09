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
	std::map<uint32_t, uint32_t>   pc;
	std::map<uint32_t, bool>	   speculative;

	void clearSpeculative() {
		for (auto it = speculative.cbegin(); it != speculative.cend();) {
			if (it->second) {
				predicted.erase(it->first);
				actual.erase(it->first);
				// pc won't have the entry
				speculative.erase(it++);
			} else {
				++it;
			}
		}
	}
};

#endif
