#include <functional>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "OCPlatform.h"
#include "OCApi.h"

#include <boost/program_options.hpp>

#include "CadrolizerResource.hpp"

// For "wait" implementation:
#include <mutex>
#include <condition_variable>

using namespace OC;
using namespace std;
namespace PH = std::placeholders;
namespace po = boost::program_options;

const string PROGRAM_NAME = "cadrolizer";

/** Path to the configuration file. */
const string CONFIG_FILE = "/etc/cadrolizer.conf";

// Taken from IoTivity examples:
// A condition variable will free the mutex it is given, then do a non-
// intensive block until 'notify' is called on it.  In this case, since we
// don't ever call cv.notify, this should be a non-processor intensive version
// of while(true);
void stop()
{
	mutex blocker;
	condition_variable cv;
	unique_lock<mutex> lock(blocker);
	cv.wait(lock);
}


/**
 * Write parameter NAME with VALUE to the configuration file.
 *
 * @param name Parameter name.
 * @param value Parameter value.
 */
void write_settings(const char* name, const char* value)
{
        ofstream settings_file(CONFIG_FILE);
        settings_file << name << " = " << value;
}

/**
 * Read application configuration from the file.
 */
void read_settings(po::options_description& desc,
                   po::variables_map&       vm)
{
        std::ifstream settings_file(CONFIG_FILE);

        // Clear the map.
        vm = po::variables_map();

        po::store(po::parse_config_file(settings_file , desc), vm);
        po::notify(vm);
}


void printHelp()
{
	cout << "Usage: " << PROGRAM_NAME << " [options]" << endl
	     << endl
             << "Options:" << endl
             << "  -d         Enable debug mode." << endl
             << "  -h         Print this message and exit." << endl;
}

// Entry point
int main(int argc, char* argv[])
{
        string services;
        po::options_description desc("Options");

        desc.add_options()
                ("services", po::value<string>(&services), "services");

        po::variables_map vm;
        read_settings(desc, vm);

        cout << services << endl;

	try {
		CadrolizerResource *cz = CadrolizerResource::getInstance();
                cz->setServices(services);

		stop();
	} catch (OCException e) {
		// TODO: Handle errors
	}
}
