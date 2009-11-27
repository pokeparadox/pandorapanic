//-----------------------------
// Includes
//-----------------------------
#include "StateDarkTreasure.h"

//---------------------------
// Defines
//---------------------------


//-----------------------------
// Constructor & Destructor
//-----------------------------
StateDarkTreasure::StateDarkTreasure()
{
    m_Engine = new Raycast();
    m_Engine->init(32, 32, 480, 320);
    m_Engine->loadTexture("images/DarkTreasure/Textures/1.png");
    m_Engine->loadTexture("images/DarkTreasure/Textures/2.png");

    m_Border.loadBackground("images/DarkTreasure/Border.png");
    m_Border.setUseHardware(true);
    m_Border.setTransparentColour(MAGENTA);
    m_Border.setPosition(Vector2di(0,0));

    m_Music.loadMusic("music/Arena/Music.ogg");
    m_Music.setLooping(true);
    //m_Music.play();
}
StateDarkTreasure::~StateDarkTreasure()
{
    delete m_Engine;
}
//-----------------------------
// Methods
//-----------------------------
void StateDarkTreasure::init()
{

}

#ifdef PENJIN_SDL
void StateDarkTreasure::render(SDL_Surface* screen)
{
    m_Engine->render(screen);
    m_Border.render(screen);
}
#endif
void StateDarkTreasure::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    m_Engine->userInput(input);
}
