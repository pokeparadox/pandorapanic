#ifndef STATEACHIEVEMENTS_H
#define STATEACHIEVEMENTS_H

#include "userStates.h"     // Make sure your state is listen in this include.

#include "AchievementSystem.h"

#include "Background.h"
#include "BackBuffer.h"
#include "Random.h"
#include "Text.h"
#include "AnimatedSprite.h"
#include "StringUtility.h"

class StateAchievements : public BaseState
{
    public:
        StateAchievements();
        virtual ~StateAchievements();
        virtual void init();
        virtual void clear();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void userInput();

	private:
        Background back;
        SDL_Surface* bgBuffer;
        BackBuffer buff;
        int offset;
        Text text;
        AnimatedSprite buttonSheet;
};
#endif	//	STATEACHIEVEMENTS_H
