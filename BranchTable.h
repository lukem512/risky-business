// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __BRANCHTABLE_H
#define __BRANCHTABLE_H

#include <map>
#include <list>
#include <climits>
#include <random>
#include <iostream>

enum Prediction {
	TAKEN,
	NOT_TAKEN,
	STALLED,
	HALTED,
	UNKNOWN
};

class BranchTableEntry {
public:
	// This is just for programatic housekeeping
	int id;

	// These would be stored in hardware
	uint32_t location;
	uint32_t pc;
	Prediction predicted;
	Prediction actual;
	bool speculative;

	BranchTableEntry(uint32_t location, Prediction p, bool speculative) {
		id = rand() % INT_MAX;
		this->location = location;
		this->pc = 0;
		this->predicted = p;
		this->actual = UNKNOWN;
		this->speculative = speculative;
	}

	BranchTableEntry(uint32_t location, uint32_t pc, Prediction p, Prediction a, bool speculative) {
		id = rand() % INT_MAX;
		this->location = location;
		this->pc = pc;
		this->predicted = p;
		this->actual = a;
		this->speculative = speculative;
	}

	void copy(const BranchTableEntry e) {
		std::cout << "Inside copy" << std::endl;
		this->location = e.location;
		this->pc = e.pc;
		this->predicted = e.predicted;
		this->actual = e.actual;
		this->speculative = e.speculative;
	}

	bool operator==(const BranchTableEntry& rhs) const {
		return (rhs.id == this->id);
	}
};

// This class encapsulates a translation table
// between a branch at PC and the direction.
class BranchTable {
public:
	std::map<uint32_t, Prediction> predicted;
	std::map<uint32_t, Prediction> actual;
	std::map<uint32_t, uint32_t>   pc;
	std::map<uint32_t, bool>	   speculative;

	std::list<BranchTableEntry *>	entries;

	// Add a new entry to the table
	void add(uint32_t location, Prediction p, bool speculative) {
		BranchTableEntry* entry = new BranchTableEntry(location, p, speculative);
		entries.push_back(entry);
	}

	// Get the first entry in the table with specified location
	BranchTableEntry* get(uint32_t location) {
		for (auto it = entries.begin(); it != entries.end(); ++it) {
		    if ((*it)->location == location) {
		    	return *it;
		    }
		}
	}

	// Updates the first entry in the table that matches
	bool update(BranchTableEntry* e) {
		// for (int i = 0; i < entries.size(); i++) {
		// 	if (entries[i]id == e.id) {
		//     	std::cout << "Found entry with ID " << entries[i].id << std::endl;
		//     	entries[i].copy(e);
		//     	return true;
		//     }
		// }
		// for (auto it = entries.begin(); it != entries.end(); ) {
		//     if (it->id == e.id) {
		//     	std::cout << "Found entry with ID " << it->id << std::endl;
		//     	it->copy(e);
		//     	return true;
		//     } else {
		//     	++it;
		//     }
		// }
		return false;
	}

	// Erases the first entry matching e
	void remove(BranchTableEntry* e) {
		for (auto it = entries.begin(); it != entries.end(); ) {
		    if ((*it)->id == e->id) {
		    	entries.erase(it++);
		    	return;
		    } else {
		    	++it;
		    }
		}
	}

	// Clear all entries flagged as speculative
	void clearSpeculative() {
		for (auto it = entries.begin(); it != entries.end(); ) {
		    if ((*it)->speculative) {
		    	entries.erase(it++);
		    } else {
		    	++it;
		    }
		}
	}

	// void clearSpeculative() {
	// 	for (auto it = speculative.cbegin(); it != speculative.cend();) {
	// 		if (it->second) {
	// 			predicted.erase(it->first);
	// 			actual.erase(it->first);
	// 			// pc won't have the entry
	// 			speculative.erase(it++);
	// 		} else {
	// 			++it;
	// 		}
	// 	}
	// }
};

#endif
