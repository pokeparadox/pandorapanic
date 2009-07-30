#include "StateSegway.h"
#include <cmath>
using std::sin;
using std::cos;
using std::tan;

double tempX;
double tempY;
double temp;

StateSegway::StateSegway()
{
    background0.loadBackground("images/Segway/back2.png");
}

StateSegway::~StateSegway()
{}

void StateSegway::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        tv = SDL_EnableUNICODE(1);
        speed=0;
        acc=0;
        start.x = 230;
        start.y = 0;
    }
    // set rotiations for all platforms (default = 0)
    rotation[0] = 10;
    rotation[1] = 40;
    rotation[2] = 0;
    rotation[3] = 0;
    rotation[4] = 0;
    for (int i=0;i<platforms;i++)
    {
        platform[i].loadSprite("images/Segway/platform.png");
        platform[i].setTransparentColour(MAGENTA);
        platform[i].setRotation(rotation[i]);
    }

    platform[0].setPosition(105,200);
    platform[1].setPosition(400,220);
    platform[2].setPosition(105,440);
    platform[3].setPosition(356,440);
    platform[4].setPosition(600,300);
    segway1.loadSprite("images/Segway/segway.png");
    segway1.setTransparentColour(WHITE);
    segway1.setPosition(start);
    segway1.setOnPlatform(true);
}

void StateSegway::userInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
            {
                if((char)(event.key.keysym.unicode) == 'a') acc=-0.5;         // decrease accelaration
                else if ((char)(event.key.keysym.unicode) == 'd') acc=0.5;    // increase acceleration
                else if ((char)(event.key.keysym.unicode) == 's') acc=0;      // no acceleration
            }
            break;
        }
    }
}

void StateSegway::render(SDL_Surface *screen)
{

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
    background0.render(screen);

    text.setPosition(10,10);
    text.loadFont("font/AlphaMaleModern.ttf",50);
    text.print(screen,"X: ");
    text.setPosition(165,10);
    text.print(screen,(segway1.getX()));
    text.setPosition(10,150);
    text.print(screen,"Y: ");
    text.setPosition(165,150);
    text.print(screen,segway1.getY());
    text.setPosition(600,10);
    text.print(screen,segway1.getOnPlatform());
    for (int i=0;i<platforms;i++)
    {
        platform[i].render(screen);
    }
    segway1.clear();
    if (acc == 0)
        segway1.loadSprite("images/Segway/segway.png");
    else if (acc > 0)
        segway1.loadSprite("images/Segway/segway_forward.png");
    else
        segway1.loadSprite("images/Segway/segway_backward.png");
    segway1.render(screen);

}

void StateSegway::update()
{
    static bool initialiseMeOnce = false;
    if (initialiseMeOnce == false)
    {
        tempX = segway1.getX();
        tempY = segway1.getY();
        initialiseMeOnce = true;
    }
    tempX += segway1.getXVelocity();           // integration of x velocity
    tempY -= segway1.getYVelocity();           // integration of y velocity
    while (tempY - temp < -74 && segway1.getOnPlatform())
        tempY = tempY + 1;
    while (tempY - temp > -70 && segway1.getOnPlatform())
        tempY = tempY - 1;
    segway1.update(acc, this);
    segway1.setPosition(tempX , tempY);        // set new position

    if (segway1.getX() > 780 && segway1.getY() == 228)
        setNextState(STATE_MAIN);

    if (tempX < -15 || tempX > 800 || tempY > 480)
        nullifyState();
}

Segway::Segway()
{

    velocity.x = 0;
    velocity.y = 0;
}

Segway::~Segway()
{
}

void Segway::update(double acc, StateSegway* obj)
{
    static bool freefall = true;
    onplatform = false;         // default
    for (i=0;i<platforms;i++)
    {
        // collision check with each platform
        temp = obj->platform[i].getY() - ((tan(pi*obj->rotation[i]/180))*(tempX - 110 - obj->platform[i].getX())); // precalculation for speedup
        if (tempX +15 >= obj->platform[i].getX() + 125 - ( (cos(pi*obj->rotation[i]/180))*125 )   &&   // check left edge
            tempX +15 <= obj->platform[i].getX() + 125 + ( (cos(pi*obj->rotation[i]/180))*125 )   &&   // check right edge
            tempY +75 >= temp                                                                     &&   // check upper edge
            tempY +40 <= temp )                                                                     // check lower edge
        {
            onplatform = true;      // switch to true if on platform i
            break;                  // break when platform found ; Segway stands on platform i
        }
    }
    if (onplatform)                 // check if on any platform
    {
        acc-= (sin(pi*obj->rotation[i]/180))*0.5f ;         // increase/decrease accelaration depending on incline
        velocity.x += cos(pi*obj->rotation[i]/180)*acc;  // integrate x acceleration
        velocity.y += sin(pi*obj->rotation[i]/180)*acc;  // integrate y acceleration

        if (freefall == true)       // just reached a platform
        {
            velocity.x = cos(pi*obj->rotation[i]/180)*((cos(pi*obj->rotation[i]/180)*velocity.x)+(sin(pi*obj->rotation[i]/180)*velocity.y));    // calculate new x velocity depending on new platform angle
            velocity.y = sin(pi*obj->rotation[i]/180)*((cos(pi*obj->rotation[i]/180)*velocity.x)+(sin(pi*obj->rotation[i]/180)*velocity.y));    // calculate new y velocity depending on new platform angle
            acc = 0;
            velocity.x += cos(pi*obj->rotation[i]/180)*acc;  // integrate x acceleration
            velocity.y += sin(pi*obj->rotation[i]/180)*acc;  // integrate y acceleration
            tempY = temp - 72;
        }
        freefall = false;

    }
    else
    {
        velocity.y--;           // fall down!
        freefall = true;
    }
}

