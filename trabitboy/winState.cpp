#include "winState.h"
#include "StatePunchWrestler.h"
#include "userStates.h"     // Make sure your state is listed in this include.//images/MadWrestlerPunch/agro.png"

#ifdef PENJIN_SDL
void WinState::render(
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
void WinState::render()
{
    if(image)
        agro.render();
    else
        agro2.render();
}
#endif

void WinState::update( StatePunchWrestler * s){
timer ++;
if(timer > 20){
        image = !image;
        timer =0;
}
nextStateTimer++;
if( nextStateTimer > 120 ){

    s->goToNextMiniGameWin();
}

}

// doesn't work
void WinState::userInput(
SimpleJoy* input){


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

void WinState::winSound(){
        uWin.play();
}


void WinState::init(int speed){
//        agro.loadSprite("images/MadWrestlerPunch/agro.png");
    agro.loadSprite("images/MadWrestlerPunch/win.png");
    agro2.loadSprite("images/MadWrestlerPunch/win2.png");
    agro.setPosition( 144,96 );
    agro2.setPosition( 144,96 );
    image = false;
    timer = 0;
    nextStateTimer = 0;
    uWin.loadMusic("music/MadWrestlerPunch/uwin.ogg");
    uWin.setLooping(false);
}
