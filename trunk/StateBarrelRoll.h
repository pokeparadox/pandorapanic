#ifndef STATEBARRELROLL_H_INCLUDED
#define STATEBARRELROLL_H_INCLUDED


#include "userStates.h"     // Make sure your state is listed in this include.

#include "AnimatedSprite.h"
#include "Background.h"

class StateBarrelRoll : public BaseState
{
    public:
        StateBarrelRoll();
        virtual ~StateBarrelRoll();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface* screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void userInput();

    private:
    Sprite roller;
    Background bg;

    float zoom;

    float ang;
};

#endif // STATEBARRELROLL_H_INCLUDED
