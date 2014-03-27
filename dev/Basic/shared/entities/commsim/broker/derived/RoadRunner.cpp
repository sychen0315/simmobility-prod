#include "RoadRunner.hpp"

#include <sstream>
#include <boost/assign/list_of.hpp>
#include <json/json.h>

#include "conf/ConfigManager.hpp"
#include "conf/ConfigParams.hpp"
#include "workers/Worker.hpp"

#include "entities/commsim/broker/base/Common.hpp"
#include "entities/commsim/comm_support/AgentCommUtility.hpp"
#include "entities/commsim/connection/ConnectionHandler.hpp"
#include "entities/commsim/connection/ConnectionServer.hpp"
#include "entities/commsim/event/subscribers/base/ClientHandler.hpp"
#include "entities/commsim/event/RegionsAndPathEventArgs.hpp"

#include "entities/commsim/client/derived/android/AndroidClientRegistration.hpp"
#include "entities/commsim/client/derived/ns3/NS3ClientRegistration.hpp"

#include "geospatial/RoadRunnerRegion.hpp"
#include "entities/profile/ProfileBuilder.hpp"

//todo :temprorary
#include "entities/commsim/message/base/BasicMessageFactory.hpp"
#include "entities/commsim/message/derived/roadrunner-android/RoadRunnerFactory.hpp"
#include "entities/commsim/message/derived/roadrunner-ns3/Ns3Factory.hpp"

#include "entities/commsim/wait/WaitForAndroidConnection.hpp"
#include "entities/commsim/wait/WaitForNS3Connection.hpp"
#include "entities/commsim/wait/WaitForAgentRegistration.hpp"

sim_mob::Roadrunner_Broker::Roadrunner_Broker(const MutexStrategy& mtxStrat, int id ,std::string commElement_, std::string commMode_) :
Broker(mtxStrat, id, commElement_, commMode_)
{
	Print() << "Creating Broker for RoadRunner" << std::endl;
	configure();
	/*
	 * the following code can be repeating considering the subclassing:
	 * \code
	//Various Initializations
	connection.reset(new ConnectionServer(*this));
	brokerCanTickForward = false;
	m_messageReceiveCallback = boost::function<void(boost::shared_ptr<ConnectionHandler>, std::string)>
		(boost::bind(&Broker::messageReceiveCallback,this, _1, _2));

//	Print() << "Creating Roadrunner Broker [" << this << "]" << std::endl;
//	configure(); //already done at the time of creation
 * \endcode
 */
}

