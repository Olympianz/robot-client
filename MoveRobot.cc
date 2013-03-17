#include <iostream>
#include <iomanip>

#include "MoveRobot.h"
#include "helpers.h"

MoveHandler::MoveHandler(std::vector<ArClientBase *> &clients,
    std::vector<int> &keys, std::vector<std::string> &keysInfo)
  : myClients(clients), myKeys(keys), myKeysInfo(keysInfo),
    myClientIndex(0), myClient(myClients[0]),
    myTransRatio(0), myRotRatio(0), mySpeedLimit(80),
    myIsWandering(false), myIsSafe(true)
{
}



// Attach keypress handlers
MoveKeyBoardHandler::MoveKeyBoardHandler(
    std::vector<ArClientBase *> &clients,
    std::vector<int> &keys, std::vector<std::string> &keysInfo,
    ArKeyHandler *keyHandler)
  : MoveHandler(clients, keys, keysInfo), 
    myKeyHandler(keyHandler),
    myForwardFtr(this, &MoveKeyBoardHandler::forward),
    myBackwardFtr(this, &MoveKeyBoardHandler::backward),
    myTurnLeftFtr(this, &MoveKeyBoardHandler::turnLeft),
    myTurnRightFtr(this, &MoveKeyBoardHandler::turnRight),
    myWanderFtr(this, &MoveKeyBoardHandler::wander),
    myStopFtr(this, &MoveKeyBoardHandler::stop),
    myUnsafeFtr(this, &MoveKeyBoardHandler::unsafe),
    myNextRobotFtr(this, &MoveKeyBoardHandler::nextRobot),
    myPrevRobotFtr(this, &MoveKeyBoardHandler::prevRobot)
{
  myKeyHandler->addKeyHandler(myKeys[0], &myForwardFtr);
  myKeyHandler->addKeyHandler(myKeys[1], &myBackwardFtr);
  myKeyHandler->addKeyHandler(myKeys[2], &myTurnLeftFtr);
  myKeyHandler->addKeyHandler(myKeys[3], &myTurnRightFtr);
  myKeyHandler->addKeyHandler(myKeys[4], &myWanderFtr);
  myKeyHandler->addKeyHandler(myKeys[5], &myStopFtr);
  myKeyHandler->addKeyHandler(myKeys[6], &myUnsafeFtr);
  myKeyHandler->addKeyHandler(myKeys[7], &myNextRobotFtr);
  myKeyHandler->addKeyHandler(myKeys[8], &myPrevRobotFtr);

  displayKeys();
}

// For keyboard, the key press checking is done
// by the keyhandler so only send the movement
// values
void MoveKeyBoardHandler::update()
{
  if (!myClient->dataExists("ratioDrive") || myIsWandering) return;

  ArNetPacket packet;
  packet.doubleToBuf(myTransRatio);
  packet.doubleToBuf(myRotRatio);
  packet.doubleToBuf(mySpeedLimit);

  myClient->requestOnce("ratioDrive", &packet);
  myTransRatio = 0;
  myRotRatio = 0;
}

// print keys and description on command line
void MoveKeyBoardHandler::displayKeys()
{
}

// fix movement values
void MoveKeyBoardHandler::forward() { myTransRatio = 100; }
void MoveKeyBoardHandler::backward() { myTransRatio = -100; }
void MoveKeyBoardHandler::turnLeft() { myRotRatio = 100; }
void MoveKeyBoardHandler::turnRight() { myRotRatio = -100; }

// alternate between wander and normal mode
void MoveKeyBoardHandler::wander()
{
  if (!myClient->dataExists("wander")) return;

  myIsWandering = !myIsWandering;

  if (myIsWandering) {
    myClient->requestOnce("wander");
    std::cout << "\t" << myClient->getRobotName() 
      << " wander mode" << std::endl;
  }
  else stop();
}

// stop the robot
void MoveKeyBoardHandler::stop()
{
  if (!myClient->dataExists("stop")) return;
  myClient->requestOnce("stop");
  std::cout << "\t" << myClient->getRobotName() 
    << " stop mode" << std::endl;
}

