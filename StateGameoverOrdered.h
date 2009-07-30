#ifndef STATEGAMEOVERORDERED_H
#define STATEGAMEOVERORDERED_H

#include "userStates.h"     // Make sure your state is listen in this include.
#include "ScoreScreen.h"
#include "Background.h"
#include "Text.h"
#include "Sound.h"
#include "Music.h"
#include "BackBuffer.h"
#include "Random.h"

/*This is the hi scores for the Pandoreed mode*/

class StateGameoverOrdered : public BaseState
{
    public:
        StateGameoverOrdered();
        virtual void init();
        virtual void clear();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void userInput();
        virtual ~StateGameoverOrdered();

	private:
        Background back;
        Sprite logo;
        SDL_Surface* bgBuffer;
        BackBuffer buff;
        ScoreScreen scoreScreen;
		Music music;
		Text text;
		vector <Emitter> emit;
};
#endif	//	STATEGAMEOVERORDERED_H
