#ifndef __CADROLIZER_RESOURCE_HPP__
#define __CADROLIZER_RESOURCE_HPP__

#include "OCPlatform.h"
#include "OCApi.h"
#include "os.hpp"

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
        string m_services;
        string m_description;

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
		EntityHandler cb = bind(&CadrolizerResource::entityHandler,
                                        this, PH::_1);
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

        /**
         * Set services.
         */
        void setServices(const string& services) {
                m_services = services;
        }

        /**
         * Set the description.
         */
        void setDescription(const string& description) {
                m_description = description;
        }

        OCResourceHandle getHandle() {
                return m_cadrolizerHandle;
        }

        OCRepresentation get() {
                string hostname = OS::getHostname();
                string uptime   = to_string(OS::getUptime());
                string ipaddr   = OS::getIpAddress();
                m_cadrolizerRep.setValue("hostname",    string(hostname));
                m_cadrolizerRep.setValue("description", m_description);
                m_cadrolizerRep.setValue("uptime",      uptime);
                m_cadrolizerRep.setValue("ip-address",  ipaddr);
                m_cadrolizerRep.setValue("services",    m_services);
                return m_cadrolizerRep;
        }

	OCStackResult handleGet(shared_ptr<OCResourceRequest> pRequest);
	OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request);
};

#endif /* ifndef __CADROLIZER_RESOURCE_HPP__ */

// HackThing.hpp ends here

