#ifndef STATESEGWAY_H_INCLUDED
#define STATESEGWAY_H_INCLUDED

#include "userStates.h"
#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"


#ifdef PLATFORM_GP2X
    #include "MMUHack.h"
#endif
#ifndef PLATFORM_WII
    #include "GP2XJoy.h"
#else
    #include "WiiJoy.h"
#endif

#define pi 3.14159265

#define platforms 5

class StateSegway;
class Segway : public Sprite
{
    public:
        Segway();
        ~Segway();
        void update(double acc, StateSegway* obj);
        bool getOnPlatform(){return onplatform;}
        void setOnPlatform(bool boo){onplatform=boo;}
        Vector2df getVelocity(){return velocity;}
        double getXVelocity(){return velocity.x;}
        double getYVelocity(){return velocity.y;}
        int i;

    private:
        bool onplatform;
        Vector2df velocity;
};

class StateSegway : public BaseState
{
    public:
        StateSegway();
        virtual ~StateSegway();
        virtual void init();
        virtual void render(SDL_Surface *screen);
        virtual void update();
        virtual void userInput();

        Text text;
        Timer timer1;
        Background background0;
        int speed;
        float acc;
        int tv;
        Vector2di start;        // Segway Start position
        int rotation[5];
        Sprite platform[5];
        Segway segway1;

        Music backsound;

    #ifdef PLATFORM_GP2X
		MMUHack hack;
	#endif
};




#endif // StateSegway_H_INCLUDED
