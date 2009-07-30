#ifndef ACTIVE_GAME_SUBSTATE
#define ACTIVE_GAME_SUBSTATE
#include <SDL/SDL.h>
#include "SimpleJoy.h"
#include "trabitboy/blowEnum.h"

class StatePunchWrestler;

class ActiveGameSubState {


	public:
        virtual void init(int speed)=0;
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen)=0;
        #else
            virtual void render()=0;
        #endif
        virtual void update(StatePunchWrestler * s)=0;
        virtual void userInput(SimpleJoy* input)=0;

};
#endif
