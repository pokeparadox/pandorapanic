#include "mgBaseState.h"

mgBaseState::mgBaseState()
{
    //ctor
    storedVolume = 100;
}

mgBaseState::~mgBaseState()
{
    //dtor
}

void mgBaseState::buttonsOverlay(SDL_Surface* screen)
{

}

void mgBaseState::muteToggle()
{
    Music music;
    if(mixer.getGlobalVolume() != 0)
    {
        storedVolume = mixer.getGlobalVolume();
        mixer.setGlobalVolume(0);
        music.setVolume(0);
    }
    else
    {
        mixer.setGlobalVolume(storedVolume);
        music.setVolume(storedVolume);
    }
}

void mgBaseState::quitToTitle()
{
    nullifyState();
}

void mgBaseState::quit()
{
    setNextState(2);//STATE_TITLE
}

void mgBaseState::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if(input->isL() && input->isX())
        quit();
    else if(input->isL() && input->isY())
        quitToTitle();

    if(input->isR())
    {
        muteToggle();
        input->resetKeys();
    }


    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}
