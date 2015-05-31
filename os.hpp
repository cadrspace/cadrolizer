// os.hpp -- OS related utils (header).

#ifndef __OS_HPP__
#define __OS_HPP__

using namespace std;

namespace OS {
        time_t getUptime();
        string getHostname();
        string getIpAddress();
}

#endif  /* ifndef __OS_HPP__ */

// os.hpp ends here
