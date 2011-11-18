/* Copyright Singapore-MIT Alliance for Research and Technology */

#include "GeneralPathMover.hpp"

#include <limits>
#include <algorithm>
#include <stdexcept>

#include "geospatial/RoadSegment.hpp"
#include "geospatial/Link.hpp"
#include "geospatial/Lane.hpp"
#include "util/GeomHelpers.hpp"

using namespace sim_mob;
using std::vector;



void sim_mob::GeneralPathMover::setPath(vector<WayPoint> wp_path, bool firstSegMoveFwd)
{
	fullPath.clear();
	for(vector<WayPoint>::iterator it=wp_path.begin(); it!=wp_path.end(); it++) {
		if(it->type_ == WayPoint::ROAD_SEGMENT) {
			fullPath.push_back(it->roadSegment_);
		}
	}

	currSegmentIt = fullPath.begin();
	isMovingForwards = firstSegMoveFwd;
	generateNewPolylineArray();
	distAlongPolyline = 0;
	inIntersection = false;
	distMovedInSegment = 0;
}


void sim_mob::GeneralPathMover::setPath(const vector<RoadSegment*>& path, bool firstSegMoveFwd)
{
	fullPath.clear();
	for(vector<RoadSegment*>::const_iterator it=path.begin(); it!=path.end(); it++) {
		fullPath.push_back(*it);
	}

	currSegmentIt = fullPath.begin();
	isMovingForwards = firstSegMoveFwd;
	generateNewPolylineArray();
	distAlongPolyline = 0;
	inIntersection = false;
	distMovedInSegment = 0;
}

void sim_mob::GeneralPathMover::generateNewPolylineArray()
{
	//Simple; just make sure to take the forward direction into account.
	polypointsList = (*currSegmentIt)->getLanes().front()->getPolyline();
	if (!isMovingForwards) {
		std::reverse(polypointsList.begin(), polypointsList.end());
	}
	currPolypoint = polypointsList.begin();
	nextPolypoint = polypointsList.begin()+1;
	currPolylineLength = sim_mob::dist(&(*currPolypoint), &(*nextPolypoint));
}

bool sim_mob::GeneralPathMover::isPathSet() const
{
	return !fullPath.empty();
}

bool sim_mob::GeneralPathMover::isDoneWithEntireRoute() const
{
	return currSegmentIt==fullPath.end();
}

void sim_mob::GeneralPathMover::leaveIntersection()
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(!inIntersection, "Not actually in an Intersection!");

	//Unset flag; move to the next segment.
	inIntersection = false;
	actualMoveToNextSegmentAndUpdateDir();
}

bool sim_mob::GeneralPathMover::isMovingForwardsOnCurrSegment() const
{
	return isMovingForwards;
}


//This is where it gets a little complex.
void sim_mob::GeneralPathMover::advance(double fwdDistance)
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");

	//Taking precedence above everything else is the intersection model. If we are in an intersection,
	//  simply update the total distance and return (let the user deal with it). Also udpate the
	//  current polyline length to always be the same as the forward distance.
	if (inIntersection) {
		distAlongPolyline += fwdDistance;
		currPolylineLength = distAlongPolyline;
		return;
	}

	//Next, if we are truly at the end of the path, we should probably throw an error for trying to advance.
	throwIf(isDoneWithEntireRoute(), "Entire path is already done.");

	//Move down the current polyline. If this brings us to the end point, go to the next polyline
	distAlongPolyline += fwdDistance;
	distMovedInSegment += fwdDistance;
	while (distAlongPolyline>=currPolylineLength) {
		advanceToNextPolyline();
	}
}


void sim_mob::GeneralPathMover::advanceToNextPolyline()
{
	//An error if we're still at the end of this polyline
	throwIf(nextPolypoint==polypointsList.end(), "Polyline can't advance");

	//Subtract distance moved thus far
	distAlongPolyline -= currPolylineLength;

	//Advance pointers
	currPolypoint++;
	nextPolypoint++;

	//Update length, OR move to a new Segment
	if (nextPolypoint != polypointsList.end()) {
		currPolylineLength = sim_mob::dist(&(*currPolypoint), &(*nextPolypoint));
	} else {
		advanceToNextRoadSegment();
	}
}


