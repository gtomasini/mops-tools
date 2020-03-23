#pragma once
#include <pocogger/logger_file.h>
#include <pocogger/poco_logger.h>

#include "avoidance_idl/avoidanceTypeSupportImpl.h"
#include "daa_datatypes_icd/IcdAvoidanceOutput.h"
#include "swim_opendds/subscription.h"

#include "avoidance_suscriber/avoidance_path.h"
#include "tp_server/mailslotServer.h"
#include "daa_datatypes_icd/IcdFlightPlan.h"
#include "avoidance_path_2_xml.h"

/*
This app writes to both mailSlots: TPServer and Remixer
So it must connect to both at the beginning
*/
using namespace swim;

class avoidance_subscriber : public opendds::reader::on_listener<daa_dds::avoidance_fatpaths_buffer>    {
	const int _BUFFER_SIZE = 1024;//mail slot

	void on_data_available(daa_dds::avoidance_fatpaths_buffer const&) override;
	auto to_avoidance_path(IcdAvoidanceOutput::FatPathsAndWayPoints const&) const->avoidance_path;
	poco::logger_file _logger;

	opendds::subscriber<daa_dds::avoidance_fatpaths_buffer> _subscriber;

	//udp server (assesment OPP)
	std::string _udp_server;
	int _udp_port;

	HANDLE _hTPserverMailslot = INVALID_HANDLE_VALUE;
	int send2TPserver(const std::string &msg);

	xmlParameters _xmlPar;
	
public:
    using entity_t = swim::opendds::entity;
    using configurator_t = poco::configurator;

    avoidance_subscriber() = default;
    virtual ~avoidance_subscriber();

    int initialize(configurator_t const&, poco::logger_file &log);
	int createTPserverMailSlot();
	    void run();

private:

};
