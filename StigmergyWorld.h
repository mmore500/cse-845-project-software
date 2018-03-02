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
	int xDim, yDim, lifeTime;
	vector<vector<int>> world;
	int inputSize = 1;
	int outputSize = 1;
	void generateMap();
	void showWorld();
	struct offsets{
		int x(int dir){
			int r = 0;
			switch(dir){
				case 1:
					r = 1;
					break;
				case 3:
					r = -1;
					break;
			}
			return r;
		}

		int y(int dir){
			int r = 0;
			switch(dir){
				case 0:
					r = -1;
					break;
				case 2:
					r = 1;
					break;
			}
			return r;
		}
	};
	static struct offsets o;
	
	//used in map generation
	struct cell{
		int x,y;

		bool isInBounds(int xDim, int yDim, int dir){
			return (x + 2*o.x(dir) >0) && (x + 2*o.x(dir) < xDim) && (y + 2*o.y(dir) > 0) && (y + 2*o.y(dir) < yDim);
		}

		bool nextIsUnvisited(int dir, vector<vector<int>> w){
			return w[x+2*o.x(dir)][y+2*o.y(dir)] == 1;
		}

		cell next(int dir){
			return cell(x+2*o.x(dir), y+2*o.y(dir));
		}

		cell(int _x,int _y){x=_x; y=_y;};
	};
public:
	//User Parameters
	static shared_ptr<ParameterLink<int>> evaluationsPerGenerationPL;
	static shared_ptr<ParameterLink<int>> lifeTimePL;
	static shared_ptr<ParameterLink<int>> xDimPL;
	static shared_ptr<ParameterLink<int>> yDimPL;
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

