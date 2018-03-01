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
//MABE Parameters
shared_ptr<ParameterLink<string>> StigmergyWorld::groupNamePL = Parameters::register_parameter("WORLD_STIGMERGY_NAMES-groupNameSpace", (string)"root::", "namespace of group to be evaluated");
shared_ptr<ParameterLink<string>> StigmergyWorld::brainNamePL = Parameters::register_parameter("WORLD_STIGMERGY_NAMES-brainNameSpace", (string)"root::", "namespace for parameters used to define brain");

StigmergyWorld::StigmergyWorld(shared_ptr<ParametersTable> _PT):AbstractWorld(_PT) {
	// columns to be added to ave file
	popFileColumns.clear();
	popFileColumns.push_back("score");
}

void StigmergyWorld::generateMap(){
	return;
}

void StigmergyWorld::evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug) {
	auto brain = org->brains[brainNamePL->get(PT)];
	for (int eval = 0; eval < evaluationsPerGenerationPL->get(PT); eval++) {
		for (int time = 0; time < lifeTime; time++){
			brain->resetBrain();
			brain->setInput(0, 1);
			brain->update();
			int i = 1;
			int dummy = Bit(brain->readOutput(i));
			double score = 1.0;
			org->dataMap.append("score", score);
		}
	}	
}