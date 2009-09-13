
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

#define     FIRE_FAIL                   0
#define     FIRE_PASS                   1
#define     FIRE_MAX_TOWERS		        10
#define     FIRE_MAX_FLOORS		        6

#define     FIRE_DEBUG                  1

enum results_t {
	RESULT_NONE,
	RESULT_NOWATER,
	RESULT_NOFIRES,
	RESULT_NOFLOOR
};

enum pickle_states_t {
	STATE_INACTIVE,
	STATE_TOWERTO,
	STATE_TOWERFROM,
	STATE_FIRE_START,
    STATE_FIRE_ACTIVE
};

struct sFire {
	int16_t x, y;
	int8_t	size;
	int8_t	regentimer;
	int8_t  frame;
};

struct sTower {
	int16_t x, y;
	vector<int8_t>  floors;
	int8_t          burntimer;
	int16_t         smoketimer;
    Emitter	        smokeblack;
    Emitter	        smokewhite;
	sFire           fire;
};

struct sCannon {
	int8_t	power;
	int8_t	angle;
	int8_t	chgrate;
	int16_t	water;
	bool	shooting;
	bool	moving;
	int16_t tip_x;
	int16_t tip_y;
    Vector2df velocity;
    Vector2df acceleration;
};

struct sWater {
    int16_t x, y;
    int16_t vel_x;
    int16_t vel_y;
};

struct sPickle {
    int16_t x, y;
    uint8_t target;
    uint8_t state;
    uint8_t dir;
    uint8_t frame;
    uint8_t lpf;
    sFire   fire;
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
        Text                debugText;
        vector<sWater>      water;
        Background			background;
        Sprite				sprCannon;
        Sprite				sprWater;
        Image				imgFire;
        Image				imgFloor;
        Image				imgPickle;
        sCannon             cannon;
        sTower              towers[FIRE_MAX_TOWERS];
        sPickle             pickle;
        bool                moving;
        bool                shooting;
        bool                firstrun;
        int16_t             screen_width;
        int16_t             screen_height;

#if FIRE_DEBUG
        #define DEBUG_STRING_MAX 100
        char debug_firestats[FIRE_MAX_TOWERS][DEBUG_STRING_MAX];
        char debug_floorstats[FIRE_MAX_TOWERS][DEBUG_STRING_MAX];
        char debug_cannonstats[DEBUG_STRING_MAX];
#endif
        void SetupGame( int8_t lvl );
        results_t CheckGameConditions( void );
        int8_t GameResult( results_t result );
        void FireRegen( void );
        void FloorBurn( void );
        void PickleAI( void );
        void MoveParticles( void );
        void ShootWater( void );
        bool CheckRectCollision( SDL_Rect* boxA, SDL_Rect* boxB );
        int16_t RandomValue( int16_t low, int16_t high );
#ifdef PENJIN_SDL
        void RenderSprites( SDL_Surface* screen );
        void RenderCannon( SDL_Surface* screen );
    #if FIRE_DEBUG
        void RenderDebug( SDL_Surface* screen );
    #endif
#else
        void RenderFloorsAndFires( void );
        void RenderCannon( void );
    #if FIRE_DEBUG
        void RenderDebug( void );
    #endif
#endif
};

#endif // FIRE_H_INCLUDED
