#include "StateBarrelRoll.h"


StateBarrelRoll::StateBarrelRoll()
{
    nullify = false;
    roller.loadSprite("images/Barrel Roll/Bullet.png");
    roller.setPosition(368,216);
    roller.setScale(1.0f);
    zoom = 1.0;
    ang = 0.0;
}


StateBarrelRoll::~StateBarrelRoll()
{

}

void StateBarrelRoll::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(input->isLeft())
    {
        ang += 0.3;
        //Input.resetKeys();
    }
    if(input->isRight())
    {
        ang -= 0.3;
        //Input.resetKeys();
    }

    if(input->isUp())
    {
       zoom += 0.001;
    }

    if(input->isDown())
    {
        if(zoom > 0.3)
        {
            zoom -= 0.001;
        }
    }

    if(input->isSelect())//I wasnt sure what exit key was...
    {
        setNextState(STATE_MAIN);
        //nullifyState();
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateBarrelRoll::render(SDL_Surface *screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
	roller.render(screen);
}
#else
    void StateBarrelRoll::render()
    {
        roller.render();
    }
#endif

void StateBarrelRoll::update()
{

    roller.setRotation(ang);
    roller.setScale(zoom);

    //setNextState(STATE_MAIN);
}
