#ifndef STATETITLE_H
#define STATETITLE_H

#include "mgBaseState.h"
#include "PenjinStates.h"

/*
This state is the PandoraPanic minigame selectiom screen
There are several options
PandoraPanic - play a random set of mini games with 4 lives
Pandordered - play mini games in order with 4 lives
SelectionBox - play individual minigames
*/

#include "Menu.h"   //  Menu system
#include "Music.h"
#include "Emitter.h"
#include "Background.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Random.h"
#include "Timer.h"
#include "ButtonPrompter.h"

class StateTitle : public mgBaseState
{
    public:
        StateTitle();
        virtual ~StateTitle();

        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface* screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void userInput();

    private:
        void getReady();
        void setupMainMenu();
        void mainMenu();

        SDL_Surface* bgBuffer;
        Emitter emit;
        Background back;
        Sprite splash;
        Sprite  startButton;

        Colour backColour;
        Colour newColour;
        Menu menu;
        int choice;
        bool showPrompt;
        bool splashDone;
        bool gradual;
        uint gradualScaler;
        bool spinLogo;
        bool scaleLogo;
        float angle;
        float angleDelta;
        Vector2df scale;
        Vector2df scaleDelta;
        bool returnToCentre;
        int showStart;

        Music music;
        Timer timer;
        Timer startTimer;
        ButtonPrompter prompt;
};

#endif // STATETITLE_H
