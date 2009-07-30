#ifndef LOSE_STATE
#define LOSE_STATE

#include "trabitboy/activeGameSubState.h"
#include "Sprite.h"
#include "Background.h"
#include "SimpleJoy.h"
#include "Music.h"
class StatePunchWrestler;

class LoseState : public ActiveGameSubState{
        Sprite agro,agro2;
bool image;
int timer ;
int nextStateTimer;
Music falseMove;


public:
#ifdef PENJIN_SDL
void render(
SDL_Surface *screen
);
#else
void render();
#endif

void loseSound();
void update(StatePunchWrestler * s);

void userInput(SimpleJoy* input);

void init(int speed);


};
#endif
