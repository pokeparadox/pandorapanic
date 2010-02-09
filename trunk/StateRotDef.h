#ifndef STATEROTDEF_H_INCLUDED
#define STATEROTDEF_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    B-ZaR's Rotating Turret mini game for Pandora Panic
*/

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
        #else
            virtual void render();
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
        Sound endSound;
        bool shooting;
        bool enemyKilled;
        bool playerKilled;
        bool gameEnd;
        int turretDirection;
        uchar turnSpeed;
        Vector2df shotPosition;
        Vector2df shotVelocity;
        Vector2df enemyPosition;
        Vector2df enemyVelocity;
};

#endif // STATESPACEINVADERS_H_INCLUDED
