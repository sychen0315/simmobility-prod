// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#include "geo10-driver.hpp"

#include <iostream>

#include "geo10-pimpl.hpp"

bool sim_mob::xml::InitAndLoadXML()
{
	std::cout << "In InitAndLoadXML (sim_mob::XML)\n";

/*	try {
		// Instantiate individual parsers.
		//
		::sim_mob::xml::SimMobility_t_pimpl SimMobility_t_p;
		::sim_mob::xml::GeoSpatial_t_pimpl GeoSpatial_t_p;
		::sim_mob::xml::RoadNetwork_t_pimpl RoadNetwork_t_p;
		::sim_mob::xml::Nodes_pimpl Nodes_p;
		::sim_mob::xml::UniNodes_pimpl UniNodes_p;
		::sim_mob::xml::UniNode_t_pimpl UniNode_t_p;
		::xml_schema::unsigned_int_pimpl unsigned_int_p;
		::sim_mob::xml::Point2D_t_pimpl Point2D_t_p;
		::xml_schema::unsigned_long_pimpl unsigned_long_p;
		::xml_schema::string_pimpl string_p;
		::sim_mob::xml::connectors_t_pimpl connectors_t_p;
		::sim_mob::xml::connector_t_pimpl connector_t_p;
		::sim_mob::xml::Intersections_pimpl Intersections_p;
		::sim_mob::xml::intersection_t_pimpl intersection_t_p;
		::sim_mob::xml::RoadSegmentsAt_t_pimpl RoadSegmentsAt_t_p;
		::sim_mob::xml::Multi_Connectors_t_pimpl Multi_Connectors_t_p;
		::sim_mob::xml::Multi_Connector_t_pimpl Multi_Connector_t_p;
		::sim_mob::xml::ChunkLengths_t_pimpl ChunkLengths_t_p;
		::sim_mob::xml::ChunkLength_t_pimpl ChunkLength_t_p;
		::xml_schema::unsigned_short_pimpl unsigned_short_p;
		::sim_mob::xml::offsets_t_pimpl offsets_t_p;
		::sim_mob::xml::offset_t_pimpl offset_t_p;
		::sim_mob::xml::separators_t_pimpl separators_t_p;
		::sim_mob::xml::separator_t_pimpl separator_t_p;
		::xml_schema::boolean_pimpl boolean_p;
		::sim_mob::xml::LanesVector_t_pimpl LanesVector_t_p;
		::sim_mob::xml::DomainIslands_t_pimpl DomainIslands_t_p;
		::sim_mob::xml::DomainIsland_t_pimpl DomainIsland_t_p;
		::sim_mob::xml::roundabouts_pimpl roundabouts_p;
		::sim_mob::xml::roundabout_t_pimpl roundabout_t_p;
		::xml_schema::float_pimpl float_p;
		::xml_schema::int_pimpl int_p;
		::sim_mob::xml::EntranceAngles_t_pimpl EntranceAngles_t_p;
		::sim_mob::xml::EntranceAngle_t_pimpl EntranceAngle_t_p;
		::sim_mob::xml::Links_pimpl Links_p;
		::sim_mob::xml::link_t_pimpl link_t_p;
		::sim_mob::xml::Segments_pimpl Segments_p;
		::sim_mob::xml::fwdBckSegments_t_pimpl fwdBckSegments_t_p;
		::sim_mob::xml::segment_t_pimpl segment_t_p;
		::xml_schema::short_pimpl short_p;
		::sim_mob::xml::PolyLine_t_pimpl PolyLine_t_p;
		::sim_mob::xml::PolyPoint_t_pimpl PolyPoint_t_p;
		::sim_mob::xml::Lanes_pimpl Lanes_p;
		::sim_mob::xml::lane_t_pimpl lane_t_p;
		::sim_mob::xml::RoadItems_t_pimpl RoadItems_t_p;
		::sim_mob::xml::BusStop_t_pimpl BusStop_t_p;
		::sim_mob::xml::ERP_Gantry_t_pimpl ERP_Gantry_t_p;
		::sim_mob::xml::crossing_t_pimpl crossing_t_p;
		::sim_mob::xml::PointPair_t_pimpl PointPair_t_p;
		::sim_mob::xml::RoadBump_t_pimpl RoadBump_t_p;
		::sim_mob::xml::TripChains_t_pimpl TripChains_t_p;
		::sim_mob::xml::TripChain_t_pimpl TripChain_t_p;
		::xml_schema::integer_pimpl integer_p;
		::sim_mob::xml::Trip_t_pimpl Trip_t_p;
		::sim_mob::xml::TripchainItemType_pimpl TripchainItemType_p;
		::sim_mob::xml::TripchainItemLocationType_pimpl TripchainItemLocationType_p;
		::sim_mob::xml::SubTrips_t_pimpl SubTrips_t_p;
		::sim_mob::xml::SubTrip_t_pimpl SubTrip_t_p;
		::sim_mob::xml::Activity_t_pimpl Activity_t_p;

		// Connect the parsers together.
		//
		SimMobility_t_p.parsers(GeoSpatial_t_p, TripChains_t_p);

		GeoSpatial_t_p.parsers(RoadNetwork_t_p);

		RoadNetwork_t_p.parsers(Nodes_p, Links_p);

		Nodes_p.parsers(UniNodes_p, Intersections_p, roundabouts_p);

		UniNodes_p.parsers(UniNode_t_p);

		UniNode_t_p.parsers(unsigned_int_p, Point2D_t_p, unsigned_long_p,
				string_p, connectors_t_p);

		Point2D_t_p.parsers(unsigned_int_p, unsigned_int_p);

		connectors_t_p.parsers(connector_t_p);

		connector_t_p.parsers(unsigned_long_p, unsigned_long_p);

		Intersections_p.parsers(intersection_t_p);

		intersection_t_p.parsers(unsigned_int_p, Point2D_t_p, unsigned_long_p,
				string_p, RoadSegmentsAt_t_p, Multi_Connectors_t_p,
				ChunkLengths_t_p, offsets_t_p, separators_t_p, LanesVector_t_p,
				LanesVector_t_p, DomainIslands_t_p);

		RoadSegmentsAt_t_p.parsers(unsigned_long_p);

		Multi_Connectors_t_p.parsers(Multi_Connector_t_p);

		Multi_Connector_t_p.parsers(unsigned_long_p, connectors_t_p);

		ChunkLengths_t_p.parsers(ChunkLength_t_p);

		ChunkLength_t_p.parsers(unsigned_short_p, unsigned_int_p);

		offsets_t_p.parsers(offset_t_p);

		offset_t_p.parsers(unsigned_short_p, unsigned_int_p);

		separators_t_p.parsers(separator_t_p);

		separator_t_p.parsers(unsigned_short_p, boolean_p);

		LanesVector_t_p.parsers(unsigned_long_p);

		DomainIslands_t_p.parsers(DomainIsland_t_p);

		DomainIsland_t_p.parsers(unsigned_short_p, boolean_p);

		roundabouts_p.parsers(roundabout_t_p);

		roundabout_t_p.parsers(unsigned_int_p, Point2D_t_p, unsigned_long_p,
				string_p, RoadSegmentsAt_t_p, Multi_Connectors_t_p,
				ChunkLengths_t_p, offsets_t_p, separators_t_p, LanesVector_t_p,
				float_p, int_p, EntranceAngles_t_p);

		EntranceAngles_t_p.parsers(EntranceAngle_t_p);

		EntranceAngle_t_p.parsers(unsigned_short_p, unsigned_int_p);

		Links_p.parsers(link_t_p);

		link_t_p.parsers(unsigned_int_p, string_p, unsigned_int_p,
				unsigned_int_p, Segments_p);

		Segments_p.parsers(fwdBckSegments_t_p, fwdBckSegments_t_p);

		fwdBckSegments_t_p.parsers(segment_t_p);

		segment_t_p.parsers(unsigned_long_p, unsigned_int_p, unsigned_int_p,
				short_p, unsigned_int_p, unsigned_int_p, string_p, PolyLine_t_p,
				Lanes_p, RoadItems_t_p, PolyLine_t_p);

		PolyLine_t_p.parsers(PolyPoint_t_p);

		PolyPoint_t_p.parsers(string_p, Point2D_t_p);

		Lanes_p.parsers(lane_t_p);

		lane_t_p.parsers(unsigned_long_p, unsigned_int_p, boolean_p, boolean_p,
				boolean_p, boolean_p, boolean_p, boolean_p, boolean_p,
				boolean_p, boolean_p, boolean_p, boolean_p, boolean_p,
				boolean_p, boolean_p, boolean_p, boolean_p, PolyLine_t_p);

		RoadItems_t_p.parsers(BusStop_t_p, ERP_Gantry_t_p, crossing_t_p,
				RoadBump_t_p);

		BusStop_t_p.parsers(unsigned_short_p, Point2D_t_p, Point2D_t_p,
				string_p, string_p, boolean_p, boolean_p, boolean_p,
				unsigned_int_p);

		ERP_Gantry_t_p.parsers(unsigned_short_p, Point2D_t_p, Point2D_t_p,
				string_p);

		crossing_t_p.parsers(unsigned_short_p, Point2D_t_p, Point2D_t_p,
				string_p, PointPair_t_p, PointPair_t_p);

		PointPair_t_p.parsers(Point2D_t_p, Point2D_t_p);

		RoadBump_t_p.parsers(unsigned_short_p, Point2D_t_p, Point2D_t_p,
				string_p, unsigned_long_p);

		TripChains_t_p.parsers(TripChain_t_p);

		TripChain_t_p.parsers(integer_p, Trip_t_p, Activity_t_p);

		Trip_t_p.parsers(integer_p, TripchainItemType_p, unsigned_int_p,
				string_p, string_p, integer_p, unsigned_int_p,
				TripchainItemLocationType_p, unsigned_int_p,
				TripchainItemLocationType_p, SubTrips_t_p);

		SubTrips_t_p.parsers(SubTrip_t_p);

		SubTrip_t_p.parsers(string_p, boolean_p, string_p);

		Activity_t_p.parsers(integer_p, TripchainItemType_p, unsigned_int_p,
				string_p, string_p, string_p, unsigned_int_p,
				TripchainItemLocationType_p, boolean_p, boolean_p, boolean_p);

		// Parse the XML document.
		//
		::xml_schema::document doc_p (SimMobility_t_p,"http://www.smart.mit.edu/geo","SimMobility");

		SimMobility_t_p.pre();
		doc_p.parse("data/XML_OutPut.xml");
		SimMobility_t_p.post_SimMobility_t();
	} catch (const ::xml_schema::exception& e) {
		std::cout <<"XML parsing failed: " <<e.what() << std::endl;
		return false;
	} catch (const std::ios_base::failure&) {
		std::cerr <<"Io error on file: " <<"data/XML_OutPut.xml" << std::endl;
		return false;
	}*/
	throw std::runtime_error("Driver will be updated later.");
	return false;
}

