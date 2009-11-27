#ifndef StateDarkTreasure_H
#define StateDarkTreasure_H

#include "PenjinStates.h"

#include "Raycast.h"
#include "Background.h"
#include "Music.h"


class StateDarkTreasure : public BaseState
{
    public:
        StateDarkTreasure();
        virtual ~StateDarkTreasure();

        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface* screen);
        #else
            virtual void render();
        #endif
        virtual void userInput();

    private:

        Raycast* m_Engine;

        Background m_Border;

        Music m_Music;
};

#endif // StateDarkTreasure_H
