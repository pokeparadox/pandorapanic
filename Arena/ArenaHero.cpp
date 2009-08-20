//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "ArenaHero.h"

//---------------------------
// Defines
//---------------------------
#define ENVIRONMENT (ArenaEnvironment::GetSingleton())

//-----------------------------------------------------
// Constructor & Destructor
//-----------------------------------------------------
ArenaHero::ArenaHero() : m_X(312), m_Y(260), m_Direction(DIR_UP)
{
    //------------------------
    // Standing
    //------------------------

    m_bmpStandingUp.loadFrames("images/Arena/Hero/HeroStandingUp.png", 1, 1);
    m_bmpStandingUp.setLooping(true);
    m_bmpStandingUp.setTransparentColour(MAGENTA);
    m_bmpStandingUp.setFrameRate(DECI_SECONDS);

    m_bmpStandingDown.loadFrames("images/Arena/Hero/HeroStandingDown.png", 1, 1);
    m_bmpStandingDown.setLooping(true);
    m_bmpStandingDown.setTransparentColour(MAGENTA);
    m_bmpStandingDown.setFrameRate(DECI_SECONDS);

    m_bmpStandingLeft.loadFrames("images/Arena/Hero/HeroStandingLeft.png", 1, 1);
    m_bmpStandingLeft.setLooping(true);
    m_bmpStandingLeft.setTransparentColour(MAGENTA);
    m_bmpStandingLeft.setFrameRate(DECI_SECONDS);

    m_bmpStandingRight.loadFrames("images/Arena/Hero/HeroStandingRight.png", 1, 1);
    m_bmpStandingRight.setLooping(true);
    m_bmpStandingRight.setTransparentColour(MAGENTA);
    m_bmpStandingRight.setFrameRate(DECI_SECONDS);

    //------------------------
    // Walking
    //------------------------

    m_bmpWalkingUp.loadFrames("images/Arena/Hero/HeroWalkingUp.png", 4, 1);
    m_bmpWalkingUp.setLooping(true);
    m_bmpWalkingUp.setTransparentColour(MAGENTA);
    m_bmpWalkingUp.setTimerScaler((float)1000 / 10.0f);

    m_bmpWalkingDown.loadFrames("images/Arena/Hero/HeroWalkingDown.png", 4, 1);
    m_bmpWalkingDown.setLooping(true);
    m_bmpWalkingDown.setTransparentColour(MAGENTA);
    m_bmpWalkingDown.setTimerScaler((float)1000 / 10.0f);

    m_bmpWalkingLeft.loadFrames("images/Arena/Hero/HeroWalkingLeft.png", 4, 1);
    m_bmpWalkingLeft.setLooping(true);
    m_bmpWalkingLeft.setTransparentColour(MAGENTA);
    m_bmpWalkingLeft.setTimerScaler((float)1000 / 10.0f);

    m_bmpWalkingRight.loadFrames("images/Arena/Hero/HeroWalkingRight.png", 4, 1);
    m_bmpWalkingRight.setLooping(true);
    m_bmpWalkingRight.setTransparentColour(MAGENTA);
    m_bmpWalkingRight.setTimerScaler((float)1000 / 10.0f);

    //------------------------
    // Sword swinging
    //------------------------

    m_bmpSwordUp.loadFrames("images/Arena/Hero/HeroSwordUp.png", 5, 1);
    m_bmpSwordUp.setLooping(false);
    m_bmpSwordUp.setTransparentColour(MAGENTA);
    m_bmpSwordUp.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSwordDown.loadFrames("images/Arena/Hero/HeroSwordDown.png", 5, 1);
    m_bmpSwordDown.setLooping(false);
    m_bmpSwordDown.setTransparentColour(MAGENTA);
    m_bmpSwordDown.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSwordLeft.loadFrames("images/Arena/Hero/HeroSwordLeft.png", 5, 1);
    m_bmpSwordLeft.setLooping(false);
    m_bmpSwordLeft.setTransparentColour(MAGENTA);
    m_bmpSwordLeft.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSwordRight.loadFrames("images/Arena/Hero/HeroSwordRight.png", 5, 1);
    m_bmpSwordRight.setLooping(false);
    m_bmpSwordRight.setTransparentColour(MAGENTA);
    m_bmpSwordRight.setFrameRate(FIFTEEN_FRAMES);

    m_bmpShadow.loadSprite("images/Arena/Hero/HeroShadow.png");
    m_bmpShadow.setAlpha(72);

    //Set the current image to Hero facing up
    m_bmpCurrent = m_bmpStandingUp;

    m_IsSwinging = false;

    m_HitRegionPtr = new HitRegion();
    m_HitRegionPtr->init(m_X + 80, m_Y + 128, 32, 32);

    m_SwordRegionPtr = new HitRegion();

    m_sndSword.loadSound("sounds/Arena/SwordSwing.wav");

    m_bmpCurrent.setPosition(m_X, m_Y);
    m_bmpShadow.setPosition(m_X, m_Y);
}
ArenaHero::~ArenaHero()
{

}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
void ArenaHero::render(SDL_Surface* screen)
{
    m_bmpShadow.render(screen);

    m_bmpCurrent.render(screen);
    //m_HitRegionPtr->render(screen);
    //m_SwordRegionPtr->render(screen);
}
void ArenaHero::userInput(SimpleJoy* input)
{

    if(!m_IsSwinging)
    {
        HandleFacing(input);
        HandleCurrentImage(input);


        //-----------------------------------------------------
        // Sword swinging
        //-----------------------------------------------------

        if(input->isA())
        {
            m_sndSword.play();
            m_IsSwinging = true;

            //UP
            if(m_Direction == DIR_UP)
            {
                m_bmpCurrent = m_bmpSwordUp;
            }
            //DOWN
            else if(m_Direction == DIR_DOWN)
            {
                m_bmpCurrent = m_bmpSwordDown;
            }
            //LEFT
            else if(m_Direction == DIR_LEFT)
            {
                m_bmpCurrent = m_bmpSwordLeft;
            }
            //RIGHT
            else if(m_Direction == DIR_RIGHT)
            {
                m_bmpCurrent = m_bmpSwordRight;
            }
        }

        //-----------------------------------------------------
        // Walking movement
        //-----------------------------------------------------

        //UP
        else if(input->isUp() && !input->isDown() && !input->isLeft() && !input->isRight())
        {
            Move(0, -4);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(0, 4);
        }

        //UPRIGHT
        else if(input->isUpRight() || (input->isUp() && input->isRight()))
        {
            Move(4, -4);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(-4, 4);
        }

        //RIGHT
        else if(input->isRight() && !input->isUp() && !input->isDown() && !input->isLeft())
        {
            Move(4, 0);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(-4, 0);
        }

        //DOWNRIGHT
        else if(input->isDownRight() || (input->isDown() && input->isRight()))
        {
            Move(4, 4);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(-4, -4);
        }

        //DOWN
        else if(input->isDown() && !input->isUp() && !input->isLeft() && !input->isRight())
        {
            Move(0, 4);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(0, -4);
        }

        //DOWNLEFT
        else if(input->isDownLeft() || (input->isDown() && (input->isLeft())))
        {
            Move(-4, 4);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(4, -4);
        }

        //LEFT
        else if(input->isLeft() && !input->isUp() && !input->isDown() && !input->isRight())
        {
            Move(-4, 0);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(4, 0);
        }

        //UPLEFT
        else if(input->isUpLeft() || (input->isUp() && input->isLeft()))
        {
            Move(-4, -4);
            if(m_HitRegionPtr->hitTest(ENVIRONMENT->GetCollisionMap())) Move(4, 4);
        }
    }
    else SwingSword();
}
void ArenaHero::HandleFacing(SimpleJoy* input)
{
    //UP
    if(input->isUp())
    {
        m_Direction = DIR_UP;
    }

    //DOWN
    if(input->isDown())
    {
        m_Direction = DIR_DOWN;
    }

    //LEFT
    if(input->isLeft())
    {
        m_Direction = DIR_LEFT;
    }

    //RIGHT
    if(input->isRight())
    {
        m_Direction = DIR_RIGHT;
    }
}
void ArenaHero::HandleCurrentImage(SimpleJoy* input)
{
    //-----------------------------
    // Standing
    //-----------------------------

    //UP
    if(m_Direction == DIR_UP && !input->isUp())
    {
        m_bmpCurrent = m_bmpStandingUp;
    }
    //DOWN
    else if(m_Direction == DIR_DOWN && !input->isDown())
    {
        m_bmpCurrent = m_bmpStandingDown;
    }
    //LEFT
    else if(m_Direction == DIR_LEFT && !input->isLeft())
    {
        m_bmpCurrent = m_bmpStandingLeft;
    }
    //RIGHT
    else if(m_Direction == DIR_RIGHT && !input->isRight())
    {
        m_bmpCurrent = m_bmpStandingRight;
    }

    //-----------------------------
    // Walking
    //-----------------------------

    //UP
    if(input->isUp())
    {
        m_bmpCurrent = m_bmpWalkingUp;
    }
    //DOWN
    else if(input->isDown())
    {
        m_bmpCurrent = m_bmpWalkingDown;
    }
    //LEFT
    else if(input->isLeft())
    {
        m_bmpCurrent = m_bmpWalkingLeft;
    }
    //RIGHT
    else if(input->isRight())
    {
        m_bmpCurrent = m_bmpWalkingRight;
    }

}
void ArenaHero::Move(int x, int y)
{
    m_X += x;
    m_Y += y;
    m_HitRegionPtr->move(x, y);
    m_SwordRegionPtr->move(x, y);
}
 void ArenaHero::SwingSword()
 {
    if (m_bmpCurrent.hasFinished())
    {
        //UP
        if(m_Direction == DIR_UP)
        {
            m_bmpCurrent = m_bmpStandingUp;
        }
        //DOWN
        else if(m_Direction == DIR_DOWN)
        {
            m_bmpCurrent = m_bmpStandingDown;
        }
        //LEFT
        else if(m_Direction == DIR_LEFT)
        {
            m_bmpCurrent = m_bmpStandingLeft;
        }
        //RIGHT
        else if(m_Direction == DIR_RIGHT)
        {
            m_bmpCurrent = m_bmpStandingRight;
        }

        m_IsSwinging = false;
    }
 }
