#ifndef MGBASESTATE_H
#define MGBASESTATE_H

#include "MiniGames.h"
#include "Text.h"
#include "Sound.h"
#include "Music.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "BaseState.h"

//  Provides functionality in the pause screen of the minigames
class mgBaseState : public BaseState
{
    public:
        mgBaseState();
        virtual ~mgBaseState();


    protected:
        void buttonsOverlay(SDL_Surface* screen);
        void muteToggle();
        void quitToTitle();
        void quit();

        bool isSubState();

        virtual void pauseInput();
        Text pauseText;
        AnimatedSprite buttonSheet;
        AnimatedSprite muteSprite;
        Sprite exitSprite;
        Sprite titleSprite;
        Sound mixer;
        int storedVolume;
};

#endif // MGBASESTATE_H
