#ifndef USERSTATES_H
#define USERSTATES_H
/*
Modify this file to tailor the states needed by your specific game.
STATE_NULL is vital to the operation of this state system, you are strongly advised not to remove it from the
BaseState class.

STATE_TITLE is considered the default state, so you are strongly advised to leave this in this file also.

NOTE: Be sure to #include ALL state headers which correspond to these modes.
*/

#include "BaseState.h"
#include "StateCredits.h"
#include "StateGameover.h"
#include "StateGameoverOrdered.h"
#include "StateTitle.h"
#include "StateMain.h"
#include "StateArena.h"
#include "StateBallDrop.h"          //  Zacaj - Catch the ball
#include "StatePuttPutt.h"
#include "StateSpaceInvaders.h"     //  pstudio - Shoot the invader
#include "StateSpellingContest.h"   //  pstudio's Spelling Contest mini game for Pandora Panic
#include "StateBadVista.h"
#include "StateDistractedMath.h"
#include "StatePunchWrestler.h"
#include "StateDoubleNubTrouble.h"
#include "StateReflex.h"
#include "StateOneButtonBandit.h"
#include "StatePong.h"
#include "StatePanicAttack.h"
#include "StatePanJoust.h"
#include "StateJumper.h"
#include "StateRotDef.h"
#include "StateMemoryBlocks.h"
#include "StateSnatchABeer.h"
#include "StateTheCountJob.h"
#include "StateFire.h"

#include "StateTileEditor.h"
#include "StateBarrelRoll.h"
#include "StatePlander.h"
//	STATE_NULL		//	Exit the program, this is defined in the BaseState
//	STATE_BASE		//	Template state, does nothing particularly useful
enum STATE_MODES
{
	STATE_TITLE = 2,
	STATE_MAIN,
	/// Mini games
	STATE_ARENA,
	STATE_BALLDROP,
	STATE_SPACEINVADERS,
	STATE_SPELLINGCONTEST,
	STATE_BADVISTA,
	STATE_DISTRACTEDMATH,
	STATE_REFLEX,
	STATE_DOUBLENUBTROUBLE,
	STATE_PUNCHWRESTLER,
	STATE_PONG,
	STATE_ONEBUTTONBANDIT,
    STATE_PANICATTACK,
    STATE_PANJOUST,
    STATE_JUMPER,
    STATE_PUTTPUTT,
    STATE_ROT_DEF,
    STATE_MEMORY_BLOCKS,
    STATE_PLANDER,
    STATE_SNATCHABEER,
    STATE_COUNTJOB,
    STATE_FIRE,

    //STATE_BARRELROLL,
    /// Other stuff
	STATE_CREDITS,
	STATE_GAMEOVER,
	STATE_GAMEOVERORDERED,
	STATE_TILEEDITOR,
};

#endif	//	USERSTATES_H
