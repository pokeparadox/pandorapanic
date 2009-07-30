#ifndef STATEDOUBLENUBTROUBLE_H_INCLUDED
#define STATEDOUBLENUBTROUBLE_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    nEW pROJECT

*/


#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"
#include "Random.h"

class StateDoubleNubTrouble : public BaseState
{
    public:
        StateDoubleNubTrouble();
        virtual void init();
    #ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
    #else
        virtual void render();
    #endif
        virtual void update();
        virtual void userInput();
        virtual void onPause();
        virtual void onResume();
    #ifdef PENJIN_SDL
        virtual void pauseScreen(SDL_Surface* screen);
    #else
        virtual void pauseScreen();
    #endif
        virtual ~StateDoubleNubTrouble();

    private:
        Timer inputlimiter;
        Vector2di scope;
        Vector2di scope2;


        Background background;
        Sprite mysprite[10];

        Sprite target[10];

        Sound shot;

        int nubstate;
        int fallvel;
        int birdnumber;
        int tijdelijk;

        Text test;
        Text messageText;
        Text text;
        Timer teller;
        Timer shotdelay;

        int timelimit;

        int ronde;

        float birdangle;

        string usrinput;

        float vertvel;


        Vector2di targetpos[10];


        int birddead[10];
        int killcount;
        int penalty;



        int losestate;
        int tv;
};

#endif // STATEDOUBLENUBTROUBLE_H_INCLUDED