// alternate between unsafe and safe drive
void MoveKeyBoardHandler::unsafe()
{
  if (!myClient->dataExists("setSafeDrive") || myIsWandering) return;

  myIsSafe = !myIsSafe;

  ArNetPacket packet;
  if (myIsSafe) {
    packet.byteToBuf(1);
    std::cout << "\t" << myClient->getRobotName() 
      << " safe drive" << std::endl;
  }
  else {
    packet.byteToBuf(0);
    std::cout << "\t" << myClient->getRobotName() 
      << " unsafe drive" << std::endl;
  }
  myClient->requestOnce("setSafeDrive", &packet);
}

// control next robot
void MoveKeyBoardHandler::nextRobot()
{
  myClientIndex++;
  if (myClientIndex >= myClients.size()) myClientIndex = 0;
  myClient = myClients[myClientIndex];
  echo("keyboard controls", myClient->getRobotName());
}

// control previous robot
void MoveKeyBoardHandler::prevRobot()
{
  if (myClientIndex == 0) myClientIndex = myClients.size() - 1;
  else myClientIndex--;
  myClient = myClients[myClientIndex];
  echo("keyboard controls", myClient->getRobotName());
}

static std::string moveKeyToString(int c)
{
  std::string keyName;
  switch (c) {
    case ArKeyHandler::UP:
      keyName = "UP arrow";
      break;
    case ArKeyHandler::DOWN:
      keyName = "DOWN arrow";
      break;
    case ArKeyHandler::LEFT:
      keyName = "LEFT arrow";
      break;
    case ArKeyHandler::RIGHT:
      keyName = "RIGHT arrow";
      break;
    case ArKeyHandler::PAGEUP:
      keyName = "PAGEUP arrow";
      break;
    case ArKeyHandler::PAGEDOWN:
      keyName = "PAGEDOWN arrow";
      break;
    default:
      keyName = char(c);
      break;
  }
  return keyName;
}

int moveKeys[10];
int moveKeysInfo[10];

// Tell the user about the control keys
void displayKeys() 
{
  const int leftMargin = 5;
  const int keyColWidth = 20;
  const int descColWidth = 30;

  std::cout << std::endl << "Keyboard controls" << std::endl;
  std::cout << std::setw(leftMargin) << "|"
    << std::setw(keyColWidth) << "Key" 
    << std::setw(descColWidth) << "Description" 
    << std::endl
    << std::string(60, '-')
    << std::endl;

  for (int i = 0; i < sizeof(moveKeys)/sizeof(moveKeys[0]); i++) {
  std::cout << std::setw(leftMargin) << "|"
    << std::setw(keyColWidth) << moveKeyToString(moveKeys[i])
    << std::setw(descColWidth) << moveKeysInfo[i]
    << std::endl;
  }

  std::cout << std::endl;
}