//configure the brokers behavior
//publishers, blockers(synchronizers) and message handlers... to name a few
void sim_mob::Roadrunner_Broker::configure()
{
	//Only configure once.
	if (!configured_.check()) {
		return;
	}

	std::string client_mode = ConfigManager::GetInstance().FullConfig().getCommSimMode(commElement);

	sim_mob::Worker::GetUpdatePublisher().subscribe(sim_mob::event::EVT_CORE_AGENT_UPDATED,
                this,
                &Roadrunner_Broker::onAgentUpdate,
                (event::Context)sim_mob::event::CXT_CORE_AGENT_UPDATE);

	//	client registration handlers
	//	Also listen to publishers who announce registration of new clients...
	ClientRegistrationHandlerMap[comm::ANDROID_EMULATOR].reset(new sim_mob::AndroidClientRegistration());
	registrationPublisher.registerEvent(comm::ANDROID_EMULATOR);
	registrationPublisher.subscribe((event::EventId)comm::ANDROID_EMULATOR, this, &Roadrunner_Broker::onClientRegister);

	if(client_mode == "android-ns3") {
		ClientRegistrationHandlerMap[comm::NS3_SIMULATOR].reset(new sim_mob::NS3ClientRegistration());
		registrationPublisher.registerEvent(comm::NS3_SIMULATOR);
		//listen to publishers who announce registration of new clients...
		registrationPublisher.subscribe((event::EventId)comm::NS3_SIMULATOR, this, &Roadrunner_Broker::onClientRegister);
	}


	publisher.registerEvent(COMMEID_LOCATION);
	publisher.registerEvent(COMMEID_TIME);
	publisher.registerEvent(COMMEID_REGIONS_AND_PATH);
	serviceList.push_back(sim_mob::Services::SIMMOB_SRV_LOCATION);
	serviceList.push_back(sim_mob::Services::SIMMOB_SRV_TIME);
	serviceList.push_back(sim_mob::Services::SIMMOB_SRV_REGIONS_AND_PATH);

	//NS-3 has its own publishers
	if (client_mode == "android-ns3") {
		publisher.registerEvent(COMMEID_ALL_LOCATIONS);
		serviceList.push_back(sim_mob::Services::SIMMOB_SRV_ALL_LOCATIONS);
	}

/*
 * based on the event publisher design, the following code can be replaced by a single
 * publisher. we leave it here for comparison. one snippet should be finally chosen:
 * \code
	BrokerPublisher* onlyLocationsPublisher = new BrokerPublisher();
	onlyLocationsPublisher->registerEvent(COMMEID_LOCATION);

	publishers.insert(std::make_pair(
		sim_mob::Services::SIMMOB_SRV_LOCATION,
		PublisherList::Value(onlyLocationsPublisher))
	);

	//NS-3 has its own publishers
	if(client_mode == "android-ns3") {
		BrokerPublisher* allLocationsPublisher = new BrokerPublisher();
		allLocationsPublisher->registerEvent(COMMEID_LOCATION);
		publishers.insert(std::make_pair(
			sim_mob::Services::SIMMOB_SRV_ALL_LOCATIONS,
			PublisherList::Value(allLocationsPublisher))
		);
	}

	BrokerPublisher* timePublisher = new BrokerPublisher();
	timePublisher->registerEvent(COMMEID_TIME);
	publishers.insert(std::make_pair(
		sim_mob::Services::SIMMOB_SRV_TIME,
		PublisherList::Value(timePublisher))
	);
	\endcode
*/


	//TODO: We need to move the DIFFERENT message types here into the handleLookup.
	//      MOST of them should be the default; the android-ns3 and android-only settings should only change a few of them.
	std::map<unsigned int, void*> messageFactories;

	if(client_mode == "android-ns3") {
		//boost::shared_ptr<sim_mob::MessageFactory<std::vector<sim_mob::comm::MsgPtr>, std::string> > android_factory(new sim_mob::roadrunner::RoadRunnerFactory(true));
		//boost::shared_ptr<sim_mob::MessageFactory<std::vector<sim_mob::comm::MsgPtr>, std::string> > ns3_factory(new sim_mob::rr_android_ns3::NS3_Factory());


		messageFactories.insert(std::make_pair(comm::ANDROID_EMULATOR, new sim_mob::roadrunner::RoadRunnerFactory(true)));
		messageFactories.insert(std::make_pair(comm::NS3_SIMULATOR, new sim_mob::rr_android_ns3::NS3_Factory()));
	//	messageFactories.insert(std::make_pair(comm::UNKNOWN_CLIENT, new sim_mob::BasicMessageFactory()));

		//note that both client types refer to the same message factory belonging to roadrunner application. we will modify this to a more generic approach later-vahid
//		messageFactories.insert(std::make_pair(comm::ANDROID_EMULATOR, android_factory));
//		messageFactories.insert(std::make_pair(comm::NS3_SIMULATOR, ns3_factory));

		//We assume the "UNKNOWN" type knows about connection messages.
//		boost::shared_ptr<sim_mob::MessageFactory<std::vector<sim_mob::comm::MsgPtr>, std::string> > basic_factory(new sim_mob::BasicMessageFactory());
//		messageFactories.insert(std::make_pair(comm::UNKNOWN_CLIENT, basic_factory));
	} else if (client_mode == "android-only") {
		//boost::shared_ptr<sim_mob::MessageFactory<std::vector<sim_mob::comm::MsgPtr>, std::string> > android_factory(new sim_mob::roadrunner::RoadRunnerFactory(false));

		messageFactories.insert(std::make_pair(comm::ANDROID_EMULATOR, new sim_mob::roadrunner::RoadRunnerFactory(false)));
	//	messageFactories.insert(std::make_pair(comm::UNKNOWN_CLIENT, new sim_mob::BasicMessageFactory()));

		//note that both client types refer to the same message factory belonging to roadrunner application. we will modify this to a more generic approach later-vahid
		//messageFactories.insert(std::make_pair(comm::ANDROID_EMULATOR, android_factory));

		//We assume the "UNKNOWN" type knows about connection messages.
		//boost::shared_ptr<sim_mob::MessageFactory<std::vector<sim_mob::comm::MsgPtr>, std::string> >basic_factory(new sim_mob::BasicMessageFactory());
		//messageFactories.insert(std::make_pair(comm::UNKNOWN_CLIENT, basic_factory));
	}

	// wait for connection criteria for this broker
	clientBlockers.insert(std::make_pair(comm::ANDROID_EMULATOR,
			boost::shared_ptr<WaitForAndroidConnection>(new
					WaitForAndroidConnection(*this,MIN_CLIENTS))));


	if(client_mode == "android-ns3") {
		clientBlockers.insert(std::make_pair(comm::NS3_SIMULATOR,
				boost::shared_ptr<WaitForNS3Connection>(new WaitForNS3Connection(*this))));

	}

	// wait for connection criteria for this broker
	agentBlockers.insert(std::make_pair(0,
			boost::shared_ptr<WaitForAgentRegistration>(new WaitForAgentRegistration(*this,MIN_AGENTS))));

}


