#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <sstream>
#include <string>
#include <iostream>

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

        OCResourceHandle getHandle() {
                return m_cadrolizerHandle;
        }

        /**
         * Get the current machine uptime.
         */
        time_t getUptime() {
                double upsecs;
                time_t uptime;
                FILE *fp = fopen("/proc/uptime", "r");
                if (fp != NULL)
                {
                        char buf[BUFSIZ];
                        int res;
                        char *b = fgets(buf, BUFSIZ, fp);
                        if (b == buf)
                        {
                                /* The following sscanf must use the C locale/ */
                                setlocale(LC_NUMERIC, "C");
                                res = sscanf(buf, "%lf", &upsecs);
                                setlocale(LC_NUMERIC, "");
                                if (res == 1)
                                        uptime = (time_t) upsecs;
                        }

                        fclose (fp);
                }

                return uptime;
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

        /**
         * Get current IP addresses for each interface of the machine.
         *
         * @return Comma-separated list of interfaces with their IP
         * addresses as a string.
         */
        string getIpAddress() {
                struct ifaddrs* ifAddrsStruct = NULL;
                struct ifaddrs* ifa = NULL;
                ostringstream os;
                auto ifaToStr = [](struct ifaddrs* ifa,
                                   int             type,
                                   int             len) -> string {
                        void* tmpAddrPtr = NULL;
                        struct sockaddr_in* saddr
                                = (struct sockaddr_in*) ifa->ifa_addr;
                        char addrBuf[len];
                        tmpAddrPtr = &(saddr)->sin_addr;
                        inet_ntop(type, tmpAddrPtr, addrBuf, len);
                        return string(addrBuf);
                };

                getifaddrs(&ifAddrsStruct);

                for (ifa = ifAddrsStruct; ifa != NULL; ifa = ifa->ifa_next) {
                        if (! ifa->ifa_addr)
                                continue;

                        if (ifa->ifa_addr->sa_family == AF_INET) {
                                os << ifa->ifa_name << ":"
                                   << ifaToStr(ifa, AF_INET, INET_ADDRSTRLEN);
                                if (ifa->ifa_next)
                                        os << ",";
                        } else if (ifa->ifa_addr->sa_family == AF_INET6) {
                                os << ifa->ifa_name << ":"
                                   << ifaToStr(ifa, AF_INET6, INET6_ADDRSTRLEN);
                                if (ifa->ifa_next)
                                        os << ",";
                        }

                }

                return os.str();
        }

        OCRepresentation get() {
                string hostname = getHostname();
                string uptime   = to_string(getUptime());
                string ipaddr   = getIpAddress();
                m_cadrolizerRep.setValue("hostname",   string(hostname));
                m_cadrolizerRep.setValue("uptime",     uptime);
                m_cadrolizerRep.setValue("ip-address", ipaddr);
                return m_cadrolizerRep;
        }

	OCStackResult handleGet(shared_ptr<OCResourceRequest> pRequest);
	OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request);
};

// HackThing.hpp ends here
