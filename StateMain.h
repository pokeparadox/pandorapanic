#ifndef STATEMAIN_H
#define STATEMAIN_H

/*
This is a stepping stone state that will always run in between each mini game.
It performs differing tasks depending on what has been stored in variables.
*/
#include "userStates.h"
#include "MiniGames.h"
#include "Random.h"
#include "Sprite.h"
#include "Colour.h"
#include "Music.h"
#include "ButtonPrompter.h"

class StateMain : public BaseState
{
    public:
        StateMain();
        virtual ~StateMain();

        virtual void init();
        virtual void update();
        virtual void userInput();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface* screen);
        #else
            virtual void render();
        #endif

    private:
        void loadCommon();  //  Loads common resources
        void winLoseLoad(); //  Loads resources needed in both winning and losing.
        void loadWin();     //  Loads resources only needed if a game is won
        void loadLose();    //  Loads resources neededif a game is lost
        void genPreview(uint next);  // Load a substate for previewing purposes.

        SDL_Surface* bgBuffer;
        SDL_Surface* preview;
        bool gotoNext;
        int selection;
        Text text;
        Sprite logo;
        Sprite bar;
        Sprite loseWin;
        Sprite life;
        Sprite lostLife;
        Background back;
        Colour backColour;
        Music music;

        bool playWin;
        bool playFail;
        Timer timer;
        ButtonPrompter prompt;
        Vector2di upArrow, downArrow, leftArrow, rightArrow, bButton;
};

#endif // STATEMAIN_H
