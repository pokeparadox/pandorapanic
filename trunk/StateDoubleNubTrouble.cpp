#include "StateDoubleNubTrouble.h"
StateDoubleNubTrouble::StateDoubleNubTrouble()
{
    nullify = false;
    teller.setMode(SECONDS);
    shotdelay.setMode(SECONDS);
}

StateDoubleNubTrouble::~StateDoubleNubTrouble()
{
}

void StateDoubleNubTrouble::init()
{
    // set initial variables
    text.loadFont("font/bip.ttf",32);
    text.setColour(RED);
    inputlimiter.setMode(MILLI_SECONDS);
    inputlimiter.start();
    background.loadBackground("images/DoubleNubTrouble/back.png");
    test.loadFont("font/bip.ttf",90);
    test.setColour(Colour(RED));
    messageText.loadFont("font/bip.ttf",80);
    messageText.setColour(Colour(GREEN));
    SDL_Rect bounds;
    bounds.x = bounds.y = 0;
    bounds.w = 800;
    bounds.h = 480;
    scope.x = 500;
    scope.y = 240;
    scope2.x = 250;
    scope2.y = 240;
    nubstate = 0;
    ronde = 1;
    killcount = 0;
    shot.loadSound("sounds/DoubleNubTrouble/shot.ogg");
    //set difficulty with birdnumber

    /*if (variables[2].getInt() < 70)
    {
        birdnumber = (variables[2].getInt() / 10) + 2;
    }
    else
    {*/
      birdnumber = 9;
    //}
    timelimit = 2;
    penalty = 0;
    shotdelay.start();

    for ( int i = 0; i < birdnumber; i++)
    {
        birddead[i] = 0;
        targetpos[i].x = rand()%750;
        targetpos[i].y = rand()%430;
        target[i].loadSprite("images/DoubleNubTrouble/bird.png");
        target[i].setPosition(targetpos[i].x,targetpos[i].y);
        target[i].setTransparentColour(MAGENTA);
    }
    for (int i = 0; i<2; i++)
    {
        mysprite[i].loadSprite("images/DoubleNubTrouble/target.png");
        mysprite[i].setTransparentColour(MAGENTA);
    }
    mysprite[0].setPosition(scope);
    mysprite[1].setPosition(scope2);
    tv = SDL_EnableUNICODE(1); //enables unicode and saves the previous state
    GFX::setClearColour(BLACK);
    teller.start();
}

void StateDoubleNubTrouble::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
    if(input->isQuit())
    nullifyState();
    #endif

    if (input->isStart())
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
    if (nubstate == 1) // see if game is running
    {
        if(input->isR() && !isPaused) // check for shot
        {
            shot.play();
            tijdelijk = killcount;

            // Check if scopes align with targets and kill the birds

            for ( int i = 0; i < birdnumber; i++)
            {
                if(targetpos[i].x >= scope.x - 40 && targetpos[i].x <= scope.x + 40
                && targetpos[i].y >= scope.y - 40 && targetpos[i].y <= scope.y + 40)
                {
                    if (birddead[i] == 0)
                    {
                        killcount = killcount + 1;
                    }
                    birddead[i] = 1;
                }
                if(targetpos[i].x >= scope2.x-40 && targetpos[i].x <= scope2.x + 40
                && targetpos[i].y >= scope2.y-40 && targetpos[i].y <= scope2.y + 40)
                {
                    if (birddead[i] == 0)
                    {
                        killcount = killcount + 1;
                    }
                    birddead[i] = 1;
                }
                if (killcount == birdnumber)
                {
                    variables[0].setInt(1);
                    SDL_EnableUNICODE(tv);
                    setNextState(STATE_MAIN);
                }
            }

            // Penalty if

            if (tijdelijk + 2 > killcount)
            {
                if (shotdelay.getScaledTicks() > 0)
                {
                    penalty = penalty + 1;

                }
            }
            shotdelay.start();
        }

        else
        {
            if (!isPaused)
            {
                // make sure the scopes stay on screen and move their location

                if (scope2.x > -30 && scope2.x < 770)
                {
                    scope2.x = scope2.x + input->getLeftStickX()*0.5f;
                }
                if (scope2.y > -30 && scope2.y < 450)
                {
                    scope2.y = scope2.y + input->getLeftStickY()*0.5f;
                }
                if (scope.x > -30 && scope.x < 770)
                {
                scope.x = scope.x + input->getRightStickX()*0.5f;
                }
                if (scope2.y > -30 && scope2.y < 450)
                {
                scope.y = scope.y + input->getRightStickY()*0.5f;
                }
            }
        }
    }
}

