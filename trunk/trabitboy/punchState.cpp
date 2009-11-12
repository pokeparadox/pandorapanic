#include "punchState.h"
#include "StatePunchWrestler.h"

#ifdef PENJIN_SDL
void PunchState::render(
SDL_Surface *screen
){
SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,255,255,255));
    if(image){

        //display first pic
        //printf("first pic");
        if( toDisplay == RIGHT_HOOK ){
            punch.render(screen);
        }else if( toDisplay == LEFT_HOOK ){
            left.render(screen);
        }else if( toDisplay == KICK ){
            kick.render(screen);
        }
    }else{
        //display second pic
        if( toDisplay == RIGHT_HOOK ){
            punch2.render(screen);
        }else if( toDisplay == LEFT_HOOK ){
            left2.render(screen);
        }else if( toDisplay == KICK ){
            kick2.render(screen);
        }
    }
}
#else
void PunchState::render()
{
    if(image){

        //display first pic
        //printf("first pic");
        if( toDisplay == RIGHT_HOOK ){
            punch.render();
        }else if( toDisplay == LEFT_HOOK ){
            left.render();
        }else if( toDisplay == KICK ){
            kick.render();
        }
    }else{
        //display second pic
        if( toDisplay == RIGHT_HOOK ){
            punch2.render();
        }else if( toDisplay == LEFT_HOOK ){
            left2.render();
        }else if( toDisplay == KICK ){
            kick2.render();
        }
    }
}
#endif

void PunchState::update( StatePunchWrestler * s ){
timer ++;
nextStateTimer++;
if(timer > 20/timeFactor){
        image = !image;
        timer =0;
   if( toDisplay == KICK ){
       mKick.play();
   }else if ( toDisplay == LEFT_HOOK ){
       mLeft.play();
   }else if ( toDisplay == RIGHT_HOOK ){
        mRight.play();
   }

}
if( nextStateTimer > 120/timeFactor){
    s->goToAgroState();

}
}

void PunchState::userInput(SimpleJoy* input){

}

void PunchState::setDisplay(blow b, int t){
    timer = 0;
    nextStateTimer = 0;
    toDisplay = b;
    timeFactor = t;
}

void PunchState::init(int speed){
//        agro.loadSprite("images/MadWrestlerPunch/agro.png");
    punch.loadSprite("images/MadWrestlerPunch/punch.png");
    punch2.loadSprite("images/MadWrestlerPunch/punch2.png");
    kick.loadSprite("images/MadWrestlerPunch/kick.png");
    kick2.loadSprite("images/MadWrestlerPunch/kick2.png");
    left.loadSprite("images/MadWrestlerPunch/left.png");
    left2.loadSprite("images/MadWrestlerPunch/left2.png");
    punch.setPosition(144,96);
    punch2.setPosition(144,96);
    kick.setPosition(144,96);
    kick2.setPosition(144,96);
    left.setPosition(144,96);
    left2.setPosition(144,96);

    mKick.loadSound("sounds/MadWrestlerPunch/pat.ogg");
    mLeft.loadSound("sounds/MadWrestlerPunch/bung.ogg");
    mRight.loadSound("sounds/MadWrestlerPunch/bung.ogg");

    image = false;
    timer = 0;
    nextStateTimer = 0;
}
