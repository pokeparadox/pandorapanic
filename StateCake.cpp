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

    mariela.down();
    mariela.update();

    int level = variables[2].getInt();
    if(level > 5)
    {
        relight = true;
        relightInterval = (240/(float)level) +0.5f;
    }
    else
    {
        relightInterval = 30;
        relight = false;
    }
    if(level < 40)
        limit = 60 - level;
    else
        limit = 15;

    timer.setMode(MILLI_SECONDS);
    timer.start(limit*1000);
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
    text.print(timer.getTimeLeft()*0.001f);
    /*
    TODO Varies time checks to relight the candles
    Timer count-down which you must blow the candles out in.
    */
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

    // If player hasn't blown all the candles out try to relight some
    if(NumberUtility::isMultiple((timer.getTicks()*0.001f),relightInterval))
    {
        for(int i = NUM_CANDLES-1; i >=0; --i)
        {
            if(!candles[i].isLit() && relight)
            {
                relight = false;
                candles[i].light();
                break;
            }
        }
    }
}

void StateCake::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if(input->isA())
    {
        relight = true;
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
