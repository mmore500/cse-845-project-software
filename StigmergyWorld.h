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
	//current location and direction of the agent, begins at home location facing a random direction.
	int agentX, agentY, agentD, homeX, homeY, foodX, foodY;

	//stores if the agent has food or not
	bool agentF;

	//vision input
	int visionConeArea = 8;
	// walls (OOB, obstacles), food, home, other agnets, empty
	int visionConeBits = 5;

	std::vector<std::vector<std::vector<int>>> defineVisionCone();
	std::vector<std::vector<std::vector<int>>> visionConeOffsets;
	//(optional) compass
	//NS/EW
	int compassSize = 2;

	//stigmergy signal location input (3x3 grid)
	int stigmergyLocationInputSize = 9;
	std::vector<std::vector<std::vector<int>>> defineStigmergySensor();
	std::vector<std::vector<std::vector<int>>> stigmergyProximitySensorOffsets;

	//stores if agent wishes to read  or write stigmergy message
	//bool agentR, agentW;
	//bits per stigmergy message
	int stigmergyContentSize;

	//food in hand (bit)
	int foodInHandSize = 1;

	//tracks how much food is left at the currently available location
	int foodCount;
	void spawnFood(int x, int y);

	int inputSize;

	// either you do or you do not, there is no try
	//int stigmergyWriteControlSize = 1;

	// two bit tank movement
	int movementControlSize = 2;

	// either you do or you do not, there is no try
	// one bit to control whether read stigmergy
	//int stigmergyReadControlSize = 1;

	int outputSize;

	int xDim, yDim, lifeTime;
	vector<vector<int>> world;
	vector<vector<int>> stigmergyMap;
	void generateMap();
	void showWorld();
	void showStigmergyMap();
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
	static struct offsets off;

	//used in map generation
	struct cell{
		int x,y;

		bool nextIsInBounds(int xDim, int yDim, int dir){
			return (x + 2*off.x(dir) >0) && (x + 2*off.x(dir) < xDim) && (y + 2*off.y(dir) > 0) && (y + 2*off.y(dir) < yDim);
		}

		bool nextIsUnvisited(int dir, vector<vector<int>> w){
			return w[x+2*off.x(dir)][y+2*off.y(dir)] == 1;
		}

		cell next(int dir){
			return cell(x+2*off.x(dir), y+2*off.y(dir));
		}

		cell(int _x,int _y){x=_x; y=_y;};
	};

	double wallPercent;

	void resetWorld();
public:
	//User Parameters
	static shared_ptr<ParameterLink<int>> evaluationsPerGenerationPL;
	static shared_ptr<ParameterLink<int>> lifeTimePL;
	static shared_ptr<ParameterLink<int>> stigmergyBitsPL;
	static shared_ptr<ParameterLink<int>> xDimPL;
	static shared_ptr<ParameterLink<int>> yDimPL;
	static shared_ptr<ParameterLink<double>> wallPercentPL;
	//MABE Parameters
	static shared_ptr<ParameterLink<string>> groupNamePL;
	static shared_ptr<ParameterLink<string>> brainNamePL;

	StigmergyWorld(shared_ptr<ParametersTable> _PT = nullptr);
	virtual ~StigmergyWorld() = default;

	virtual void evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug);

	virtual void evaluate(map<string, shared_ptr<Group>>& groups, int analyze, int visualize, int debug) {
		//StigmergyWorld::generateMap();
		int popSize = groups[groupNamePL->get(PT)]->population.size();
		for (int i = 0; i < popSize; i++) {
			evaluateSolo(groups[groupNamePL->get(PT)]->population[i], analyze, visualize, debug);
		}
	}

	virtual unordered_map<string, unordered_set<string>> requiredGroups() override {
		return { { groupNamePL->get(PT),{ "B:" + brainNamePL->get(PT) + "," + to_string(inputSize)+ "," + to_string(outputSize) } } };
	}
};
