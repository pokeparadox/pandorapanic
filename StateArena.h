//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef STATEARENA_H_INCLUDED
#define STATEARENA_H_INCLUDED

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "PenjinStates.h"     // Make sure your state is listed in this include.

#include "Arena/ArenaEnvironment.h"
#include "Arena/ArenaEffectSystem.h"
#include "Arena/ArenaHero.h"
#include "HitRegion.h"
#include "Arena/ArenaMonsterList.h"

#include "Music.h"

#include "Sprite.h"
#include "Text.h"

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
    virtual void pauseScreen(SDL_Surface* screen);
    virtual void userInput();
    virtual void pauseInput();

    //-------------------------------------------------
	// Datamembers
	//-------------------------------------------------

	int m_EndCounter;

    ArenaHero* m_HeroPtr;
    ArenaMonsterList* m_MonsterListPtr;

    Music m_Music;

    AnimatedSprite buttonSheet;
    Text pauseText;

};

#endif // STATEARENA_H_INCLUDED
