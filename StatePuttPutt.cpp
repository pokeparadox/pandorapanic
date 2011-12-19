#include "StatePuttPutt.h"
StatePuttPutt::StatePuttPutt()
{
    nullify = false;

    text.loadFont("font/chromo.ttf", 48);
    text.setColour(Colour(YELLOW));
    command.loadFont("font/origami.ttf",48);
    command.setRelativity(false);
    //command.setPosition(380,216);
    command.setPosition(165,216);
    counter.setMode(SECONDS);
    hole.loadSprite("images/PuttPutt/Hole.png");
    ball.loadSprite("images/PuttPutt/Ball.png");
    background.loadBackground("images/PuttPutt/Background.png");

    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(BLACK);
}

StatePuttPutt::~StatePuttPutt()
{

}

void StatePuttPutt::init()
{
    prepareForPutt = true;
    putting = false;
    traveling = false;
    travelState = 0;
    completed = false;

    hole.setPosition(350,150);
    ball.setPosition(353,450);

    gMeterX = 715;
    gMeterY = 250;
    gMeterW = 50;
    gMeterH = 200;

    focusY = gMeterY+gMeterH;
    focusH = 2;

    int levelOffset = variables[2].getInt() - 10; //don't start scaling until after level 10

    focusVel = -7; //-7, -8, -9, -10, -11, -12
    sweetH = 20; //20, 18, 16
    countdown = 5; //5, 4, 3, 2

    if (levelOffset>=0)
    {
        focusVel = focusVel - 1 - (levelOffset / 4);
        if (focusVel < -12)
        {
            focusVel = -12;
        }
        sweetH = sweetH - 2 - (levelOffset / 4);
        if (sweetH < 16)
        {
            sweetH = 16;
        }
        countdown = countdown - 1 - (levelOffset / 4);
        if (countdown < 2)
        {
            countdown = 2;
        }
    }

    //focusY = gMeterY+gMeterH;
    //focusH = 2;

    int sweetPos = gMeterH - sweetH;
    sweetY = gMeterY + rand() % sweetPos;

    ballVel = 0;
    if(!isSubState())
    {
        putt.loadSound("sounds/PuttPutt/Putter.ogg");
        sunk.loadSound("sounds/PuttPutt/Sunk.ogg");
        gasp.loadSound("sounds/PuttPutt/Gasp.ogg");
    }
    counter.start();
}

void StatePuttPutt::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(input->isB() && !putting)
    {
        //putt.play();
        putting = true;
    }

    if(input->isStart())//I wasnt sure what exit key was...
    {
        pauseToggle();
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void DrawRect(int startX,int startY, int width, int height, int thickness, Uint32 Color, SDL_Surface *surface)
{
    SDL_Rect outline;
    //top
    outline.x = startX;
    outline.y = startY;
    outline.w = width;
    outline.h = thickness;
    SDL_FillRect(surface,&outline,Color);
    //end top

    //bottom
    //outline.y = startY + height;
    outline.y = startY + height - thickness;
    SDL_FillRect(surface,&outline,Color);
    //end bottom

    //left
    outline.y = startY;
    outline.w = thickness;
    outline.h = height;
    SDL_FillRect(surface,&outline,Color);
    //end left

    //right
    //outline.x = startX + width;
    outline.x = startX + width - thickness;
    SDL_FillRect(surface,&outline,Color);
    //end right
}

void StatePuttPutt::render(SDL_Surface *screen)
{
	background.render(screen);

        //Golf Meter
        SDL_Rect gBG;
        gBG.x = gMeterX;
        gBG.y = gMeterY;
        gBG.w = gMeterW;
        gBG.h = gMeterH;
        SDL_FillRect(screen,&gBG,SDL_MapRGB(screen->format,0,0,0));

        SDL_Rect sweetSpot;
        sweetSpot.x = gBG.x;
        sweetSpot.y = sweetY;
        sweetSpot.w = gBG.w;
        sweetSpot.h = sweetH;
        SDL_FillRect(screen,&sweetSpot,SDL_MapRGB(screen->format,127,255,0));

        SDL_Rect focus;
        focus.x = gBG.x;
        focus.y = focusY;
        focus.w = gBG.w;
        focus.h = focusH; //2
        SDL_FillRect(screen,&focus,SDL_MapRGB(screen->format,255,255,255));

        //Golf Bar Border
        DrawRect(gBG.x,gBG.y,gBG.w,gBG.h,2,SDL_MapRGB(screen->format,255,255,255),screen);
        //end Golf Meter

        hole.render(screen);
        ball.render(screen);
/*
        if(putting)
        {

        }
*/
        if(counter.isStarted())
        {
            text.setPosition(0,430);
            text.print(screen,"Time: ");
            text.setPosition(100,430);
            text.print(screen,countdown - counter.getScaledTicks());

            if(counter.getTicks() <= 1500)
            {
                command.setPosition(100,216);
                command.print(screen, "Make the putt");
            }
        }
        if(completed)
        {
            if(travelState==0)
            {
                command.setPosition(165,216);
                command.print(screen, "Nice putt!");
            }
            else
            {
                command.setPosition(225,216);
                command.print(screen, "Oh no!");
            }
        }
}
void StatePuttPutt::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    if(!isSubState())
        pauseSymbol(screen);
    pauseText.setPosition(20,180);
    pauseText.print(screen, "Stop the power meter within the green bar!");
    pauseText.setPosition(20,220);
    pauseText.print(screen, "Press     to make the putt!");
    buttonSheet.setCurrentFrame(11);
    buttonSheet.setPosition(104,220);
    buttonSheet.render(screen);
    buttonsOverlay(screen);
}
#else
    void StatePuttPutt::render()
    {
        background.render();
/*
        //Golf Meter
        SDL_Rect gBG;
        gBG.x = gMeterX;
        gBG.y = gMeterY;
        gBG.w = gMeterW;
        gBG.h = gMeterH;
        SDL_FillRect(screen,&gBG,SDL_MapRGB(screen->format,0,0,0));

        SDL_Rect sweetSpot;
        sweetSpot.x = gBG.x;
        sweetSpot.y = sweetY;
        sweetSpot.w = gBG.w;
        sweetSpot.h = sweetH;
        SDL_FillRect(screen,&sweetSpot,SDL_MapRGB(screen->format,127,255,0));

        SDL_Rect focus;
        focus.x = gBG.x;
        focus.y = focusY;
        focus.w = gBG.w;
        focus.h = focusH; //2
        SDL_FillRect(screen,&focus,SDL_MapRGB(screen->format,255,255,255));

        //Golf Bar Border
        DrawRect(gBG.x,gBG.y,gBG.w,gBG.h,2,SDL_MapRGB(screen->format,255,255,255),screen);
        //end Golf Meter
*/
        hole.render();
        ball.render();
/*
        if(putting)
        {

        }
*/
        if(counter.isStarted())
        {
            text.setPosition(0,430);
            text.print("Time: ");
            text.setPosition(100,430);
            text.print(countdown - counter.getScaledTicks());

            if(counter.getTicks() <= 1500)
            {
                command.setPosition(100,216);
                command.print("Make the putt");
            }
        }
        if(completed)
        {
            if(travelState==0)
            {
                command.setPosition(165,216);
                command.print("Nice putt!");
            }
            else
            {
                command.setPosition(225,216);
                command.print("Oh no!");
            }
        }
    }
    void DrawRect(int startX,int startY, int width, int height, int thickness, Uint32 Color)
    {}
