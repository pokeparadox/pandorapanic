#include "PanJoust/HeroBird.h"

HeroBird::HeroBird()
{
    onplatform = 0;
    herowalking.loadFrames("images/PanJoust/herowalking.png",4,1);
    herowalking.setTransparentColour(MAGENTA);
    herowalking.setFrameRate(DECI_SECONDS);

    herowalkingright.loadFrames("images/PanJoust/herowalkingright.png",4,1);
    herowalkingright.setTransparentColour(MAGENTA);
    herowalkingright.setFrameRate(DECI_SECONDS);

    bird.loadFrames("images/PanJoust/herotempanim.png",8,1);
    bird.setTransparentColour(MAGENTA);
    bird.setFrameRate(DECI_SECONDS);

    birdright.loadFrames("images/PanJoust/herotempanimright.png",8,1);
    birdright.setTransparentColour(MAGENTA);
    birdright.setFrameRate(DECI_SECONDS);

    wingflap.loadSound("sounds/PanJoust/swoosh.ogg");
    walking.loadSound("sounds/PanJoust/walking.ogg");

    velocity.x = 0;
    velocity.y = 0;
    direction = false;
}

HeroBird::~HeroBird()
{
}

void HeroBird::wingFlap()
{
    bird.setCurrentFrame(0);
    birdright.setCurrentFrame(8);
    changeYVelocity(-38);
    wingflap.play();
}

#ifdef PENJIN_SDL
void HeroBird::render(SDL_Surface* screen)
{
    if (onplatform == false )
    {
        if (direction == false)
        {
            bird.setPosition(position);
            bird.render(screen);
        }
        else
        {
            birdright.setPosition(position);
            birdright.render(screen);
        }
    }
    else
    {

        if (direction == false)
        {
            herowalking.setPosition(position);
            herowalking.render(screen);
        }
        else
        {
            herowalkingright.setPosition(position);
            herowalkingright.render(screen);
        }
    }
}
#else
void HeroBird::render()
{
    if (onplatform == false )
    {
        if (direction == false)
        {
            bird.setPosition(position);
            bird.render();
        }
        else
        {
            birdright.setPosition(position);
            birdright.render();
        }
    }
    else
    {

        if (direction == false)
        {
            herowalking.setPosition(position);
            herowalking.render();
        }
        else
        {
            herowalkingright.setPosition(position);
            herowalkingright.render();
        }
    }
}
#endif
void HeroBird::update()
{

    // Draw and check birdpositions
#ifdef PENJIN_FIXED
    position.x = fixedpoint::fix2int(position.x + (velocity.x/20));
    position.y = fixedpoint::fix2int(position.y + (velocity.y/4));
#else
    position.x = position.x + (velocity.x/20);
    position.y = position.y + (velocity.y/4);
#endif
    if (position.y < -30)
        {
            velocity.y = 0;
            position.y = 0;
        }

    if (position.x < -60)
    {
        position.x = 780;
    }

    if (position.x > 780)
    {
        position.x = -60;
    }


    //if (onplatform = false)
    //{
        velocity.y = velocity.y + 3;
    //}

    if (velocity.x > 0)
    {
        velocity.x = velocity.x -2;
    }
    if (velocity.x < 0)
    {
        velocity.x = velocity.x +2;
    }

    if (velocity.x > 270)
    {
        velocity.x = 270;
    }
    if (velocity.x < -270)
    {
        velocity.x = -270;
    }
    if (velocity.y < - 50)
    {
        velocity.y = - 50;
    }

    bird.update();
    birdright.update();

    if (velocity.x > 30 || velocity.x < -30)
    {

        herowalking.update();
        herowalkingright.update();
    }

    if (onplatform == true && (velocity.x > 60 || velocity.x < -60))
    {
        walking.play();
    }

}
