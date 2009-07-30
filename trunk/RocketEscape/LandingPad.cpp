#include "LandingPad.h"

LandingPad::LandingPad()
{
    //ctor
    width = 1;
    height = 0;
    platformDims.x = 0;
    platformDims.y = 0;
    string path = "images/RocketEscape/";
    string ext = ".png";
    centre.loadSprite(path+"platform"+ext);
    lights[0].loadFrames(path+"leftlight"+ext,2,1);
    lights[0].setFrameRate(SECONDS);
    lights[1].loadFrames(path+"rightlight"+ext,2,1);
    lights[1].setFrameRate(SECONDS);
    rocketLanded = false;
}

LandingPad::~LandingPad()
{
    //dtor
}

void LandingPad::setPosition(Vector2di pos)
{
    position = pos;
    lights[0].setPosition(pos);
    mirror.setPosition(Vector2di(pos.x+2,pos.y+9));
    pos.x += lights[0].getWidth();
    centre.setPosition(pos);
    if(width>1)
    {
        for(uint i = 0; i < width; ++i)
            pos.x+=centre.getWidth();
    }
    /// TODO account for platform height
    lights[1].setPosition(pos);
    platformDims.x = (pos.x + lights[1].getWidth())-position.x-4;
    platformDims.y = lights[1].getHeight()-9;
    mirror.setDimensions(platformDims);
}

#ifdef PENJIN_SDL
void LandingPad::render(SDL_Surface* screen)
{
    if(width>1)
    {
        Vector2di start(position.x + lights[0].getWidth(),position.y);
        for(uint i = 0; i < width; ++i)
        {
            centre.setPosition(start);
            centre.render(screen);
            start.x+=centre.getWidth();
        }
    }
    else
        centre.render(screen);
    for(int i = 1; i>=0; --i)
        lights[i].render(screen);

/*
    mirror.setColour(BLUE);
    mirror.render(screen);*/
}
#else
    void LandingPad::render()
    {

    }
#endif
void LandingPad::rocketCollision()
{
    float left[2];
    float right[2];
    float top[2];
    float bottom[2];
    bool collision = true;
    int xOffset = 4;
    int yOffset = 10;
    left[0] = rocket->getPosition().x + xOffset;
    left[1] = position.x;
    right[0] = left[0] + 64 - xOffset;
    right[1] = left[1] + platformDims.x;
    top[0] = rocket->getPosition().y;
    top[1] = position.y+yOffset;
    bottom[0] = top[0] + 64;
    bottom[1] = top[1] + platformDims.y-yOffset;

    if (bottom[0] < top[1])
        collision = false;
    else if (top[0] > bottom[1])
        collision = false;
    else if (right[0] < left[1])
        collision = false;
    else if (left[0] > right[1])
        collision = false;
    if(collision == true)
    {
        Vector2di rockMid = rocket->getOldPosition();
        rocket->setPosition(rockMid);
        Vector2di platMid = position;
        rockMid += Vector2di(32,32);
        platMid.x += (platformDims.x*0.5f);
        platMid.y += (platformDims.y*0.5f);
        if(rockMid.x < platMid.x || rockMid.x > platMid.x)
            rocket->invertVelocityX();
        if(rockMid.y < platMid.y || rockMid.y > platMid.y)
            rocket->invertVelocityY();
        rocket->thrustKill();
        /// Check rocket is slow enough and actually landed
        Vector2df v = rocket->getVelocity();

        if(abs(v.x) < 0.1f)
            rocketLanded = true;
    }
}

bool LandingPad::hasRocketLanded()const
{
    return rocketLanded;
}

void LandingPad::update()
{
    rocketCollision();
    for(int i = 1; i>=0; --i)
        lights[i].update();
    mirror.update();
}
