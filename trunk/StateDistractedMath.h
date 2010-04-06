#ifndef STATEDISTRACTEDMATH_H_INCLUDED
#define STATEDISTRACTEDMATH_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    Distracted Math by MarkoeZ for the Pandora panic project

    Big chuncks rudely stolen from the Spelling Contest and Space Invaders ;)

    My first cpp experience

*/


#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"
#include "Random.h"

class StateDistractedMath : public mgBaseState
{
    public:
        StateDistractedMath();
        virtual void init();
    #ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
        virtual void pauseScreen(SDL_Surface *screen);
    #else
        virtual void render();
        virtual void pauseScreen();
    #endif
        virtual void update();
        virtual void userInput();
        virtual void onPause();
        virtual void onResume();
        virtual ~StateDistractedMath();


        Timer inputlimiter;
        Text text;
        Text command;
        Text telfont;
        Timer teller;
        Background background0;
        Background background1;
        Background background2;
        Background background3;

        Music backsound;

        bool hasWon;

        int losestate;
        int mathstate;
        int rnd;
        int back;
        int status;
        int oper;
        int randnumberbig;
        int number;
        int number2;
        int number3;
        //int tv;
        int round;
        int quickrand;
        int victory;
        int level;
        int difficulty;
        string calculation;
        string word;
        string usrInput;

        float MathInputUnicode;
        int MathInputNumeric;
};

#endif // STATEDISTRACTEDMATH_H_INCLUDED
