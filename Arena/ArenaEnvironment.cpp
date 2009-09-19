//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "ArenaEnvironment.h"

//---------------------------
// Static
//---------------------------
ArenaEnvironment* ArenaEnvironment::m_EnvironmentPtr = NULL;

//-----------------------------------------------------
// Constructor & Destructor
//-----------------------------------------------------
ArenaEnvironment::ArenaEnvironment()
{
    m_bmpArena.loadBackground("images/Arena/Background.png");

    m_bmpBorder.loadBackground("images/Arena/Border.png");
    m_bmpBorder.setTransparentColour(MAGENTA);

    m_CollisionMapPtr = NULL;
    m_CollisionMapPtr = new CollisionMap();
    m_CollisionMapPtr->loadImage("images/Arena/Mask.png");
}
ArenaEnvironment::~ArenaEnvironment()
{
     if(m_CollisionMapPtr)
    {
        delete m_CollisionMapPtr;
        m_CollisionMapPtr = NULL;
    }
    if(m_EnvironmentPtr)
        delete m_EnvironmentPtr;
}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
ArenaEnvironment* ArenaEnvironment::GetSingleton()
{
	if ( m_EnvironmentPtr == NULL) m_EnvironmentPtr = new ArenaEnvironment();
	return m_EnvironmentPtr;
}
void ArenaEnvironment::render(SDL_Surface* screen)
{
    m_bmpArena.render(screen);
}
void ArenaEnvironment::renderBorder(SDL_Surface* screen)
{
    m_bmpBorder.render(screen);
}
void ArenaEnvironment::update()
{

}
CollisionMap* ArenaEnvironment::GetCollisionMap()
{
    return m_CollisionMapPtr;
}
