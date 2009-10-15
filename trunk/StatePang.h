#ifndef STATEPANG_H_INCLUDED
#define STATEPANG_H_INCLUDED

#include "userStates.h"

#include "AnimatedSprite.h"
#include "Text.h"

namespace PangMiniGame
{
    class PangGame;
};

class StatePang : public BaseState
{
    public:
        StatePang();

        virtual ~StatePang();

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

    private:

        void    MiniGameComplete( bool success );

        int     GetRequestedLevelNumber( );

        PangMiniGame::PangGame * m_pPangGame;
        Text pauseText;
        AnimatedSprite buttonSheet;
};

// Implement this or input won't work...
inline void StatePang::userInput( )
{
}

#endif // STATEPANG_H_INCLUDED