void StatePuttPutt::pauseScreen()
{
    // Pause screen
    pauseSymbol();
    pauseText.setPosition(20,180);
    pauseText.print("Stop the power meter within the green bar!");
    pauseText.setPosition(20,220);
    pauseText.print("Press     to make the putt!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,220);
    buttonSheet.render();

}
#endif
void StatePuttPutt::onPause()
{
    counter.pause();
}

void StatePuttPutt::onResume()
{
    counter.unpause();
}

void StatePuttPutt::update()
{
        if(prepareForPutt)
        {
            if(focusY+focusVel<gMeterY || focusY+focusVel>gMeterY+gMeterH)
            {
                focusVel*=-1;
            }
            focusY+=focusVel;
        }
        if(putting)
        {
            if(focusY  >= sweetY + (sweetH / 2) - 1 and focusY <= sweetY + (sweetH / 2) + 1)  //perfect
            {
                ACHIEVEMENTS->logEvent("PERFECT_PUTT");
            }
            if(focusY + focusH >= sweetY and focusY <= sweetY + sweetH)  //ball in
            {
                travelState = 0;
                ballVel = 4;
            }
            else if(focusY < sweetY)  //travel too far
            {
                ballVel = 7;
                travelState = 1;
            }
            else //travel too short
            {
                ballVel = 2;
                travelState = 2;
            }
            prepareForPutt = false;
            putting = false;
            traveling = true;
            //counter.pause();
            counter.stop();
            putt.play();
        }
        if(traveling)
        {
            if(travelState==0)  //into hole
            {
                if(ball.getY() - ballVel <= hole.getY() + hole.getHeight())
                //if(ball.hasCollided(hole))
                {
                    ball.clear();
                    sunk.play();
                    traveling = false;
                    completed = true;
                }
                else
                {
                    ball.setY(ball.getY()-ballVel);
                }
            }
            else if(travelState==1)  //passing hole
            {
                if(ball.getY() - ballVel <= hole.getY() - 60)
                {
                    gasp.play();
                    traveling = false;
                    completed = true;
                }
                else
                {
                    ball.setY(ball.getY()-ballVel);
                }
            }
            else  //undershot hole
            {
                if(ball.getY() - ballVel <= hole.getY() + 100)
                {
                    gasp.play();
                    traveling = false;
                    completed = true;
                }
                else
                {
                    ball.setY(ball.getY()-ballVel);
                }
            }
        }
        if(completed)
        {
            if(travelState==0)
            {
                if(!sunk.isPlaying())
                {
                    variables[0].setInt(1);
                    setNextState(STATE_MAIN);
                }
            }
            else
            {
                if(!gasp.isPlaying())
                {
                    variables[0].setInt(0);
                    setNextState(STATE_MAIN);
                }
            }
        }
        if(counter.getScaledTicks() > countdown-1)
        {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
}