// Check for keys pressed on joystick and orientation of the stick
// itself when in manual mode
void checkJoy(ArJoyHandler *joy, const std::vector<ArClientBase *> &clients)
{
  static unsigned int currClientIndex = 0;
  static ArClientBase *client = clients[currClientIndex];
  static bool manMode = false;
  static double myTransRatio = 0, myRotRatio = 0;
  static int x = 0, y = 0;

  // control previous robot
  if (joy->getButton(4)) {
    if (currClientIndex == 0) 
      currClientIndex = clients.size() - 1;
    else
      currClientIndex--;
    client = clients[currClientIndex];
    echo("joystick controls", client->getRobotName());
  }
  // control next robot
  else if (joy->getButton(5)) {
    currClientIndex++;
    if (currClientIndex >= clients.size()) 
      currClientIndex = 0;
    client = clients[currClientIndex];
    echo("joystick controls", client->getRobotName());
  }
  // stop the robot
  else if (joy->getButton(2)) {
    if (!client->dataExists("stop")) return;
    else std::cout << "\t" << client->getRobotName() 
      << " stop mode" << std::endl;

    manMode = false;
    client->requestOnce("stop");
    myTransRatio = 0;
    myRotRatio = 0;
  }
  // robot is automatic
  else if (joy->getButton(3)) {
    if (!client->dataExists("wander")) return;
    else std::cout << "\t" << client->getRobotName() 
      << " auto mode" << std::endl;
    manMode = false;
    client->requestOnce("wander");
    myTransRatio = 0;
    myRotRatio = 0;
  }
  // auto all
  else if (joy->getButton(6)) {
    echo("ALL ROBOTS IN AUTO MODE");
    for (unsigned int i = 0; i < clients.size(); i++)
      clients[i]->requestOnce("wander");
  }
  // stop all
  else if (joy->getButton(7)) {
    echo("ALL ROBOTS IN STOP MODE");
    for (unsigned int i = 0; i < clients.size(); i++)
      clients[i]->requestOnce("stop");
  }
  // safe driving
  else if (joy->getButton(8)) {
    if (!client->dataExists("setSafeDrive")) return;
    else std::cout << "\t" << client->getRobotName() 
      << " safe drive" << std::endl;

    ArNetPacket packet;
    packet.byteToBuf(1);
    client->requestOnce("setSafeDrive", &packet);
  }
  // unsafe driving
  else if (joy->getButton(9)) {
    if (!client->dataExists("setSafeDrive")) return;
    else std::cout << "\t" << client->getRobotName() 
      << " unsafe drive" << std::endl;

    ArNetPacket packet;
    packet.byteToBuf(0);
    client->requestOnce("setSafeDrive", &packet);
  }

  // manually control the robot
  if (joy->getButton(1)) {
    if (!client->dataExists("ratioDrive")) return;
    else if (!manMode) {
      std::cout << "\t" << client->getRobotName() 
	<< " manual mode" << std::endl;
    }

    // disable other modes and turn on manual mode
    if (!manMode) {
      client->requestOnce("stop");
      myTransRatio = 0;
      myRotRatio = 0;
      manMode = true;
    }

    joy->getAdjusted(&x, &y);
    myRotRatio = -x;
    myTransRatio = y;
    ArNetPacket packet;
    packet.doubleToBuf(myTransRatio);
    packet.doubleToBuf(myRotRatio);
    packet.doubleToBuf(75);

    client->requestOnce("ratioDrive", &packet);
    myTransRatio = 0;
    myRotRatio = 0;
  }
  else {
    if (!client->dataExists("stop")) return;
    else if (manMode) {
      std::cout << "\t" << client->getRobotName() 
	<< " stop mode" << std::endl;

      manMode = false;
      client->requestOnce("stop");
      myTransRatio = 0;
      myRotRatio = 0;
    }
  }
}

// Display joystick controls
void joyInfoDisplay()
{
  std::string moveKeysInfo[] = {
    "move up", "move down", "move left", "move right", 
    "auto mode", "drive mode", "stop mode", "select previous robot",
    "select next robot", "auto all robots", "stop all robots",
    "safe drive", "unsafe drive"
  };

  std::string moveKeys[] = {
    "JOYSTICK UP", "JOYSTICK DOWN", "JOYSTICK LEFT", "JOYSTICK RIGHT", 
    "BUTTON 3", "TRIGGER", "BUTTON 2", "BUTTON 4", "BUTTON 5",
    "BUTTON 6", "BUTTON 7", "BUTTON 8", "BUTTON 9"
  };

  const int leftMargin = 5;
  const int keyColWidth = 20;
  const int descColWidth = 30;

  std::cout << std::endl << "Joystick controls" << std::endl;
  std::cout << std::setw(leftMargin) << "|"
    << std::setw(keyColWidth) << "Key" 
    << std::setw(descColWidth) << "Description" 
    << std::endl
    << std::string(60, '-')
    << std::endl;

  for (int i = 0; i < sizeof(moveKeys)/sizeof(moveKeys[0]); i++) {
  std::cout << std::setw(leftMargin) << "|"
    << std::setw(keyColWidth) << moveKeys[i]
    << std::setw(descColWidth) << moveKeysInfo[i]
    << std::endl;
  }

  std::cout << std::endl;
}
