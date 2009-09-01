#ifndef STATEBALLDROP_H_INCLUDED
#define STATEBALLDROP_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    Zacaj's Balldrop mini game for Pandora Panic
    some modifications made by billykater
*/

#include "ButtonPrompter.h"

class StateBallDrop : public BaseState
{
    public:
        StateBallDrop();
        virtual void init();
    #ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
    #else
        virtual void render();
    #endif
        virtual void update();

        virtual ~StateBallDrop();
        virtual void userInput();

        char r,g,b;
        float ballx,bally;
        int x;
        int ballz;
        int cnt;
        float dir;
        Text text;
        float yadd;
        ButtonPrompter buttonPrompter;
        bool gotoNext;
        Music music;

        /////////////////////////////
        //Added by billykater
        const static unsigned int BALL_SPEED = 1;
        /////////////////////////////
};

#endif // STATEBALLDROP_H_INCLUDED