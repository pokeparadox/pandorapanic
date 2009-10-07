#include "StateBallDrop.h"
StateBallDrop::StateBallDrop()
{
    nullify = false;
    cnt=0;
    //srand(time(NULL)); // I seed random numbers on app start - Poke
    ballx=rand()%600+100;
    bally=40;
    ballz=25;
    x=400;
    dir=rand()%4-2;
    text.loadFont("font/origami.ttf", 48);
    text.setColour(Colour(BLACK));
    yadd=-8;
    buttonPrompter.display(ButtonPrompter::BUTTON_RIGHT, 385, 370);
    buttonPrompter.display(ButtonPrompter::BUTTON_LEFT, 350, 370);
    gotoNext = false;
    GFX::setClearColour(WHITE);

    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(BLACK);
}

void StateBallDrop::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        music.loadMusic("music/Balldrop/bd_shortfall.ogg");
        music.setLooping(true);
        music.play();

        input->setScaler(0.00007f);
        input->setDeadZoneX(2);
    }
}

StateBallDrop::~StateBallDrop()
{

}

void StateBallDrop::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif


    /////////////////////////////
    //Added by billykater
    if(input->isLeft())
    {
        x -= BALL_SPEED;
        if(x < 40)
            x = 40;
    }
    else if(input->isRight())
    {
        x += BALL_SPEED;
        if(x > (800 - 40))
            x = 800 - 40;
    }
    else
    {
        x+=input->getLeftStickX()+0.5f;
        if(x < 40)
            x = 40;
        if(x > (800 - 40))
            x = 800 - 40;
    }

    /////////////////////////////

    if(input->isStart())//I wasnt sure what exit key was...
    {
        isPaused = !isPaused;
        music.playPause();
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateBallDrop::render(SDL_Surface *screen)
{
        GFX::clearScreen(screen);
        if(cnt<3)
        {
            text.setPosition(300,216);
            aaellipseRGBA(screen,(int)x,400,40,40,0,0,0,255);
            SDL_Rect r;
            r.x=0;
            r.y=350;
            r.w=800;
            r.h=50;
            SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,255,255,255));
            #ifdef PENJIN_FIXED
            aaellipseRGBA(screen,(int)ballx.intValue<<16,(int)bally.intValue<<16,ballz,ballz,0,0,0,255);
            #else
            aaellipseRGBA(screen,(int)ballx,(int)bally,ballz,ballz,0,0,0,255);
            #endif
        }
        if(cnt<2)
        {
            bally+=8;
            if(bally>400)cnt++;
            text.print(screen,"CATCH!");
        }
        if(cnt==2)
        {
            if(ballx-6<x-40||ballx+6>x+40)
                cnt=3;
            else
                cnt=4;
            text.print(screen,"CATCH!");
        }
        if(cnt==3)
        {
            /*text.setPosition(375,216);
            text.print(screen,":(");*/
            variables[0].setInt(0);
            gotoNext = true;
            GFX::clearScreen();
        }
        if(cnt==4)
        {
            /*text.setPosition(375,216);
            text.print(screen,":)");*/
            variables[0].setInt(1);
            gotoNext = true;
            GFX::clearScreen();
        }
        //buttonPrompter.render(screen);
}
void StateBallDrop::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    pauseSymbol(screen);
    text.setPosition(300,216);
    text.setColour(WHITE);
    text.print(screen,"CATCH!");
    text.setColour(BLACK);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Catch the ball!");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Use     and     to navigate!");
    buttonPrompter.renderImage(14,100,220);
    buttonPrompter.renderImage(15,200,220);
}
#else
void StateBallDrop::render()
{
    GFX::clearScreen();
        if(cnt<3)
        {
            text.setPosition(300,216);
            //aaellipseColor(screen,(int)x,400,40,40,SDL_MapRGBA(screen->format,0,0,0,255));
            SDL_Rect r;
            r.x=0;
            r.y=350;
            r.w=800;
            r.h=50;
            //SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,255,255,255));
            #ifdef PENJIN_FIXED
            //aaellipseColor(screen,(int)ballx.intValue<<16,(int)bally.intValue<<16,ballz,ballz,SDL_MapRGBA(screen->format,0,0,0,255));
            #else
            //aaellipseColor(screen,(int)ballx,(int)bally,ballz,ballz,SDL_MapRGBA(screen->format,0,0,0,255));
            #endif
        }
        if(cnt<2)
        {
            bally+=8;
            if(bally>400)cnt++;
                text.print("CATCH!");
        }
        if(cnt==2)
        {
            if(ballx-6<x-40||ballx+6>x+40)
                cnt=3;
            else
                cnt=4;
            text.print("CATCH!");
        }
        if(cnt==3)
        {
            /*text.setPosition(375,216);
            text.print(screen,":(");*/
            variables[0].setInt(0);
            gotoNext = true;
            GFX::clearScreen();
        }
        if(cnt==4)
        {
            /*text.setPosition(375,216);
            text.print(screen,":)");*/
            variables[0].setInt(1);
            gotoNext = true;
            GFX::clearScreen();
        }
        //buttonPrompter.render(screen);
}
void StateBallDrop::pauseScreen()
{
    // Pause screen
    pauseSymbol();
    text.setPosition(300,216);
    text.setColour(WHITE);
    text.print("CATCH!");
    text.setColour(BLACK);
    pauseText.setPosition(50,180);
    pauseText.print("Catch the ball!");
    pauseText.setPosition(50,220);
    pauseText.print("Use     and     to navigate!");
    buttonPrompter.renderImage(14,100,220);
    buttonPrompter.renderImage(15,200,220);
}
#endif

void StateBallDrop::update()
{
    if(gotoNext)
        setNextState(STATE_MAIN);
}
