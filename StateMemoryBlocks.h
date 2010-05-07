#ifndef STATEMEMORYBLOCKS_H_INCLUDED
#define STATEMEMORYBLOCKS_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    MemoryBlocks game
*/
#include "Background.h"
#include "Image.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Rectangle.h"
#include <vector>
#include <string>

#define SHOW_TIME 3000

class StateMemoryBlocks : public mgBaseState
{
    public:

        struct BlockSlot
        {
            BlockSlot(): current(0), target(0) {}
            BlockSlot(int c, int t): current(c), target(t) {}
            int current;
            int target;
        };

        StateMemoryBlocks();
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

        virtual ~StateMemoryBlocks();
        virtual void userInput();

        Text command;
        Text timer;
        Timer counter;
        Background background;
        std::vector<BlockSlot> combination;
        unsigned int combinationLength;
        unsigned int inputTime;
        std::vector<Image*> blocks;
        Image slot;
        Sprite correct;
        Sprite wrong;
        AnimatedSprite cursor;
        int currentSlot;
        int endStatus;
        bool skipToEnd;
};

#endif // STATESPACEINVADERS_H_INCLUDED
