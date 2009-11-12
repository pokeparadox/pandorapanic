#include "StatePunchWrestler.h"
StatePunchWrestler::StatePunchWrestler()
{

    //new , is to me


    current = &agroState;


    //mostly to clean
    nullify = false;

    counter.setMode(SECONDS);



    //textobject for instructions
    text.loadFont("font/bip.ttf", 24);
    text.setPosition(50,20);


}

void StatePunchWrestler::goToPunchState(blow b){

    current = &punchState;
    punchState.setDisplay(b,levelNumber);

}

void StatePunchWrestler::goToAgroState(){

    current = &agroState;
    // agro state holds all game data and should not be inited again
    // that would mean overwritten
//    current->init();
}

void StatePunchWrestler::goToLoseState(){

    current = &loseState;
//    current->init(levelNumber);
    loseState.loseSound();
}

void StatePunchWrestler::goToWinState(){
text.setPosition(50,20);
    current = &winState;
//    current->init(levelNumber);
    winState.winSound();
}

void StatePunchWrestler::goToNextMiniGameWin(){
    variables[0].setInt(1);
    setNextState(STATE_MAIN);
}

void StatePunchWrestler::goToNextMiniGameLose(){
    variables[0].setInt(0);
    setNextState(STATE_MAIN);
}



StatePunchWrestler::~StatePunchWrestler()
{

}

void StatePunchWrestler::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        music.loadMusic("music/PunchWrestler/pw_lv2.ogg");
        music.setLooping(true);
        music.play();
    }
        levelNumber = (variables[2].getInt() / 5)+0.5f;
        if (levelNumber < 1){levelNumber = 1;}

    //mine
    agroState.beforeInit(levelNumber);
    agroState.init(levelNumber);
    punchState.init(levelNumber);
    winState.init(levelNumber);
    loseState.init(levelNumber);
    counter.start();
}

void StatePunchWrestler::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    // The pausecheck
    if (input->isStart())
    {
        isPaused = !isPaused;
        input->resetKeys();
    }

    //useless right now
    current->userInput(input);

//    if(input.isA() && current == &agroState)
//    {
//        goToPunchState();
//    }
}



void StatePunchWrestler::onPause()
{
    counter.pause();
}

void StatePunchWrestler::onResume()
{
    counter.unpause();
}

#ifdef PENJIN_SDL
void StatePunchWrestler::render(SDL_Surface *screen)
{
    current->render(screen);
}

void StatePunchWrestler::pauseScreen(SDL_Surface* screen)
{

    text.setPosition(52,182);
    text.setColour(Colour(WHITE));
    text.print(screen, "Push the D-Pad button corresponding to the arrow on screen");
    text.setPosition(50,180);
    text.setColour(Colour(BLACK));
    text.print(screen, "Push the D-Pad button corresponding to the arrow on screen");
}
#else
void StatePunchWrestler::render()
{
    current->render();
}

void StatePunchWrestler::pauseScreen()
{
    pauseSymbol();
    text.setPosition(52,182);
    text.setColour(Colour(WHITE));
    text.print("Push the D-Pad button corresponding to the arrow on screen");
    text.setPosition(50,180);
    text.setColour(Colour(BLACK));
    text.print("Push the D-Pad button corresponding to the arrow on screen");
}
#endif

void StatePunchWrestler::update()
{
    current->update(this);
}
