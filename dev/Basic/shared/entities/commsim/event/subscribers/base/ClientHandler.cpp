//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

/*
 * ClientHandler.cpp
 *
 *  Created on: May 28, 2013
 *      Author: vahid
 */

#include "ClientHandler.hpp"
#include "entities/commsim/Broker.hpp"
#include "entities/commsim/event/JsonSerializableEventArgs.hpp"

using namespace sim_mob;

sim_mob::ClientHandler::ClientHandler(sim_mob::Broker& broker, boost::shared_ptr<sim_mob::ConnectionHandler> conn, sim_mob::AgentCommUtilityBase* agentComm, const sim_mob::Agent* agent, std::string clientId)
	: broker(broker), valid(true), connHandle(conn), agentComm(agentComm), agent(agent), clientId(clientId)
{
	if (!conn) {
		throw std::runtime_error("Cannot create a client handler with a null connection handler.");
	}
}

sim_mob::ClientHandler::~ClientHandler()
{
}

sim_mob::Broker& sim_mob::ClientHandler::getBroker()
{
	return broker;
}

void sim_mob::ClientHandler::setRequiredServices(const std::set<sim_mob::Services::SIM_MOB_SERVICE>& requiredServices)
{
	this->requiredServices = requiredServices;
}

const std::set<sim_mob::Services::SIM_MOB_SERVICE>& sim_mob::ClientHandler::getRequiredServices()
{
	return requiredServices;
}

void sim_mob::ClientHandler::sendSerializedMessageToBroker(sim_mob::event::EventId id, sim_mob::event::Context context, sim_mob::event::EventPublisher* sender, const sim_mob::BaseCommsimEventArgs& argums)
{
	//now send to broker's buffer
	getBroker().insertSendBuffer(shared_from_this(), argums.serialize());
}

bool sim_mob::ClientHandler::isValid() const
{
	return valid;
}

void sim_mob::ClientHandler::setValidation(bool value)
{
	valid = value;
}
