//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "ArenaMonsterList.h"

//---------------------------
// Defines
//---------------------------
#define ENVIRONMENT (ArenaEnvironment::GetSingleton())
#define EFFECTSYSTEM (ArenaEffectSystem::GetSingleton())

//-----------------------------------------------------
// Constructor & Destructor
//-----------------------------------------------------
ArenaMonsterList::ArenaMonsterList()
{
    for(int counter = 0; counter < MAX; ++counter)
    {
        m_MonsterArr[counter] = 0;
    }

    m_sndDie.loadSound("sounds/Arena/EnemyDie.wav");
}
ArenaMonsterList::~ArenaMonsterList()
{
   	for (int counter = 0; counter < MAX; ++counter)
	{
		delete m_MonsterArr[counter];
		m_MonsterArr[counter] = 0;
	}
}
//-----------------------------------------------------
// Methodes
//-----------------------------------------------------
void ArenaMonsterList::render(SDL_Surface* screen)
{
    int renderedmonsters = 0;

    for(int yposition = 0; yposition < 480; yposition += 1)
    {
        if(renderedmonsters == GetMonsterAmount()) break;

        for(int counter = 0; counter < MAX; ++counter)
        {
            if (m_MonsterArr[counter] != 0 && m_MonsterArr[counter]->GetY() == yposition)
            {
                m_MonsterArr[counter]->render(screen);
                renderedmonsters += 1;
            }
        }
    }
}
void ArenaMonsterList::update()
{
    HandleCollision();

    for(int counter = 0; counter < MAX; ++counter)
    {
        if (m_MonsterArr[counter] != 0)
		{
            m_MonsterArr[counter]->HandleAll();
            m_MonsterArr[counter]->update();
		}
    }
}
void ArenaMonsterList::HandleCollision()
{
    //-----------------------------------------------------
    // Collision Checking
    //-----------------------------------------------------

    for(int counter = 0; counter < MAX; ++counter)
    {
        if (m_MonsterArr[counter] != 0)
		{
		    if(!(m_MonsterArr[counter]->GetDead()))
		    {
                int direction = m_MonsterArr[counter]->GetDirection();

                if(direction == DIR_UP)
                {
                    m_MonsterArr[counter]->Move(0, -4);
                    for(int hrcounter = 0; hrcounter < MAX; hrcounter += 1)
                    {
                        if(m_MonsterArr[hrcounter] != 0 && m_MonsterArr[hrcounter] != m_MonsterArr[counter])
                        {
                            if(m_MonsterArr[counter]->GetHitRegion()->hitTest(m_MonsterArr[hrcounter]->GetHitRegion()))
                            {
                                m_MonsterArr[counter]->CantWalk();
                            }
                        }
                    }
                    m_MonsterArr[counter]->Move(0, 4);
                }
                else if(direction == DIR_DOWN)
                {
                    m_MonsterArr[counter]->Move(0, 4);
                    for(int hrcounter = 0; hrcounter < MAX; hrcounter += 1)
                    {
                        if(m_MonsterArr[hrcounter] != 0 && m_MonsterArr[hrcounter] != m_MonsterArr[counter])
                        {
                            if(m_MonsterArr[counter]->GetHitRegion()->hitTest(m_MonsterArr[hrcounter]->GetHitRegion()))
                            {
                                m_MonsterArr[counter]->CantWalk();
                            }
                        }
                    }
                    m_MonsterArr[counter]->Move(0, -4);
                }
                else if(direction == DIR_LEFT)
                {
                    m_MonsterArr[counter]->Move(-4, 0);
                    for(int hrcounter = 0; hrcounter < MAX; hrcounter += 1)
                    {
                        if(m_MonsterArr[hrcounter] != 0 && m_MonsterArr[hrcounter] != m_MonsterArr[counter])
                        {
                            if(m_MonsterArr[counter]->GetHitRegion()->hitTest(m_MonsterArr[hrcounter]->GetHitRegion()))
                            {
                                m_MonsterArr[counter]->CantWalk();
                            }
                        }
                    }
                    m_MonsterArr[counter]->Move(4, 0);
                }
                else if(direction == DIR_RIGHT)
                {
                    m_MonsterArr[counter]->Move(4, 0);
                    for(int hrcounter = 0; hrcounter < MAX; hrcounter += 1)
                    {
                        if(m_MonsterArr[hrcounter] != 0 && m_MonsterArr[hrcounter] != m_MonsterArr[counter])
                        {
                            if(m_MonsterArr[counter]->GetHitRegion()->hitTest(m_MonsterArr[hrcounter]->GetHitRegion()))
                            {
                                m_MonsterArr[counter]->CantWalk();
                            }
                        }
                    }
                    m_MonsterArr[counter]->Move(-4, 0);
                }
		    }
		    else
		    {
                if(m_MonsterArr[counter]->HitTest(ENVIRONMENT->GetCollisionMap()))
                {
                    EFFECTSYSTEM->SetEffect(m_MonsterArr[counter]->GetX(), m_MonsterArr[counter]->GetY(), FX_EXPLOSION);
                    Remove(m_MonsterArr[counter]);
                }
		    }
        }
    }
}
void ArenaMonsterList::Add(ArenaMonster* monsterPtr)
{
	int position = 0;
	while (position < MAX && m_MonsterArr[position] != 0) position += 1;

	if (position != MAX)
	{
		m_MonsterArr[position] = monsterPtr;
	}
}
void ArenaMonsterList::Remove(ArenaMonster* monsterPtr)
{
	int position = 0;
	while (position < MAX && m_MonsterArr[position] != monsterPtr) position += 1;

	if (position != MAX)
	{
	    m_MonsterArr[position] = new ArenaMonster();
		delete m_MonsterArr[position];
		m_MonsterArr[position] = 0;

		if(m_sndDie.isPlaying())
		{
		    m_sndDie.stop();
		}
		m_sndDie.play();
	}
}
ArenaMonster* ArenaMonsterList::HitTest(HitRegion* regionPtr)
{
	for (int counter = 0; counter < MAX; ++counter)
	{
		if (m_MonsterArr[counter] != 0 && m_MonsterArr[counter]->HitTest(regionPtr) == true)
			return m_MonsterArr[counter];
	}
	return 0;
}
int ArenaMonsterList::GetMonsterAmount()
{
    int result = 0;

    for (int counter = 0; counter < MAX; counter += 1)
	{
		if (m_MonsterArr[counter] != 0) result += 1;
	}
	return result;
}
