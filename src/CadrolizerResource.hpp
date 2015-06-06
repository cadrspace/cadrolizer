/* CadrolizerResource.hpp -- Cadrolizer IoTivity resource.
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

#ifndef __CADROLIZER_RESOURCE_HPP__
#define __CADROLIZER_RESOURCE_HPP__

#include <sstream>
#include <iostream>
#include <syslog.h>

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
        bool m_isShutdownAllowed;

	// IoTivity-specific:
	OCRepresentation m_cadrolizerRep;
	OCResourceHandle m_cadrolizerHandle;

public:
	CadrolizerResource()
		: m_name("cadrolizer"),
		  m_typeName("core.cadrolizer"),
		  m_cadrolizerUri("/a/cadrolizer"),
                  m_isShutdownAllowed(false) {
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

        /**
         * Set remote shutdown permission.
         */
        void setShutdownPermission(bool isEnabled) {
                m_isShutdownAllowed = isEnabled;
        }

        OCResourceHandle getHandle() {
                return m_cadrolizerHandle;
        }

        /**
         * Handle a PUT request.
         *
         * @param rep A resource representation.
         */
        void put(OCRepresentation& rep) {
                string state;
                try {
                        if (rep.getValue("state", state)) {
                                ostringstream os;
                                os << "State: " << state << endl;
                                syslog(LOG_INFO, os.str().c_str());
                        }
                } catch (exception& e) {
                        cout << e.what() << endl;
                }
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
        OCStackResult handlePut(shared_ptr<OCResourceRequest> pRequest);
	OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request);
};

#endif /* ifndef __CADROLIZER_RESOURCE_HPP__ */

/* CadrolizerResource.hpp ends here. */
