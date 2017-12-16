#pragma once
#include "playermanager.h"

class Client {

private:

	IrrlichtDevice *irrDevice;

public:

	bool done;
	ITimer *irrTimer;
	ILogger *irrLog;

	PlayerManager * playerManager;

	LocalController * controller;
	AIController * npc;

	Scene * scene;

	Client();
	
};
