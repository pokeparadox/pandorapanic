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

    m_bmpSwordUp.loadFrames("images/Arena/Hero/HeroSwordUp.png", 4, 1);
    m_bmpSwordUp.setLooping(false);
    m_bmpSwordUp.setTransparentColour(MAGENTA);
    m_bmpSwordUp.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSwordDown.loadFrames("images/Arena/Hero/HeroSwordDown.png", 4, 1);
    m_bmpSwordDown.setLooping(false);
    m_bmpSwordDown.setTransparentColour(MAGENTA);
    m_bmpSwordDown.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSwordLeft.loadFrames("images/Arena/Hero/HeroSwordLeft.png", 4, 1);
    m_bmpSwordLeft.setLooping(false);
    m_bmpSwordLeft.setTransparentColour(MAGENTA);
    m_bmpSwordLeft.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSwordRight.loadFrames("images/Arena/Hero/HeroSwordRight.png", 4, 1);
    m_bmpSwordRight.setLooping(false);
    m_bmpSwordRight.setTransparentColour(MAGENTA);
    m_bmpSwordRight.setFrameRate(FIFTEEN_FRAMES);

    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroHit.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroHit.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingLeft.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingUp.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingRight.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingDown.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingLeft.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingUp.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingRight.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingDown.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingLeft.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingUp.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingRight.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingDown.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingDown.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroStandingDown.png");
    m_bmpSpinning.loadFrame("images/Arena/Hero/HeroDead.png");
    m_bmpSpinning.setLooping(false);
    m_bmpSpinning.setTransparentColour(MAGENTA);
    m_bmpSpinning.setFrameRate(FIFTEEN_FRAMES);

    m_bmpShadow.loadSprite("images/Arena/Hero/HeroShadow.png");
    m_bmpShadow.setAlpha(72);

    //Set the current image to Hero facing up
    m_bmpCurrentPtr = NULL;
    m_bmpCurrentPtr = &m_bmpStandingUp;

    m_IsSwinging = false;
    m_IsDying = false;
    m_RenderFix = false;

    m_HitRegionPtr = NULL;
    m_HitRegionPtr = new HitRegion();
    m_HitRegionPtr->init(m_X + 80, m_Y + 128, 32, 32);

    m_SwordRegionPtr = NULL;
    m_SwordRegionPtr = new HitRegion();

    m_sndSword.loadSound("sounds/Arena/SwordSwing.wav");

    m_bmpCurrentPtr->setPosition(m_X, m_Y);
    m_bmpShadow.setPosition(m_X, m_Y);
}
ArenaHero::~ArenaHero()
{
    m_bmpCurrentPtr = 0;

    if(m_HitRegionPtr)
        delete m_HitRegionPtr;
    if(m_SwordRegionPtr)
        delete m_SwordRegionPtr;
}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
void ArenaHero::render(SDL_Surface* screen)
{
    m_bmpShadow.render(screen);

    if(m_bmpCurrentPtr == &m_bmpSwordRight || m_bmpCurrentPtr == &m_bmpSwordDown || m_bmpCurrentPtr == &m_bmpSwordLeft || m_bmpCurrentPtr == &m_bmpSwordUp) m_RenderFix = true;
    else m_RenderFix = false;

    m_bmpCurrentPtr->render(screen);

    if(m_bmpCurrentPtr == &m_bmpSpinning)
    {
        if(m_bmpCurrentPtr->getCurrentFrame() == 0 || m_bmpCurrentPtr->getCurrentFrame() == 1)
        {
            Rectangle flash;
            flash.setColour(WHITE);
            flash.setDimensions(800, 480);
            flash.setPosition(0, 0);
            flash.render(screen);
        }
        else
        {
            Rectangle dark;
            dark.setColour(BLACK);
            dark.setDimensions(800, 480);
            dark.setPosition(0, 0);
            dark.render(screen);

            m_bmpCurrentPtr->render(screen);
        }

    }
    //m_HitRegionPtr->render(screen);
    //m_SwordRegionPtr->render(screen);
}
void ArenaHero::userInput(SimpleJoy* input)
{
    if(!m_IsDying)
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
                    m_bmpCurrentPtr = &m_bmpSwordUp;
                }
                //DOWN
                else if(m_Direction == DIR_DOWN)
                {
                    m_bmpCurrentPtr = &m_bmpSwordDown;
                }
                //LEFT
                else if(m_Direction == DIR_LEFT)
                {
                    m_bmpCurrentPtr = &m_bmpSwordLeft;
                }
                //RIGHT
                else if(m_Direction == DIR_RIGHT)
                {
                    m_bmpCurrentPtr = &m_bmpSwordRight;
                }

                m_bmpCurrentPtr->setCurrentFrame(0);
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
    }
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
        m_bmpCurrentPtr = &m_bmpStandingUp;
    }
    //DOWN
    else if(m_Direction == DIR_DOWN && !input->isDown())
    {
        m_bmpCurrentPtr = &m_bmpStandingDown;
    }
    //LEFT
    else if(m_Direction == DIR_LEFT && !input->isLeft())
    {
        m_bmpCurrentPtr = &m_bmpStandingLeft;
    }
    //RIGHT
    else if(m_Direction == DIR_RIGHT && !input->isRight())
    {
        m_bmpCurrentPtr = &m_bmpStandingRight;
    }

    //-----------------------------
    // Walking
    //-----------------------------

    //UP
    if(input->isUp())
    {
        m_bmpCurrentPtr = &m_bmpWalkingUp;
    }
    //DOWN
    else if(input->isDown())
    {
        m_bmpCurrentPtr = &m_bmpWalkingDown;
    }
    //LEFT
    else if(input->isLeft())
    {
        m_bmpCurrentPtr = &m_bmpWalkingLeft;
    }
    //RIGHT
    else if(input->isRight())
    {
        m_bmpCurrentPtr = &m_bmpWalkingRight;
    }

}
void ArenaHero::Move(int x, int y)
{
    m_X += x;
    m_Y += y;
    m_HitRegionPtr->move(x, y);
    m_SwordRegionPtr->move(x, y);
}
 void ArenaHero::SetDead()
 {
     m_IsDying = true;
     m_bmpCurrentPtr = &m_bmpSpinning;
     m_bmpCurrentPtr->setPosition(m_X, m_Y);
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
    if(!m_IsDying)
    {
        //-----------------------------------
        // Update the SwordRegion position
        //-----------------------------------

        //UP
        if(m_Direction == DIR_UP)
        {
            m_SwordRegionPtr->init(m_X + 56, m_Y + 32, 80, 64);
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

        if (m_bmpCurrentPtr->hasFinished())
        {
            //UP
            if(m_Direction == DIR_UP)
            {
                m_bmpCurrentPtr = &m_bmpStandingUp;
                m_bmpSwordUp.setCurrentFrame(0);
            }
            //DOWN
            else if(m_Direction == DIR_DOWN)
            {
                m_bmpCurrentPtr = &m_bmpStandingDown;
                m_bmpSwordDown.setCurrentFrame(0);
            }
            //LEFT
            else if(m_Direction == DIR_LEFT)
            {
                m_bmpCurrentPtr = &m_bmpStandingLeft;
                m_bmpSwordLeft.setCurrentFrame(0);
            }
            //RIGHT
            else if(m_Direction == DIR_RIGHT)
            {
                m_bmpCurrentPtr = &m_bmpStandingRight;
                m_bmpSwordRight.setCurrentFrame(0);
            }

            m_IsSwinging = false;
        }

        //-----------------------------------
        // Update the current animation
        //-----------------------------------

        m_bmpCurrentPtr->setPosition(m_X, m_Y);

        if(m_bmpCurrentPtr == &m_bmpSwordRight || m_bmpCurrentPtr == &m_bmpSwordDown || m_bmpCurrentPtr == &m_bmpSwordLeft || m_bmpCurrentPtr == &m_bmpSwordUp)
        {
            if(m_RenderFix) m_bmpCurrentPtr->update();
        }
        else m_bmpCurrentPtr->update();

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
    else
    {
        m_bmpCurrentPtr->setPosition(m_X, m_Y);
        m_bmpCurrentPtr->update();
    }
}
