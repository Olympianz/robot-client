#ifndef CLIENT_SOCKETS_CHONG_H_XXX
#define CLIENT_SOCKETS_CHONG_H_XXX

#include <vector>
#include "ArNetworking.h"
#include "Aria.h"
#include "ConfigFileReader.h"
#include "ClientSocketUnit.h"

/* This class aims to monitor and log the traffic 
 * between robot servers and client by reading the 
 * traffic from corresponding socket port of each
 * robotic client
 */

class ClientSockets {
    public:
        typedef std::vector<ArSocket *>::size_type sock_sz;
        
        ClientSockets ( ClientSocketUnit& );

        std::vector<long>& getData (std::vector<long>&);
        std::vector<long>  getData ();

        void printData ();

    private:
        std::vector<ArSocket *> socks; 
        ClientSocketUnit &units; 
};


#endif
