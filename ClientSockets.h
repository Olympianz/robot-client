#ifndef CLIENT_SOCKETS_CHONG_H_XXX
#define CLIENT_SOCKETS_CHONG_H_XXX

#include <vector>
#include "Aria.h"

/* This class aims to monitor and log the traffic 
 * between robot servers and client by reading the 
 * traffic from corresponding socket port of each
 * robotic client
 */

class ClientSockets {
    public:
        ClientSockets(  std::vector<ArClientBase *>& clientBases, 
                        std::vector<Arsocket *>& clSockets = NULL);
        std::vector<long>& getData (std::vector<long>&);   
    private:
        std::vector<ArClientBase *> v_clients;
        std::vector<ArSocket *> v_socks;
        void getSockets (std::vector<ArClientBase *>&);
};







#endif
