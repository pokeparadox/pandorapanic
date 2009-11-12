//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "ArenaMonster.h"

//---------------------------
// Defines
//---------------------------
#define ENVIRONMENT (ArenaEnvironment::GetSingleton())
#define EFFECTSYSTEM (ArenaEffectSystem::GetSingleton())

//-----------------------------------------------------
// Constructor & Destructor
//-----------------------------------------------------
ArenaMonster::ArenaMonster() : m_X(0), m_Y(0), m_vX(0), m_vY(0), m_DeathSpeedX(0), m_DeathSpeedY(0), m_Direction(0), m_HitRegionPtr(0)
{
    //------------------------
    // Standing
    //------------------------

    m_bmpStandingUp.loadFrames("images/Arena/Monster/MonsterStandingUp.png", 1, 1);
    m_bmpStandingUp.setLooping(true);
    m_bmpStandingUp.setTransparentColour(MAGENTA);
    m_bmpStandingUp.setFrameRate(DECI_SECONDS);

    m_bmpStandingDown.loadFrames("images/Arena/Monster/MonsterStandingDown.png", 1, 1);
    m_bmpStandingDown.setLooping(true);
    m_bmpStandingDown.setTransparentColour(MAGENTA);
    m_bmpStandingDown.setFrameRate(DECI_SECONDS);

    m_bmpStandingLeft.loadFrames("images/Arena/Monster/MonsterStandingLeft.png", 1, 1);
    m_bmpStandingLeft.setLooping(true);
    m_bmpStandingLeft.setTransparentColour(MAGENTA);
    m_bmpStandingLeft.setFrameRate(DECI_SECONDS);

    m_bmpStandingRight.loadFrames("images/Arena/Monster/MonsterStandingRight.png", 1, 1);
    m_bmpStandingRight.setLooping(true);
    m_bmpStandingRight.setTransparentColour(MAGENTA);
    m_bmpStandingRight.setFrameRate(DECI_SECONDS);

    //------------------------
    // Walking
    //------------------------

    m_bmpWalkingUp.loadFrames("images/Arena/Monster/MonsterWalkingUp.png", 4, 1);
    m_bmpWalkingUp.setLooping(true);
    m_bmpWalkingUp.setTransparentColour(MAGENTA);
    m_bmpWalkingUp.setFrameRate(DECI_SECONDS);

    m_bmpWalkingDown.loadFrames("images/Arena/Monster/MonsterWalkingDown.png", 4, 1);
    m_bmpWalkingDown.setLooping(true);
    m_bmpWalkingDown.setTransparentColour(MAGENTA);
    m_bmpWalkingDown.setFrameRate(DECI_SECONDS);

    m_bmpWalkingLeft.loadFrames("images/Arena/Monster/MonsterWalkingLeft.png", 4, 1);
    m_bmpWalkingLeft.setLooping(true);
    m_bmpWalkingLeft.setTransparentColour(MAGENTA);
    m_bmpWalkingLeft.setFrameRate(DECI_SECONDS);

    m_bmpWalkingRight.loadFrames("images/Arena/Monster/MonsterWalkingRight.png", 4, 1);
    m_bmpWalkingRight.setLooping(true);
    m_bmpWalkingRight.setTransparentColour(MAGENTA);
    m_bmpWalkingRight.setFrameRate(DECI_SECONDS);

    //------------------------
    // Spinning
    //------------------------

    m_bmpSpinning.loadFrame("images/Arena/Monster/MonsterStandingDown.png");
    m_bmpSpinning.loadFrame("images/Arena/Monster/MonsterStandingLeft.png");
    m_bmpSpinning.loadFrame("images/Arena/Monster/MonsterStandingUp.png");
    m_bmpSpinning.loadFrame("images/Arena/Monster/MonsterStandingRight.png");
    m_bmpSpinning.setLooping(true);
    m_bmpSpinning.setTransparentColour(MAGENTA);
    m_bmpSpinning.setFrameRate(DECI_SECONDS);

    m_bmpCurrentPtr = NULL;
    m_bmpCurrentPtr = &m_bmpStandingDown;

    m_bmpShadow.loadSprite("images/Arena/Monster/MonsterShadow.png");
    m_bmpShadow.setAlpha(72);

    m_RandCheckCounter = rand() % 32 + 1;

    m_IsWalking = true;
    m_CanWalk = true;
    m_IsDead = false;
    m_SmokeSlowdown = true;
}
ArenaMonster::~ArenaMonster()
{
    m_bmpCurrentPtr = NULL;

    if(m_HitRegionPtr)
        delete m_HitRegionPtr;
}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
void ArenaMonster::init(int x, int y, int vX, int vY, int direction)
{
    m_X = x;
    m_Y = y;

    m_vX = vX;
    m_vY = vY;

    m_Direction = direction;

    //Set the current image
    if(direction == DIR_UP)
    {
        m_bmpCurrentPtr = &m_bmpStandingUp;
    }
    else if(direction == DIR_DOWN)
    {
        m_bmpCurrentPtr = &m_bmpStandingDown;
    }
    else if(direction == DIR_LEFT)
    {
        m_bmpCurrentPtr = &m_bmpStandingLeft;
    }
    else if(direction == DIR_RIGHT)
    {
         m_bmpCurrentPtr = &m_bmpStandingRight;
    }

    m_HitRegionPtr = NULL;
    m_HitRegionPtr = new HitRegion();
    m_HitRegionPtr->init(m_X, m_Y + 16, 64, 48);

    m_bmpCurrentPtr->setPosition(m_X, m_Y);
    m_bmpShadow.setPosition(m_X, m_Y);
}
void ArenaMonster::render(SDL_Surface* screen)
{
    m_bmpShadow.render(screen);

    m_bmpCurrentPtr->render(screen);
    //m_HitRegionPtr->render(screen);
}
void ArenaMonster::HandleCurrentImage()
{
    //-----------------------------
    // Standing
    //-----------------------------

    if(!m_IsWalking)
    {
        //UP
        if(m_Direction == DIR_UP)
        {
            m_bmpCurrentPtr = &m_bmpStandingUp;
        }
        //DOWN
        else if(m_Direction == DIR_DOWN)
        {
            m_bmpCurrentPtr = &m_bmpStandingDown;
        }
        //LEFT
        else if(m_Direction == DIR_LEFT)
        {
            m_bmpCurrentPtr = &m_bmpStandingLeft;
        }
        //RIGHT
        else if(m_Direction == DIR_RIGHT)
        {
            m_bmpCurrentPtr = &m_bmpStandingRight;
        }
    }

    //-----------------------------
    // Walking
    //-----------------------------

    else if(m_IsWalking)
    {
        //UP
        if(m_Direction == DIR_UP)
        {
            m_bmpCurrentPtr = &m_bmpWalkingUp;
        }
        //DOWN
        else if(m_Direction == DIR_DOWN)
        {
            m_bmpCurrentPtr = &m_bmpWalkingDown;
        }
        //LEFT
        else if(m_Direction == DIR_LEFT)
        {
            m_bmpCurrentPtr = &m_bmpWalkingLeft;
        }
        //RIGHT
        else if(m_Direction == DIR_RIGHT)
        {
            m_bmpCurrentPtr = &m_bmpWalkingRight;
        }
    }
}
void ArenaMonster::HandleMovement()
{
    if(m_IsWalking && m_CanWalk)
    {
        //UP
        if(m_Direction == DIR_UP)
        {
            Move(0, m_vY * -1);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(0, m_vY);
        }
        //DOWN
        else if(m_Direction == DIR_DOWN)
        {
            Move(0, m_vY);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(0, m_vY * -1);
        }
        //LEFT
        else if(m_Direction == DIR_LEFT)
        {
            Move(m_vX * -1, 0);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(m_vX, 0);
        }
        //RIGHT
        else if(m_Direction == DIR_RIGHT)
        {
            Move(m_vX, 0);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(m_vX * -1, 0);
        }
    }

    m_CanWalk = true;
}
void ArenaMonster::Move(int x, int y)
{
    m_X += x;
    m_Y += y;
    m_HitRegionPtr->move(x, y);
}
HitRegion* ArenaMonster::GetHitRegion()
{
    return m_HitRegionPtr;
}
bool ArenaMonster::HitTest(HitRegion* regionPtr)
{
	return m_HitRegionPtr->hitTest(regionPtr);
}
bool ArenaMonster::HitTest(CollisionMap* mapPtr)
{
    return m_HitRegionPtr->hitTest(mapPtr);
}
int ArenaMonster::GetDirection()
{
    return m_Direction;
}
bool ArenaMonster::GetWalking()
{
    return m_IsWalking;
}
int ArenaMonster::GetX()
{
    return m_X;
}
int ArenaMonster::GetY()
{
    return m_Y;
}
bool ArenaMonster::GetDead()
{
    return m_IsDead;
}
void ArenaMonster::CantWalk()
{
    m_CanWalk = false;
}
void ArenaMonster::HandleAll()
{
    if(!m_IsDead)
    {
        //-----------------------------------
        // Handle Walking AI
        //-----------------------------------

        if(m_RandCheckCounter == 0)
        {
            int olddir = m_Direction;
            m_Direction = rand() % 4 + 1;

            //while the new direction is the opposite of the old one
            while((olddir == 1 && m_Direction == 2) || (olddir == 2 && m_Direction == 1) ||
                  (olddir == 3 && m_Direction == 4) || (olddir == 4 && m_Direction == 3))
            {
                //Then we take another direction
                m_Direction = rand() % 4 + 1;
            }

            bool oldstanding = m_IsWalking;
            int isstanding = rand() % 3 + 1;
            if(isstanding == 1 && oldstanding)
            {
                m_Direction = olddir;
                m_IsWalking = false;
            }
            else m_IsWalking = true;

            m_RandCheckCounter += 1;
        }
        else if(m_RandCheckCounter >= 64) m_RandCheckCounter = 0;
        else m_RandCheckCounter += 1;

        //Handle the Facing bools, current render image and movement
        HandleCurrentImage();
        HandleMovement();

        if(m_RandCheckCounter != 0) m_RandCheckCounter += rand() % 8 + 1;
    }
    else
    {
        Move(m_DeathSpeedX, m_DeathSpeedY);
        if(m_SmokeSlowdown)
        {
            EFFECTSYSTEM->SetEffect(m_X, m_Y, FX_SMOKE);
            m_SmokeSlowdown = false;
        }
        else m_SmokeSlowdown = true;

    }

}
 void ArenaMonster::SetDead(int herox, int heroy)
 {
     m_IsDead = true;

     m_bmpCurrentPtr = &m_bmpSpinning;

    //----------------------------------------------------------
    // Calculate the Speed - "Math-gic" by foxblock
    //----------------------------------------------------------

     // Insert the desired moving speed here:
     const double speed = 25.0;

     // Position difference between monster and hero
     const Vector2df diff = Vector2df((m_X + m_bmpCurrentPtr->getWidth() / 2)-(herox + 96.0),(m_Y + m_bmpCurrentPtr->getHeight() / 2)-(heroy + 112.0));

     // Math magic
     m_DeathSpeedX = round( diff.x / sqrt( pow(diff.x,2) + pow(diff.y,2) ) * speed );
     m_DeathSpeedY = round( diff.y / sqrt( pow(diff.x,2) + pow(diff.y,2) ) * speed );
 }

void ArenaMonster::update()
{
    //-----------------------------------
    // Update the current animation
    //-----------------------------------

    m_bmpCurrentPtr->setPosition(m_X, m_Y);
    m_bmpCurrentPtr->update();

    m_bmpShadow.setPosition(m_X, m_Y);

    //-----------------------------------
    // Update all other animations
    //-----------------------------------

    m_bmpStandingUp.update();
    m_bmpStandingDown.update();
    m_bmpStandingLeft.update();
    m_bmpStandingRight.update();

    m_bmpWalkingUp.update();
    m_bmpWalkingDown.update();
    m_bmpWalkingLeft.update();
    m_bmpWalkingRight.update();
}
