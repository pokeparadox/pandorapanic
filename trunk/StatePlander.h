#ifndef STATEPLANDER_H
#define STATEPLANDER_H

#include "userStates.h"
#include "RocketEscape/Rocket.h"
#include "RocketEscape/LandingPad.h"
#include "ParticleMirror.h"
#include "Music.h"

class StatePlander : public BaseState
{
    public:
        StatePlander();
        virtual ~StatePlander();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface* screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void userInput();

    private:
        enum
        {
            numStars = 15,
            numBackStars = 40
        };
        Music ambient;
        Rocket rocket;
        ParticleMirror floor;
        AnimatedSprite star;
        Pixel backStar;
        Vector2di starPos[numStars];
        Vector2di backStarPos[numBackStars];

        LandingPad pad;
        void initStars();
        #ifdef PENJIN_SDL
            void renderFuelMeter(SDL_Surface* screen);
            void renderStars(SDL_Surface* screen);
        #else
            void renderFuelMeter();
            void renderStars();
        #endif
};

#endif // STATEPLANDER_H
