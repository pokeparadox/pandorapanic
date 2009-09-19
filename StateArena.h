//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef STATEARENA_H_INCLUDED
#define STATEARENA_H_INCLUDED

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "userStates.h"     // Make sure your state is listed in this include.
#include "Arena/ArenaEnvironment.h"
#include "Arena/ArenaEffectSystem.h"
#include "Arena/ArenaHero.h"
#include "HitRegion.h"
#include "Arena/ArenaMonsterList.h"

#include "Music.h"

#include "Sprite.h"

//-----------------------------------------------------
// Arena Class
//-----------------------------------------------------
class StateArena : public BaseState
{
public:
    StateArena();   // Constructor
    virtual ~StateArena();   // Destructor

 	//-------------------------------------------------
	// Methods
	//-------------------------------------------------

    virtual void init();
    virtual void render(SDL_Surface *screen);
    virtual void update();
    virtual void onPause();
    virtual void onResume();
    virtual void pauseScreen(SDL_Surface* screen);
    virtual void userInput();

    //-------------------------------------------------
	// Datamembers
	//-------------------------------------------------

	int m_FrameRateCounter, m_EndCounter;

    ArenaHero* m_HeroPtr;
    ArenaMonsterList* m_MonsterListPtr;

    Music m_Music;

    Sprite m_Clouds, m_Clouds2;

};

#endif // STATEARENA_H_INCLUDED