HitRegion* ArenaHero::GetHitRegion()
{
    return m_HitRegionPtr;
}
HitRegion* ArenaHero::GetSwordRegion()
{
    return m_SwordRegionPtr;
}
int ArenaHero::GetX()
{
    return m_X;
}
int ArenaHero::GetY()
{
    return m_Y;
}
bool ArenaHero::GetSwinging()
{
    return m_IsSwinging;
}
void ArenaHero::update()
{
    //-----------------------------------
    // Update the SwordRegion position
    //-----------------------------------

    //UP
    if(m_Direction == DIR_UP)
    {
        m_SwordRegionPtr->init(m_X + 56, m_Y + 32, 80, 32);
    }
    //DOWN
    else if(m_Direction == DIR_DOWN)
    {
        m_SwordRegionPtr->init(m_X + 56, m_Y + 184, 80, 32);
    }
    //LEFT
    else if(m_Direction == DIR_LEFT)
    {
        m_SwordRegionPtr->init(m_X + 16, m_Y + 72, 32, 80);
    }
    //RIGHT
    else if(m_Direction == DIR_RIGHT)
    {
        m_SwordRegionPtr->init(m_X + 144, m_Y + 72, 32, 80);;
    }

    //-----------------------------------
    // Update the current animation
    //-----------------------------------

    m_bmpCurrent.setPosition(m_X, m_Y);
    m_bmpCurrent.update();

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
