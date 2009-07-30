#ifndef PUNCH_STATE
#define PUNCH_STATE
#include "trabitboy/activeGameSubState.h"
#include "Sprite.h"
#include "Background.h"
#include "Sound.h"
//#include "trabitboy/blowEnum.h"
class StatePunchWrestler;


class PunchState : public ActiveGameSubState{
//        Sprite agro;
        Sprite punch,punch2,kick,kick2,left,left2;
Sound mKick,mLeft,mRight;
bool image;
long timer,  nextStateTimer ;
int timeFactor;
blow toDisplay;

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

void setDisplay( blow b , int timeFactor);

};
#endif
