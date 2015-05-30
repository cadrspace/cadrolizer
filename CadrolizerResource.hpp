#include "OCPlatform.h"
#include "OCApi.h"

using namespace std;
using namespace OC;
namespace PH = std::placeholders;

class CadrolizerResource
{
public:
	string m_name;
	string m_typeName;
	string m_cadrolizerUri;

	string m_machineName;

	// IoTivity-specific:
	OCRepresentation m_cadrolizerRep;
	OCResourceHandle m_cadrolizerHandle;

public:
	CadrolizerResource()
		: m_name("cadrolizer"),
		  m_typeName("core.cadrolizer"),
		  m_cadrolizerUri("/a/cadrolizer") {
		m_cadrolizerRep.setUri(m_cadrolizerUri);
		m_cadrolizerRep.setValue("name", m_name);
		m_cadrolizerRep.setValue("hostname", "test");

		string resourceInterface = DEFAULT_INTERFACE;
		uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
		EntityHandler cb = bind(&CadrolizerResource::entityHandler,  this, PH::_1);
		OCStackResult result = OCPlatform::registerResource(
			m_cadrolizerHandle,
			m_cadrolizerUri,
			m_typeName,
			resourceInterface,
			cb,
			resourceProperty);

		if (result != OC_STACK_OK)
			cout << "Could not create the resource";
	}

	/* Get a CadrolizerResource instance. */
        static CadrolizerResource *getInstance() {
                static CadrolizerResource *instance = new CadrolizerResource();
                return instance;
        }

        OCResourceHandle getHandle() {
                return m_cadrolizerHandle;
        }

        /**
         * Get the current hostname.  Return the hostname as a
         * string.
         */
        string getHostname() {
                const size_t HOSTNAME_SZ = 255;
		char  hostname[HOSTNAME_SZ];

                int res = gethostname(hostname, HOSTNAME_SZ);
                if (res != 0)
                        throw "Could not get the hostname";

                return string(hostname);
        }

        OCRepresentation get() {
                string hostname = getHostname();
		m_cadrolizerRep.setValue("hostname", string(hostname));
                return m_cadrolizerRep;
        }

	OCStackResult handleGet(shared_ptr<OCResourceRequest> pRequest);
	OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request);
};

// HackThing.hpp ends here
