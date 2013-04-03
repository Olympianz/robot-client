#ifndef CLIENT_SOCKETS_CHONG_H_XXX
#define CLIENT_SOCKETS_CHONG_H_XXX

#include <vector>
#include "Aria.h"
#include "ConfigFileReader.h"

/* This class aims to monitor and log the traffic 
 * between robot servers and client by reading the 
 * traffic from corresponding socket port of each
 * robotic client
 */

class ClientSockets {
    public:
        ClientSockets(  std::vector<ArClientBase *>&, const std::vector<HostInfo>& );

        std::vector<long>& getData (std::vector<long>&);
        std::vector<long>  getData ();

        void printData ();

    private:
        std::vector<ArSocket *> v_socks; 
        const std::vector<ArClientBase *>& v_clients;
        const std::vector<HostInfo>& hostsinfo; 
};


#endif
