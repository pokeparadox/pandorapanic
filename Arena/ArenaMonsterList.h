//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ArenaMonsterList_H
#define ArenaMonsterList_H

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Background.h"
#include "Arena/ArenaMonster.h"
#include "Arena/ArenaEnvironment.h"
#include "Arena/ArenaEffectSystem.h"
#include "HitRegion.h"
#include "Sound.h"


//-----------------------------------------------------
// ArenaMonsterList Class
//-----------------------------------------------------
class ArenaMonsterList
{
public:
    ArenaMonsterList();   //Constructor
    virtual ~ArenaMonsterList();  //Destructor

	//-------------------------------------------------
	// Methods
	//-------------------------------------------------
    void render(SDL_Surface* screen);
    void update();
    void HandleCollision();

    void Add(ArenaMonster* monsterPtr);
    void Remove(ArenaMonster* monsterPtr);
    ArenaMonster* HitTest(HitRegion* regionPtr);

    int GetMonsterAmount();

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
	static const int MAX = 16;
    ArenaMonster* m_MonsterArr[MAX];

    Sound m_sndDie;
};

#endif // ArenaMonsterList_H
