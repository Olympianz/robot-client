#include "SuperClient.h"
#include <iostream>
#include <fstream>

using namespace std;

long SuperClient::dataSent(){
}

long SuperClient::dataReceived(){
}

void SuperClient::receivedDataToFile(bool plot){
  myDataMutex.lock();
  myPacketTrackingMutex.lock();
  std::map<unsigned int, Tracker *>::iterator it;

  unsigned int command;
  Tracker *tracker = NULL;
  long seconds;

  seconds = myTrackingStarted.secSince();
  if (seconds == 0)
    seconds = 1;

  const char *namePtr;
  char name[512];

  long packetsReceivedTcp = 0;
  long bytesReceivedTcp = 0;
  long packetsReceivedUdp = 0;
  long bytesReceivedUdp = 0;

  for (it = myTrackingReceivedMap.begin(); it != myTrackingReceivedMap.end(); it++)
    {
      command = (*it).first;
      tracker = (*it).second;

      packetsReceivedTcp += tracker->myPacketsTcp;
      bytesReceivedTcp += tracker->myBytesTcp;
      packetsReceivedUdp += tracker->myPacketsUdp;
      bytesReceivedUdp += tracker->myBytesUdp;

      if ((namePtr = getName(command, true)) != NULL && namePtr[0] != '\0')
	snprintf(name, sizeof(name), "%s", namePtr);
      // if we're command 255 or less and there's no name its probably
      // one of the server commands we don't really need to track
      else if (command <= 255)
	continue;
      // we should know what the name of everything other then the
      // server command is, but print if we don't. just in case
      else
	snprintf(name, sizeof(name), "#%d", command);
    }

  ofstream logfile;
  logfile.open ("DataLog.txt", ios::app);
  if (plot){
    logfile << seconds;
    logfile << " ";
    logfile << bytesReceivedTcp + bytesReceivedUdp;
    logfile << endl;
  }

  else {
    logfile << "Data is recorded " << seconds << " seconds after activation from " << myLogPrefix.c_str(); 
    logfile << '\n' <<  "Packets received: " << packetsReceivedTcp + packetsReceivedUdp << ", ";
    logfile << "data received: " << bytesReceivedTcp + bytesReceivedUdp << " bytes.";
    logfile << '\n' << "Average data receiving rate is " << (bytesReceivedTcp + bytesReceivedUdp)/seconds << " B/sec.";
  }

  long packetsSentTcp = 0;
  long bytesSentTcp = 0;
  long packetsSentUdp = 0;
  long bytesSentUdp = 0;

  for (it = myTrackingSentMap.begin(); it != myTrackingSentMap.end(); it++)
    {
      command = (*it).first;
      tracker = (*it).second;

      packetsSentTcp += tracker->myPacketsTcp;
      bytesSentTcp += tracker->myBytesTcp;
      packetsSentUdp += tracker->myPacketsUdp;
      bytesSentUdp += tracker->myBytesUdp;

      if ((namePtr = getName(command, true)) != NULL && namePtr[0] != '\0')
	snprintf(name, sizeof(name), "%s", namePtr);
      // if we're command 255 or less and there's no name its probably
      // one of the server commands we don't really need to track
      else if (command <= 255)
	continue;
      // we should know what the name of everything other then the
      // server command is, but print if we don't. just in case
      else
	snprintf(name, sizeof(name), "#%d", command);
    }

  if (!plot){
    logfile << '\n' <<  "Packets sent: " << packetsSentTcp + packetsSentUdp << ", ";
    logfile << "data sent: " << bytesSentTcp + bytesSentUdp << " bytes.";
    logfile << '\n' << "Average data sending rate is " << (bytesSentTcp + bytesSentUdp)/seconds << " B/sec.";
    logfile << '\n' << '\n';
  }
  logfile.close();
     
  myPacketTrackingMutex.unlock();
  myDataMutex.unlock();
}