void sim_mob::GeneralPathMover::advanceToNextRoadSegment()
{
	//An error if we're already at the end of this road segment
	throwIf(currSegmentIt==fullPath.end(), "Road segment can't advance");
	distMovedInSegment = 0;

	//If we are approaching a new Segment, the Intersection driving model takes precedence.
	// In addition, no further processing occurs. This means advancing a very large amount will
	// leave the user inside an intersection even if he/she would normally be beyond it.
	//Note that distAlongPolyline should still be valid.
	if ((*currSegmentIt)->getLink() != (*(currSegmentIt+1))->getLink()) {
		inIntersection = true;
		return;
	}

	//Move to the next Segment
	actualMoveToNextSegmentAndUpdateDir();
}

void sim_mob::GeneralPathMover::actualMoveToNextSegmentAndUpdateDir()
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(isDoneWithEntireRoute(), "Entire path is already done.");

	//Move
	currSegmentIt++;

	//Just in case
	distMovedInSegment=0;

	//Is this new segment in reverse?
	const Node* prevNode = isMovingForwards ? (*(currSegmentIt-1))->getEnd() : (*(currSegmentIt-1))->getStart();
	if ((*currSegmentIt)->getStart() == prevNode) {
		isMovingForwards = true;
	} else if ((*currSegmentIt)->getEnd() == prevNode) {
		isMovingForwards = false;
	} else {
		//Presumably, we could enable something like this later, but it would require advanced
		//  knowledge of which Segments face forwards.
		throw std::runtime_error("Can't jump around to arbitrary nodes with GeneralPathMover.");
	}

	//Now generate a new polyline array.
	generateNewPolylineArray();
}


const RoadSegment* sim_mob::GeneralPathMover::getCurrSegment() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(isDoneWithEntireRoute(), "Entire path is already done.");
	return *currSegmentIt;
}
const Link* sim_mob::GeneralPathMover::getCurrLink() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(isDoneWithEntireRoute(), "Entire path is already done.");
	return getCurrSegment()->getLink();
}
const Point2D& sim_mob::GeneralPathMover::getCurrPolypoint() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(isDoneWithEntireRoute(), "Entire path is already done."); //Not technically wrong, but probably an error.
	return *currPolypoint;
}
const Point2D& sim_mob::GeneralPathMover::getNextPolypoint() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(isDoneWithEntireRoute(), "Entire path is already done.");
	return *nextPolypoint;
}

double sim_mob::GeneralPathMover::getCurrDistAlongPolyline() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");

	//If we're done, returning zero makes sense
	if (isDoneWithEntireRoute()) {
		return 0;
	}

	//Limiting by the total distance makes sense.
	return std::min(distAlongPolyline, currPolylineLength);
}

double sim_mob::GeneralPathMover::getCurrDistAlongRoadSegment() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");

	return distMovedInSegment;
}

double sim_mob::GeneralPathMover::getCurrPolylineTotalDist() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");
	throwIf(isDoneWithEntireRoute(), "Entire path is already done.");
	return currPolylineLength;
}


DPoint sim_mob::GeneralPathMover::getPosition() const
{
	throwIf(!isPathSet(), "GeneralPathMover path not set.");

	//If we're done, return the position of the last poly-point
	if (isDoneWithEntireRoute()) {
		return DPoint(currPolypoint->getX(),currPolypoint->getY());
	}

	//Else, scale a vector like normal
	DynamicVector movementVect(currPolypoint->getX(),currPolypoint->getY(), nextPolypoint->getX(), nextPolypoint->getY());
	movementVect.scaleVectTo(getCurrDistAlongPolyline()).translateVect();
	return DPoint(movementVect.getX(), movementVect.getY());
}






