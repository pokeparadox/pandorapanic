#ifndef STATEROTDEF_H_INCLUDED
#define STATEROTDEF_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    B-ZaR's Rotating Turret mini game for Pandora Panic
*/

#ifdef _DEBUG
#define     ROTDEF_DEBUG  1
#endif

#include "Background.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Sound.h"
#include "LUT.h"

class StateRotDef : public mgBaseState
{
    public:
        StateRotDef();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
            virtual void pauseScreen(SDL_Surface* screen);
            #if ROTDEF_DEBUG
            void RenderDebug( SDL_Surface* screen );
            #endif
        #else
            virtual void render();
            #if ROTDEF_DEBUG
            void RenderDebug();
            #endif
        #endif
        virtual void update();
        virtual void onPause();
        virtual void onResume();
        virtual ~StateRotDef();
        virtual void userInput();

        Text command;
        Timer counter;
        Background background;
        Sprite turret;
        Sprite turretBase;
        Sprite enemy;
        Sprite shot;
        AnimatedSprite explosion;
        Sound rotateSound;
        Sound shotSound;
        Sound winSound;
        Sound loseSound;
        bool shooting;
        bool enemyKilled;
        bool playerKilled;
        bool gameEnd;
        float turretDirection;
        uchar turnSpeed;
        Vector2df shotPosition;
        Vector2df shotVelocity;
        Vector2df enemyPosition;
        Vector2df enemyVelocity;
#if ROTDEF_DEBUG
        #define DEBUG_STRING_MAX 100
        Text debugText;
        char debug_bulletstats[DEBUG_STRING_MAX];
#endif
};

#endif // STATESPACEINVADERS_H_INCLUDED
