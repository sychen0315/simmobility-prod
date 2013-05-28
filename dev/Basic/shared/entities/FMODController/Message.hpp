/*
 * Message.hpp
 *
 *  Created on: May 22, 2013
 *      Author: zhang
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_
#include "string"
#include "vector"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <json/json.h>

namespace sim_mob {

namespace FMOD
{

class Message {
public:
	Message();
	virtual ~Message();
	virtual std::string BuildToString() { return msg_;}
protected:
	std::string msg_;
	int messageID_;
};

class Msg_Request : public Message {
public:
	std::string current_time;
	std::string client_id;
	std::string origin;
	std::string destination;
	std::string departure_time_early;
	std::string departure_time_late;
	int		seat_num;
public:
	virtual std::string BuildToString();

};

class Msg_Vehicle_Init : public Message {
public:
	struct Supply_object
	{
		std::string vehicle_id;
		std::string node_id;
	};
	std::vector<Supply_object> vehicles;
};

class Msg_Offer : public Message {
public:
	std::string client_id;
	struct OFFER
	{
		std::string schdule_id;
		int service_type;
		int fare;
		std::string departure_time_early;
		std::string depature_time_late;
		std::string arival_time_early;
		std::string arrival_time_late;
		int travel_time;
		int travel_distance;
	};
	std::vector<OFFER> offers;
};

class Msg_Accept : public Message {
public:
	std::string current_time;
	std::string schedule_id;
	std::string departure_time;
	std::string arrival_time;
};

class Msg_Confirmation : public Message {
public:
	std::string client_id;
	std::string schedule_id;
};

class Msg_Vehicle_Stop : public Message {
public:
	std::string vehicle_id;
	int	event_type;
	std::string schedule_id;
	std::string stop_id;
	std::vector<std::string> boarding_passengers;
	std::vector<std::string> aligting_passengers;
};

class Msg_Vehicle_Pos : public Message {
public:
	std::string vehicle_id;
	std::string latitude;
	std::string longtitude;
};

class Msg_Schedule : public Message {
	std::string vehicle_id;
	std::string schedule_id;
	std::vector<std::string> replace_schedules;
	int service_type;
	struct STOP
	{
		std::string stop_id;
		std::string arrival_time;
		std::string depature_time;
	};
	std::vector<STOP> stop_schdules;
	struct PASSENGER
	{
		std::string client_id;
		int price;
	};
	std::vector<PASSENGER> passengers;
	struct ROUTE
	{
		std::string id;
		int type;
	};
	std::vector<ROUTE> routes;

};
}

} /* namespace sim_mob */

typedef boost::shared_ptr<sim_mob::FMOD::Message> msg_ptr;

#endif /* MESSAGE_HPP_ */