#ifdef PENJIN_SDL
void StateDoubleNubTrouble::pauseScreen(SDL_Surface* screen)
{
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    text.setPosition(40,200);
    text.setColour(WHITE);
    text.print(screen, "Shoot 2 birds with one shot or lose seconds");
}
#else
void StateDoubleNubTrouble::pauseScreen()
{
    pauseSymbol();
    text.setPosition(40,200);
    text.setColour(WHITE);
    text.print("Shoot 2 birds with one shot or lose seconds");
}
#endif

void StateDoubleNubTrouble::onPause()
{
    teller.pause();
}

void StateDoubleNubTrouble::onResume()
{
    teller.unpause();
}

#ifdef PENJIN_SDL
void StateDoubleNubTrouble::render(SDL_Surface *screen)
{

    for ( int i = 0; i < birdnumber; ++i)
    {
        if (birddead[i] == 1)
        {
            targetpos[i].y = targetpos[i].y + 20;
            target[i].setPosition(targetpos[i].x,targetpos[i].y);
        }
    }
	GFX::clearScreen(screen);
    background.render(screen);
    if (nubstate == 0)
        {
            messageText.setPosition(80, 200);
            messageText.print(screen, "Grab your nubs!");
        }
    if (nubstate == 1)
    {
        for ( int i = 0; i < birdnumber; ++i)
        {
            target[i].render(screen);
        }
        mysprite[0].setPosition(scope.x, scope.y);
        mysprite[0].render(screen);
        mysprite[1].setPosition(scope2.x, scope2.y);
        mysprite[1].render(screen);
    }
    test.setPosition(50,30);
    test.print(screen, (timelimit - teller.getScaledTicks() - penalty));
}
#else
void StateDoubleNubTrouble::render()
{
    for ( int i = 0; i < birdnumber; ++i)
    {
        if (birddead[i] == 1)
        {
            targetpos[i].y = targetpos[i].y + 20;
            target[i].setPosition(targetpos[i].x,targetpos[i].y);
        }
    }
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
    background.render();
    if (nubstate == 0)
        {
            messageText.setPosition(80, 200);
            messageText.print("Grab your nubs!");
        }
    if (nubstate == 1)
    {
        for ( int i = 0; i < birdnumber; ++i)
        {
            target[i].render();
        }
        mysprite[0].setPosition(scope.x, scope.y);
        mysprite[0].render();
        mysprite[1].setPosition(scope2.x, scope2.y);
        mysprite[1].render();
    }
    test.setPosition(50,30);
    test.print((timelimit - teller.getScaledTicks() - penalty));
}
#endif

void StateDoubleNubTrouble::update()
{
    #ifdef PLATFORM_PC
    /// Skip nub trouble if user is unable to play and give a free point!
        if(SDL_NumJoysticks()<1)
        {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }
    #endif
        if (killcount == birdnumber)
        {
            variables[0].setInt(1);
            SDL_EnableUNICODE(tv);
            setNextState(STATE_MAIN);

        }
        // Hier alle actieve shit
        if(teller.getScaledTicks() > (timelimit - 1 - penalty))
        {

            if (nubstate == 0)
            {
                nubstate++;
                teller.start();
                timelimit = 19;
            }
            else
            {
                variables[0].setInt(0);
                SDL_EnableUNICODE(tv);
                setNextState(STATE_MAIN);
            }

        }
}
