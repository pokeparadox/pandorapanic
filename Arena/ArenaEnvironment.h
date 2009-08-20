//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ArenaEnvironment_H
#define ArenaEnvironment_H

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Background.h"
#include "CollisionMap.h"

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
    Background m_bmpArena, m_bmpBorder;

    CollisionMap* m_CollisionMapPtr;
};

#endif // ArenaEnvironment_H
