//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "ArenaEffectSystem.h"

//---------------------------
// Static
//---------------------------
ArenaEffectSystem* ArenaEffectSystem::m_EffectSystemPtr = NULL;

//-----------------------------------------------------
// Constructor & Destructor
//-----------------------------------------------------
ArenaEffectSystem::ArenaEffectSystem()
{
    for(int counter = 0; counter < MAX; ++counter)
    {
        m_EffectArr[counter] = new ArenaEffect();
    }
}
ArenaEffectSystem::~ArenaEffectSystem()
{
    for(int counter = 0; counter < MAX; ++counter)
    {
        delete m_EffectArr[counter];
    }
}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
ArenaEffectSystem* ArenaEffectSystem::GetSingleton()
{
	if ( m_EffectSystemPtr == NULL) m_EffectSystemPtr = new ArenaEffectSystem();
	return m_EffectSystemPtr;
}
void ArenaEffectSystem::render(SDL_Surface* screen)
{
    for(int counter = 0; counter < MAX; ++counter)
    {
        if(!(m_EffectArr[counter]->GetType() == FX_MONSTERHIT) && m_EffectArr[counter]->Exists()) m_EffectArr[counter]->render(screen);
    }

    for(int counter = 0; counter < MAX; ++counter)
    {
        if(m_EffectArr[counter]->GetType() == FX_MONSTERHIT && m_EffectArr[counter]->Exists()) m_EffectArr[counter]->render(screen);
    }
}
void ArenaEffectSystem::update()
{
    //---------------------------
    // Effects
    //---------------------------
    for(int counter = 0; counter < MAX; ++counter)
    {
        if(m_EffectArr[counter]->Exists()) m_EffectArr[counter]->update();
    }
}
void ArenaEffectSystem::SetEffect(int x, int y, int type)
{
	int position = 0;
	while (position < MAX && m_EffectArr[position]->Exists()) position += 1;

	if (position != MAX)
	{
		m_EffectArr[position]->init(x, y, type);
	}
}
