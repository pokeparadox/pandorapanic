#include "StateCake.h"

StateCake::StateCake()
{
    //ctor
}

StateCake::~StateCake()
{
    //dtor
}

void StateCake::init()
{
    back.loadBackground("images/BirthdayCake/cake.png");
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);

    int i = -1;
    candles[++i].setPosition(Vector2df(390,15));
    candles[++i].setPosition(Vector2df(390,310));
    candles[++i].setPosition(Vector2df(220,240));
    candles[++i].setPosition(Vector2df(560,240));
    candles[++i].setPosition(Vector2df(220,90));
    candles[++i].setPosition(Vector2df(560,90));

    text.loadFont("font/unispace.ttf",18);
    text.setPosition(30, 30);
    text.setColour(BLUE);
    text.setBoundaries(Vector2di(0,0),Vector2di(getStateXResolution(),getStateYResolution()));
    text.setRelativity(false);

    //  following is only to position the shadow and HitRegion in the correct place in the Preview image.
    mariela.down();
    mariela.update();

    int level = variables[2].getInt();
    if(level < 5)
    {
        for(int i = NUM_CANDLES-1; i>= 0; --i)
        {
            candles[i].setRelightLimit(30000);
        }
    }
    else if(level < 16)
    {
        for(int i = NUM_CANDLES-1; i>= 0; --i)
        {
            candles[i].setRelightLimit(((180/(float)level) +0.5f)*1000);
        }
    }
    else
    {
        for(int i = NUM_CANDLES-1; i>= 0; --i)
        {
            candles[i].setRelightLimit(((180/16.0f) +0.5f)*1000);
        }
    }
    if(level < 35)
        limit = 60 - level;
    else
        limit = 25;

    timer.setMode(SECONDS);
    timer.start(limit);
}

void StateCake::render(SDL_Surface* screen)
{
    back.render(screen);

    //  Z Check Mariela
    for(int ypos = 32; ypos < 400; ypos += 1)
    {
        for(int i = NUM_CANDLES-1; i>= 0; --i)
        {
            Vector2df c = candles[i].getPosition();
            if(candles[i].getPosition().y + 48 == ypos)
                candles[i].render(screen);
        }
        if(mariela.getHitRegion()->getY() == ypos)
            mariela.render(screen);
    }
    text.setPosition(30, 30);
    text.print(timer.getTimeLeft());
}

void StateCake::update()
{
    mariela.update();
    if(mariela.canBlowCandle())
    {
        // find the nearest candle
        Vector2df mPos = mariela.getPosition();
        int shortD = 999999999;
        uint shortIndex;
        for(int i = NUM_CANDLES-1; i >=0; --i)
        {
            Vector2df tPos = candles[i].getPosition();
            tPos -= mPos;
            int l = tPos.lengthSquared();
            if(l < shortD)
            {
                shortD = l;
                shortIndex = i;
            }
        }
        candles[shortIndex].blow();
    }
    int litCandles = 0;
    for(int i = NUM_CANDLES-1; i >=0; --i)
    {
        candles[i].update();
        if(candles[i].isLit())
            ++litCandles;
    }
    if(litCandles <= 0)
    {
        variables[0].setInt(1);
        setNextState(STATE_MAIN);
    }
    else if(timer.hasFinished())
    {
        variables[0].setInt(0);
        setNextState(STATE_MAIN);
    }
}

void StateCake::userInput()
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
    if(input->isA())
    {
        mariela.blow();
    }
    else if(input->isUp())
        mariela.up();
    else if(input->isDown())
        mariela.down();
    else if(input->isLeft())
        mariela.left();
    else if(input->isRight())
        mariela.right();
    else
        mariela.stop();
}

void StateCake::pauseScreen(SDL_Surface* screen)
{
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);

    text.setColour(BLUE);
    text.setPosition(50,180);
    text.print(screen, "Use the   s to move around.");
    text.setPosition(50,220);
    text.print(screen, "Press    to blow out all the candles!");
    buttonSheet.setCurrentFrame(15);
    buttonSheet.setPosition(140,180);
    buttonSheet.render(screen);
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(120,220);
    buttonSheet.render(screen);
}

void StateCake::onPause()
{
    timer.pause();
}

void StateCake::onResume()
{
    timer.unpause();
}

void StateCake::pauseInput()
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
