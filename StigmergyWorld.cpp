//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/Hintzelab/MABE/wiki/License

#include "StigmergyWorld.h"
//User Parameters
shared_ptr<ParameterLink<int>> StigmergyWorld::evaluationsPerGenerationPL = Parameters::register_parameter("WORLD_STIGMERGY-evaluationsPerGeneration", 1, "Number of times to test each Genome per generation (useful with non-deterministic brains)");
shared_ptr<ParameterLink<int>> StigmergyWorld::lifeTimePL = Parameters::register_parameter("WORLD_STIGMERGY-lifeTime", 1000, "Number of time units an agent lives for");
shared_ptr<ParameterLink<int>> StigmergyWorld::xDimPL = Parameters::register_parameter("WORLD_STIGMERGY-xDim", 15, "width of world");
shared_ptr<ParameterLink<int>> StigmergyWorld::yDimPL = Parameters::register_parameter("WORLD_STIGMERGY-yDim", 15, "height of world");


//MABE Parameters
shared_ptr<ParameterLink<string>> StigmergyWorld::groupNamePL = Parameters::register_parameter("WORLD_STIGMERGY_NAMES-groupNameSpace", (string)"root::", "namespace of group to be evaluated");
shared_ptr<ParameterLink<string>> StigmergyWorld::brainNamePL = Parameters::register_parameter("WORLD_STIGMERGY_NAMES-brainNameSpace", (string)"root::", "namespace for parameters used to define brain");

StigmergyWorld::StigmergyWorld(shared_ptr<ParametersTable> _PT):AbstractWorld(_PT) {
	lifeTime = lifeTimePL->get(PT);
	xDim = xDimPL->get(PT);
	if (xDim % 2 == 0){
		xDim++;
	}
	yDim = yDimPL->get(PT);
	if (yDim % 2 == 0){
		yDim++;
	}
	// columns to be added to ave file
	popFileColumns.clear();
	popFileColumns.push_back("score");
}

//Generates a map to evaluate organisms in
void StigmergyWorld::generateMap(){
	//The following algorithm performs a random DFS from the top left corner of the maze
	//to construct the hallways. Note: outer most band is a buffer and should be left solid.

	vector<cell> stack;
	auto allOnes = vector<vector<int>>(xDim, vector<int>(yDim, 1));
	world = allOnes;
	auto current = cell(1,1);
	world[1][1] = 0;
	stack.push_back(current);

	while(not stack.empty()){
		vector<cell> possibleNext;
		for(int i=0;i<4;i++){
			if(current.isInBounds(xDim, yDim, i)){
				if(current.nextIsUnvisited(i, world)){
					possibleNext.push_back(current.next(i));
				}
			}
		}
		if(not possibleNext.empty()){
			auto target = possibleNext[Random::getIndex(possibleNext.size())];
			stack.push_back(current);
			world[target.x][target.y] = 0;
			world[(current.x + target.x) / 2][(current.y + target.y) / 2] = 0;
			current = target;
		} else {
			if(not stack.empty()){
				current = stack.back();
				stack.pop_back();
			}
		}
	}
	showWorld();
}


void StigmergyWorld::showWorld(){
	for(int j=0;j<yDim;j++){
		for(int i=0;i<xDim;i++){
			printf("%s",world[i][j] ? "██":"  ");
		}
		printf("\n");
	}
}


void StigmergyWorld::evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug) {
	auto brain = org->brains[brainNamePL->get(PT)];
	for (int eval = 0; eval < evaluationsPerGenerationPL->get(PT); eval++) {
		brain->resetBrain();
		double score = 1.0;
		for (int time = 0; time < StigmergyWorld::lifeTime; time++){
			//reset brain I/O to avoid agents using deactivated nodes as hidden memory.
			brain->resetInputs();
			brain->resetOutputs();

			//vision input
				// walls (OOB, obstacles), food, home, other agnets
			//(optional) compass
				//NS/EW
			//stigmergy signal location input
				//3x3 grid
			//stigmergy signal content input
				//under agent
			//food in hand
				//bit
			brain->setInput(0, 1);
			brain->update();
			int i = 0;
			int dummy = Bit(brain->readOutput(i));
			//write

			//movement
				//90 degrees only
				//00 is wait
			//food up/down (automatic)
				//add to score
			//set read bit

		}
		org->dataMap.append("score", score);
	}	
}