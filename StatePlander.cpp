#include "StatePlander.h"

StatePlander::StatePlander()
{
    //ctor
}

StatePlander::~StatePlander()
{
    //dtor
}

void StatePlander::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        ambient.loadMusic("music/Plander/ambient.ogg");
        ambient.setLooping(true);
        ambient.play();
        rocket.loadSound("sounds/Plander/hit.ogg");
    }
    SDL_Rect bounds;
    bounds.x = 0;
    bounds.y = -500;
    bounds.w = getStateXResolution();
    bounds.h = getStateYResolution();
    collect.loadFrames("images/RocketEscape/treasure.png",4,1);
    collect.setFrameRate(DECI_SECONDS);
    int jitter = Random::nextInt(-30,30);
    rocket.setBounds(bounds);
    rocket.setStartPosition(Vector2di(getStateXResolution()*0.2f+jitter, getStateYResolution()-100));
    rocket.setPosition(Vector2di(getStateXResolution()*0.2f+jitter, getStateYResolution()-164));
    rocket.setWrapSides(true);
    rocket.linkCollectible(&collect);
    pad.linkRocket(&rocket);

    for(int i = 2; i >= 0; --i)
    {
        floor.linkEmitter(rocket.getLeftThruster(i));
        floor.linkEmitter(rocket.getRightThruster(i));
        pad.linkEmitter(rocket.getLeftThruster(i));
        pad.linkEmitter(rocket.getRightThruster(i));
    }

    floor.setPosition(Vector2di(0,getStateYResolution()-100));
    floor.setDimensions(Vector2di(getStateXResolution(),100));
    floor.setColour(Colour(128,128,128));
    float width = 60/(float)variables[2].getInt();
    pad.setWidth(NumberUtility::limit((int)width,1,15));
    rocket.setFuel(NumberUtility::limit((width/30.0f)*100,5.0f,100.0f));
    collect.setReward((variables[2].getInt()*1.2f)+0.5f);
    pad.setPosition(Vector2di(-25+getStateXResolution()*0.5f + Random::nextInt(-30,30),getStateYResolution()-123));
    initStars();


    fuelMeter[0].setPosition(*xRes * 0.1f,*yRes * 0.1f);
    fuelMeter[0].setDimensions(102,20);
    fuelMeter[0].setColour(WHITE);
    fuelMeter[1].setPosition((*xRes * 0.1f)+1,(*yRes * 0.1f)+1);
    fuelMeter[1].setDimensions(rocket.getFuel()+0.5f,18);
    fuelMeter[1].setColour(RED);

    GFX::setClearColour(BLACK);
}

void StatePlander::initStars()
{
    star.loadFrames("images/RocketEscape/stars.png",4,1);
    star.setLooping(true);
    star.setPosition(-100,-100);
    // set random X coords
    Random::setLimits(20,*xRes*0.5f);
    starPos[numStars-1].x = Random::nextInt();
    for(int i = numStars-2; i>=0; --i)
    {
        starPos[i].x = starPos[i+1].x + Random::nextInt() + star.getWidth();
        if(starPos[i].x > *xRes)
            starPos[i].x -= *xRes;
    }
    // set random Y coords
    Random::setLimits(20,*yRes*0.5f);
    starPos[numStars-1].y = Random::nextInt() + star.getHeight();
    for(int i = numStars-2; i>=0; --i)
    {
        starPos[i].y = starPos[i+1].y + Random::nextInt();
        if(starPos[i].y > *yRes-150)
            starPos[i].y -= *yRes-150;
    }

    /// Setup other stars
    //backStar.setPosition(-100,-100);
    // set random X coords
    Random::setLimits(20,*xRes*0.5f);
    backStarPos[numBackStars-1].x = Random::nextInt();
    for(int i = numBackStars-2; i>=0; --i)
    {
        backStarPos[i].x = backStarPos[i+1].x + Random::nextInt();
        if(backStarPos[i].x > *xRes)
            backStarPos[i].x -= *xRes;
    }
    // set random Y coords
    Random::setLimits(20,*yRes*0.5f);
    backStarPos[numBackStars-1].y = Random::nextInt();
    for(int i = numBackStars-2; i>=0; --i)
    {
        backStarPos[i].y = backStarPos[i+1].y + Random::nextInt();
        if(backStarPos[i].y > *yRes-150)
            backStarPos[i].y -= *yRes-150;
    }
}

