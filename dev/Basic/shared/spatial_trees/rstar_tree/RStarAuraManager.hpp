//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

#pragma once

#include <set>

#include "R_tree.hpp"
#include "metrics/Length.hpp"
#include "spatial_trees/TreeImpl.hpp"

namespace sim_mob
{

//Forward declarations.
class Point2D;
class Agent;
class Lane;


class RStarAuraManager : public TreeImpl
{
public:
	//No initialization required.
	virtual void init() {}

	//Note: The pointers in removedAgentPointers will be deleted after this time tick; do *not*
	//      save them anywhere.
	virtual void update(int time_step, const std::set<sim_mob::Agent*>& removedAgentPointers);

	//Not used.
	virtual void registerNewAgent(const Agent* ag) {}

	virtual std::vector<Agent const *> agentsInRect(const Point2D& lowerLeft, const Point2D& upperRight, const sim_mob::Agent* refAgent) const;

	virtual std::vector<Agent const *> nearbyAgents(const Point2D& position, const Lane& lane, centimeter_t distanceInFront, centimeter_t distanceBehind, const sim_mob::Agent* refAgent) const;

	//virtual std::vector<Agent const *> advanced_agentsInRect(const Point2D& lowerLeft, const Point2D& upperRight, TreeItem* item) const {return agentsInRect(lowerLeft, upperRight);}

	///Return Agents near to a given Position, with offsets (and Lane) taken into account.
	//virtual std::vector<Agent const *> advanced_nearbyAgents(const Point2D& position, const Lane& lane, centimeter_t distanceInFront, centimeter_t distanceBehind, TreeItem* item) const {return nearbyAgents(position, lane, distanceInFront, distanceBehind);}


private:
	sim_mob::R_tree tree_rstar;

	/* First dirty version... Will change eventually.
	 * This method is called from within the update of the AuraManager.
	 * This method increments the vehicle count for the road segment
	 * on which the Agent's vehicle is currently in.
	 */
//	void updateDensity(const Agent* ag);
};
}
