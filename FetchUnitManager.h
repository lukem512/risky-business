// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNITMANAGER_H
#define __FETCHUNITMANAGER_H

#include <vector>
#include <string>

#include "FetchUnit.h"

#define DEFAULT_FU_COUNT 1

class FetchUnitManager {
private:
	bool debug;
	std::vector<FetchUnit> fus;

	void init(unsigned int n = DEFAULT_FU_COUNT) {
		debug = false;
		fus.assign(width, FetchUnit());
	};

public:
	FetchUnitManager();
	FetchUnitManager(unsigned int n);
	void setDebug(bool debug);
	bool getDebug();
	std::string toString();
	FetchUnit* getAvailableFetchUnit();
};

#endif