sim_mob::Entity::UpdateStatus sim_mob::Roadrunner_Broker::update(timeslice now) {
	if (EnableDebugOutput) {
		Print() << "Broker tick:" << now.frame() << std::endl;
	}

	//step-1 : Create/start the thread if this is the first frame.
	//TODO: transfer this to frame_init
	if (now.frame() == 0) {
		connection->start(); //Not profiled; this only happens once.
	}

	if (EnableDebugOutput) {
		Print() << "=====================ConnectionStarted =======================================\n";
	}

	PROFILE_LOG_COMMSIM_UPDATE_BEGIN(currWorkerProvider, this, now);

	//Step-2: Ensure that we have enough clients to process
	//(in terms of client type (like ns3, android emulator, etc) and quantity(like enough number of android clients) ).
	//Block the simulation here(if you have to)
	waitAndAcceptConnections();

	if (EnableDebugOutput) {
		Print() << "===================== wait Done =======================================\n";
	}

	//step-3: Process what has been received in your receive container(message queue perhaps)
	size_t numAgents = getNumConnectedAgents();
	processIncomingData(now);
	if (EnableDebugOutput) {
		Print() << "===================== processIncomingData Done =======================================\n";
	}

	//step-4: if need be, wait for all agents(or others)
	//to complete their tick so that you are the last one ticking)
	PROFILE_LOG_COMMSIM_LOCAL_COMPUTE_BEGIN(currWorkerProvider, this, now, numAgents);
	waitForAgentsUpdates();
	PROFILE_LOG_COMMSIM_LOCAL_COMPUTE_END(currWorkerProvider, this, now, numAgents);

	if (EnableDebugOutput) {
		Print() << "===================== waitForAgentsUpdates Done =======================================\n";
	}

	//step-5: signal the publishers to publish their data
	processPublishers(now);
	if (EnableDebugOutput) {
		Print() << "===================== processPublishers Done =======================================\n";
	}

//	step-5.5:for each client, append a message at the end of all messages saying Broker is ready to receive your messages
	sendReadyToReceive();

	//This may have changed (or we should at least log if it did).
	numAgents = getNumConnectedAgents();

	//step-6: Now send all what has been prepared, by different sources, to their corresponding destications(clients)
	PROFILE_LOG_COMMSIM_MIXED_COMPUTE_BEGIN(currWorkerProvider, this, now, numAgents);
	processOutgoingData(now);
	PROFILE_LOG_COMMSIM_MIXED_COMPUTE_END(currWorkerProvider, this, now, numAgents);
	if (EnableDebugOutput) {
		Print() << "===================== processOutgoingData Done =======================================\n";
	}

	//step-7:
	//the clients will now send whatever they want to send(into the incoming messagequeue)
	//followed by a Done! message.That is when Broker can go forwardClientList::pair clientByType;
	PROFILE_LOG_COMMSIM_ANDROID_COMPUTE_BEGIN(currWorkerProvider, this, now);
	waitForClientsDone();
	PROFILE_LOG_COMMSIM_ANDROID_COMPUTE_END(currWorkerProvider, this, now);
	if (EnableDebugOutput) {
		Print() << "===================== waitForClientsDone Done =======================================\n";
	}

	//step-8:
	//Now that all clients are done, set any properties on new clients.
	if (!newClientsWaitingOnRegionEnabling.empty()) {
		setNewClientProps();
		if (EnableDebugOutput) {
			Print() << "===================== setNewClientProps Done =======================================\n";
		}
	}

	//step-9: final steps that should be taken before leaving the tick
	//prepare for next tick.
	cleanup();

	PROFILE_LOG_COMMSIM_UPDATE_END(currWorkerProvider, this, now);

	return UpdateStatus(UpdateStatus::RS_CONTINUE);
}

