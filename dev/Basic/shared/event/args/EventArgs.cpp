/*
 * Copyright Singapore-MIT Alliance for Research and Technology
 *  
 * File:   EventArgs.cpp
 * Author: Pedro Gandola <pedrogandola@smart.mit.edu>
 * 
 * Created on March 7, 2013, 3:26 PM
 */

#include "EventArgs.hpp"

using namespace sim_mob::event;

EventArgs::EventArgs() {
}

EventArgs::EventArgs(const EventArgs& source) {
}

EventArgs::~EventArgs() {
}

EventArgs& EventArgs::operator=(const EventArgs& source) {
    return *this;
}

