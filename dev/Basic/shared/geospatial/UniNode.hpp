/* Copyright Singapore-MIT Alliance for Research and Technology */

#pragma once

#include <vector>
#include <map>
#include <set>

#include "GenConfig.h"

#include "Node.hpp"
namespace geo
{
class UniNode_t_pimpl;
class GeoSpatial_t_pimpl;
}
namespace sim_mob
{


//Forward declarations
class Point2D;
class Link;
class Lane;
class LaneConnector;
class RoadSegment;

namespace aimsun
{
//Forward declarations
class Loader;
}


/**
 * A Node where two to four RoadSegments from within the same Link meet.
 *
 * \author Seth N. Hetu
 * \author LIM Fung Chai
 *
 * This usually occurs because lane rules (or the actual number of lanes) change.
 *
 * \note
 * Currently, UniNodes are slightly restricted: They MUST always contain either ONE or TWO
 *   paths through them, one in the forward direction and another in the reverse. See the note
 *   in Loader::ProcessSection() about this limitation. In the general case, we can say that a
 *   UniNode should simply be a Node that occurs between RoadSegments in the same Link.
 *
 * Each lane from an incoming segment connects directly to one Lane in an outgoing segment which
 *   is NOTE heading back to the same source node.
 */
class UniNode : public sim_mob::Node {
	friend class sim_mob::aimsun::Loader;
	friend class ::geo::UniNode_t_pimpl;
	friend class ::geo::GeoSpatial_t_pimpl;
public:
	UniNode(int x, int y) : Node(x, y) {}

	///Retrieve the outgoing Lane at this Node.
	const sim_mob::Lane* getOutgoingLane(const sim_mob::Lane& from) const;
	const std::map<const sim_mob::Lane*, sim_mob::Lane* > & getConnectors() const {return connectors;}

	///Helper method: Build the connectors vector dynamically by aligning a lane in the "from" Road Segment with one
	/// in the "to" Road Segment.
	///NOTE: The "from/to" laneID pairs will definitely be cleaned up later; for now I'm just trying
	//       to get them to output something decent. At the moment they MUST correspond to "firstPair", "secondPair". ~Seth
	static void buildConnectorsFromAlignedLanes(UniNode* node, std::pair<unsigned int, unsigned int> fromToLaneIDs1, std::pair<unsigned int, unsigned int> fromToLaneIDs2);

	const std::vector<const sim_mob::RoadSegment*>& getRoadSegments() const;

protected:
	std::map<const sim_mob::Lane*, sim_mob::Lane* > connectors;

	///Bookkeeping: which RoadSegments meet at this Node?
	//  NOTE: If the RoadSegments in secondPair are null; then this is a one-way UniNode.
	//  As "from->to"
	std::pair<const sim_mob::RoadSegment*, const sim_mob::RoadSegment*> firstPair;
	std::pair<const sim_mob::RoadSegment*, const sim_mob::RoadSegment*> secondPair;

	//Avoid iterating confusion
	mutable std::vector<const sim_mob::RoadSegment*> cachedSegmentsList;




};



}