/*
 * Signal.hpp
 *
 *  Created on: 2011-7-18
 *      Author: xrm
 */

#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#pragma once

#include <vector>

#include "../constants.h"
#include "Entity.hpp"


namespace sim_mob
{

/**
 * Basic Signal class.
 *
 * NOTES:
 *1.The signal light uses adaptive signal control strategy (SCATS).
 *
 *2.The input value for SCATS is the degree of saturation(DS),this value
 *  can be recorded by loop detector, for now the loop section is
 *  nothing.We can define the value of DS as default or random.
 *
 *3.I am not so sure about how to add class Signal into the Basic Project,
 *  such as which folder to put it in, entities or roles? It needs to be
 *  updated and every agent should be able to get its information.
 *
 */
class Signal {



public:
	Signal();

	//DS:degree of saturation
	void updateSignal(double DS[4]);

	//set for the parameters in SCATS
	void updateprevCL() {prevCL=currCL;}
	void updatecurrCL() {currCL=nextCL;}
	void updateprevRL1 (double RL1){prevRL1=RL1;}
	void updateprevRL2 (double RL2){prevRL2=RL2;}
	void setnextCL (double DS);

	void setCL (double prevCL1, double currCL1, double nextCL1)
	{prevCL = prevCL1; currCL = currCL1; nextCL = nextCL1;}
	void setRL (double RL1, double RL2)
	{prevRL1 = RL1; prevRL2 = RL2;}

	//initialize the SplitPlan for SCATS
	void startSplitPlan();
	void setnextSplitPlan(double DS[4]);
	void updatecurrSplitPlanID(){currSplitPlanID = nextSplitPlanID;}
	void updatecurrSplitPlan()
	{
		for(int i = 0; i < 4; i++)currSplitPlan[i] = nextSplitPlan[i];
	}

	//Offset
	void setnextOffset(double nextCL);
	void updateOffset(){currOffset=nextOffset;}


	//get the parameters in SCATS
	double getprevCL() {return prevCL;}
	double getcurrCL() {return currCL;}
	double getnextCL() {return nextCL;}
	double getpreRL1() {return prevRL1;}
	double getpreRL2() {return prevRL2;}
	int getcurrSplitPlanID() {return currSplitPlanID;}
	int getnextSplitPlanID() {return nextSplitPlanID;}
	double getcurrOffset() {return currOffset;}
	double getnextOffset() {return nextOffset;}


	double fmax(double proDS[4]);
	int fmin_ID(double maxproDS[6]);
	int calvote(int vote1, int vote2, int vote3, int vote4, int vote5);

private:
	//previous,current and next cycle length
	double prevCL,currCL,nextCL;

	//two previous RL for calculating the current RL0
	double prevRL1,prevRL2;

	//SplitPlan that can be chosen to use
	double SplitPlan1[4], SplitPlan2[4], SplitPlan3[4], SplitPlan4[4], SplitPlan5[4];

	//current and next SplitPlan
	double currSplitPlan[4],nextSplitPlan[4];


	int currSplitPlanID,nextSplitPlanID;

	//votes for determining next SplitPlan
	int vote1, vote2, vote3, vote4, vote5;

	//current and next Offset
	double currOffset,nextOffset;

};

}


#endif /* SIGNAL_HPP_ */
