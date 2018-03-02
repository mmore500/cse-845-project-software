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

//used in map generation
struct cell{
	int x,y;
	cell(int _x,int _y){x=_x; y=_y;};
};

//Generates a map to evaluate organisms in
void StigmergyWorld::generateMap(){
	//The following algorithm performs a random DFS from the top left corner of the maze
	//to construct the hallways. Note: outer most band is a buffer and should be left solid.
	int xm[4]={0,1,0,-1};
	int ym[4]={-1,0,1,0};
	vector<cell> stack;
	world.resize(xDim);
	for(int i=0;i<xDim;i++){
		world[i].resize(yDim);
		for(int j=0;j<yDim;j++){
			world[i][j]=1;
		}
	}
	cell currentCell=cell(1,1);
	world[1][1]=0;
	stack.push_back(currentCell);
	while(stack.size()>0){
		vector<cell> possibleNextBranch;
		for(int i=0;i<4;i++){
			if((currentCell.x+(2*xm[i])>0) && (currentCell.x+(2*xm[i])<xDim) && (currentCell.y+(2*ym[i])>0) && (currentCell.y+(2*ym[i])<yDim)){
				if(world[currentCell.x+(2*xm[i])][currentCell.y+(2*ym[i])] == 1){
					possibleNextBranch.push_back(cell(currentCell.x+(2*xm[i]),currentCell.y+(2*ym[i])));
				}
			}
		}
		if(possibleNextBranch.size()>0){
			cell targetCell=possibleNextBranch[Random::getIndex(possibleNextBranch.size())];
			stack.push_back(currentCell);
			world[targetCell.x][targetCell.y]=0;
			world[(currentCell.x+targetCell.x)/2][(currentCell.y+targetCell.y)/2]=0;
			currentCell=targetCell;
		} else {
			if(stack.size()>0){
				currentCell=stack.back();
				stack.pop_back();
			}
		}
	}
	StigmergyWorld::showWorld();
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