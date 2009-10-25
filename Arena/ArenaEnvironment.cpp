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

    m_bmpBorder.loadFrames("images/Arena/Border.png", 2, 1);
    m_bmpBorder.setTransparentColour(MAGENTA);
    m_bmpBorder.setFrameRate(DECI_SECONDS);

    m_Clouds.loadSprite("images/Arena/Clouds.png");
    m_Clouds.setTransparentColour(MAGENTA);
    m_Clouds.setAlpha(72);
    m_Clouds.setPosition(0,0);

    m_Clouds2.loadSprite("images/Arena/Clouds.png");
    m_Clouds2.setTransparentColour(MAGENTA);
    m_Clouds2.setAlpha(72);
    m_Clouds.setPosition(-800,0);

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
    m_Clouds.render(screen);
    m_Clouds2.render(screen);

    m_bmpBorder.render(screen);
}
void ArenaEnvironment::update()
{
     if(m_Clouds.getX() == 800) m_Clouds.setPosition(-800, m_Clouds.getY());
    else m_Clouds.setPosition(m_Clouds.getX() + 2, m_Clouds.getY());

    if(m_Clouds2.getX() == 800) m_Clouds2.setPosition(-800, m_Clouds2.getY());
    else m_Clouds2.setPosition(m_Clouds2.getX() + 2, m_Clouds2.getY());

    m_bmpBorder.update();
}
CollisionMap* ArenaEnvironment::GetCollisionMap()
{
    return m_CollisionMapPtr;
}
