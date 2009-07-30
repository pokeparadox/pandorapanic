#include "agroState.h"
#include "StatePunchWrestler.h"


void AgroState::beforeInit( int outterLevelNumber ){
    levelNumber = outterLevelNumber;
}

#ifdef PENJIN_SDL
void AgroState::render(
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

    //test only
    if( scenario[scenarioCounter] == LEFT_HOOK ){
        aButton.render(screen);
    }
    if( scenario[scenarioCounter] == RIGHT_HOOK ){
        bButton.render(screen);
    }
    if( scenario[scenarioCounter] == KICK ){
        upArrow.render(screen);
    }

}
#else
void AgroState::render()
{}
#endif

void AgroState::update( StatePunchWrestler * s){
timer ++;
if(timer > 20){
        image = !image;
        timer =0;
}
loseTimer ++;
if( loseTimer > ennemyReactionTime ){
    s->goToLoseState();
}
if( scenarioCounter+1 > levelNumber ){
    // we cleared scenario !!!! won
    s->goToWinState();
}else
if(polled != UA_NONE ){
        if( (polled == UA_LEFT_HOOK && scenario[scenarioCounter] != LEFT_HOOK)
            || (polled == UA_RIGHT_HOOK && scenario[scenarioCounter] != RIGHT_HOOK)
            || (polled == UA_KICK && scenario[scenarioCounter] != KICK)
            ){
               s->goToLoseState();
        }else
        if( polled == UA_RIGHT_HOOK && scenario[scenarioCounter] == RIGHT_HOOK) {
               scenarioCounter++;
               s->goToPunchState(RIGHT_HOOK);
        }else
        if( polled == UA_LEFT_HOOK && scenario[scenarioCounter] == LEFT_HOOK) {
               scenarioCounter++;
               s->goToPunchState(LEFT_HOOK);
        }else
        if( polled == UA_KICK && scenario[scenarioCounter] == KICK) {
               scenarioCounter++;
               s->goToPunchState(KICK);
        }
}
//if( polled == UA_LEFT_HOOK || polled == UA_RIGHT_HOOK ){
//        s->goToPunchState();
//}

}

// only polling input here
void AgroState::userInput(SimpleJoy* input)
{
    //default
    polled = UA_NONE;


    if(input->isLeft() ){
        polled = UA_LEFT_HOOK;
    }

    if(input->isRight() ){

        polled = UA_RIGHT_HOOK;
    }

    if( input->isUp() ){
        polled = UA_KICK;
    }

}

void AgroState::init(int speed){
//        agro.loadSprite("images/MadWrestlerPunch/agro.png");
    agro.loadSprite("images/MadWrestlerPunch/agro.png");
    agro2.loadSprite("images/MadWrestlerPunch/agro2.png");
    aButton.loadSprite("images/MadWrestlerPunch/a_button.png");
    bButton.loadSprite("images/MadWrestlerPunch/b_button.png");
    upArrow.loadSprite("images/MadWrestlerPunch/up_arrow.png");
    agro.setPosition(144,96);
    agro2.setPosition(144,96);
    aButton.setPosition(20,200);
    bButton.setPosition(600,200);
    upArrow.setPosition(400,0);

    image = false;
    timer = 0;
    loseTimer = 0;

    ennemyReactionTime = 120;

    //level number defines the number of slots we fill
    //populating scenario
    int i;
    int scenarioLength;
    if(levelNumber <= 20 ){
        scenarioLength= levelNumber;
    }else{
            //only 20 slots
            scenarioLength = 20;
    }

    for( i=0; i<scenarioLength;i++){
        int random = rand()%10;
        if( random <3 ){
        scenario[i]= LEFT_HOOK;
            }else if (random <7){
                scenario[i]=RIGHT_HOOK;
            }else{
                scenario[i]=KICK;
            }
    }

    // initing counter for current play item in scenario
    scenarioCounter = 0;

}
