#ifndef MYGAME_H
#define MYGAME_H
#include "Engine.h"
#include "SimpleJoy.h"
#include "AchievementWinAll.h"

/*
PROJECT:	Pandora Panic
This is the customised game engine class, all overridden code is written here and differs depending on your game.
*/


class MyGame : public Engine
{
	public:
		MyGame();
		virtual ~MyGame();


		//virtual PENJIN_ERRORS argHandler(int argc, char** argv);
		virtual PENJIN_ERRORS init();
		virtual void stateManagement();
};


#endif	//	MYGAME_H