#ifdef PENJIN_SDL
void StatePlander::renderStars(SDL_Surface* screen)
{
    int f = star.getCurrentFrame();
    int divider = numStars-1*0.25f;
    for(int i = numStars-1; i>=0; --i)
    {
        star.setPosition(starPos[i]);
        int fr = f + (i/divider);
        if(fr > 3)
            fr = 0;
        else if(fr < 0)
            fr = 3;
        star.setCurrentFrame(fr);
        star.render(screen);
    }
    star.setCurrentFrame(f);

    Random::setLimits(0,3);
    int colour;
    for(int i = numBackStars-1; i>=0; --i)
    {
        backStar.setPosition(backStarPos[i]);
        colour = Random::nextInt();
        if(colour == 0)
            backStar.setColour(128,128,128);
        else if(colour == 1)
            backStar.setColour(YELLOW);
        else if(colour == 2)
            backStar.setColour(WHITE);
        else
            backStar.setColour(64,64,255);
        backStar.render(screen);
    }
}

void StatePlander::renderFuelMeter(SDL_Surface* screen)
{
    fuelMeter[1].setDimensions(rocket.getFuel()+0.5f,18);

    fuelMeter[0].render(screen);
    fuelMeter[1].render(screen);
}

void StatePlander::render(SDL_Surface* screen)
{
    GFX::clearScreen(screen);
    renderStars(screen);
    floor.render(screen);
    collect.render(screen);
    rocket.render(screen);
    pad.render(screen);
    renderFuelMeter(screen);
}
#else
void StatePlander::renderStars()
{
    int f = star.getCurrentFrame();
    int divider = numStars-1*0.25f;
    for(int i = numStars-1; i>=0; --i)
    {
        star.setPosition(starPos[i]);
        int fr = f + (i/divider);
        if(fr > 3)
            fr = 0;
        else if(fr < 0)
            fr = 3;
        star.setCurrentFrame(fr);
        star.render();
    }
    star.setCurrentFrame(f);

    Random::setLimits(0,3);
    int colour;
    for(int i = numBackStars-1; i>=0; --i)
    {
        backStar.setPosition(backStarPos[i]);
        colour = Random::nextInt();
        if(colour == 0)
            backStar.setColour(128,128,128);
        else if(colour == 1)
            backStar.setColour(YELLOW);
        else if(colour == 2)
            backStar.setColour(WHITE);
        else
            backStar.setColour(64,64,255);
        backStar.render();
    }
}

void StatePlander::renderFuelMeter()
{
    fuelMeter[1].setDimensions(rocket.getFuel()+0.5f,18);

    fuelMeter[0].render();
    fuelMeter[1].render();
}

void StatePlander::render()
{
    GFX::clearScreen();
    renderStars();
    floor.render();
    collect.render();
    rocket.render();
    pad.render();
    renderFuelMeter();
}
#endif

void StatePlander::update()
{
    collect.update();
    rocket.update();
    if(rocket.checkCollectible())
    {
        collect.setPosition(-100,-100); // move it out of the way
        float colFuel = rocket.getFuel() + collect.getReward();
        if(colFuel>100)
        {
            int lives = variables[4].getInt();
            if(lives<3)
            {
                ++lives;
                variables[4].setInt(lives);
            }
            fuelMeter[1].setColour(BLUE);
        }
        else
            rocket.setFuel(colFuel);
    }
    pad.update();
    if(pad.hasRocketLanded())
    {
        variables[0].setInt(1);
        setNextState(STATE_MAIN);
    }
    if(rocket.hasLanded())
    {
        variables[0].setInt(0);
        setNextState(STATE_MAIN);
    }
    floor.update();
    star.update();
}

void StatePlander::userInput()
{
    input->update();
#ifdef PLATFORM_PC
    if(input->isQuit())
        nullifyState();
#endif
    if(!isPaused)
    {
        if(input->isLeft() || (input->isL() && !input->isR()) || input->isA())
            rocket.rotateLeft();
        else if(input->isRight() || (input->isR() && !input->isL()) || input->isB())
            rocket.rotateRight();

        if(input->isUp() || input->isY() || (input->isL() && input->isR()))
            rocket.thrustIncrease();
        else if (input->isDown() || input->isX())
            rocket.thrustDecrease();
        else
            rocket.thrustKill();
        /*else    //  Checl analog
        {

        }*/
    }
    if(input->isStart())
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
}
