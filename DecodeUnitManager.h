// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __DECODENUNITMANAGER_H
#define __DECODENUNITMANAGER_H

#include <vector>
#include <string>

#include "DecodeUnit.h"

class DecodeUnitManager {
private:
	bool debug;
	ExecutionUnitManager* eum;
	std::vector<DecodeUnit> dus;

public:
	DecodeUnitManager(unsigned int n, ExecutionUnitManager* eum);
	void setDebug(bool debug);
	bool getDebug();
	std::string toString();
	DecodeUnit* getAvailableDecodeUnit();
	void tick();
};

#endif
