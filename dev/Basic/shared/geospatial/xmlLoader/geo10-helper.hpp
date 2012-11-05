#pragma once

///\file geo10-helper.hpp
///  This file contains the "helper" namespace, which contains various classes and typedefs useful for
///  working with the geo10 parsers. It is included with the skeleton file, so all classes should have access to it.

#include <map>
#include <set>

#include "geospatial/Node.hpp"
#include "geospatial/Lane.hpp"
#include "geospatial/Link.hpp"
#include "geospatial/UniNode.hpp"
#include "geospatial/MultiNode.hpp"
#include "geospatial/RoadSegment.hpp"


namespace sim_mob {
namespace xml {
namespace helper {

//Was: geo_UniNode_Connectors_type
typedef std::set<std::pair<unsigned long,unsigned long> > UniNodeConnectors;

//Was: geo_MultiNode_Connectors_type
typedef std::map<unsigned long, helper::UniNodeConnectors > MultiNodeConnectors;

///POD type for returning from "Nodes".
struct NodesRes {
	std::set<sim_mob::UniNode*> uniNodes;
	std::set<sim_mob::MultiNode*> multiNodes;
};



///The "Bookkeeping" class exists as an optimization for lookup (the other option is to propagate IDs back to the RoadNetwork level).
///  A new object of this class type is passed by reference to all relevant classes in the parser construction phase.
///  Note that this class cannot optimize items which are referenced by ID *before* they are created.
///This class is not thread-safe, but it would be fairly simple to make it so (via a multiple-readers/single-writer paradigm; see
///  Shared<>'s Locked<> version for sample code). Anyway, for now we are not reading the road network in parallel.
class Bookkeeping {
public:
	void addNode(sim_mob::Node* node) {
		unsigned long id = node->getID();
		if (nodeLookup.count(id)>0) {
			throw std::runtime_error("Node already registered with bookkeeper.");
		}
		nodeLookup[id] = node;
	}
	sim_mob::Node* getNode(unsigned long id) const {
		std::map<unsigned long, sim_mob::Node*>::const_iterator it = nodeLookup.find(id);
		if (it!=nodeLookup.end()) {
			return it->second;
		}
		throw std::runtime_error("No Node exists in bookkeeper with the requested id.");
	}

	void addLane(sim_mob::Lane* lane) {
		unsigned long id = lane->getLaneID();
		if (laneLookup.count(id)>0) {
			throw std::runtime_error("Lane already registered with bookkeeper.");
		}
		laneLookup[id] = lane;
	}
	sim_mob::Lane* getLane(unsigned long id) const {
		std::map<unsigned long, sim_mob::Lane*>::const_iterator it = laneLookup.find(id);
		if (it!=laneLookup.end()) {
			return it->second;
		}
		throw std::runtime_error("No Lane exists in bookkeeper with the requested id.");
	}

	void addSegment(sim_mob::RoadSegment* segment) {
		unsigned long id = segment->getSegmentID();
		if (segmentLookup.count(id)>0) {
			throw std::runtime_error("Segment already registered with bookkeeper.");
		}
		segmentLookup[id] = segment;
	}
	sim_mob::RoadSegment* getSegment(unsigned long id) const {
		std::map<unsigned long, sim_mob::RoadSegment*>::const_iterator it = segmentLookup.find(id);
		if (it!=segmentLookup.end()) {
			return it->second;
		}
		throw std::runtime_error("No Segment exists in bookkeeper with the requested id.");
	}

	void addLink(sim_mob::Link* link) {
		unsigned long id = link->getLinkId();
		if (linkLookup.count(id)>0) {
			throw std::runtime_error("Link already registered with bookkeeper.");
		}
		linkLookup[id] = link;
	}
	sim_mob::Link* getLink(unsigned long id) const {
		std::map<unsigned long, sim_mob::Link*>::const_iterator it = linkLookup.find(id);
		if (it!=linkLookup.end()) {
			return it->second;
		}
		throw std::runtime_error("No Link exists in bookkeeper with the requested id.");
	}



	//TODO: These are temporary!
	typedef std::map<unsigned long,std::set<std::pair<unsigned long,unsigned long> > > MNConnect;
	typedef std::set<std::pair<unsigned long,unsigned long> > UNConnect;
	void addMultiNodeLaneConnectorCache(sim_mob::MultiNode* id, const MNConnect& item) {
		if (multiNodeLaneConnectorsCache.count(id)>0) {
			throw std::runtime_error("MultiNodeLaneConnector already registered with bookkeeper.");
		}
		multiNodeLaneConnectorsCache[id] = item;
	}
	MNConnect getMultiNodeLaneConnectorCache(sim_mob::MultiNode* id) const {
		std::map<sim_mob::MultiNode*, MNConnect>::const_iterator it = multiNodeLaneConnectorsCache.find(id);
		if (it!=multiNodeLaneConnectorsCache.end()) {
			return it->second;
		}
		throw std::runtime_error("No MultiNodeLaneConnector exists in bookkeeper with the requested id.");
	}
	void addUniNodeLaneConnectorCache(sim_mob::UniNode* id, const UNConnect& item) {
		if (uniNodeLaneConnectorsCache.count(id)>0) {
			throw std::runtime_error("UniNodeLaneConnector already registered with bookkeeper.");
		}
		uniNodeLaneConnectorsCache[id] = item;
	}
	UNConnect getUniNodeLaneConnectorCache(sim_mob::UniNode* id) const {
		std::map<sim_mob::UniNode*, UNConnect>::const_iterator it = uniNodeLaneConnectorsCache.find(id);
		if (it!=uniNodeLaneConnectorsCache.end()) {
			return it->second;
		}
		throw std::runtime_error("No UniNodeLaneConnector exists in bookkeeper with the requested id.");
	}


private:
	std::map<unsigned long, sim_mob::Node*> nodeLookup;
	std::map<unsigned long, sim_mob::Lane*> laneLookup;
	std::map<unsigned long, sim_mob::RoadSegment*> segmentLookup;
	std::map<unsigned long, sim_mob::Link*> linkLookup;

	//
	//TODO: The following items need to be removed once we merge our XML code.
	//
	//Can remove if Connectors are specified after Segments and Lanes.
	std::map<sim_mob::MultiNode*, MNConnect> multiNodeLaneConnectorsCache;
	std::map<sim_mob::UniNode*, UNConnect> uniNodeLaneConnectorsCache;
};


}}} //End sim_mob::xml::helper namespace
