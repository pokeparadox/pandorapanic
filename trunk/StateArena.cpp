//-----------------------------
// Includes
//-----------------------------
#include "StateArena.h"

//---------------------------
// Defines
//---------------------------
#define ENVIRONMENT (ArenaEnvironment::GetSingleton())
#define EFFECTSYSTEM (ArenaEffectSystem::GetSingleton())

//-----------------------------
// Constructor & Destructor
//-----------------------------
StateArena::StateArena() : m_FrameRateCounter(0), m_EndCounter(0)
{
    m_HeroPtr = NULL;
    m_HeroPtr = new ArenaHero();

    m_MonsterListPtr = NULL;
    m_MonsterListPtr = new ArenaMonsterList();

    m_Clouds.loadSprite("images/Arena/Clouds.png");
    m_Clouds.setTransparentColour(MAGENTA);
    m_Clouds.setAlpha(72);
    m_Clouds.setPosition(0,0);

    m_Clouds2.loadSprite("images/Arena/Clouds.png");
    m_Clouds2.setTransparentColour(MAGENTA);
    m_Clouds2.setAlpha(72);
    m_Clouds.setPosition(-800,0);

    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);
}
StateArena::~StateArena()
{
    //m_Music.stop();

    if(m_HeroPtr)
        delete m_HeroPtr;
    if(m_MonsterListPtr)
        delete m_MonsterListPtr;
}
//-----------------------------
// Methods
//-----------------------------
void StateArena::init()
{
    //Adding Monsters to the MonsterList
    ArenaMonster* tempmonsterPtr;

    //Row 1
    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(300, 72, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(380, 72, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);


    //Row 2
    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(172, 136, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(252, 136, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(332, 136, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(412, 136, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(492, 136, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(572, 136, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    //Row 3
    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(92, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(172, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(252, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(332, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(412, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(492, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(572, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(652, 200, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    if(variables.size()<SUBSTATE_TRIGGER)
    {
        m_Music.loadMusic("music/Arena/Music.ogg");
        m_Music.setLooping(true);
        m_Music.play();
    }
}
void StateArena::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(m_FrameRateCounter == 0)
    {
        m_HeroPtr->userInput(input);
        m_FrameRateCounter += 1;
    }
    else if (m_FrameRateCounter == 12) m_FrameRateCounter = 0;
    else m_FrameRateCounter += 1;

    //Collision
    if(m_HeroPtr->GetSwinging())
    {
        ArenaMonster* monsterhitPtr = m_MonsterListPtr->HitTest(m_HeroPtr->GetSwordRegion());
        if (monsterhitPtr != 0 && !monsterhitPtr->GetDead())
        {
            EFFECTSYSTEM->SetEffect(monsterhitPtr->GetX(), monsterhitPtr->GetY(), FX_MONSTERHIT);
            monsterhitPtr->SetDead(m_HeroPtr->GetX(), m_HeroPtr->GetY());
        }
    }
    if (input->isStart())
    {
            isPaused = !isPaused;
            input->resetKeys();
    }
}
void StateArena::render(SDL_Surface *screen)
{
    ENVIRONMENT->render(screen);

    EFFECTSYSTEM->render(screen);

    m_MonsterListPtr->render(screen);

    m_HeroPtr->render(screen);

    ENVIRONMENT->renderBorder(screen);

    m_Clouds.render(screen);
    m_Clouds2.render(screen);
}
void StateArena::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Kill all the monsters!");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Press     to swing your sword!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,220);
    buttonSheet.render(screen);
}
void StateArena::onPause()
{

}
void StateArena::onResume()
{

}
void StateArena::update()
{
    if(!isPaused)
    {
        m_HeroPtr->update();

        m_MonsterListPtr->update();

        EFFECTSYSTEM->update();

        ArenaMonster* monsterhitPtr = m_MonsterListPtr->HitTest(m_HeroPtr->GetHitRegion());
        if (monsterhitPtr != 0)
        {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }

        if(m_MonsterListPtr->GetMonsterAmount() == 0) m_EndCounter += 1;

        if(m_EndCounter == 64)
        {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }
    }

    if(m_Clouds.getX() == 800) m_Clouds.setPosition(-800, m_Clouds.getY());
    else m_Clouds.setPosition(m_Clouds.getX() + 2, m_Clouds.getY());

    if(m_Clouds2.getX() == 800) m_Clouds2.setPosition(-800, m_Clouds2.getY());
    else m_Clouds2.setPosition(m_Clouds2.getX() + 2, m_Clouds2.getY());
}
