// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __BRANCHPREDICTIONTABLE_H
#define __BRANCHPREDICTIONTABLE_H

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

class BranchPredictionTableEntry {
public:
	// This is just for programatic housekeeping
	int id;

	// These would be stored in hardware
	uint32_t location;
	uint32_t pc;
	Prediction predicted;
	Prediction actual;
	bool speculative;

	BranchPredictionTableEntry(uint32_t location, Prediction p, bool speculative) {
		id = rand() % INT_MAX;
		copy(location, 0, p, UNKNOWN, speculative);
	}

	BranchPredictionTableEntry(uint32_t location, uint32_t pc, Prediction p, Prediction a, bool speculative) {
		id = rand() % INT_MAX;
		copy(location, pc, p, a, speculative);
	}

	void copy(const BranchPredictionTableEntry e) {
		copy(e.location, e.pc, e.predicted, e.actual, e.speculative);
	}

	void copy(uint32_t location, uint32_t pc, Prediction p, Prediction a, bool speculative) {
		this->location = location;
		this->pc = pc;
		this->predicted = p;
		this->actual = a;
		this->speculative = speculative;
	}

	bool operator==(const BranchPredictionTableEntry& rhs) const {
		return (rhs.id == this->id);
	}
};

// This class encapsulates a translation table
// between a branch at PC and the direction.
class BranchPredictionTable {
public:
	std::list<BranchPredictionTableEntry *>	entries;

	// Add a new entry to the table
	void add(uint32_t location, Prediction p, bool speculative) {
		BranchPredictionTableEntry* entry = new BranchPredictionTableEntry(location, p, speculative);
		entries.push_back(entry);
	}

	// Get the first entry in the table with specified location
	BranchPredictionTableEntry* get(uint32_t location) {
		for (auto it = entries.begin(); it != entries.end(); ++it) {
		    if ((*it)->location == location) {
		    	return *it;
		    }
		}
	}

	// Erases the first entry matching e
	void remove(BranchPredictionTableEntry* e) {
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
};

#endif
