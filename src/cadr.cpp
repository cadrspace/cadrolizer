/* cadr.cpp -- Cadrolizer management tool.
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

#include <cstdlib>

#include <mutex>
#include <condition_variable>
#include <boost/algorithm/string.hpp>

#include "OCPlatform.h"
#include "OCApi.h"

// This header should be included after IoTivity headers, otherwise the
// program won't compile.
#include <boost/program_options.hpp>

#include "common.hpp"

using namespace std;
using namespace OC;
namespace po = boost::program_options;
namespace PH = std::placeholders;

const int SUCCESS_RESPONSE = 0;

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
 * Get the uptime of a machine.  Return the uptime value (in days) as
 * a string.
 */
string get_uptime(const OCRepresentation &rep)
{
        double uptime = atof(rep.getValue<string>("uptime").c_str());
        return to_string(uptime / 60 / 60 / 24);
}

/**
 * Replace all occurences of a string.
 *
 * @param str A string in which occurences should be replaced.
 * @param from A string that should be replaced.
 * @param to A string for substitution.
 */
void replace_all_x(std::string&  str,
                   const string& from,
                   const string& to)
{
        if (from.empty())
                return;

        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != string::npos) {
                str.replace(start_pos, from.length(), to);
                // In case 'to' contains 'from', like replacing 'x' with 'yx'
                start_pos += to.length();
        }
}

void print_list(const string list)
{
        vector<string> v;
        boost::split(v, list, boost::is_any_of("\t "));

        for (vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
                cout << "  "
                     << *it
                     << endl;
        }
}

/* Print information about the found resource. */
void onGet(const string               id,
           const HeaderOptions        &headerOptions,
           const OCRepresentation     &rep,
           const int                  eCode)
{
        string hostname;

        DEBUG("onGet: Called\n");
        if (! (eCode == SUCCESS_RESPONSE))
                return;

        cout << "ID: " << id << endl;

        if (rep.hasAttribute("name")) {
                string name = rep.getValue<string>("name");
                DEBUG("onGet: name: %s\n", name.c_str());
                DEBUG("onGet: hasAttribute\n");
        }

        if (rep.hasAttribute("hostname")) {
                hostname = rep.getValue<string>("hostname");
                cout << "Hostname:     "
                     << hostname
                     << endl;
        }

        if (rep.hasAttribute("description")) {
                cout << "Description:  "
                     << rep.getValue<string>("description")
                     << endl;
        }

        if (rep.hasAttribute("state")) {
                cout << "State:        "
                     << rep.getValue<string>("state")
                     << endl;;
        }

        if (rep.hasAttribute("uptime")) {
                cout << "Uptime:       "
                     << get_uptime(rep) << " day(s)"
                     << endl;
        }

        if (rep.hasAttribute("ip-address")) {
                cout << "IP Address:" << endl;
                print_list(rep.getValue<string>("ip-address"));
        }

        if (rep.hasAttribute("services")) {
                string services = rep.getValue<string>("services");
                if (rep.hasAttribute("hostname"))
                        replace_all_x(services, "<host>", hostname);
                cout << "Services:" << endl;
                print_list(services);
        }

        if (rep.hasAttribute("shutdown-allowed?")) {
                cout << "Shutdown allowed?: "
                     << rep.getValue<string>("shutdown-allowed?")
                     << endl;
        }

        cout << "--" << endl;
}

void foundResource(shared_ptr<OCResource> resource)
{
        string resourceURI;
        string hostAddress;

        if (! resource) {
                cout << "Resource is invalid" << endl;
                return;
        }

        try {
                resourceURI = resource->uri();
                hostAddress = resource->host();

                DEBUG("Discovered resource:\n");
                DEBUG("\tURI:  %s\n", resourceURI.c_str());
                DEBUG("\tHost: %s\n", hostAddress.c_str());
                DEBUG("List of resource types:\n");
                if (debug) {
                        for (auto &type : resource->getResourceTypes())
                                DEBUG("%s\t", type.c_str());
                        DEBUG("List of resource interfaces:\n");
                        for (auto &interface : resource->getResourceInterfaces())
                                DEBUG("%s\t", interface.c_str());
                }
        } catch (exception &e) {
                // TODO: Handle errors.
                cout << e.what() << endl;
        }



        // Make GET request:
        QueryParamsMap test;
        ostringstream id;
        id << resource->uniqueIdentifier();
        GetCallback cb = bind(onGet, id.str(), PH::_1, PH::_2, PH::_3);
        resource->get(test, cb);
}

enum shutdown_action  {
        SHUTDOWN,
        REBOOT
};

void shutdown_resource(shared_ptr<OCResource> resource,
                       enum shutdown_action   action)
{
        if (! resource) {
                cout << "Resource is invalid" << endl;
                return;
        }

        try {
                OCRepresentation rep;
                string state;

                switch(action) {
                case SHUTDOWN:
                        state  = "shutdown";
                        break;
                case REBOOT:
                        state  = "reboot";
                        break;
                default:
                        throw "Unknown action";
                };

                rep.setValue("state", state);

                QueryParamsMap query_params_map;

                PutCallback on_put = [](const HeaderOptions&    headerOptions,
                                        const OCRepresentation& rep,
                                        const int               eCode) -> void {
                        if (eCode != SUCCESS_RESPONSE)
                                throw "Could not put the representation";
                };

                resource->put(rep, query_params_map, on_put);
        } catch (exception &e) {
                cout << e.what() << endl;
        }
}

const string make_discovery_uri(const string rt)
{
        ostringstream request_uri;
        request_uri << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << rt;
        return request_uri.str();
}

static const string CADROLIZER_URI = make_discovery_uri("core.cadrolizer");

void find_resource(const OC::FindCallback cb)
{
        DEBUG("CADROLIZER_URI: %s\n", CADROLIZER_URI.c_str());
        try {
                OCPlatform::findResource(
                        "",
                        CADROLIZER_URI,
                        CT_DEFAULT,
                        cb);
        } catch (OCException &e) {
                // TODO: Handle errors.
                cout << e.what() << endl;
        }
}

/* Locate cadrolizers. */
void locate() {
        find_resource(foundResource);
        stop();
}

void shutdown(enum shutdown_action action) {
        OC::FindCallback cb = bind(shutdown_resource, PH::_1, action);
        find_resource(cb);
        stop();
}

/**
 * Configure IoTivity platform.
 */
void configure_platform()
{
        PlatformConfig cfg {
                ServiceType::InProc,
                ModeType::Client,
                "0.0.0.0",
                0,
                QualityOfService::LowQos
        };

        OCPlatform::Configure(cfg);
}

/* Entry point */
int main(int argc, char* argv[])
{
        po::options_description desc("Options");
        desc.add_options()
                ("help,h", "Display this help message")
                ("locate,l", "Locate cadrolizers around.")
                ("shutdown", "Shutdown hosts")
                ("reboot", "Reboot hosts")
                ("debug", "Enable debug mode");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
                cout << desc << endl;
                exit(0);
        }

        if (vm.count("debug")) {
                debug = true;
        }

        if (vm.count("shutdown")) {
                configure_platform();
                shutdown(SHUTDOWN);
        }

        if (vm.count("reboot")) {
                configure_platform();
                shutdown(REBOOT);
        }

        if (vm.count("locate")) {
                configure_platform();
                locate();
        }

        return 0;
}

/* cadr.cpp ends here. */
