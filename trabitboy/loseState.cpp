#include "loseState.h"
#include "StatePunchWrestler.h"
#include "userStates.h"     // Make sure your state is listed in this include.//images/MadWrestlerPunch/agro.png"

#ifdef PENJIN_SDL
void LoseState::render(
SDL_Surface *screen
){
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,255,255,255));

    if(image){

        //display first pic
//        printf("first pic");
        agro.render(screen);
    }else{
        //display second pic
//        printf("second pic ");
        agro2.render(screen);
    }
}
#else
void LoseState::render()
{
    //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,255,255,255));
    if(image)
        agro.render();
    else
        agro2.render();
}
#endif

void LoseState::loseSound(){
    falseMove.play();
}

void LoseState::update( StatePunchWrestler * s){
timer ++;
if(timer > 20){
        image = !image;
        timer =0;
}
nextStateTimer++;
if( nextStateTimer > 120 ){

    s->goToNextMiniGameLose();
}

}

// doesn't work
void LoseState::userInput(SimpleJoy* input){

//    Input.update();
//    if( Input.isLeft() ){
//        printf("left \n ");
//    }

//    Input.update();

//    if(Input.isA() ){
//        printf("agro state, a pushed ");
//        goToPunchFlag = true;
//    }
}

void LoseState::init(int speed){
//        agro.loadSprite("images/MadWrestlerPunch/agro.png");
    agro.loadSprite("images/MadWrestlerPunch/lose2.png");
    agro2.loadSprite("images/MadWrestlerPunch/lose.png");
    agro.setPosition( 144,96);
    agro2.setPosition( 144,96);
    image = false;
    timer = 0;
    nextStateTimer = 0;
    falseMove.loadMusic("music/MadWrestlerPunch/falsemove.ogg");
    falseMove.setLooping(false);
}
