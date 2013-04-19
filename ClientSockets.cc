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
        udpSockets.push_back( CSunits.getClient(i)->getUdpSocket() );
	tcpSockets.push_back( CSunits.getClient(i)->getTcpSocket() );
    }
}

vector<long>& ClientSockets::getData(vector<long>& trafData){
  //  bool udpIsConnected = updConnected();

  for ( sock_sz i = 0; i != size(); ++i){
    if( udpConnected() ) {
        trafData.push_back(udpSockets[i]->getBytesRecvd());
    }
    else {
      trafData.push_back(tcpSockets[i]->getBytesRecvd());
    }
  }
   return trafData;
}

vector<long> ClientSockets::getData() {
    vector<long> temp;
    
    for ( sock_sz i = 0; i != size(); ++i)
      if ( udpConnected() )
        temp.push_back( udpSockets[i]->getBytesRecvd() );
      else 
	temp.push_back( tcpSockets[i]->getBytesRecvd() );
    return temp;
}

// need to define a data class for storing robot name and data
void ClientSockets::printData () {
    vector<long> data = getData();
    typedef vector<long>::size_type lsize;

    for ( lsize i = 0; i != data.size(); ++i){
        cout << "Data receivied from " << units.getRobName(i) << " is " << data[i] << "Bytes" << endl;
    }
}
    
// return the size of ClientSockets, which could be either upd or tcp
 ClientSockets::sock_sz ClientSockets::size () {
   ClientSockets::sock_sz udpSize = udpSockets.size();
   ClientSockets::sock_sz tcpSize = tcpSockets.size();
   ClientSockets::sock_sz bigSize = udpSize > tcpSize ? udpSize : tcpSize;
   return bigSize;
 }

 // check if it is udpconnected or tcpconnected
 bool ClientSockets::udpConnected() {
   bool udpConnected;
   ClientSockets::sock_sz bigSize = size();
   if ( bigSize  == udpSockets.size())
     udpConnected = true;
   else 
     udpConnected = false;
   
   return udpConnected;
 }







