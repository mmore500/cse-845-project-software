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
	int inputSize = 1;
	int outputSize = 1;
	void generateMap();
	int lifeTime = 1;
public:
	//User Parameters
	static shared_ptr<ParameterLink<int>> evaluationsPerGenerationPL;
	static shared_ptr<ParameterLink<int>> lifeTimePL;
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

