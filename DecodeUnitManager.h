// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __DECODENUNITMANAGER_H
#define __DECODENUNITMANAGER_H

#include <vector>
#include <string>
#include <queue>

#include "DecodeUnit.h"

class DecodeUnitManager {
private:
	bool debug;
	bool outOfOrder;
	ExecutionUnitManager* eum;
	Scoreboard* score;
	std::vector<DecodeUnit> dus;
	int lastIssued;
	std::queue<int> waiting;

public:
	DecodeUnitManager(unsigned int n, ExecutionUnitManager* eum);
	DecodeUnitManager(unsigned int n, Scoreboard* score);
	void setDebug(bool debug);
	bool getDebug();
	void setOutOfOrder(bool outOfOrder);
	bool getOutOfOrder();
	std::string toString();
	DecodeUnit* getAvailableDecodeUnit();
	void tick();
	void clearPipeline();
};

#endif
