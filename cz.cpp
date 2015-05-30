#include <cstdlib>

#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"

#include "common.hpp"

using namespace std;
using namespace OC;

const int SUCCESS_RESPONSE = 0;
const string PROGRAM_NAME = "cz";

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
string getUptime(const OCRepresentation &rep)
{
        double uptime = atof(rep.getValue<string>("uptime").c_str());
        return to_string(uptime / 60 / 60 / 24);
}

/* Print information about the found resource. */
void onGet(const HeaderOptions     &headerOptions,
           const OCRepresentation  &rep,
           const int               eCode)
{
	DEBUG("onGet: Called\n");
        if (! (eCode == SUCCESS_RESPONSE))
                return;

        if (rep.hasAttribute("name")) {
		string name = rep.getValue<string>("name");
		DEBUG("onGet: name: %s\n", name.c_str());
		DEBUG("onGet: hasAttribute\n");
        }

	if (rep.hasAttribute("hostname")) {
		cout << "Hostname: " << rep.getValue<string>("hostname")
		     << endl;
	}

        if (rep.hasAttribute("uptime")) {
                cout << "Uptime:   " << getUptime(rep) << " day(s)"
                     << endl;
        }
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
        resource->get(test, &onGet);
}

void printHelp()
{
        cout << "Usage: " << PROGRAM_NAME << " [options]" << endl
             << endl
             << "Options:" << endl
	     << "  -l         List cadrolizers." << endl
	     << "  -d         Enable debug mode." << endl
             << "  -h         Print this message and exit." << endl;
}

/* List cadrolizers. */
void list() {
	try {
		OCPlatform::findResource(
			"",
			"coap://224.0.1.187/oc/core?rt=core.cadrolizer",
			&foundResource);
	} catch (OCException &e) {
                // TODO: Handle errors.
                cout << e.what() << endl;
        }

	stop();
}

/* Entry point */
int main(int argc, char* argv[])
{
	int opt;

        PlatformConfig cfg {
                ServiceType::InProc,
                ModeType::Client,
                "0.0.0.0",
                0,
                QualityOfService::LowQos
        };

        OCPlatform::Configure(cfg);

        while ((opt = getopt(argc, argv, "dhl")) > 0) {
                switch (opt) {
		case 'l':
			list();
			break;
                case 'd':
                        debug = true;
                        break;
                case 'h':
                        printHelp();
                        exit (0);
                        break;  // Never reached
                };
        }

        return 0;
}
