#include <vector>
#include <iostream>
#include "ArNetworking.h"
#include "ConfigFileReader.h"
#include "ClientSocketUnit.h"

using std::vector;      using std::cout;

ClientSocketUnit::ClientSocketUnit(vector<ArClientBase *> &client, const vector<HostInfo> &hInfo)
                                    :clients(client), hostInfo(hInfo) { }

const char* ClientSocketUnit::getRobName(ArClientBase *client, const HostInfo& hInfo){
    char nameBuffer[100];
    sprintf(nameBuffer, "robot_%s", hInfo.ip);
    const char * robotName = strdup(nameBuffer);
    client->setRobotName(robotName);
    return client->getRobotName();
}
    
const char* ClientSocketUnit::getRobName(unsigned long index){
    if (index > clients.size() - 1){
        cout << "Your input index is out of range, please try again!";
        return NULL;
    }
    else {
    return getRobName(clients[index], hostInfo[index]);
    }
}

unsigned long ClientSocketUnit::size(){
    return clients.size();
}

ArClientBase* ClientSocketUnit::getClient(unsigned long index){
    if (index > clients.size() -1){
        cout << "Your input index is out of range, please try again!";
        return NULL;
    }
    else {
        return clients[index];
    }
}    
