#include "mgBaseState.h"

mgBaseState::mgBaseState()
{
    //ctor
    storedVolume = 100;
    uint xRes = GFX::getXResolution();
    uint yRes = GFX::getYResolution();
    const string path = "images/";
    const string ext = ".png";
    muteSprite.loadFrames(path+"sys/mute"+ext,2,1);
    muteSprite.setPosition(xRes - (xRes*0.04f) - muteSprite.getWidth(),yRes*0.04f);
    muteSprite.setCurrentFrame(0);

    buttonSheet.loadFrames(path+"ButtonPrompter/ButtonsSheet"+ext,10,2);

    exitSprite.loadSprite(path+"sys/exit"+ext);
    exitSprite.setPosition(xRes - (xRes*0.04f) - exitSprite.getWidth(),yRes - (yRes*0.04f) - exitSprite.getWidth());
    titleSprite.loadSprite(path+"sys/title"+ext);
    titleSprite.setPosition(xRes - (xRes*0.04f) - titleSprite.getWidth(),exitSprite.getY() - exitSprite.getWidth()-2);
}

mgBaseState::~mgBaseState()
{
    //dtor
}

void mgBaseState::buttonsOverlay(SDL_Surface* screen)
{
    if(!isSubState())
    {
        buttonSheet.setPosition(muteSprite.getX()-buttonSheet.getWidth()-2,muteSprite.getY());
        buttonSheet.setCurrentFrame(19); // R
        buttonSheet.render(screen);
        muteSprite.setCurrentFrame(mixer.getGlobalVolume() == 0);
        muteSprite.render(screen);

        buttonSheet.setPosition(exitSprite.getX()-2*buttonSheet.getWidth(),exitSprite.getY());
        buttonSheet.setCurrentFrame(18); // L
        buttonSheet.render(screen);
        buttonSheet.setPosition(exitSprite.getX()-buttonSheet.getWidth(),exitSprite.getY());
        buttonSheet.setCurrentFrame(13); // X
        buttonSheet.render(screen);
        exitSprite.render(screen);

        buttonSheet.setPosition(titleSprite.getX()-2*buttonSheet.getWidth(),titleSprite.getY());
        buttonSheet.setCurrentFrame(18); // L
        buttonSheet.render(screen);
        buttonSheet.setPosition(titleSprite.getX()-buttonSheet.getWidth(),titleSprite.getY());
        buttonSheet.setCurrentFrame(12); // X
        buttonSheet.render(screen);
        titleSprite.render(screen);
    }
}

void mgBaseState::muteToggle()
{
    Music music;
    if(mixer.getGlobalVolume() != 0)
    {
        storedVolume = mixer.getGlobalVolume();
        mixer.setGlobalVolume(0);
        music.setVolume(0);
        muteSprite.setCurrentFrame(1);
    }
    else
    {
        mixer.setGlobalVolume(storedVolume);
        music.setVolume(storedVolume);
        muteSprite.setCurrentFrame(0);
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
        render();
        muteToggle();
        input->resetKeys();
    }


    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}

bool mgBaseState::isSubState()
{
    return !(variables.size()<SUBSTATE_TRIGGER);
}

