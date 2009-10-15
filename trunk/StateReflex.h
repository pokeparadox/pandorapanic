#ifndef STATEREFLEX_H_INCLUDED
#define STATEREFLEX_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    oclbdk's Rhythm mini game for Pandora Panic
*/

class StateReflex : public BaseState
{
    public:
        StateReflex();
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

        virtual ~StateReflex();
        virtual void userInput();

        Text text;
        Text pauseText;
        Timer counter;
        int guide_xOffset;
        int gameDuration;
        int turnDuration;

        int currDirection;
        int DOWN, LEFT, UP, RIGHT;
        bool correct;
        int correctStart;

        Sprite circle;
        Sprite arrow;

        Sound leftSound;
        Sound rightSound;
        Sound upSound;
        Sound downSound;
};

#endif // STATEREFLEX_H_INCLUDED
