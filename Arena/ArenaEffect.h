//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ArenaEffect_H
#define ArenaEffect_H

#define FX_EXPLOSION (1)
#define FX_MONSTERHIT (2)
#define FX_SMOKE (3)

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "AnimatedSprite.h"

//-----------------------------------------------------
// ArenaEffect Class
//-----------------------------------------------------
class ArenaEffect
{
public:

    ArenaEffect();   //Constructor
    virtual ~ArenaEffect();  //Destructor

	//-------------------------------------------------
	// Methods
	//-------------------------------------------------
	void init(int x, int y, int type);
    void render(SDL_Surface* screen);
    void update();

    bool Exists();
    int GetType();

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
	int m_X, m_Y, m_Type;
	bool m_Exists, m_RenderFix;
    AnimatedSprite m_bmpEffect;
};
#endif // ArenaEffect_H
