// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __FETCHUNITMANAGER_H
#define __FETCHUNITMANAGER_H

#include <vector>
#include <string>

#include "FetchUnit.h"
#include "MemoryLocation.h"
#include "DecodeUnitManager.h"

class FetchUnitManager {
private:
	bool debug;
	DecodeUnitManager* dum;
	std::vector<FetchUnit> fus;

public:
	FetchUnitManager(unsigned int width, DecodeUnitManager* dum);
	void setDebug(bool debug);
	bool getDebug();
	std::string toString();
	FetchUnit* getAvailableFetchUnit();
	bool tick(std::vector<MemoryLocation>* m, bool pipeline);
};

#endif
