#ifndef STATEPLANDER_H
#define STATEPLANDER_H

#include "PenjinStates.h"

#include "RocketEscape/Rocket.h"
#include "RocketEscape/LandingPad.h"
#include "RocketEscape/Collectible.h"
#include "ParticleMirror.h"
#include "Music.h"
#include "Rectangle.h"

class StatePlander : public BaseState
{
    public:
        StatePlander();
        virtual ~StatePlander();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface* screen);
            virtual void pauseScreen(SDL_Surface* screen);
        #else
            virtual void render();
            virtual void pauseScreen();
        #endif
        virtual void update();
        virtual void userInput();
        virtual void pauseInput();

    private:
        enum
        {
            numStars = 15,
            numBackStars = 40
        };
        Music ambient;
        Rocket rocket;
        ParticleMirror floor;
        Text pauseText;
        AnimatedSprite buttonSheet;
    // Twinkly Stars
        AnimatedSprite star;
        Sprite planet;
        Sprite ground;
        Pixel backStar;
        Vector2di starPos[numStars];
        Vector2di backStarPos[numBackStars];

        LandingPad pad;
        Collectible collect;
        Rectangle fuelMeter[2];
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
