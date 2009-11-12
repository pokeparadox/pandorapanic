//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ArenaMonster_H
#define ArenaMonster_H

#define DIR_UP (1)
#define DIR_DOWN (2)
#define DIR_LEFT (3)
#define DIR_RIGHT (4)


//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "HitRegion.h"
#include "ArenaEnvironment.h"
#include "ArenaEffectSystem.h"

//-----------------------------------------------------
// ArenaMonster Class
//-----------------------------------------------------
class ArenaMonster
{
public:
    ArenaMonster();   //Constructor
    virtual ~ArenaMonster();  //Destructor

	//-------------------------------------------------
	// Public Methods
	//-------------------------------------------------

	//Main
	void init(int x, int y, int vX, int vY, int direction);
    void render(SDL_Surface* screen);
    void update();

    //Getters & Setters
    HitRegion* GetHitRegion();
    int GetDirection();
    bool GetWalking();
    int GetX();
    int GetY();
    bool GetDead();

    //Others
    void CantWalk();
    void SetDead(int herox, int heroy);
    void Move(int x, int y);
    bool HitTest(HitRegion* regionPtr);
    bool HitTest(CollisionMap* mapPtr);

    void HandleAll();
    //-------------------------------------------------
	// Private Methods
	//-------------------------------------------------
    void HandleFacing();
    void HandleCurrentImage();
    void HandleMovement();

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
    int m_X, m_Y, m_vX, m_vY, m_DeathSpeedX, m_DeathSpeedY, m_Direction, m_RandomDirection, m_RandCheckCounter;
    bool m_IsWalking, m_CanWalk, m_IsDead, m_SmokeSlowdown;
    AnimatedSprite m_bmpStandingUp, m_bmpStandingDown, m_bmpStandingLeft, m_bmpStandingRight, m_bmpWalkingUp, m_bmpWalkingDown, m_bmpWalkingLeft, m_bmpWalkingRight, m_bmpSpinning;
    AnimatedSprite* m_bmpCurrentPtr;
    Sprite m_bmpShadow;

    HitRegion* m_HitRegionPtr;
};

#endif // ArenaMonster_H
