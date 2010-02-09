#ifndef STATESPELLINGCONTEST_H_INCLUDED
#define STATESPELLINGCONTEST_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    pstudio's Spelling Contest mini game for Pandora Panic
*/

#include "Background.h"
#include "TextDoc.h"
#include "AnimatedSprite.h"
#include "Music.h"
#include "StringUtility.h"

class StateSpellingContest : public mgBaseState
{
    public:
        StateSpellingContest();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
            virtual void pauseScreen(SDL_Surface* screen);
        #else
            virtual void render();
        #endif
        virtual void update();

        virtual void onPause();
        virtual void onResume();

        virtual ~StateSpellingContest();
        virtual void userInput();
        // We do our own input handling in this mini game

    private:
        TextDoc doc;
        string word;
        uint wordLength;
        uint correctChars;
        int pv;

        Text text;
        Timer counter;
        uint sTime;
        Background background;
        AnimatedSprite boy;
        AnimatedSprite girl;
        int status; // 0 - info, 1 - game, 2 - end

        Music sound;

        #ifdef PENJIN_SDL
            void renderInfo(SDL_Surface *screen);
            void renderGame(SDL_Surface *screen);
            void renderEnd(SDL_Surface *screen);
        #else
            void renderInfo();
            void renderGame();
            void renderEnd();
        #endif

        void updateInfo();
        void updateGame();
        void updateEnd();

        bool gotoNext;
};

#endif // STATESPELLINGCONTEST_H_INCLUDED
