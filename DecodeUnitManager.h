// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __DECODENUNITMANAGER_H
#define __DECODENUNITMANAGER_H

#include <vector>
#include <string>

#include "DecodeUnit.h"

#define DEFAULT_DU_COUNT 1

class DecodeUnitManager {
private:
	bool debug;
	std::vector<DecodeUnit> dus;

	void init(unsigned int n = DEFAULT_DU_COUNT) {
		debug = false;
		dus.assign(width, DecodeUnit());
	};

public:
	DecodeUnitManager();
	DecodeUnitManager(unsigned int n);
	void setDebug(bool debug);
	bool getDebug();
	std::string toString();
	DecodeUnit* getAvailableDecodeUnit();
};

#endif
