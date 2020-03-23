#include <pocogger/logger_file.h>
#include <pocogger/poco_logger.h>
#include <pococonf/configurator.h>
#include "avoidance_suscriber/avoidance_subscriber.h"

int main(int argc, char *argv[]){
    poco::logger_file _logger;
    avoidance_subscriber _subscriber;
    std::string a_config_file = ((argc < 2) ? "avoidance_suscriber.ini" : argv[1]);
    std::string a_log_level = ((argc < 3) ? "information" : argv[2]);

	if (argc < 3) {
		std::cout << "you can write: " << argv[0] << " configFile.ini logLevel" << std::endl;
	}
	try {
        _logger.set_name("avoidance_suscriber");
        _logger.purge_on_open();
        _logger.set_max_capacity(50/*Mb*/);
        _logger.set_standard_format(a_log_level);
        _logger.set_logger_level(a_log_level);
        _logger.write_log_start();

        logger_notice(Poco::format("Config file is %s and argc is %i", a_config_file, argc));

        poco::configurator _configurator;
        _configurator.load(a_config_file);
		_subscriber.initialize(_configurator, _logger);
        _subscriber.run();
    }
    catch (const Poco::Exception& ex)  {
        logger_error(Poco::format("Oh there was an exception in the avoidance subscriber: %s", ex.displayText()));
        return 1;
    }
    return 0;
}

