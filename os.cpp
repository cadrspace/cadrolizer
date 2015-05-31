// os.cpp -- OS related utils.

#include <unistd.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <sstream>
#include <string>
#include <iostream>

#include "os.hpp"

using namespace std;

/**
 * Get the current machine uptime.
 */
time_t OS::getUptime() {
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
string OS::getHostname() {
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
string OS::getIpAddress() {
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

// os.cpp ends here
