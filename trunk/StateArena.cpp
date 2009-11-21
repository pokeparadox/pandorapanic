//-----------------------------
// Includes
//-----------------------------
#include "StateArena.h"

//---------------------------
// Defines
//---------------------------
#define ENVIRONMENT (ArenaEnvironment::GetSingleton())
#define EFFECTSYSTEM (ArenaEffectSystem::GetSingleton())

#define DEFAULTSPEED 4

//-----------------------------
// Constructor & Destructor
//-----------------------------
StateArena::StateArena() : m_EndCounter(0)
{
    m_HeroPtr = NULL;
    m_HeroPtr = new ArenaHero();

    m_MonsterListPtr = NULL;
    m_MonsterListPtr = new ArenaMonsterList();

    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);
}
StateArena::~StateArena()
{
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
    int level = variables[2].getInt();
    int speedLevel = min(floor((level-7.0)/9.0),5.0);
    int monsterLevel = (level-7)-speedLevel * 9;

    //Adding Monsters to the MonsterList
    ArenaMonster* tempmonsterPtr;

    //Row 1
    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(300, 72, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);

    tempmonsterPtr = new ArenaMonster();
    tempmonsterPtr->init(380, 72, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
    m_MonsterListPtr->Add(tempmonsterPtr);


    //Row 2
    if (level > 5)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(172, 136, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 3)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(252, 136, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 1)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(332, 136, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 2)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(412, 136, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 4)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(492, 136, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 6)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(572, 136, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }


    //Row 3
    if (level > 7 && monsterLevel > 7)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(92, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 5)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(172, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 3)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(252, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 1)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(332, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 0)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(412, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 2)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(492, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 4)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(572, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

    if (level > 7 && monsterLevel > 6)
    {
        tempmonsterPtr = new ArenaMonster();
        tempmonsterPtr->init(652, 200, DEFAULTSPEED + speedLevel, DEFAULTSPEED + speedLevel, DIR_DOWN);
        m_MonsterListPtr->Add(tempmonsterPtr);
    }

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

    m_HeroPtr->userInput(input);

    //Collision
    if(m_HeroPtr->GetSwinging() && !m_HeroPtr->GetDying())
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
        pauseToggle();
        input->resetKeys();
    }
}

void StateArena::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}

void StateArena::render(SDL_Surface *screen)
{
    ENVIRONMENT->render(screen);

    EFFECTSYSTEM->render(screen);

    m_MonsterListPtr->render(screen);

    m_HeroPtr->render(screen);

    if(!m_HeroPtr->GetDying()) ENVIRONMENT->renderBorder(screen);
}
void StateArena::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Kill all the monsters but don't get hit!");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Press     to swing your sword!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,220);
    buttonSheet.render(screen);
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
            m_HeroPtr->SetDead();
            m_Music.stop();
        }

        if(m_MonsterListPtr->GetMonsterAmount() == 0 || m_HeroPtr->GetDying()) m_EndCounter += 1;

        if(m_EndCounter == 64)
        {
            if(m_HeroPtr->GetDying())
            {
                variables[0].setInt(0);
                setNextState(STATE_MAIN);
            }
            else if(m_MonsterListPtr->GetMonsterAmount() == 0)
            {
                variables[0].setInt(1);
                setNextState(STATE_MAIN);
            }
        }
    }

    ENVIRONMENT->update();
}
