//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "ArenaEffect.h"

//-----------------------------------------------------
// Constructor & Destructor
//-----------------------------------------------------
ArenaEffect::ArenaEffect() : m_X(0), m_Y(0)
{
    m_bmpEffect.setLooping(false);
    m_bmpEffect.setTransparentColour(MAGENTA);
    m_bmpEffect.setFrameRate(DECI_SECONDS);

    m_Exists = false;
    m_Type = 0;
}
ArenaEffect::~ArenaEffect()
{

}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
void ArenaEffect::init(int x, int y, int type)
{
    if(type == FX_EXPLOSION)
    {
        m_X = x - 68;
        m_Y = y - 96;

        m_bmpEffect.loadFrames("images/Arena/Effects/Explosion.png", 4, 1);
    }
    else if(type == FX_MONSTERHIT)
    {
        m_X = x - 8;
        m_Y = y - 8;

        m_bmpEffect.loadFrames("images/Arena/Effects/MonsterHit.png", 4, 1);
    }
    else if(type == FX_SMOKE)
    {
        m_X = x;
        m_Y = y;

        m_bmpEffect.loadFrames("images/Arena/Effects/Smoke.png", 4, 1);
    }

    m_Type = type;

    m_Exists = true;
    m_RenderFix = false;

    m_bmpEffect.rewind();
}
void ArenaEffect::render(SDL_Surface* screen)
{
    m_RenderFix = true;
    m_bmpEffect.render(screen);
}
void ArenaEffect::update()
{
    if(m_bmpEffect.hasFinished())
    {
        m_Exists = false;
    }

    m_bmpEffect.setPosition(m_X, m_Y);
     if(m_RenderFix) m_bmpEffect.update();
}
bool ArenaEffect::Exists()
{
    return m_Exists;
}
int ArenaEffect::GetType()
{
    return m_Type;
}
