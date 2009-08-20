//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ArenaEffectSystem_H
#define ArenaEffectSystem_H

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Arena/ArenaEffect.h"

//-----------------------------------------------------
// ArenaEffectSystem Class
//-----------------------------------------------------
class ArenaEffectSystem
{
private:
    ArenaEffectSystem();   //Constructor

    static ArenaEffectSystem* m_EffectSystemPtr;

public:
    virtual ~ArenaEffectSystem();  //Destructor

	//-------------------------------------------------
	// Methods
	//-------------------------------------------------
	static ArenaEffectSystem* GetSingleton();
    void render(SDL_Surface* screen);
    void update();

    void SetEffect(int x, int y, int type);

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
	static const int MAX = 64;
    ArenaEffect* m_EffectArr[MAX];
};
#endif // ArenaEffectSystem_H
