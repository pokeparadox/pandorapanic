#ifndef AGRO_STATE
#define AGRO_STATE
#include "trabitboy/activeGameSubState.h"
#include "Sprite.h"
#include "Background.h"
#include "SimpleJoy.h"
//#include "trabitboy/blowEnum.h"
#include "trabitboy/userActionEnum.h"

class StatePunchWrestler;

class AgroState : public ActiveGameSubState{
//        Sprite agro;
        Sprite agro,agro2;
int ennemyReactionTime;

bool image;
int timer, loseTimer, levelNumber, scenarioCounter ;
//bool goToPunchFlag,goToLoseFlag;
userAction polled; //user action polled is put here

Sprite aButton,bButton,upArrow;

//scenario
blow scenario[20];


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

//to pass down particular data
void beforeInit( int levelNumber );
void init(int speed);

};
#endif
