#include <functional>
#include <cstdlib>

#include "OCPlatform.h"
#include "OCApi.h"

#include "CadrolizerResource.hpp"

// For "wait" implementation:
#include <mutex>
#include <condition_variable>

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

const string PROGRAM_NAME = "cadrolizer";

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
	try {
		CadrolizerResource *cz = CadrolizerResource::getInstance();
		stop();
	} catch (OCException e) {
		// TODO: Handle errors
	}
}
