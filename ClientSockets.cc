#include <iostream>
#include <vector>

#include "ArNetworking.h"
#include "Aria.h"
#include "ClientSockets.h"
#include "ClientSocketUnit.h"

/* This class aims to monitor and log the traffic 
 * between robot servers and client by reading the 
 * traffic from corresponding socket port of each
 * robotic client
 */

using std::vector;      using std::cout;
using std::endl;

ClientSockets::ClientSockets ( ClientSocketUnit &CSunits ):units(CSunits){
    typedef ClientSocketUnit::CSU_size unit_size;

    for (unit_size i = 0; i != CSunits.size(); ++i){
        socks.push_back( CSunits.getClient(i)->getUdpSocket() );
    }
}

vector<long>& ClientSockets::getData(vector<long>& trafData){
    for ( sock_sz i = 0; i != socks.size(); ++i)
        trafData.push_back(socks[i]->getBytesRecvd());
    return trafData;
}

vector<long> ClientSockets::getData() {
    vector<long> temp;

    for ( sock_sz i = 0; i != socks.size(); ++i)
        temp.push_back(socks[i]->getBytesRecvd());
    return temp;
}

// need to define a data class for storing robot name and data
void ClientSockets::printData () {
    vector<long> data = getData();
    typedef vector<long>::size_type lsize;

    for ( lsize i = 0; i != data.size(); ++i){
        cout << "Data receivied from " << *(units.getRobotName(i)) << " is " << data[i] << "Bytes" << endl;
    }
}
    







