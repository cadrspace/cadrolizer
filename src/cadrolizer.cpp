/* cadrolizer.cpp -- Cadrolizer implementation.
 *
 * Copyright (C) 2015 Artyom V. Poptsov <poptsov.artyom@gmail.com>
 *
 * This file is part of Cadrolizer.
 *
 * Cadrolizer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cadrolizer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cadrolizer.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        string description;
        po::options_description desc("Options");

        desc.add_options()
                ("services", po::value<string>(&services), "services")
                ("description", po::value<string>(&description), "description");

        po::variables_map vm;
        read_settings(desc, vm);

	try {
		CadrolizerResource *cz = CadrolizerResource::getInstance();
                cz->setServices(services);
                cz->setDescription(description);

		stop();
	} catch (OCException e) {
		// TODO: Handle errors
	}
}

/* cadrolizer.cpp ends here. */
