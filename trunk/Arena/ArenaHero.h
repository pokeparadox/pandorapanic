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
    void userInput(SimpleJoy* input);
    void update();
    HitRegion* GetHitRegion();
    HitRegion* GetSwordRegion();

    int GetX();
    int GetY();
    bool GetSwinging();


    //-------------------------------------------------
	// Private Methods
	//-------------------------------------------------
    void HandleFacing(SimpleJoy* input);
    void HandleCurrentImage(SimpleJoy* input);
    void Move(int x, int y);
    void SwingSword();

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
    int m_X, m_Y, m_Direction;
    bool m_IsSwinging;
    AnimatedSprite m_bmpStandingUp, m_bmpStandingDown, m_bmpStandingLeft, m_bmpStandingRight, m_bmpWalkingUp, m_bmpWalkingDown, m_bmpWalkingLeft, m_bmpWalkingRight;
    AnimatedSprite m_bmpSwordUp, m_bmpSwordDown, m_bmpSwordLeft, m_bmpSwordRight;

    AnimatedSprite* m_bmpCurrentPtr;

    Sprite m_bmpShadow;

    HitRegion *m_HitRegionPtr, *m_SwordRegionPtr;

    Sound m_sndSword;

};

#endif // ARENAHERO_H
