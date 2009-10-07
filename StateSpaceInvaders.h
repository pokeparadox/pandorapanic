#ifndef STATESPACEINVADERS_H_INCLUDED
#define STATESPACEINVADERS_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    pstudio's Space Invaders mini game for Pandora Panic
*/


#include "Background.h"
#include "Sprite.h"
#include "Emitter.h"
#include "AnimatedSprite.h"
class StateSpaceInvaders : public BaseState
{
    public:
        StateSpaceInvaders();
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


        virtual ~StateSpaceInvaders();
        virtual void userInput();

        Emitter emit;
        vector <Emitter> blast;

        Text text;
        Text command;
        Timer counter;
        Background background;
        Sprite ship;
        Sprite alien;
        Sprite shot;
        bool shooting;
        float alienDir;
        float shipVel;
        Text pauseText;
        AnimatedSprite buttonSheet;
};

#endif // STATESPACEINVADERS_H_INCLUDED
