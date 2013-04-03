#include <iostream>
#include <vector>

#include "ClientSockets.h"
#include "Aria.h"

/* This class aims to monitor and log the traffic 
 * between robot servers and client by reading the 
 * traffic from corresponding socket port of each
 * robotic client
 */

using std::vector;      using std::cout;
using std::endl;

ClientSockets::ClientSockets (  vector<ArClientBase *>& clientBases,
                                const vector<HostInfo>& hinfo) {
    hostsinfo = hinfo;

       typedef vector<ArClientBase *>::size_type clt_sz;

    for ( clt_sz i = 0; i != clientBases.size(); ++i){
        v_socks.push_back( clientBases[i].getUdpSocket() );
        v_clients.push_back( clientBases[i] );
    }
}

vector<long>& ClientSockets::getData(vector<long>& trafData){
    typedef vector<ArSocket *>::size_type sock_sz;

    for ( sock_sz i = 0; i != v_sockes.size(); ++i)
        trafData.push_back(v_sockes[i]->getBytesRecvd());
    return trafData;
}

vector<long> ClientSockets::getData() {
    typedef vector<ArSocket *>::size_type sock_sz;
    vector<long> temp;

    for ( sock_sz i = 0; i != v_sockes.size(); ++i)
        temp.push_back(v_sockes[i]->getBytesRecvd());
    return temp;
}

// need to define a data class for storing robot name and data
void ClientSockets::printData () {
    typedef vector<long>::size_type lsize;
    vector<long> temp = getData();

    for ( lsize i = 0; i != temp.size(); ++i){
         

}
    







