#ifndef STATEPUNCHWRESTLER_H_INCLUDED
#define STATEPUNCHWRESTLER_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    PunchWrestler
*/

#include "Background.h"
#include "Sprite.h"
//#include "ButtonPrompter.h"
#include "TextDoc.h"

#include "trabitboy/agroState.h"
#include "trabitboy/punchState.h"
#include "trabitboy/winState.h"
#include "trabitboy/loseState.h"

class StatePunchWrestler : public mgBaseState
{
    public:
        StatePunchWrestler();
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

        virtual ~StatePunchWrestler();
        virtual void userInput();

        Text text;

        //new
        AgroState agroState;
        PunchState punchState;
        WinState winState;
        ActiveGameSubState * current;
        LoseState loseState;
        Music music;

        int levelNumber;
        //afterwards is probably to strip

        Timer counter;

    //mine
    void goToPunchState(blow b);
    void goToAgroState();
    void goToWinState();
    void goToLoseState();
    void goToNextMiniGameWin();
    void goToNextMiniGameLose();
};

#endif // STATEPUNCHWRESTLER_H_INCLUDED
