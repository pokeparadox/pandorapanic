//-----------------------------------------------------
// Defines
//-----------------------------------------------------
#ifndef ARENAHERO_H
#define ARENAHERO_H

#define DIR_UP (1)
#define DIR_DOWN (2)
#define DIR_LEFT (3)
#define DIR_RIGHT (4)

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "SimpleJoy.h"
#include "HitRegion.h"
#include "ArenaEnvironment.h"
#include "Sound.h"

//-----------------------------------------------------
// ArenaHero Class
//-----------------------------------------------------
class ArenaHero
{
public:
    ArenaHero();   //Constructor
    virtual ~ArenaHero();  //Destructor

	//-------------------------------------------------
	// Public Methods
	//-------------------------------------------------
    void render(SDL_Surface* screen);
    void userInput();
    void update();
    HitRegion* GetHitRegion();
    HitRegion* GetSwordRegion();
    void setInput(SimpleJoy* joy){input = joy;}

    int GetX();
    int GetY();
    bool GetSwinging();
    bool GetDying(){return m_IsDying;}


    //-------------------------------------------------
	// Private Methods
	//-------------------------------------------------
    void HandleFacing();
    void HandleCurrentImage();
    void Move(int x, int y);
    void SetDead();

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
    int m_X, m_Y, m_Direction;
    bool m_IsSwinging, m_IsDying, m_RenderFix;
    AnimatedSprite m_bmpStandingUp, m_bmpStandingDown, m_bmpStandingLeft, m_bmpStandingRight, m_bmpWalkingUp, m_bmpWalkingDown, m_bmpWalkingLeft, m_bmpWalkingRight,
    m_bmpSwordUp, m_bmpSwordDown, m_bmpSwordLeft, m_bmpSwordRight, m_bmpSpinning;

    AnimatedSprite* m_bmpCurrentPtr;

    Sprite m_bmpShadow;

    HitRegion *m_HitRegionPtr, *m_SwordRegionPtr;

    Sound m_sndSword;

    SimpleJoy* input;

};

#endif // ARENAHERO_H
