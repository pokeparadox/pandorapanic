#ifndef STATESNATCHABEER_H_INCLUDED
#define STATESNATCHABEER_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
   foxblock's attemt to create a minigame for PandoraPanic
*/


#include "Background.h"
#include "Sprite.h"

class StateSnatchABeer : public BaseState
{
    public:
        enum GameStates {
            IDLE,
            WIN,
            WIN2,
            NOTRY,
            MISS,
            MISS2,
            NOTRY2,
            MISSCLOSE
        };

        StateSnatchABeer();
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


        virtual ~StateSnatchABeer();

        virtual void userInput();

        Timer counter;
        Background background;
        AnimatedSprite enemy;
        AnimatedSprite player;
        Sprite bar;
        Sprite beer;
        Sprite action;
        Music barmusic;
        Sound sGlass;
        Sound sEnemy;
        Sound sPlayer;
        Text pauseText;
        bool started;
        bool animationStarted;
        bool playOnce;
        bool secondChance;
        GameStates status;
        int animFrame;
        int gravity;
        int winZones[3][2];
        float delay;
        float angle;
};

#endif // STATESNATCHABEER_H_INCLUDED
