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
shared_ptr<ParameterLink<int>> StigmergyWorld::stigmergyBitsPL = Parameters::register_parameter("WORLD_STIGMERGY-stigmergyBits", 1, "Number of bits in stigmergic signal");


//MABE Parameters
shared_ptr<ParameterLink<string>> StigmergyWorld::groupNamePL = Parameters::register_parameter("WORLD_STIGMERGY_NAMES-groupNameSpace", (string)"root::", "namespace of group to be evaluated");
shared_ptr<ParameterLink<string>> StigmergyWorld::brainNamePL = Parameters::register_parameter("WORLD_STIGMERGY_NAMES-brainNameSpace", (string)"root::", "namespace for parameters used to define brain");

StigmergyWorld::StigmergyWorld(shared_ptr<ParametersTable> _PT):AbstractWorld(_PT)
, lifeTime(lifeTimePL->get(PT))
, stigmergyContentSize(stigmergyBitsPL->get(PT))
, inputSize(
	visionConeSize + compassSize + stigmergyLocationInputSize +
	stigmergyContentSize + foodInHandSize
	)
, outputSize(
	stigmergyWriteControlSize + stigmergyContentSize +
	stigmergyReadControlSize + movementControlSize
	)
{
	// columns to be added to ave file
	popFileColumns.clear();
	popFileColumns.push_back("score");
}

//Generates a map to evaluate organisms in
void StigmergyWorld::generateMap(){
	return;
}

void StigmergyWorld::evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug) {
	auto brain = org->brains[brainNamePL->get(PT)];
	for (int eval = 0; eval < evaluationsPerGenerationPL->get(PT); eval++) {

		// inputs
		std::vector<int> visionCone(visionConeSize);
		std::vector<int> compass(compassSize);
		std::vector<int> stigmergyLocationInput(stigmergyLocationInputSize);
		std::vector<int> stigmergyContentInput(stigmergyContentSize);
		std::vector<int> foodInHand(foodInHandSize);

		// outputs
		std::vector<int> stigmergyWriteControl(stigmergyWriteControlSize);
		std::vector<int> movementControl(movementControlSize);
		std::vector<int> stigmergyContentOutput(stigmergyContentSize);
		std::vector<int> stigmergyReadControl(stigmergyReadControlSize);

		brain->resetBrain();
		double score = 1.0;
		for (int time = 0; time < StigmergyWorld::lifeTime; time++){
			//reset brain I/O to avoid agents using deactivated nodes as hidden memory.
			brain->resetInputs();
			brain->resetOutputs();

			// set inputs
			int i = 0;

			for(int cur = 0; cur < visionCone.size(); ++cur, ++i) {
				brain->setInput(i, visionCone[cur]);
			}
			for(int cur = 0; cur < compass.size(); ++cur, ++i) {
				brain->setInput(i, compass[cur]);
			}
			for(int cur = 0; cur < stigmergyLocationInput.size(); ++cur, ++i) {
				brain->setInput(i, stigmergyLocationInput[cur]);
			}
			for(int cur = 0; cur < stigmergyContentInput.size(); ++cur, ++i) {
				brain->setInput(i, stigmergyContentInput[cur]);
			}
			for(int cur = 0; cur < foodInHand.size(); ++cur, ++i) {
				brain->setInput(i, foodInHand[cur]);
			}

			// update brain
			brain->update();

			// read outputs
			int o = 0;

			for(int cur = 0; cur < stigmergyWriteControl.size(); ++cur, ++o) {
				stigmergyWriteControl[cur] = Bit(brain->readOutput(o));
			}
			for(int cur = 0; cur < movementControl.size(); ++cur, ++o) {
				movementControl[cur] = Bit(brain->readOutput(o));
			}
			for(int cur = 0; cur < stigmergyContentOutput.size(); ++cur, ++o) {
				stigmergyContentOutput[cur] = Bit(brain->readOutput(o));
			}
			for(int cur = 0; cur < stigmergyReadControl.size(); ++cur, ++o) {
				stigmergyReadControl[cur] = Bit(brain->readOutput(o));
			}

		}
		org->dataMap.append("score", score);
	}
}
