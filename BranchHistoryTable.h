// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __BRANCHHISTORYTABLE_H
#define __BRANCHHISTORYTABLE_H

#include <map>

#define DEFAULT_BRANCH_HISTORY_TABLE_SIZE 16

class BranchHistoryTable {
public:
	enum Counter {
		STRONGLY_TAKEN,
		TAKEN,
		NOT_TAKEN,
		STRONGLY_NOT_TAKEN,
		UNKNOWN
	};

private:
	std::map<uint32_t, Counter> entries;
	uint8_t size;

	// Scheme to clear an existing entry to make room for a new one
	uint32_t getEntryToRemove() {
		// For now, randomise!
		int index = (rand() % entries.size());
		std::map<uint32_t, Counter>::iterator it;
		int i = 0;
		for (it = entries.begin(); it != entries.end(); ++it) {
			if (i == index) break;
			i++;
		}
		return it->first;
	}

public:

	BranchHistoryTable(uint8_t size = DEFAULT_BRANCH_HISTORY_TABLE_SIZE) {
		this->size = size;
	}

	Counter get(uint32_t location) {
		if (entries.find(location) != entries.end()) {
			return entries[location];
		}

		return UNKNOWN;
	}

	void set(uint32_t location, Counter c) {
		if (entries.find(location) == entries.end()) {
			if (entries.size() >= size) {
				// Delete an existing entry
				unset(getEntryToRemove());
			}
		}

		if (c == UNKNOWN) {
			std::cerr << "[BHP] Cannot specify direction UNKNOWN" << std::endl;
			return;
		}

		entries[location] = c;
	}

	void unset(uint32_t location) {
		entries.erase(location);
	}

	void update(uint32_t location, bool t) {
		Counter c = get(location);

		// 2-bit saturating counter
		switch (c) {
			case STRONGLY_TAKEN:
				c = (t) ? STRONGLY_TAKEN : TAKEN;
			break;

			case TAKEN:
				c = (t) ? STRONGLY_TAKEN : NOT_TAKEN;
			break;

			case NOT_TAKEN:
				c = (t) ? TAKEN : STRONGLY_NOT_TAKEN;
			break;

			case STRONGLY_NOT_TAKEN:
				c = (t) ? NOT_TAKEN : STRONGLY_NOT_TAKEN;
			break;

			case UNKNOWN:
				c = (t) ? TAKEN : NOT_TAKEN;
			break;

			default:
				// This should never happen
				std::cerr << "[BHP] Unknown branch direction encountered" << std::endl;
			break;
		}

		set(location, c);
	}
};

#endif
