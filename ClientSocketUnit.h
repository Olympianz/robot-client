#ifndef CLIENT_SOCKET_DATA_CHONG_H_XXX
#define CLIENT_SOCKET_DATA_CHONG_H_XXX

#include <vector>
#include "ArNetworking.h"

/* This class defines a data structure storing 
 * HostInfo and Robotic Client. The client and 
 * corresponding HostInfo are those who successfully 
 * connect to servers
 */  

// forward declaration
struct HostInfo;

class ClientSocketUnit {
    public:
        ClientSocketUnit(std::vector<ArClientBase*>& , std::vector<HostInfo>);
        ClientSocketUnit(ArClientBase*, std::vector<HostInfo>);
        typedef std::vector<ArClientBase *>::size_type CSU_size;

        /* this function takes a client and one hostInfo as input, using hostInfo 
         * construct a robotname, then pass it to client. So, we can call robotName
         * through client->getRobotName
         */
        const char* getRobName(ArClientBase*, const HostInfo&);
        const char* getRobName(unsigned long); //return robot name for each input index
        unsigned long size();
        ArClientBase* getClient(unsigned long);

    private:
        std::vector<ArClientBase *> clients;
        std::vector<HostInfo> hostInfo;
        //std::vector<HostInfo > hostInfo;        
};

#endif
