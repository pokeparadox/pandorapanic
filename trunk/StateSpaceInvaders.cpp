#include "StateSpaceInvaders.h"
StateSpaceInvaders::StateSpaceInvaders()
{
    nullify = false;

    text.loadFont("font/chromo.ttf", 48);
    text.setColour(Colour(YELLOW));
    command.loadFont("font/origami.ttf",48);
    command.setRelativity(false);
    command.setPosition(315,216);
    counter.setMode(SECONDS);
    alien.loadSprite("images/SpaceInvaders/alien.png");
    ship.loadSprite("images/SpaceInvaders/ship.png");
    shot.loadSprite("images/SpaceInvaders/shot.png");
    background.loadBackground("images/SpaceInvaders/background.png");
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
}


StateSpaceInvaders::~StateSpaceInvaders()
{

}

void StateSpaceInvaders::init()
{
    SDL_Rect bounds;
    bounds.x = bounds.y = 0;
    bounds.w = 800;
    bounds.h = 480;
    emit.setBoundaries(bounds);
    emit.setMax(100);
    emit.setColour(Colour(YELLOW));
    emit.setDeviation(1.6f);
    //emit.setUpdatePeriod(5);
    emit.setGravity(Vector2df(0.0f,0.01f));
    emit.setFadeValue(-0.01f);

    blast.resize(2);
    blast[0].setBoundaries(bounds);
    blast[0].setMax(70);
    blast[0].setColour(Colour(RED));
    blast[0].setDeviation(3.2f);
    blast[0].setInvisible(50);
    //blast.setUpdatePeriod(1);
    blast[0].setLifeTime(10);
    blast[0].setGravity(Vector2df(0.0f,0.05f));
    blast[0].setFadeValue(-0.01f);

    blast[1].setBoundaries(bounds);
    blast[1].setMax(30);
    blast[1].setColour(Colour(YELLOW));
    blast[1].setDeviation(3.2f);
    blast[1].setInvisible(20);
    //blast.setUpdatePeriod(1);
    blast[1].setLifeTime(10);
    blast[1].setGravity(Vector2df(0.0f,0.05f));
    blast[1].setFadeValue(-0.01f);
    shooting = false;

    alien.setPosition(rand()%736,0);
    ship.setPosition(368,416);

    if (rand()%2)
        alienDir = 1;
    else
        alienDir = -1;

    shipVel = 0;

    counter.start();
}

void StateSpaceInvaders::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(input->isLeft())
    {
        //ship.setX(ship.getX()-6);
        shipVel -= 2.0f;
        //Input.resetKeys();
    }
    if(input->isRight())
    {
        //ship.setX(ship.getX()+6);
        shipVel += 2.0f;
        //Input.resetKeys();
    }

    if(input->isA() && !shooting)
    {
        shooting = true;
        shot.setPosition(ship.getX()+28,400);
    }

    if(input->isStart())//I wasnt sure what exit key was...
    {
        pauseToggle();
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateSpaceInvaders::render(SDL_Surface *screen)
{
	background.render(screen);


    if(shipVel >0.01f || shipVel < -0.01f)
        emit.render(screen);
    alien.render(screen);
    ship.render(screen);

    if(shooting)
    {
        for(int i = 1; i >= 0; --i)
            blast[i].render(screen);
        shot.render(screen);
    }
    text.setPosition(0,0);
    text.print(screen,"Time: ");
    text.setPosition(100,0);
    text.print(screen,9 - counter.getScaledTicks());

    if(counter.getTicks() <= 500)
        command.print(screen, "Shoot!");
}

void StateSpaceInvaders::pauseScreen(SDL_Surface* screen)
{
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.setColour(WHITE);
    pauseText.print(screen, "Shoot the alien! press    to shoot.");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(365,180);
    buttonSheet.render(screen);
}
#else
    void StateSpaceInvaders::render()
    {
        background.render();


        if(shipVel >0.01f || shipVel < -0.01f)
            emit.render();
        alien.render();
        ship.render();

        if(shooting)
        {
            for(int i = 1; i >= 0; --i)
                blast[i].render();
            shot.render();
        }
        text.setPosition(0,0);
        text.print("Time: ");
        text.setPosition(100,0);
        text.print(9 - counter.getScaledTicks());

        if(counter.getTicks() <= 500)
            command.print("Shoot!");
    }
    void StateSpaceInvaders::pauseScreen()
    {
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.setColour(WHITE);
        pauseText.print("Shoot the alien! press   to shoot");
    }
#endif

void StateSpaceInvaders::onPause()
{
    counter.pause();
}

void StateSpaceInvaders::onResume()
{
    counter.unpause();
}

void StateSpaceInvaders::update()
{
        emit.setAcceleration(Vector2df(-shipVel*0.5f,0));
        emit.setPosition(Vector2di(ship.getX()+32,ship.getY()+32));
        emit.update();


        alien.setX(alien.getX() + alienDir*6);
        if(alien.getX() <= 0 || alien.getX() >= 736)
        {
            alienDir *= -1;
            alien.setY(alien.getY() + 32);
        }

    //    if(alien.getY() >= 416)
    //    {
    //        variables[0].setInt(0);
    //        gotoNext = true;
    //    }

        if(shooting)
        {
            for(int i = 1; i >= 0; --i)
            {
                blast[i].setPosition(Vector2di(shot.getX()+4,shot.getY()+16));
                blast[i].update();
            }
            shot.setY(shot.getY()-16);
            if(shot.getX()-8 >= alien.getX() && shot.getX() <= alien.getX()+64
            && shot.getY()-16 >= alien.getY() && shot.getY() <= alien.getY()+64)
            {
                variables[0].setInt(1);
                setNextState(STATE_MAIN);
            }

            if(shot.getY() < 0)
            {
                variables[0].setInt(0);
                setNextState(STATE_MAIN);
            }
        }

        alienDir *= 1.005; //increase speed

        if(shipVel < -8)
            shipVel = -8;
        if(shipVel > 8)
            shipVel = 8;

        ship.setX(ship.getX() + shipVel);
        if(ship.getX() < 0)
            ship.setX(0);
        if(ship.getX() > 736)
            ship.setX(736);

        if (shipVel < -0.1)
            shipVel += 0.1;
        if (shipVel > 0.1)
            shipVel -= 0.1;
        if (shipVel > -0.1 && shipVel < 0.1)
            shipVel = 0;

        if(counter.getScaledTicks() > 8)
        {
            //gotoNext = true;
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
}

void StateSpaceInvaders::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}
