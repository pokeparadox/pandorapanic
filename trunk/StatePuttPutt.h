#ifndef STATEPUTTPUTT_H_INCLUDED
#define STATEPUTTPUTT_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    DasFool's Putt Putt mini game for Pandora Panic
    * basis borrowed from pstudio's Space Invaders
*/


#include "Background.h"
#include "Sprite.h"
#include "Emitter.h"

class StatePuttPutt : public BaseState
{
    public:
        StatePuttPutt();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void onPause();
        virtual void onResume();

        virtual ~StatePuttPutt();
        virtual void userInput();

        Text text;
        Text command;
        Timer counter;
        Background background;

        Sprite hole;
        Sprite ball;
        Sound putt;
        Sound sunk;
        Sound gasp;
        bool prepareForPutt;
        bool putting;
        bool traveling;
        int travelState;
        bool completed;
        int countdown;

        float gMeterX;
        float gMeterY;
        float gMeterW;
        float gMeterH;
        float focusVel;
        float focusY;
        float focusH;
        float sweetY;
        float sweetH;
        float ballVel;
};

#endif // STATEPUTTPUTT_H_INCLUDED