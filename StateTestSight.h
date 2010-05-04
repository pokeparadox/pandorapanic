#ifndef STATETESTSIGHT_H_INCLUDED
#define STATETESTSIGHT_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.

#include "Sprite.h"
#include "Background.h"
#include "NumberUtility.h"
#include "Text.h"
#include "AnimatedSprite.h"

#define CUPS_AMOUNT 5
#define LEFT_CUP_X 200
#define CUPS_Y 140
#define CUP_OFFSET 80
#define MAX_Y_OFFSET 30
#define SHOW_FRAMES_MOVE 20
#define SHOW_FRAMES_HOLD 20
#define SHOW_HEIGHT 40

/***************************************************
    Test Your Sight minigame for PandoraPanic!
    Graphics by Cyrax
    Code by foxblock
***************************************************/

class StateTestSight : public mgBaseState
{
    public:
        StateTestSight();
        virtual ~StateTestSight();
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


        virtual void userInput();
    private:
        void swapCups(CRuint index1, CRuint index2);
        void showCup(CRuint index);
        void moveCups();
        float getYOffset(CRuint index);
        bool hasFinished(CRuint index) const;
        bool isMoving(CRuint index) const;
        bool isMoving() const;

        enum CUP_IDENT
        {
            ciCurrentPos=0, // the current position (including any current transition)
            ciDestination, // destination (=current position if not moving)
            ciDifference, // initial position difference when swapping
            ciShowCounter // animation counter for showCup
        };

        float cups[CUPS_AMOUNT][4]; // using CUP_IDENT for the second array
        int diffLevel;
        int correctCup;
        int numShuffles;
        int selection;

        enum GAME_PHASE
        {
            gpInit,
            gpShow,
            gpShuffle,
            gpAnswer,
            gpConclusion,
            gpExit
        };

        int phase;

        Background bg;
        Sprite img_cup;
        Sprite hand;
        Sprite coin;
        Text pauseText;
        AnimatedSprite buttonSheet;
};

#endif // STATETESTSIGHT_H_INCLUDED
