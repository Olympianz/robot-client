#include <iostream>
#include <vector>

#include "ClientSockets.h"
#include "Aria.h"

/* This class aims to monitor and log the traffic 
 * between robot servers and client by reading the 
 * traffic from corresponding socket port of each
 * robotic client
 */

using std::vector;      

ClientSockets::ClientSockets (  std::vector<ArClientBase *>& clientBases,
                                std::vector<Arsocket *>& clSockets)
                                : v_clients(clientBases), v_socks(clSockets){}

void ClientSockets::getSockets(vector<ArClientBase *>& clients){
    typedef vector<ArclientBase *>::size_type Arcb_size;
    
    ArSocket* socket = NULL;

    for ( Arcb_size i = 0; i != clients.size(); ++i){
        socket = clients[i].getUdpSocket();
        v_socks.push_back(socket);
    }
}

vector<long>& ClientSockets::getData(vector<long>& trafData){
    typedef vector<ArSocket *>::size_type sock_sz;

    for ( sock_sz i = 0; i != v_sockes.size(); ++i)
        trafData.push_back(v_sockes[i]->getBytesRecvd());

    return trafData;
}


    







