#ifndef WIN_STATE
#define WIN_STATE

#include "trabitboy/activeGameSubState.h"
#include "Sprite.h"
#include "Background.h"
#include "SimpleJoy.h"
#include "Music.h"

class StatePunchWrestler;

class WinState : public ActiveGameSubState{
        Sprite agro,agro2;
bool image;
int timer ;
int nextStateTimer;
Music uWin;


public:
#ifdef PENJIN_SDL
void render(
SDL_Surface *screen
);
#else
void render();
#endif


void update(StatePunchWrestler * s);

void userInput(SimpleJoy* input);

void init(int speed);
void winSound();

};
#endif
