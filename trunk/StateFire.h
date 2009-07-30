
#ifndef FIRE_H_INCLUDED
#define FIRE_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.

#include <vector>
#include "LUT.h"
#include "Background.h"
#include "Emitter.h"
#include "Sprite.h"

#ifdef PLATFORM_GP2X
    #include "MMUHack.h"
#endif

#define     FIRE_MAX_ACCEL              0.7f
#define     FIRE_GRAVITY                0.5f
#define     FIRE_MAX_VEL_UP             -10.5f
#define     FIRE_MAX_VEL_DN             8.2f
#ifdef PENJIN_FIXED
#define     FIRE_MAX_ANGLE              (Fixed)(45)
#else
#define     FIRE_MAX_ANGLE              45
#endif
#define     FIRE_SCREEN_WIDTH           800
#define     FIRE_SCREEN_HEIGHT          480
#define     FIRE_FAIL                   0
#define     FIRE_PASS                   1
#define     FIRE_MAX_TOWERS		        10
#define     FIRE_MAX_ABSANGLE           90	// +/- deg
#define     FIRE_MIN_POWER		        0
#define     FIRE_MAX_POWER		        100
#define     FIRE_MAX_WATER		        100
#define     FIRE_MAX_WATERUSE	        1
#define     FIRE_MAX_WATERSHOT	        1
#define     FIRE_MAX_WATERPARTS         1000
#define     FIRE_MAX_SMOKEPARTS         1000
#define     FIRE_MAX_FIRESIZE	        100
#define     FIRE_MAX_FIRESHOT	        5
#define     FIRE_MAX_FIREREGEN	        5
#define     FIRE_MAX_FIRETIMER	        100
#define     FIRE_MAX_FLOORLIFE	        100
#define     FIRE_MAX_BURNTIMER	        100

#define     FIRE_SPRITE_FIRE_ROWS       3
#define     FIRE_SPRITE_FIRE_FRAMES     4
#define     FIRE_SPRITE_FLOOR_ROWS      4
#define     FIRE_SPRITE_FLOOR_FRAMES    1
#define     FIRE_GROUND_HEIGHT          10
#define     FIRE_CANNON_HEIGHT          50
#define     FIRE_TRUCK_WIDTH            20


enum results_t {
	RESULT_NONE,
	RESULT_NOWATER,
	RESULT_NOFIRES,
	RESULT_NOFLOORS
};

struct sFire {
	int16_t x, y;
	int8_t	size;
	int8_t	regentimer;
};

struct sTower {
	int16_t x, y;
	vector<int8_t>  floors;
	int8_t          burntimer;
	sFire           fire;
    Emitter	        smokeblack;
    Emitter	        smokewhite;
};

struct sCannon {
	uint8_t	power;
	int8_t	angle;
	uint8_t	water;
	bool	shooting;
	bool	moving;
};



class Fire : public BaseState
{
    public:
        Fire();
        virtual void init();
#ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
        virtual void pauseScreen(SDL_Surface* screen);
#else
        virtual void render();
        virtual void pauseScreen();
#endif
        virtual void update();
        virtual void onPause();
        virtual void onResume();

        virtual ~Fire();
        virtual void userInput();

#ifdef PLATFORM_GP2X
		MMUHack hack;
#endif
    private:
        Text                text;
        Text                pauseText;
        Emitter             water;
        Background			background;
        Sprite				sprCannon;
        Image				imgFire;
        Image				imgFloor;
        sCannon             cannon;
        sTower              towers[FIRE_MAX_TOWERS];;
        bool moving;
        bool shooting;

        void SetupGame( int8_t lvl );
        results_t CheckGameConditions( void );
        int8_t GameResult( results_t result );
        void FireRegen( void );
        void FloorBurn( void );
        void MoveParticles( void );
        void ShootWater( void );
#ifdef PENJIN_SDL
        void RenderFloorsAndFires( SDL_Surface* screen );
        void RenderCannon( SDL_Surface* screen );
#else
        void RenderFloorsAndFires( void );
        void RenderCannon( void );
#endif
};

#endif // FIRE_H_INCLUDED
