//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ArenaEnvironment_H
#define ArenaEnvironment_H

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Background.h"
#include "Animation.h"
#include "CollisionMap.h"
#include "Sprite.h"

//-----------------------------------------------------
// ArenaEnvironment Class
//-----------------------------------------------------
class ArenaEnvironment
{
private:
    ArenaEnvironment();   //Constructor

    static ArenaEnvironment* m_EnvironmentPtr;

public:
    virtual ~ArenaEnvironment();  //Destructor

	//-------------------------------------------------
	// Methods
	//-------------------------------------------------
	static ArenaEnvironment* GetSingleton();
    void render(SDL_Surface* screen);
    void renderBorder(SDL_Surface* screen);
    void update();

    CollisionMap* GetCollisionMap();

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
    Background m_bmpArena;
    Animation m_bmpBorder;
    Sprite m_Clouds, m_Clouds2;

    CollisionMap* m_CollisionMapPtr;
};

#endif // ArenaEnvironment_H
