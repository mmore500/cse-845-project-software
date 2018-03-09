//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/Hintzelab/MABE/wiki/License

#pragma once

#include "../AbstractWorld.h"

#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

class StigmergyWorld : public AbstractWorld {
private:

	//vision input
	int visionConeArea = 9;
	// walls (OOB, obstacles), food, home, other agnets
	int visionConeSize =  visionConeArea * 4;
	//(optional) compass
	//NS/EW
	int compassSize = 2;

	//stigmergy signal location input (3x3 grid)
	int stigmergyLocationInputSize = 9;

	//stigmergy signal content input
		//under agent
	int stigmergyContentSize;

	//food in hand (bit)
	int foodInHandSize = 1;

	int inputSize;

	// either you do or you do not, there is no try
	int stigmergyWriteControlSize = 1;

	// two bit tank movement
	int movementControlSize = 2;

	// either you do or you do not, there is no try
	// one bit to control whether read stigmergy
	int stigmergyReadControlSize = 1;

	int outputSize;
	void generateMap();
	int lifeTime;

public:
	//User Parameters
	static shared_ptr<ParameterLink<int>> evaluationsPerGenerationPL;
	static shared_ptr<ParameterLink<int>> lifeTimePL;
	static shared_ptr<ParameterLink<int>> stigmergyBitsPL;
	//MABE Parameters
	static shared_ptr<ParameterLink<string>> groupNamePL;
	static shared_ptr<ParameterLink<string>> brainNamePL;

	StigmergyWorld(shared_ptr<ParametersTable> _PT = nullptr);
	virtual ~StigmergyWorld() = default;

	virtual void evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug);

	virtual void evaluate(map<string, shared_ptr<Group>>& groups, int analyze, int visualize, int debug) {
		StigmergyWorld::generateMap();
		int popSize = groups[groupNamePL->get(PT)]->population.size();
		for (int i = 0; i < popSize; i++) {
			evaluateSolo(groups[groupNamePL->get(PT)]->population[i], analyze, visualize, debug);
		}
	}

	virtual unordered_map<string, unordered_set<string>> requiredGroups() override {
		return { { groupNamePL->get(PT),{ "B:" + brainNamePL->get(PT) + "," + to_string(inputSize)+ "," + to_string(outputSize) } } };
	}
};