void sim_mob::Roadrunner_Broker::onAgentUpdate(sim_mob::event::EventId id, sim_mob::event::Context context, sim_mob::event::EventPublisher* sender, const UpdateEventArgs& argums)
{
	Broker::onAgentUpdate(id,context,sender,argums);
}

void sim_mob::Roadrunner_Broker::onClientRegister(sim_mob::event::EventId id, sim_mob::event::Context context, sim_mob::event::EventPublisher* sender, const ClientRegistrationEventArgs& argums)
{
	Broker::onClientRegister(id,context,sender,argums);

	//Enable Region support if this client requested it.
//	if (regionSupportRequired) {
	boost::shared_ptr<ClientHandler> cnnHandler = argums.getClient();
	if(cnnHandler->getRequiredServices().find(sim_mob::Services::SIMMOB_SRV_REGIONS_AND_PATH) != cnnHandler->getRequiredServices().end()){
		pendClientToEnableRegions(cnnHandler);
	}
//	}

}


void sim_mob::Roadrunner_Broker::setNewClientProps()
{
	//NOTE: I am locking this anyway, just to be safe. (In case a new Broker request arrives in the meantime). ~Seth
	boost::unique_lock<boost::mutex> lock(mutex_clientList);

	//Now, loop through each client and send it a message to inform that the Broker has registered it.
	for (std::set< boost::weak_ptr<sim_mob::ClientHandler> >::iterator it=newClientsWaitingOnRegionEnabling.begin(); it!=newClientsWaitingOnRegionEnabling.end(); it++) {
		//Attempt to resolve the weak pointer.
		boost::shared_ptr<sim_mob::ClientHandler> cHand = it->lock();
		if (cHand) {
			//NOTE: Be CAREFUL here using the Agent pointer as a Context (void*). If you have a class with multiple inheritance,
			//      the void* for different realizations of the same object may have DIFFERENT pointer values. ~Seth
			messaging::MessageBus::PublishEvent(sim_mob::event::EVT_CORE_COMMSIM_ENABLED_FOR_AGENT,
				cHand->agent,
				messaging::MessageBus::EventArgsPtr(new event::EventArgs())
			);
		} else {
			Warn() <<"Broker::setNewClientProps() -- Client was destroyed before its weak_ptr() could be resolved.\n";
		}
	}

	//These clients have been processed.
	newClientsWaitingOnRegionEnabling.clear();
}

void sim_mob::Roadrunner_Broker::pendClientToEnableRegions(boost::shared_ptr<sim_mob::ClientHandler> &clientHandler)
{
	boost::unique_lock<boost::mutex> lock(mutex_clientList);
	newClientsWaitingOnRegionEnabling.insert(clientHandler);
}
